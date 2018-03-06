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

#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF



// TicTacToe spezifische Definitionen

bool COMPUTER_TURN = true;
bool HUMAN_TURN = false;

#define COMPUTERMOVE 'O'
#define HUMANMOVE 'X'


// 2 dimensionles integer Array fuer alle möglichen Positionen der Dreiecke
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

// 2 dimensionles integer Array fuer alle möglichen Positionen der Kreise
int circle [9][3]{
        {195, 80, 25},
        {195, 160, 25},
        {195, 240, 25},
        {120, 80, 25},
        {120, 160, 25},
        {120, 240, 25},
        {40, 80, 25},
        {40, 160, 25},
        {40, 240, 25}
};

// Variablen für Begrenzungslinien
int col1 = 120;
int col2 = 200;
int row1 = 82;
int row2 = 162;

// Ein simples Tic-Tac-Toe Feld zum spielen von 0 bis 8
char board[9];
int moveIndex = 0, x, y;
int zahleingabe = 0;
int moves = 0;
int zahl; // das gewaehlte Feld
bool gueltig;

void setup(void) {
    Serial.begin(9600);
    Serial.println(F("Paint!"));

    tft.reset();

    uint16_t identifier = tft.readID();
    if(identifier == 0x9325) {
        Serial.println(F("Found ILI9325 LCD driver"));
    } else if(identifier == 0x9328) {
        Serial.println(F("Found ILI9328 LCD driver"));
    } else if(identifier == 0x4535) {
        Serial.println(F("Found LGDP4535 LCD driver"));
    }else if(identifier == 0x7575) {
        Serial.println(F("Found HX8347G LCD driver"));
    } else if(identifier == 0x9341) {
        Serial.println(F("Found ILI9341 LCD driver"));
    } else if(identifier == 0x8357) {
        Serial.println(F("Found HX8357D LCD driver"));
    } else if(identifier==0x0101)
    {
        identifier=0x9341;
        Serial.println(F("Found 0x9341 LCD driver"));
    }else {
        Serial.print(F("Unknown LCD driver chip: "));

        identifier=0x9341;

    }

    // Den Zufallsgenerator inizialisieren damit er nicht immer die selben Zahlen generiert
    randomSeed(analogRead(0));

    tft.begin(identifier);
    tft.setRotation(2);

    tft.fillScreen(BLACK);


    prepareField();

    pinMode(13, OUTPUT);
}

void circleAt(int pos){
    tft.drawCircle(circle[pos][0], circle[pos][1], circle[pos][2], YELLOW);

}

void triangleAt(int pos){
    tft.drawTriangle(triangle[pos][0], triangle[pos][1], triangle[pos][2], triangle[pos][3], triangle[pos][4], triangle[pos][5], BLUE);
}


void prepareField() {

    // Für den Anfang auf dem Board alle Felder mit Leerstring initialisieren
    for (int i=0; i<9; i++) {
        board[i] = ' ';
    }

    // Spielfeld aufbauen
    tft.drawLine(20, col1, 220, col1, WHITE);
    tft.drawLine(20, col2, 220, col2, WHITE);
    tft.drawLine(row1, 40, row1, 280, WHITE);
    tft.drawLine(row2, 40, row2, 280, WHITE);
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

// Die Funktion gibt 'true' zurück falls eine der drei aufgerufenen Prüfungen erflogreich ist ansonsten 'false'
bool gameOver(char board[])
{

    return(rowCrossed(board) || columnCrossed(board) || diagonalCrossed(board) );
}

#define MINPRESSURE 10
#define MAXPRESSURE 1000

void loop() {

    // Spiele bis jemand gewonnen oder das Spiel unentschieden ausgeht
    while (!gameOver(board) && moves != 9) {

        if ( COMPUTER_TURN ) {

            do {
                zahl =(int)random(0, 9);
                if(board[zahl] == ' ') {
                    board[zahl] = COMPUTERMOVE;
                    gueltig = true;
                }else{
                    gueltig = false;
                }
            }while(!gueltig);
            moves++ ;
            circleAt(zahl);
            COMPUTER_TURN = false;
            HUMAN_TURN = true;
        } else if ( HUMAN_TURN){
            gueltig = false;
            do {
                digitalWrite(13, HIGH);
                TSPoint p = ts.getPoint();
                digitalWrite(13, LOW);
                // if sharing pins, you'll need to fix the directions of the touchscreen pins
                pinMode(XM, OUTPUT);
                pinMode(YP, OUTPUT);

                if  ( p.z > MINPRESSURE &&  p.z < MAXPRESSURE) {

                    // scale from 0->1023 to tft.width
                    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
                    p.y = (tft.height()-map(p.y, TS_MINY, TS_MAXY, tft.height(), 0));

                    if(p.x < row1 && p.y < col1){
                        zahl = 6;
                    }else if(p.x < row2 && p.y < col1){
                        zahl =3;
                    }else if(p.y < col1){
                        zahl =0;
                    }else if(p.x < row1 && p.y < col2){
                        zahl =7;
                    }else if(p.x < row2 && p.y < col2){
                        zahl =4;
                    }else if(p.y < col2){
                        zahl =1;
                    }else if(p.x < row1){
                        zahl =8;
                    }else if(p.x < row2){
                        zahl =5;
                    }else
                    {zahl =2;
                    }

                    if (board[zahl] == ' ') {
                        board[zahl] = HUMANMOVE;
                        gueltig = true;
                    } else {
                        gueltig = false;
                    }
                }
            }while(!gueltig);

            moves++;
            triangleAt(zahl);
            HUMAN_TURN = false;
            COMPUTER_TURN = true;
        }
    }

    // Wenn das Spiel unentschieden ausgeht
    if (! gameOver(board) && moves == 9){
        tft.setRotation(tft.getRotation()+1);
        tft.setCursor(0, 30);
        tft.setTextColor(RED);
        tft.setTextSize(2);
        tft.println("It's a draw");
    }else
    {   // Den Gewinner ernennen
        tft.setRotation(tft.getRotation()+1);
        tft.setCursor(0, 30);
        tft.setTextColor(RED);
        tft.setTextSize(2);
        if (COMPUTER_TURN ) {
            tft.println("You win!");
            tft.println("Glueckwunsch!");
        }
        else if (HUMAN_TURN) {
            tft.println("Computer wins!");
        }else {
            tft.println("FATAL ERROR!");
        }

    }
    delay(5000); // wait 5 seconds
    //restart
    tft.setRotation(2);
    tft.fillScreen(BLACK);
    prepareField();
    moves = 0;
}

