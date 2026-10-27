// Dein Code mit Korrekturen. Jede Änderung ist mit "KORREKTUR n" markiert,
// das größere Spielfeld für 3 und 4 Spieler mit "DYNAMISCH n".
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <limits>   // KORREKTUR 1: wird für numeric_limits gebraucht
#include <iomanip>  // DYNAMISCH 1: für setw (Spaltennummern 10 und 11 sauber ausrichten)
#include "Header.h"
using namespace std;
const string RED = "\033[1;31m";
const string YELLOW = "\033[1;33m";
const string BLUE = "\033[1;34m";
const string RESET = "\033[0m";
// DYNAMISCH 2: Größe nicht mehr fest. R = Reihen, C = Spalten.
// Sie werden in createBoard() aus der Spielerzahl berechnet.
int R = 6, C = 7;


void CPerson::setname(string N) {
    this->name = N;
}
string CPerson::getname() {
    return this->name;
}
void CPerson::sethighscore(int H) {
    this->highscore = H;
}
int CPerson::gethighscore() {
    return this->highscore;
}
const char EMPTY = '.', P1 = 'X', P2 = 'O', P3 = '$', P4 = '&';
// DYNAMISCH 3: Ein normales Array (char b[6][7]) braucht eine feste Größe beim Übersetzen.
// Ein vector kann seine Größe beim Programmlauf ändern: ein vector von Reihen,
// jede Reihe ist ein vector von Feldern. Zugriff bleibt gleich: b[reihe][spalte].
vector<vector<char>> b;

// DYNAMISCH 4: Die Feldgröße wird an EINER Stelle berechnet (so verlangt es das Aufgabenblatt):
// 2 Spieler: 7 x 6, 3 Spieler: 9 x 8, 4 Spieler: 11 x 10
void createBoard(int spielerzahl) {
    R = 6 + 2 * (spielerzahl - 2);
    C = 7 + 2 * (spielerzahl - 2);
    b.assign(R, vector<char>(C, EMPTY));   // R Reihen mit je C leeren Feldern
}
void printBoard();
int drop(int col, char p);
bool win(int r, int c, char p);
bool draw();
bool trymove(int col, char p);
int aimove();

// KORREKTUR 2: liest eine Zahl; bei Text erneut fragen.
// Rückgabe false, wenn keine Eingabe mehr kommt (sonst würde das Programm endlos fragen).
bool readNumber(int& value) {
    while (!(cin >> value)) {
        if (cin.eof()) return false;                                // keine Eingabe mehr
        cin.clear();                                                // Fehlerstatus zurücksetzen
        cin.ignore(numeric_limits<streamsize>::max(), '\n');        // Rest der Zeile verwerfen
        cout << "Ungueltige Eingabe! Bitte nur ganze Zahlen eingeben." << endl;
    }
    return true;
}

int main() {
    int X = 0;
    // KORREKTUR 3: Das Menü läuft in einer Schleife, bis 7 (Beenden) gewählt wird
    while (X != 7) {
        // KORREKTUR 4: Das Menü zeigt jetzt die Punkte, die es im switch wirklich gibt
        cout << "\nConnect Four\n"
            << "1) Neues Spiel starten\n"
            << "2) Anzahl der Spieler festlegen\n"
            << "3) Spielernamen eingeben\n"
            << "4) Spiel speichern\n"
            << "5) Spiel laden\n"
            << "6) Highscore anzeigen\n"
            << "7) Programm beenden\n"
            << "Choice: ";
        if (!readNumber(X)) return 0;                               // Eingabeende

        switch (X)
        {
        case 1:
        {
            // KORREKTUR 6: Spielerzahl nur von 2 bis 4 annehmen (vorher ging auch -1)
            int spielerzahl = 0;
            while (spielerzahl < 2 || spielerzahl > 4) {
                cout << "Wie viele Spieler spielen mit (2 bis 4)?" << endl;
                if (!readNumber(spielerzahl)) return 0;
            }

            // DYNAMISCH 5: Spielfeld in der passenden Größe anlegen und leeren
            // (ersetzt KORREKTUR 5)
            createBoard(spielerzahl);

            // KORREKTUR 7: kein "new" mehr (vorher Tippfehler zieger/zeiger und ein Speicherleck)
            vector<CPerson> p1;
            string name;
            for (int i = 0; i < spielerzahl; i++)                   // KORREKTUR 8: int statt size_t
            {
                cout << "Spieler " << i + 1 << ", Name: " << endl;
                cin >> name;
                // KORREKTUR 14: deine Header.h hat keinen Konstruktor CPerson(string).
                // Darum zuerst ein leeres Objekt anlegen und dann setname aufrufen.
                CPerson spieler;
                spieler.setname(name);
                spieler.sethighscore(0);
                p1.push_back(spieler);
            }

            // KORREKTUR 9: alle Spieler kommen der Reihe nach dran (vorher nur X und O)
            const char symbols[4] = { P1, P2, P3, P4 };
            int current = 0;
            while (true) {
                char p = symbols[current];
                printBoard();
                string player_color = (p == P1) ? RED : (p == P2) ? YELLOW : BLUE;
                // KORREKTUR 10: den Namen anzeigen statt "Player 1"
                cout << p1[current].getname();
                cout << " [" << player_color << p << RESET;
                cout << "] choose column (1-" << C << "): ";   // DYNAMISCH 6: statt fest 7
                int c = 0;
                if (!readNumber(c)) return 0;
                c--;
                int r = drop(c, p);
                if (r == -1) { cout << "Invalid move, try again.\n"; continue; }
                if (win(r, c, p)) {
                    printBoard();
                    // KORREKTUR 11: Der Sieger ist der Spieler am Zug. Vorher stand
                    // "Computer", wenn Spieler 2 in Spalte 3 gewonnen hat (c == 2).
                    cout << player_color << p1[current].getname() << " WINS!\n" << RESET;
                    break;
                }
                if (draw()) {
                    printBoard();
                    cout << "It's a DRAW!\n";
                    break;
                }
                current = (current + 1) % spielerzahl;
            }
            break;                                                  // KORREKTUR 12: break fehlte
        }
        case 2: { // Hier wird die Anzahl der Spieler festgelegt.
            break;
        }
        case 3: { // Hier werden die Namen der Spieler eingegeben und geändert.
            break;
        }
        case 4: { // Hier wird das Spiel gespeichert.
            break;
        }
        case 5: { // Hier wird ein gespeichertes Spiel geladen.
            break;
        }
        case 6: { // Hier wird der Highscore (die Siege der Spieler) angezeigt.
            break;
        }
        case 7: { // Hier wird das Programm beendet.
            break;
        }
        default: { // KORREKTUR 13: Zahl außerhalb 1 bis 7
            cout << "Ungueltige Eingabe! Bitte eine Zahl von 1 bis 7 eingeben." << endl;
            break;
        }
        }
    }
    return 0;
}


