#include<string>
#include<iostream>
#include<vector>
using namespace std;

// Schutz: Die Datei wird nur einmal eingebunden, auch wenn sie mehrmals mit #include kommt.
#ifndef Header_h
#define Header_h

// Ein Spieler mit Namen und Punkten
class CPerson {
private:                       // nur die Klasse selbst darf das ändern
    string name ;              // Name des Spielers
    int highscore;             // Punkte (Siege) des Spielers
public:                        // das darf jeder benutzen
    void setname(string N);    // Namen speichern
    string getname();          // Namen zurückgeben
    void sethighscore(int H);  // Punkte speichern
    int gethighscore();        // Punkte zurückgeben
};
#endif // !Header_h

