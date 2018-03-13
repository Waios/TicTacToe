// Hardware spezifische Imports und Definitionen
#include <Elegoo_GFX.h>    // Core graphics library
#include <Elegoo_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>

#if defined(__SAM3X8E__)
#undef __FlashStringHelper::F(string_literal)
#define F(string_literal) string_literal
#endif


#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin


//Touch For New ILI9341 TP
#define TS_MINX 120
#define TS_MAXX 900

#define TS_MINY 70
#define TS_MAXY 920

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
// optional
#define LCD_RESET A4

Elegoo_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);


// Farben als lesbare Konstanten  16-bit color values

#define  BLACK   0x0000
// #define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
// #define CYAN    0x07FF
// #define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF



// TicTacToe spezifische Definitionen

bool COMPUTER_TURN = true;
bool HUMAN_TURN = false;

#define COMPUTERMOVE 'O'
#define HUMANMOVE 'X'


/* 2 dimensionles integer Array fuer alle möglichen Positionen der Dreiecke
int triangle[9][6]{
        {220, 80, 170, 50, 170, 110},
        {220, 160, 170, 130, 170, 190},
        {220, 240, 170, 210, 170, 270},
        {150, 80, 100, 50, 100, 110},
        {150, 160, 100, 130, 100, 190},
        {150, 240, 100, 210, 100, 270},
        {70, 80, 20, 50, 20, 110},
        {70, 160, 20, 130, 20, 190},
        {70, 240, 20, 210, 20, 270}
};
 */

// 2 dimensionles integer Array fuer alle möglichen Positionen der Kreise
int circle[9][2]{
        {195, 80},
        {195, 160},
        {195, 240},
        {120, 80},
        {120, 160},
        {120, 240},
        {40,  80},
        {40,  160},
        {40,  240}
};

// 2 dimensionles integer Array fuer alle möglichen Positionen der Kreuze
int cross[9][2]{
        {195, 80},
        {195, 160},
        {195, 240},
        {120, 80},
        {120, 160},
        {120, 240},
        {40,  80},
        {40,  160},
        {40,  240}
};

// Variablen für Begrenzungslinien
int col1 = 120;
int col2 = 200;
int row1 = 82;
int row2 = 162;

// Ein simples Tic-Tac-Toe Feld zum spielen von 0 bis 8
char board[9];
int feldeingabe = 0;
int moves = 0;
int feld = -1; // das gewaehlte Feld
bool gueltig;

void setup(void) {

    tft.reset();

    uint16_t identifier = tft.readID();
    if (identifier == 0x0101) {
        identifier = 0x9341;
        Serial.println(F("Found 0x9341 LCD driver"));
    } else {
        Serial.print(F("Unknown LCD driver chip: "));
        identifier = 0x9341;
    }

    tft.begin(identifier);//technisch bedingt
    tft.setRotation(2);// Bildschirm orientierung

    tft.fillScreen(BLACK);

    prepareField();


    pinMode(13, OUTPUT);  //technisch bedingt 
}

void circleAt(int pos) {
    tft.drawCircle(circle[pos][0], circle[pos][1], 25, YELLOW);
}

void crossAt(int pos) {
    int x = cross[pos][0];
    int y = cross[pos][1];
    int z = 25;
    tft.drawLine(x - z, y - z, x + z, y + z, GREEN);
    tft.drawLine(x + z, y - z, x - z, y + z, GREEN);


}

// Prüfe auf mindestens ein freies Feld
bool isMovesLeft() {
    for (int i = 0; i < 9; i++) {
        if (board[i] == '_') return true;
    }
    return false;
}

/*void triangleAt(int pos){
    tft.drawTriangle(triangle[pos][0], triangle[pos][1], triangle[pos][2], triangle[pos][3], triangle[pos][4], triangle[pos][5], BLUE);
}
*/

void prepareField() {

    // Für den Anfang auf dem Board alle Felder mit Leerstring initialisieren
    for (int i = 0; i < 9; i++) {
        board[i] = '_';
    }
    // Spielfeld aufbauen
    tft.drawLine(20, col1, 220, col1, WHITE);
    tft.drawLine(20, col2, 220, col2, WHITE);
    tft.drawLine(row1, 40, row1, 280, WHITE);
    tft.drawLine(row2, 40, row2, 280, WHITE);


}

