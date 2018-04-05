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
bool computerwin = false;
int draw = 0;


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

int moveRandom(char board[]) {
   bool gueltig = false;
   int zahl;
    do {
        zahl = rand() % (8 + 1 - 0) + 0;
        if (board[zahl] == '_') {
            gueltig = true;
        } else {
            gueltig = false;
        }
    }while (!gueltig);
    return zahl;
}


// Hier wird das Spieldfeld inizialisiert
void initialise(char board[])
{
    // Den Zufallsgenerator inizialisieren damit er nicht immer die selben Zahlen generiert
    srand(time(NULL));

    // Für den Anfang eines leeres Feld inizialisieren
    for (int i=0; i<9; i++) {
            board[i] = '_';
    }

    return;
}

// Ausgabe des Gewinners
void declareWinner(int whoseTurn)
{
    if (whoseTurn == COMPUTER) {
        printf("COMPUTER has won\n");
        computerwin = true;
    }
    else {
        printf("HUMAN has won\n");
computerwin = false;
    }
        return;
}

// Prüfen ob eine Linie mit drei gleichen nicht leeren Zeichen ausgefüllt ist
bool rowCrossed(char board[],char player) {


        if (board[0] == player && board[0] == board[1] && board[1] == board[2])
            return(true);
        if (board[3] == player && board[3] == board[4] && board[4] == board[5])
            return(true);
        if (board[6] == player && board[6] == board[7] && board[7] == board[8])
            return (true);

    return(false);
}

// Prüfen ob eine Spalte mit drei gleichen nicht leeren Zeichen ausgefüllt ist
bool columnCrossed(char board[],char player) {


        if (board[0] == player && board[0] == board[3] && board[3] == board[6]) {
            return(true);
        }
        if (board[1] == player && board[1] == board[4] && board[4] == board[7]) {
            return(true);
        }
        if (board[2] == player && board[2] == board[5] && board[5] == board[8]) {
            return (true);
        }

    return(false);
}

// Prüfen ob eine Diagonale mit drei gleichen nicht leeren Zeichen ausgefüllt ist
bool diagonalCrossed(char board[],char player)
{
    if (board[0] == player && board[0] == board[4] && board[4] == board[8]) {
        return (true);
    }

    if (board[2] == player && board[2] == board[4] && board[4] == board[6]) {
        return (true);
    }

    return(false);
}

// Die Funktion gibt true zurück fals eine der drei aufgerufenen Prüfungen erflogreich ist ansonsten false

bool winning(char board[], char player)
{
    return(rowCrossed(board,player) || columnCrossed(board,player) || diagonalCrossed(board,player) );
}

bool shouldDoRandomMove(){
    int randz = rand() % (10 + 1 - 0) + 0;
    if (randz < draw){
        return true;
    }else {
        return false;
    }
}



bool isMovesLeft(char board[])
{
    for (int i = 0; i<9; i++){
        if (board[i]=='_'){
            return true;
        }
    }
    return false;
}
// This is the minimax function. It considers all
// the possible ways the game can go and returns
// the value of the board
int minimax(char board[9], int depth, bool isMax) {
    if (winning(board, HUMANMOVE)){
        printf("minmax %d  , ",-10);
        return -10;
    }
    else if (winning(board, COMPUTERMOVE)){

        printf("minmax %d , ",10);
        return 10;
    }
    else if (!isMovesLeft(board)){
        printf("minmax %d , ",0);
        return 0;
    }

    // If this maximizer's move
    if (isMax) {
        int best = -1000;

        // Traverse all cells
        for (int i = 0; i<9; i++) {
                // Check if cell is empty
                if (board[i]=='_') {
                    // Make the move
                    board[i] = COMPUTERMOVE;
                    //showBoard(board);
                    // Call minimax recursively and choose
                    // the maximum value
                    best = max( best, minimax(board, depth+1, !isMax) );

                    // Undo the move
                    board[i] = '_';
                }

        }
        return best;
    }
        // If this minimizer's move
    else
    {
        int best = 1000;

        // Traverse all cells
        for (int k = 0; k<9; k++)
        {

                // Check if cell is empty
                if (board[k]=='_')
                {
                    // Make the move
                    board[k] = HUMANMOVE;
                    //showBoard(board);
                    // Call minimax recursively and choose
                    // the minimum value
                    best = min(best, minimax(board, depth+1, !isMax));

                    // Undo the move
                    board[k] = '_';
                }

        }
        return best;
    }
}

