// Vier gewinnt für 2 bis 4 Spieler in der Konsole.
// Je mehr Spieler mitspielen, desto größer ist das Spielfeld.

#include <iostream>   // für cin und cout (Eingabe und Ausgabe)
#include <vector>     // für vector (eine Liste, die wachsen kann)
#include <string>     // für string (Text)
#include <fstream>    // für Dateien (später für Speichern und Laden)
#include <limits>     // für numeric_limits (beim Wegwerfen falscher Eingaben)
#include <iomanip>    // für setw (Zahlen gleich breit ausgeben)
#include "Header.h"   // unsere Klasse CPerson
using namespace std;

// Farben für die Konsole
const string RED = "\033[1;31m";     // rot
const string YELLOW = "\033[1;33m";  // gelb
const string BLUE = "\033[1;34m";    // blau
const string RESET = "\033[0m";      // wieder normale Farbe

// R = Anzahl der Reihen, C = Anzahl der Spalten.
// createBoard() ändert sie je nach Spielerzahl.
int R = 6, C = 7;


// ----- Funktionen der Klasse CPerson (aus Header.h) -----

// Namen speichern
void CPerson::setname(string N) {
    this->name = N;
}
// Namen zurückgeben
string CPerson::getname() {
    return this->name;
}
// Punkte speichern
void CPerson::sethighscore(int H) {
    this->highscore = H;
}
// Punkte zurückgeben
int CPerson::gethighscore() {
    return this->highscore;
}

// Zeichen auf dem Spielfeld:
// '.' = leer, X = Spieler 1, O = Spieler 2, $ = Spieler 3, & = Spieler 4
const char EMPTY = '.', P1 = 'X', P2 = 'O', P3 = '$', P4 = '&';

// Das Spielfeld: eine Liste von Reihen. Jede Reihe ist eine Liste von Feldern.
// Am Anfang ist es leer. createBoard() füllt es.
// Ein Feld liest man so: b[reihe][spalte]
vector<vector<char>> b;

// Macht ein neues, leeres Spielfeld in der richtigen Größe.
// 2 Spieler: 7 x 6, 3 Spieler: 9 x 8, 4 Spieler: 11 x 10
void createBoard(int spielerzahl) {
    R = 6 + 2 * (spielerzahl - 2);           // jeder weitere Spieler: 2 Reihen mehr
    C = 7 + 2 * (spielerzahl - 2);           // jeder weitere Spieler: 2 Spalten mehr
    b.assign(R, vector<char>(C, EMPTY));     // R Reihen mit je C leeren Feldern
}

// Diese Funktionen stehen weiter unten.
// Hier sagen wir dem Compiler schon einmal, dass es sie gibt.
void printBoard();
int drop(int col, char p);
bool win(int r, int c, char p);
bool draw();
bool trymove(int col, char p);
int aimove();

// Liest eine Zahl ein.
// Bei Text statt Zahl: Fehlermeldung und noch einmal lesen.
// Gibt false zurück, wenn keine Eingabe mehr kommt.
bool readNumber(int& value) {
    while (!(cin >> value)) {                // Lesen hat nicht geklappt
        if (cin.eof()) return false;         // keine Eingabe mehr
        cin.clear();                         // Fehler löschen
        cin.ignore(numeric_limits<streamsize>::max(), '\n');   // falsche Eingabe wegwerfen
        cout << "Ungueltige Eingabe! Bitte nur ganze Zahlen eingeben." << endl;
    }
    return true;                             // eine Zahl wurde gelesen
}