// Prüfe ob das komplette Feld leer ist
bool isFieldEmpty() {
    for (int i = 0; i < 9; i++) {
        if (board[i] != '_') return false;
    }
    return true;

}

// Das ist die Minimax Funktion sie ermittelt alle Möglichkeiten für die noch freien Felder auf dem Board
// und liefert einen entsprechenden Wert zurück
int minimax(char board[9], int depth, bool isMax) {
    if (winning(board, HUMANMOVE)) {
        return -10;
    } else if (winning(board, COMPUTERMOVE)) {
        return 10;
    } else if (!isMovesLeft()) {
        return 0;
    }

    // Wenn der Computer das nächste Feld belegt(das ist der Maximizer)
    if (isMax) {
        int best = -1000;

        // Über alle Felder iterieren
        for (int i = 0; i < 9; i++) {
            // Wenn das Feld leer ist
            if (board[i] == '_') {
                // Das Feld belegen
                board[i] = COMPUTERMOVE;

                // Jetzt wird die minimax Funktion rekursiv aufgerufen
                // und durch die Funktion max der größere Wert zurückgeliefert
                best = max(best, minimax(board, depth + 1, !isMax));

                // Feld wieder löschen
                board[i] = '_';
            }

        }
        return best;
    }
        // Wenn der Spieler das nächste Feld belegt(das ist der Minimizer)
    else {
        int best = 1000;

        for (int k = 0; k < 9; k++) {
            if (board[k] == '_') {
                board[k] = HUMANMOVE;

                // Jetzt wird die minimax Funktion rekursiv aufgerufen
                // und durch die Funktion min der kleinere Wert zurückgeliefert
                best = min(best, minimax(board, depth + 1, !isMax));

                board[k] = '_';
            }

        }
        return best;
    }


}

// Finde die bestmögliche Platzierung für den nächsten Zug
int findBestMove(char board[9]) {
    int bestVal = -1000;

    int bestMove = -1;

// Wenn das Feld noch komplett leer ist setzt der Computer immer sein Kreis in das obere linke Feld
    if (isFieldEmpty()) {
        return 0;
    }
    // Alle Felder untersuchen und den mittels minimax Funktion bestimmten Wert zurückliefern
    // es wird der größte gefundene Wert zurückgeliefert
    // wird auch schon beendet wenn eine 10 brechnet wurde
    for (int i = 0; i < 9 && bestVal != 10; i++) {
        // Wenn das Feld leer ist
        if (board[i] == '_') {
            // Das Feld belegen
            board[i] = COMPUTERMOVE;

            // FÜr dieses Feld den Wert ermitteln.
            int moveVal = minimax(board, 0, false);

            // Feld wieder leeren
            board[i] = '_';

            // Wenn der neue Wert größer ist als der bisherige beste Wert
            // den neuen Wert speichern und das aktuelle Feld merken
            if (moveVal > bestVal) {
                bestMove = i; //aktuelle Array Position
                bestVal = moveVal;
            }
        }

    }

    return bestMove;

}

// Prüfen ob eine Reihe mit drei gleichen Zeichen des Spielers ausgefüllt ist
bool rowCrossed(char board[], char player) {


    if (board[0] == player && board[0] == board[1] && board[1] == board[2])
        return (true);
    if (board[3] == player && board[3] == board[4] && board[4] == board[5])
        return (true);
    if (board[6] == player && board[6] == board[7] && board[7] == board[8])
        return (true);

    return (false);
}

// Prüfen ob eine Spalte mit drei gleichen Zeichen des Spielers ausgefüllt ist
bool columnCrossed(char board[], char player) {


    if (board[0] == player && board[0] == board[3] && board[3] == board[6]) {
        return (true);
    }
    if (board[1] == player && board[1] == board[4] && board[4] == board[7]) {
        return (true);
    }
    if (board[2] == player && board[2] == board[5] && board[5] == board[8]) {
        return (true);
    }

    return (false);
}

// Prüfen ob eine Diagonale mit drei gleichen Zeichen des Spielers ausgefüllt ist
bool diagonalCrossed(char board[], char player) {
    if (board[0] == player && board[0] == board[4] && board[4] == board[8]) {
        return (true);
    }

    if (board[2] == player && board[2] == board[4] && board[4] == board[6]) {
        return (true);
    }

    return (false);
}

