#define LCD_CS A3    
#define LCD_CD A2    
#define LCD_WR A1   
#define LCD_RD A0    
// you can also just connect RESET to the arduino RESET pin
#define LCD_RESET A4
//Duemilanove/Diecimila/UNO/etc ('168 and '328 chips) microcontoller:
// Color definitions
#define  BLACK           0x0000
#define  BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0 
#define WHITE           0xFFFF

#include "TFTLCD.h"

#define BUFFERSIZE 30

TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
String inputString = "";         
String lines[BUFFERSIZE];
int readix = 0;
int writeix = 0;
int datasize = 0;
int wcount = 0;
int dcount = 0;
boolean stringComplete = false;

void setup(void) {
  Serial.begin(57600);
  Serial.println("8 Bit LCD test!");
  inputString.reserve(200);
  
  tft.reset();
  tft.initDisplay();
  
  delay(500);
  tft.fillScreen(BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(BLUE);
  tft.setTextSize(1);
  tft.setRotation(1); 
  insertBuffer("Hello World");
  showLCD(readix);
  //testLCD();
}

void loop(void) {
  if (stringComplete) {
    //tft.println(inputString); 
    //Serial.println(inputString);
    insertBuffer(inputString);
    inputString = "";
    stringComplete = false;
    wcount = 0;
    dcount++;
    
  } else {
    wcount++;
  }
  if ((wcount > 1000) && (dcount > 0)) {
    showLCD(readix);
    wcount = 0;
    dcount = 0;
  }
  
}
void testLCD()
{
  int show = 0;
  while (show < BUFFERSIZE) {
    tft.println(show);
    show++;
  }
}
void showLCD(int readix)
{
  int show = 0;
  int ix = readix;
  String str;
  
  //tft.fillScreen(BLACK);
  tft.setCursor(0, 0);
  while(show < BUFFERSIZE) {
    str = lines[ix];
    tft.print(str);
    ix++;
    ix = ix % BUFFERSIZE; 
    show++;
  }
}
int bufferEmpty()
{
  if ((writeix == readix) && (datasize == 0)) return 1;
  return 0;
}
int bufferFull()
{
  if ((writeix == readix) && (datasize == BUFFERSIZE)) return 1;
  return 0;  
}
void insertBuffer(String str)
{
   // increase write_pointer, check if at end of array
    if (++writeix >= BUFFERSIZE)  writeix = 0;
    if (writeix == readix) readix++;
    if (readix >= BUFFERSIZE) readix = 0;
    
    lines[writeix] = str;   
    //Serial.println(str);
    datasize++;
    if (datasize >= BUFFERSIZE) {
      datasize = BUFFERSIZE;
    }
}
/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    } 
  }
}