int main() {
    int X = 0;                               // Auswahl im Menü

    // Das Menü kommt immer wieder, bis man 7 wählt
    while (X != 7) {
        // Menü anzeigen
        cout << "\nConnect Four\n"
            << "1) Neues Spiel starten\n"
            << "2) Anzahl der Spieler festlegen\n"
            << "3) Spielernamen eingeben\n"
            << "4) Spiel speichern\n"
            << "5) Spiel laden\n"
            << "6) Highscore anzeigen\n"
            << "7) Programm beenden\n"
            << "Choice: ";
        if (!readNumber(X)) return 0;        // keine Eingabe mehr: Programm beenden

        switch (X)
        {
        case 1:                              // 1) Neues Spiel
        {
            // Spielerzahl fragen, bis sie 2, 3 oder 4 ist
            int spielerzahl = 0;
            while (spielerzahl < 2 || spielerzahl > 4) {
                cout << "Wie viele Spieler spielen mit (2 bis 4)?" << endl;
                if (!readNumber(spielerzahl)) return 0;
            }

            // Spielfeld in der richtigen Größe machen
            createBoard(spielerzahl);

            // Namen einlesen und die Spieler in der Liste p1 speichern
            vector<CPerson> p1;
            string name;
            for (int i = 0; i < spielerzahl; i++)
            {
                cout << "Spieler " << i + 1 << ", Name: " << endl;
                cin >> name;
                CPerson spieler;             // neuen Spieler anlegen
                spieler.setname(name);       // Namen geben
                spieler.sethighscore(0);     // mit 0 Punkten anfangen
                p1.push_back(spieler);       // hinten an die Liste hängen
            }

            // Zeichen der Spieler: Spieler 1 = X, Spieler 2 = O, Spieler 3 = $, Spieler 4 = &
            const char symbols[4] = { P1, P2, P3, P4 };
            int current = 0;                 // wer gerade dran ist (0 = Spieler 1)

            // Ein Durchlauf = ein Zug. Es geht weiter, bis jemand gewinnt oder das Feld voll ist.
            while (true) {
                char p = symbols[current];   // Zeichen vom Spieler am Zug
                printBoard();                // Spielfeld zeigen

                // Farbe wählen: X rot, O gelb, $ und & blau
                string player_color = (p == P1) ? RED : (p == P2) ? YELLOW : BLUE;

                // Frage stellen, zum Beispiel: Anna [X] choose column (1-7):
                cout << p1[current].getname();
                cout << " [" << player_color << p << RESET;
                cout << "] choose column (1-" << C << "): ";

                // Spalte lesen. Bei Text: Fehlermeldung und nur die Frage noch einmal.
                int c = 0;
                while (!(cin >> c)) {
                    if (cin.eof()) return 0; // keine Eingabe mehr: Programm beenden
                    cin.clear();             // Fehler löschen
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');   // falsche Eingabe wegwerfen
                    cout << "Ungueltige Eingabe! Bitte nur ganze Zahlen eingeben." << endl;
                    cout << p1[current].getname();
                    cout << " [" << player_color << p << RESET;
                    cout << "] choose column (1-" << C << "): ";
                }

                c--;                         // Spieler tippt ab 1, das Programm zählt ab 0
                int r = drop(c, p);          // Stein einwerfen. r = Reihe, wo er liegt.

                // Zug geht nicht: Meldung, und der gleiche Spieler ist noch einmal dran
                if (r == -1) { cout << "Invalid move, try again.\n"; continue; }

                // Gewonnen: Spielfeld und Sieger zeigen, Spiel zu Ende
                if (win(r, c, p)) {
                    printBoard();
                    // KORREKTUR 11: Der Sieger ist der Spieler am Zug. Vorher stand
                    // "Computer", wenn Spieler 2 in Spalte 3 gewonnen hat (c == 2).
                    cout << player_color << p1[current].getname() << " WINS!\n" << RESET;
                    break;
                }

                // Spielfeld voll: unentschieden, Spiel zu Ende
                if (draw()) {
                    printBoard();
                    cout << "It's a DRAW!\n";
                    break;
                }

                // Nächster Spieler. Nach dem letzten ist wieder der erste dran.
                current = (current + 1) % spielerzahl;
            }
            break;                           // zurück zum Menü
        }
        case 2: { // 2) Anzahl der Spieler festlegen: kommt noch
            break;
        }
        case 3: { // 3) Spielernamen eingeben: kommt noch
            break;
        }
        case 4: { // 4) Spiel speichern: kommt noch
            break;
        }
        case 5: { // 5) Spiel laden: kommt noch
            break;
        }
        case 6: { // 6) Highscore anzeigen: kommt noch
            break;
        }
        case 7: { // 7) Programm beenden: die Schleife hört auf, weil X == 7
            break;
        }
        default: { // eine Zahl, die nicht 1 bis 7 ist
            cout << "Ungueltige Eingabe! Bitte eine Zahl von 1 bis 7 eingeben." << endl;
            break;
        }
        }
    }
    return 0;
}


// Zeigt das Spielfeld an.
// Jedes Feld ist 3 Zeichen breit, damit auch die Nummern 10 und 11 passen.
void printBoard() {
    cout << "\n";
    for (int i = 0; i < R; i++) {            // jede Reihe, von oben nach unten
        cout << BLUE << "|" << RESET;         // linker Rand
        for (int j = 0; j < C; j++) {        // jedes Feld in dieser Reihe
            if (b[i][j] == P1) {             // X in Rot
                cout << " " << RED << b[i][j] << RESET << " ";
            }
            else if (b[i][j] == P2) {        // O in Gelb
                cout << " " << YELLOW << b[i][j] << RESET << " ";
            }
            else if (b[i][j] == P3 || b[i][j] == P4) {   // $ und & in Blau
                cout << " " << BLUE << b[i][j] << RESET << " ";
            }
            else {                           // leeres Feld ohne Farbe
                cout << " " << b[i][j] << " ";
            }
        }
        cout << BLUE << "|\n" << RESET;       // rechter Rand, dann neue Zeile
    }
    cout << BLUE << "+";                      // Linie unter dem Spielfeld
    for (int j = 0; j < C * 3; j++) cout << "-";
    cout << "+\n" << RESET << " ";
    for (int j = 0; j < C; j++) cout << setw(2) << j + 1 << " ";   // Spaltennummern 1 bis C
    cout << "\n\n";
}

