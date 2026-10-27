# Vier gewinnt (C++)

[English version](README.en.md)

Konsolenspiel „Vier gewinnt“ für 2 bis 4 Spieler in C++. Projektarbeit Programmiertechnik 2026/2027 (Aufgabenblatt 51, Dr. Michael Litera).

## Projektziel

Ein spielbares Konsolenprogramm: zuerst für zwei Spieler, dann erweitert auf bis zu vier Spieler. Dazu gehören ein Hauptmenü, Speichern und Laden eines Spielstands, ein Highscore, die Dokumentation auf GitHub, eine Präsentation und ein Handout.

## Kompilieren und starten

**Visual Studio 2022 oder neuer:** `PPProjekt.sln` öffnen und **Strg+F5** drücken.

**Ohne Visual Studio**, zum Beispiel mit MinGW:

```
g++ -std=c++17 -o vier-gewinnt PPProjekt.cpp
```

## Bedienung

| Menüpunkt | Funktion | Stand |
|---|---|---|
| 1 | Neues Spiel starten: Spieleranzahl (2 bis 4) und Namen eingeben, dann spielen | fertig |
| 2 | Anzahl der Spieler festlegen | noch nicht umgesetzt |
| 3 | Spielernamen eingeben | noch nicht umgesetzt |
| 4 | Spiel speichern | noch nicht umgesetzt |
| 5 | Spiel laden | noch nicht umgesetzt |
| 6 | Highscore anzeigen | noch nicht umgesetzt |
| 7 | Programm beenden | fertig |

Im Spiel gibt der Spieler am Zug die Nummer einer Spalte ein. Ungültige Eingaben (Text, Spalte außerhalb des Feldes, volle Spalte) werden abgelehnt, dann fragt das Programm erneut.

| Spieler | Spielfeld (Spalten × Reihen) | Symbole |
|---|---|---|
| 2 | 7 × 6 | `X` `O` |
| 3 | 9 × 8 | `X` `O` `$` |
| 4 | 11 × 10 | `X` `O` `$` `&` |

Gewonnen hat, wer vier Steine waagerecht, senkrecht oder diagonal in einer Reihe hat. Ist das Feld voll und niemand hat gewonnen, ist es unentschieden.

## Aufbau des Codes

| Teil | Aufgabe |
|---|---|
| Klasse `CPerson` (`Header.h`) | ein Spieler: Name und Highscore |
| `createBoard()` | berechnet die Feldgröße aus der Spieleranzahl und legt das leere Spielfeld an |
| `printBoard()` | gibt das Spielfeld farbig aus |
| `drop()` | lässt einen Stein in die unterste freie Zelle einer Spalte fallen |
| `win()` | prüft vom letzten Stein aus, ob vier in einer Reihe liegen |
| `draw()` | prüft, ob das Feld voll ist |
| `readNumber()` | liest eine Zahl und lehnt Text ab; beendet das Programm am Eingabeende |
| `aimove()`, `trymove()` | Computergegner (noch nicht im Menü) |

## Bekannte Einschränkungen

- Die Menüpunkte 2 bis 6 sind noch leer. Speichern, Laden und Highscore folgen.
- Der Computergegner ist programmiert, aber noch nicht im Menü erreichbar.
- Spielfeld und Spielablauf sind noch Funktionen und globale Variablen. Klassen wie `Board` und `Game` sind geplant.
- Spielernamen dürfen keine Leerzeichen enthalten.
- Die Farben brauchen eine Konsole mit ANSI-Farben (zum Beispiel Windows Terminal). In einer alten Konsole erscheinen Zeichen wie `←[1;31m`.