// Die Funktion gibt true zurück falls eine der drei aufgerufenen Prüfungen erflogreich ist ansonsten false

bool winning(char board[], char player) {
    return (rowCrossed(board, player) || columnCrossed(board, player) || diagonalCrossed(board, player));
}

#define MINPRESSURE 10
#define MAXPRESSURE 1000

void loop() {

    // Spiele bis jemand gewonnen oder das Spiel unentschieden ausgeht
    while (!(winning(board, COMPUTERMOVE) || winning(board, HUMANMOVE)) && moves != 9) {

        if (COMPUTER_TURN) {


            do {
                feld = findBestMove(board);  //das ist die komplexe Lösung mit dem rekursiven Algorythmus
                //feld =(int)random(0, 9); // das ist die simple Lösung mit dem Zufallsprinzip
                if(board[feld] == '_') {
                    board[feld] = COMPUTERMOVE;
                    gueltig = true;
                }else{
                    gueltig = false;
                }
            }while(!gueltig);

/*
            feld = findBestMove(board);
            board[feld] = COMPUTERMOVE;
            gueltig = true;
               */
            circleAt(feld);
            COMPUTER_TURN = false;
            HUMAN_TURN = true;

        } else if (HUMAN_TURN) {
            gueltig = false;
            do {
                digitalWrite(13, HIGH); // Display spezifisch
                TSPoint p = ts.getPoint(); // Touchscreen getPoint
                digitalWrite(13, LOW);
                // if sharing pins, you'll need to fix the directions of the touchscreen pins
                pinMode(XM, OUTPUT);
                pinMode(YP, OUTPUT);

                // Falls auf das Display gedrückt wurde
                // die Position des Druckpunktes bestimmen
                if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {

                    // scale from 0->1023 to tft.width
                    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
                    p.y = (tft.height() - map(p.y, TS_MINY, TS_MAXY, tft.height(), 0));
                    // Ermittlung des Punktes anhand der Feldgrenzen
                    if (p.x < row1 && p.y < col1) {
                        feld = 6;
                    } else if (p.x < row2 && p.y < col1) {
                        feld = 3;
                    } else if (p.y < col1) {
                        feld = 0;
                    } else if (p.x < row1 && p.y < col2) {
                        feld = 7;
                    } else if (p.x < row2 && p.y < col2) {
                        feld = 4;
                    } else if (p.y < col2) {
                        feld = 1;
                    } else if (p.x < row1) {
                        feld = 8;
                    } else if (p.x < row2) {
                        feld = 5;
                    } else {
                        feld = 2;
                    }
                    // Falls das Feld wirklich leer ist auf das gedrückt wurde
                    if (board[feld] == '_') {
                        board[feld] = HUMANMOVE; // Feld belegen
                        crossAt(feld);
                        gueltig = true;
                    } else {
                        gueltig = false;
                    }
                }
            } while (!gueltig);

            HUMAN_TURN = false;
            COMPUTER_TURN = true;
        }
        moves++;// nach jedem Zug um eins erhöhen
    }

    // Wenn das Spiel unentschieden ausgeht wird eine Meldung ausgegeben
    if (!(winning(board, COMPUTERMOVE) || winning(board, HUMANMOVE)) && moves == 9) {
        tft.setRotation(tft.getRotation() + 1);
        tft.setCursor(0, 2);
        tft.setTextColor(RED);
        tft.setTextSize(2);
        tft.println("It's a draw");
    } else {   // Den Gewinner ernennen
        tft.setRotation(tft.getRotation() + 1);
        tft.setCursor(0, 2);
        tft.setTextColor(RED);
        tft.setTextSize(2);
        if (COMPUTER_TURN) { //in diesem Fall hat der Mensch den letzten Zug gemacht
            tft.print("You win! Gl");
            tft.print(char(0x81));
            tft.println("ckwunsch!");
        } else if (HUMAN_TURN) { // in diesem Fall hat der Computer den letzten Zug gemacht
            tft.println("Computer wins!");
        } else {
            tft.println("FATAL ERROR!");
        }

    }
    delay(5000); //5 Sekunden Pause
    // Spielfeld aufräumen
    tft.setRotation(2);
    tft.fillScreen(BLACK);
    prepareField();
    moves = 0;
}