// Computergegner: sucht eine gute Spalte für O.
// Wird im Moment noch nicht benutzt.
int aimove() {
    // 1. Kann O sofort gewinnen? Dann diese Spalte nehmen.
    for (int c = 0; c < C; c++) {
        if (b[0][c] == EMPTY && trymove(c, P2)) return c;
    }
    // 2. Kann X sofort gewinnen? Dann diese Spalte blockieren.
    for (int c = 0; c < C; c++) {
        if (b[0][c] == EMPTY && trymove(c, P1)) return c;
    }
    // 3. Sonst eine freie Spalte möglichst nah an der Mitte nehmen.
    for (int d = 0; d <= C / 2; d++) {       // d = Abstand zur Mitte
        if (C / 2 - d >= 0 && b[0][C / 2 - d] == EMPTY) return C / 2 - d;   // links von der Mitte
        if (C / 2 + d < C && b[0][C / 2 + d] == EMPTY) return C / 2 + d;    // rechts von der Mitte
    }
    return 0;
}

// Prüft, ob p mit dem neuen Stein gewonnen hat.
// r und c = Reihe und Spalte vom neuen Stein.
// In 4 Richtungen zählen: Wie viele Steine von p liegen direkt in einer Linie?
// count fängt bei 1 an, weil der neue Stein selbst mitzählt.
bool win(int r, int c, char p) {
    int count;

    // waagerecht ( - )
    count = 1;
    // nach links zählen
    for (int j = c - 1; j >= 0 && b[r][j] == p; j--) {
        count++;
    }
    // nach rechts zählen
    for (int j = c + 1; j < C && b[r][j] == p; j++) count++;
    if (count >= 4) return true;             // 4 in einer Linie: gewonnen

    // senkrecht ( | )
    count = 1;
    // nach oben zählen
    for (int i = r - 1; i >= 0 && b[i][c] == p; i--) count++;
    // nach unten zählen
    for (int i = r + 1; i < R && b[i][c] == p; i++) count++;
    if (count >= 4) return true;

    // schräg ( \ )
    count = 1;
    // nach oben links zählen
    for (int i = r - 1, j = c - 1; i >= 0 && j >= 0 && b[i][j] == p; i--, j--) count++;
    // nach unten rechts zählen
    for (int i = r + 1, j = c + 1; i < R && j < C && b[i][j] == p; i++, j++) count++;
    if (count >= 4) return true;

    // schräg ( / )
    count = 1;
    // nach oben rechts zählen
    for (int i = r - 1, j = c + 1; i >= 0 && j < C && b[i][j] == p; i--, j++) count++;
    // nach unten links zählen
    for (int i = r + 1, j = c - 1; i < R && j >= 0 && b[i][j] == p; i++, j--) count++;
    if (count >= 4) return true;

    return false;                            // nirgends 4: noch kein Sieg
}

// Testet: Würde p mit einem Stein in Spalte col gewinnen?
// Der Stein wird nur zum Testen hingelegt und danach wieder weggenommen.
bool trymove(int col, char p) {
    for (int i = R - 1; i >= 0; i--) if (b[i][col] == EMPTY) {   // unterstes leeres Feld suchen
        b[i][col] = p;                       // Stein zum Testen hinlegen
        bool ok = win(i, col, p);            // gewinnt er damit?
        b[i][col] = EMPTY;                   // Stein wieder wegnehmen
        return ok;
    }
    return false;                            // Spalte ist voll
}

// Legt einen Stein in eine Spalte.
// Gibt die Reihe zurück, wo der Stein liegt.
// Gibt -1 zurück, wenn es nicht geht.
int drop(int col, char p) {
    // Spalte gibt es nicht oder Spalte ist voll: geht nicht
    if (col < 0 || col >= C || b[0][col] != EMPTY) return -1;

    // Von unten nach oben suchen
    for (int i = R - 1; i >= 0; i--) {
        // Ist das Feld leer?
        if (b[i][col] == EMPTY) {
            b[i][col] = p;                   // Stein hinlegen
            return i;                        // Reihe zurückgeben
        }
    }

    return -1;                               // passiert nie, muss aber da sein
}

// Prüft, ob das Spielfeld voll ist (unentschieden).
// Es reicht, die oberste Reihe anzusehen. Sie wird als letzte voll.
bool draw() {
    for (int j = 0; j < C; j++)
        if (b[0][j] == EMPTY) return false;  // noch ein Platz frei
    return true;                             // alles voll
}