// This will return the best possible move for the player
int findBestMove(char board[9]) {

    int bestVal = -1000;
    int bestMove =-1;

    // Traverse all cells, evalutae minimax function for
    // all empty cells. And return the cell with optimal
    // value.

    for (int i = 0; i < 9 && bestVal < 10; i++) {
        // Check if cell is empty
        if (board[i] == '_') {
            // Make the move
            board[i] = COMPUTERMOVE;
           // showBoard(board);
            // compute evaluation function for this move.
           int moveVal = minimax(board, 0, false);
           // printf("\n ++++ move Val= %d \n",moveVal);



            // If the value of the current move is
            // more than the best value, then update
            // best/
            if (moveVal > bestVal) {
                bestMove = i; //aktuelle Array Position
                bestVal = moveVal;
//                printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//                printf("+++++++++++++++best var = %d, best move %d \n",moveVal, bestMove);
//                printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//                showBoard(board);
            }
            // Undo the move
            board[i] = '_';
        }

    }

    printf("The value of the best Move is : %d", bestMove);

    return bestMove;

}

// Funktion zum spielen von Tic-Tac-Toe
void playTicTacToe(int nextTurn) {
    // Ein simples Tic-Tac-Toe Feld zum spielen von 0 bis 8
    char board[9];

    int moves = 0;

    // initialisieren  des Spiels
    initialise(board);

    // Zeigt die Anleitung bevor das Spiel beginnt
    showInstructions();

    if (nextTurn == COMPUTER){
        board[0]= COMPUTERMOVE;
        nextTurn = HUMAN;
        showBoard(board);
        moves++;
    }

    int zahleingabe = 0;

    // Spiele bis jemand gewonnen oder das Spiel unentschieden ausgeht
    while (!(winning(board,COMPUTERMOVE)|| winning(board,HUMANMOVE))  && moves != 9) {

        if (nextTurn == COMPUTER) {
            bool gueltig;
            int zahl;

                // simple erste Implementierung mit Zufallszahl
                if (computerwin || shouldDoRandomMove()){
                    printf("moveRandom");
                    zahl = moveRandom(board);
                }else {
                    printf("findBestMove");
                    zahl = findBestMove(board);
                }

                 board[zahl] = COMPUTERMOVE;
            printf("COMPUTER has put a %c in cell %d\n",
                   COMPUTERMOVE, zahl );
            moves++;
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

                if (board[zahleingabe] == '_') {
                    board[zahleingabe] = HUMANMOVE;
                    gueltig = true;
                } else {
                    printf("Falsches Feld! Neue Eingabe");
                    gueltig = false;
                }
            }while(!gueltig);

            printf ("HUMAN has put a %c in cell %d\n", HUMANMOVE, zahleingabe);
            moves++;
            showBoard(board);
            lastTurn = HUMAN;
            nextTurn = COMPUTER;
        }
    }

    // Wenn das Spiel unentschieden ausgeht
    if (!(winning(board,COMPUTERMOVE)|| winning(board,HUMANMOVE)) && moves == 9) {
        draw++;
        printf("It's a draw\n");
    }else
    {   // Den Gewinner ernennen
        draw = 0;
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
    // Lass das Spiel mit dem Computer
    // beginnen
    for(int x = 0; x < 5; x++){
        playTicTacToe(COMPUTER);
    }


    return (0);
}