// DYNAMISCH 7: jede Spalte ist jetzt 3 Zeichen breit, damit auch "10" und "11" passen
void printBoard() {
    cout << "\n";
    for (int i = 0; i < R; i++) {
        cout << BLUE << "|" << RESET;
        for (int j = 0; j < C; j++) {
            if (b[i][j] == P1) {
                cout << " " << RED << b[i][j] << RESET << " ";
            }
            else if (b[i][j] == P2) {
                cout << " " << YELLOW << b[i][j] << RESET << " ";
            }
            else if (b[i][j] == P3 || b[i][j] == P4) {
                cout << " " << BLUE << b[i][j] << RESET << " ";
            }
            else {
                cout << " " << b[i][j] << " ";
            }
        }
        cout << BLUE << "|\n" << RESET;
    }
    cout << BLUE << "+";
    for (int j = 0; j < C * 3; j++) cout << "-";
    cout << "+\n" << RESET << " ";
    for (int j = 0; j < C; j++) cout << setw(2) << j + 1 << " ";
    cout << "\n\n";
}
// Computergegner: wird im Moment nirgends aufgerufen (siehe Erklärung im Chat)
int aimove() {
    for (int c = 0; c < C; c++) {
        if (b[0][c] == EMPTY && trymove(c, P2)) return c;
    }
    for (int c = 0; c < C; c++) {
        if (b[0][c] == EMPTY && trymove(c, P1)) return c;
    }
    // DYNAMISCH 8: statt der festen Liste {3,2,4,1,5,0,6}: von der Mitte nach außen
    for (int d = 0; d <= C / 2; d++) {
        if (C / 2 - d >= 0 && b[0][C / 2 - d] == EMPTY) return C / 2 - d;
        if (C / 2 + d < C && b[0][C / 2 + d] == EMPTY) return C / 2 + d;
    }
    return 0;
}
bool win(int r, int c, char p) {
    int count;
    count = 1;
    for (int j = c - 1; j >= 0 && b[r][j] == p; j--) {
        count++;
    }
    for (int j = c + 1; j < C && b[r][j] == p; j++) count++;
    if (count >= 4) return true;
    count = 1;
    for (int i = r - 1; i >= 0 && b[i][c] == p; i--) count++;
    for (int i = r + 1; i < R && b[i][c] == p; i++) count++;
    if (count >= 4) return true;
    count = 1;
    for (int i = r - 1, j = c - 1; i >= 0 && j >= 0 && b[i][j] == p; i--, j--) count++;
    for (int i = r + 1, j = c + 1; i < R && j < C && b[i][j] == p; i++, j++) count++;
    if (count >= 4) return true;
    count = 1;
    for (int i = r - 1, j = c + 1; i >= 0 && j < C && b[i][j] == p; i--, j++) count++;
    for (int i = r + 1, j = c - 1; i < R && j >= 0 && b[i][j] == p; i++, j--) count++;
    if (count >= 4) return true;
    return false;
}

bool trymove(int col, char p) {
    for (int i = R - 1; i >= 0; i--) if (b[i][col] == EMPTY) {
        b[i][col] = p;
        bool ok = win(i, col, p);
        b[i][col] = EMPTY;
        return ok;
    }
    return false;
}

int drop(int col, char p) {
    if (col < 0 || col >= C || b[0][col] != EMPTY) return -1;
    for (int i = R - 1; i >= 0; i--) if (b[i][col] == EMPTY) { b[i][col] = p; return i; }
    return -1;
}

bool draw() {
    for (int j = 0; j < C; j++)
        if (b[0][j] == EMPTY) return false;
    return true;
}
