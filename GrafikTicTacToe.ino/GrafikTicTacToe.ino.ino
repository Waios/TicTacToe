
#include <Elegoo_GFX.h>    // Core graphics library
#include <Elegoo_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>

#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif


/*
#define YP 9  // must be an analog pin, use "An" notation!
#define XM 8  // must be an analog pin, use "An" notation!
#define YM A2   // can be a digital pin
#define XP A3   // can be a digital pin
*/

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin
/*
#define TS_MINX 50
#define TS_MAXX 920

#define TS_MINY 100
#define TS_MAXY 940
*/
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

// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF


Elegoo_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

#define BOXSIZE 40
#define PENRADIUS 3
int oldcolor, currentcolor;

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

int col1 = 120;
int col2 = 200;
int row1 = 82;
int row2 = 162;

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

  tft.begin(identifier);
  tft.setRotation(2);

  tft.fillScreen(BLACK);


  tft.drawLine(20, col1, 220, col1, RED);
  tft.drawLine(20, col2, 220, col2, YELLOW);
  tft.drawLine(row1, 40, row1, 280, GREEN);
  tft.drawLine(row2, 40, row2, 280, WHITE);


/*
for(int x = 0; x < 9; x++){
  triangleAt(x);
  
}

for(int y = 0; y < 9; y++){
  circleAt(y);
  
}
*/

  pinMode(13, OUTPUT);
}

void circleAt(int pos){
  tft.drawCircle(circle[pos][0], circle[pos][1], circle[pos][2], YELLOW);
  
}

void triangleAt(int pos){
  tft.drawTriangle(triangle[pos][0], triangle[pos][1], triangle[pos][2], triangle[pos][3], triangle[pos][4], triangle[pos][5], BLUE);
}




#define MINPRESSURE 10
#define MAXPRESSURE 1000

void loop()
{
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  //pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  //pinMode(YM, OUTPUT);

  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!

  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    /*
    Serial.print("X = "); Serial.print(p.x);
    Serial.print("\tY = "); Serial.print(p.y);
    Serial.print("\tPressure = "); Serial.println(p.z);
    */
    
    if (p.y < (TS_MINY-5)) {
      Serial.println("erase");
      // press the bottom of the screen to erase 
      tft.fillRect(0, BOXSIZE, tft.width(), tft.height()-BOXSIZE, BLACK);
    }
    // scale from 0->1023 to tft.width
    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    //p.x = tft.width()-map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = (tft.height()-map(p.y, TS_MINY, TS_MAXY, tft.height(), 0));
     //p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);
    


    if(p.x < row1 && p.y < col1){
      triangleAt(6);
    }else if(p.x < row2 && p.y < col1){
      triangleAt(3);
    }else if(p.y < col1){
      triangleAt(0);
    }else if(p.x < row1 && p.y < col2){
      triangleAt(7);
    }else if(p.x < row2 && p.y < col2){
      triangleAt(4);
    }else if(p.y < col2){
      triangleAt(1);
    }else if(p.x < row1){
      triangleAt(8);
    }else if(p.x < row2){
      triangleAt(5);
    }else
    {triangleAt(2);
    }
    
    
  }
}

