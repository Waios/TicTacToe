#include<bits/stdc++.h>

using namespace std;

#define COMPUTER 1
#define HUMAN 2

// Computer will move with 'O'
// and human with 'X'
#define COMPUTERMOVE 'O'
#define HUMANMOVE 'X'

int nextTurn;
int lastTurn;

// Zeigt alle Felder an und wie sie belegt sind
void showBoard(char board[])
{
    printf("\n\n");

    printf("\t\t\t  %c | %c  | %c  \n",
           board[0], board[1], board[2]);
    printf("\t\t\t--------------\n");
    printf("\t\t\t  %c | %c  | %c  \n",
           board[3], board[4], board[5]);
    printf("\t\t\t--------------\n");
    printf("\t\t\t  %c | %c  | %c  \n\n",
           board[6], board[7], board[8]);

    return;
}

// Zeigt alle Felder des Boards mit der jeweiligen Nummer des Feldes an
void showInstructions()
{
    printf("\t\t\t  Tic-Tac-Toe\n\n");
    printf("Wähle eine Nummer des Feldes von 0 bis 8 aus und spiele\n\n");

    printf("\t\t\t  0 | 1  | 2  \n");
    printf("\t\t\t--------------\n");
    printf("\t\t\t  3 | 4  | 5  \n");
    printf("\t\t\t--------------\n");
    printf("\t\t\t  6 | 7  | 8  \n\n");

    printf("-\t-\t-\t-\t-\t-\t-\t-\t-\t-\n\n");

    return;
}


// Hier wird das Spieldfeld inizialisiert
void initialise(char board[])
{
    // Den Zufallsgenerator inizialisieren damit er nicht immer die selben Zahlen generiert
    srand(time(NULL));

    // Für den Anfang eines leeres Feld inizialisieren
    for (int i=0; i<9; i++) {
            board[i] = ' ';
    }

    return;
}

// Ausgabe des Gewinners
void declareWinner(int whoseTurn)
{
    if (whoseTurn == COMPUTER)
        printf("COMPUTER has won\n");
    else
        printf("HUMAN has won\n");
    return;
}

// Prüfen ob eine Linie mit drei gleichen nicht leeren Zeichen ausgefüllt ist
bool rowCrossed(char board[]) {


        if (board[0] == board[1] && board[1] == board[2] && board[2] != ' ')
            return(true);
        if (board[3] == board[4] && board[4] == board[5] && board[5] != ' ')
            return(true);
        if (board[6] == board[7] && board[7] == board[8] && board[8] != ' ')
            return (true);

    return(false);
}

// Prüfen ob eine Spalte mit drei gleichen nicht leeren Zeichen ausgefüllt ist
bool columnCrossed(char board[]) {


        if (board[0] == board[3] && board[3] == board[6] && board[6] != ' ') {
            return(true);
        }
        if (board[1] == board[4] && board[4] == board[7] && board[7] != ' ') {
            return(true);
        }
        if (board[2] == board[5] && board[5] == board[8] && board[8] != ' ') {
            return (true);
        }

    return(false);
}

// Prüfen ob eine Diagonale mit drei gleichen nicht leeren Zeichen ausgefüllt ist
bool diagonalCrossed(char board[])
{
    if (board[0] == board[4] && board[4] == board[8] && board[8] != ' ') {
        return (true);
    }

    if (board[2] == board[4] && board[4] == board[6] && board[6] != ' ') {
        return (true);
    }

    return(false);
}

// Die Funktion gibt true zurück fals eine der drei aufgerufenen Prüfungen erflogreich ist ansonsten false
bool gameOver(char board[])
{
    return(rowCrossed(board) || columnCrossed(board) || diagonalCrossed(board) );
}

// Funktion zum spielen von Tic-Tac-Toe
void playTicTacToe(int nextTurn)
{
    // Ein simples Tic-Tac-Toe Feld zum spielen von 0 bis 8
    char board[9];

    int moves = 0;

    // initialisieren  des Spiels
    initialise(board);

    // Zeigt die Anleitung bevor das Spiel beginnt
    showInstructions();

    int moveIndex = 0, x, y;
    int zahleingabe = 0;

    // Spiele bis jemand gewonnen oder das Spiel unentschieden ausgeht
    while (gameOver(board) == false && moves != 9) {

        if (nextTurn == COMPUTER) {
            bool gueltig;
            do {

                int zahl = rand() % (8 + 1 - 0) + 0;
                if(board[zahl] == ' ') {
                    board[zahl] = COMPUTERMOVE;
                    gueltig = true;
                }else{
                    gueltig = false;
                }

            }while(!gueltig);
                printf("COMPUTER has put a %c in cell %d\n",
                       COMPUTERMOVE, moves++ );
                showBoard(board);
                lastTurn = COMPUTER;
                nextTurn = HUMAN;
            }

        else if (nextTurn == HUMAN)
        {
            bool gueltig ;
            do {

                printf("Bitte gib dein Zug ein.");
                cin >> zahleingabe;

                if (board[zahleingabe] == ' ') {
                    board[zahleingabe] = HUMANMOVE;
                    gueltig = true;
                } else {
                    printf("Falsches Feld! Neue Eingabe");
                    gueltig = false;
                }
            }while(!gueltig);

            printf ("HUMAN has put a %c in cell %d\n",
                    HUMANMOVE, moves++);
            showBoard(board);
            lastTurn = HUMAN;
            nextTurn = COMPUTER;
        }
    }

    // Wenn das Spiel unentschieden ausgeht
    if (gameOver(board) == false && moves == 9)
        printf("It's a draw\n");
    else
    {   // Den Gewinner ernennen

        if (lastTurn == COMPUTER) {
            declareWinner(COMPUTER);
        }
        else if (lastTurn == HUMAN) {
            declareWinner(HUMAN);
        }else {

            printf("FATAL ERROR!");
        }
        printf("Glückwunsch!");
    }
    return;
}

// Hier gehts los
int main()
{
    // Lass und das Spiel mit dem Computer als Beginner beginnen
    playTicTacToe(COMPUTER);

    return (0);
}