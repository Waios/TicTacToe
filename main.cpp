#include<bits/stdc++.h>

using namespace std;

#define COMPUTER 1
#define HUMAN 2

// der Computer spielt mit 'O'
// der Mensch spielt mit 'X'
#define COMPUTERMOVE 'O'
#define HUMANMOVE 'X'

int nextTurn;
int lastTurn;
bool computerWin = false;
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
        computerWin = true;
    }
    else {
        printf("HUMAN has won\n");
computerWin = false;
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
// das ist die minimax funktion. es erwägt alle
// möglichen wege das spiel zu gehen und gibt
// den wert auf das board zurück

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

    // das ist der maximizer zug
    if (isMax) {
        int best = -1000;

        // geht alle felder durch
        for (int i = 0; i<9; i++) {
                // prüft ob ein feld frei ist
                if (board[i]=='_') {
                    // macht den zug
                    board[i] = COMPUTERMOVE;
                    //showBoard(board);
                    // ruft den minimax rekursiv auf und gibt
                    // den größten wert zurück
                    best = max( best, minimax(board, depth+1, !isMax) );

                    // macht den zug rückgängig
                    board[i] = '_';
                }

        }
        return best;
    }
        // das ist der minimizer zug
    else
    {
        int best = 1000;

        // geht alle felder durch
        for (int k = 0; k<9; k++)
        {

                // prüft ob ein feld frei ist
                if (board[k]=='_')
                {
                    // macht den zug
                    board[k] = HUMANMOVE;
                    //showBoard(board);
                    // ruft die minimax funktion rekursiv auf und gibt
                    // den kleinsten wert zurück
                    best = min(best, minimax(board, depth+1, !isMax));

                    // macht den zug rückgängig
                    board[k] = '_';
                }

        }
        return best;
    }
}

// die funktion gibt den bestmöglichen zug für den computer zurück
int findBestMove(char board[9]) {

    int bestVal = -1000;
    int bestMove =-1;

    //geht alle Felder mit der minimax methode durch
    //und gib den besten wert zurück also das beste feld
    //wo der computer sein 'X' hinsetzt
    for (int i = 0; i < 9 && bestVal < 10; i++) {
        // prüft ob ein ferld frei ist
        if (board[i] == '_') {
            // macht den zug
            board[i] = COMPUTERMOVE;
           // showBoard(board);
            // bewertende funktion für diesen zug.
           int moveVal = minimax(board, 0, false);
           // printf("\n ++++ move Val= %d \n",moveVal);



            // wenn der wert vom lauf der zuüge größer ist
            // als der beste wert dann überschreibe diesen
            if (moveVal > bestVal) {
                bestMove = i; //aktuelle Array Position
                bestVal = moveVal;
//                printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//                printf("+++++++++++++++best var = %d, best move %d \n",moveVal, bestMove);
//                printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//                showBoard(board);
            }
            // mache den zug rückgängig
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

                // Implementierung mit Zufallszug und den besten Zug
                if (computerWin || shouldDoRandomMove()){
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