# Connect Four (C++)

[Deutsche Version](README.md)

Console game "Vier gewinnt" (Connect Four) for 2 to 4 players in C++. Project work Programmiertechnik 2026/2027 (assignment sheet 51, Dr. Michael Litera).

## Project goal

A playable console program: first for two players, then extended to up to four players. It includes a main menu, saving and loading a game, a highscore, the documentation on GitHub, a presentation and a handout.

## Compile and run

**Visual Studio 2022 or newer:** open `PPProjekt.sln` and press **Ctrl+F5**.

**Without Visual Studio**, for example with MinGW:

```
g++ -std=c++17 -o vier-gewinnt PPProjekt.cpp
```

## Usage

| Menu item | Function | State |
|---|---|---|
| 1 | Start a new game: enter the number of players (2 to 4) and the names, then play | done |
| 2 | Set the number of players | not implemented yet |
| 3 | Enter the player names | not implemented yet |
| 4 | Save the game | not implemented yet |
| 5 | Load a game | not implemented yet |
| 6 | Show the highscore | not implemented yet |
| 7 | Quit the program | done |

In the game, the player whose turn it is enters the number of a column. Invalid input (text, a column outside the board, a full column) is rejected, then the program asks again. The program itself speaks German and English.

| Players | Board (columns × rows) | Symbols |
|---|---|---|
| 2 | 7 × 6 | `X` `O` |
| 3 | 9 × 8 | `X` `O` `$` |
| 4 | 11 × 10 | `X` `O` `$` `&` |

Whoever has four tokens in a row horizontally, vertically or diagonally wins. If the board is full and nobody has won, it is a draw.

## Structure of the code

| Part | Task |
|---|---|
| Class `CPerson` (`Header.h`) | one player: name and highscore |
| `createBoard()` | calculates the board size from the number of players and creates the empty board |
| `printBoard()` | prints the board in colour |
| `drop()` | drops a token into the lowest free cell of a column |
| `win()` | checks from the last token whether four are in a row |
| `draw()` | checks whether the board is full |
| `readNumber()` | reads a number and rejects text; ends the program at the end of input |
| `aimove()`, `trymove()` | computer opponent (not in the menu yet) |

## Known limitations

- Menu items 2 to 6 are still empty. Saving, loading and the highscore will follow.
- The computer opponent is programmed but cannot be reached from the menu yet.
- The board and the game flow are still functions and global variables. Classes such as `Board` and `Game` are planned.
- Player names must not contain spaces.
- The colours need a console with ANSI colours (for example Windows Terminal). In an old console, characters like `←[1;31m` appear.
