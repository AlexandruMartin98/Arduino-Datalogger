//definire ecran
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>

#ifdef ADAFRUIT_HALLOWING
  #define TFT_CS        39 // Hallowing display control pins: chip select
  #define TFT_RST       37 // Display reset
  #define TFT_DC        38 // Display data/command select
  #define TFT_BACKLIGHT  7 // Display backlight pin
#else
  // For the breakout board, you can use any 2 or 3 pins.
  // These pins will also work for the 1.8" TFT shield.
  #define TFT_CS        10
  #define TFT_RST        8 // Or set to -1 and connect to Arduino RESET pin
  #define TFT_DC         9
#endif


// senzor lumina
int photocellPin = 0;     // the cell and 10K pulldown are connected to a0
int ValoareSenzorLumina;     // the analog reading from the sensor divider  



//senzor fum
//int redLed = 12;
//int greenLed = 11;
//int buzzer = 10;
int smokeA0 = A5;
int sensorThres = 400; // Your threshold value



//senzor temperatura digital
#include <OneWire.h> 
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 2  
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature ValoareSenzorTemperatura(&oneWire);


//card
#include <SD.h>
File myFile;


//ecran
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);


//date senzori
int count = 0;
int lumina[64];
int gazfum[64];
int temp[64];


void setup(void) { 

//initializare vectori
  for(int i=0;i<64;i++){
    lumina[i]=0;
    gazfum[i]=0;
    temp[i]=0;
  }


  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST77XX_BLACK);


  // senzor lumina// senzor lumina// senzor lumina// senzor lumina
  Serial.begin(9600);  


  //senzor fum//senzor fum//senzor fum//senzor fum
  //pinMode(redLed, OUTPUT);
  //pinMode(greenLed, OUTPUT);
  //pinMode(buzzer, OUTPUT);
  pinMode(smokeA0, INPUT);



  //senzor temperatura digital//senzor temperatura digital//senzor temperatura digital
  ValoareSenzorTemperatura.begin(); 



  // creare fisier
  Serial.print(F("Initializing SD card..."));
  if (!SD.begin(7)) {
    Serial.println(F("failed!"));
  }
  else Serial.println(F("done."));
  

  myFile = SD.open("DB.csv", FILE_WRITE);
  Serial.print(F("DB opening..."));
  if (myFile){
    Serial.println(F("OK\n"));
    //myFile.println("Lumina,\"Gaz & Fum\",\"Temperatura\"");
  }
  else Serial.println(F("failed\n"));

  //Serial.println(F(""));
  
}


void loop(void) {

  String string="";

  // senzor lumina// senzor lumina// senzor lumina// senzor lumina// senzor lumina// senzor lumina// senzor lumina// senzor lumina//
  ValoareSenzorLumina = analogRead(photocellPin);
  string=string+ValoareSenzorLumina+",";  
 
  Serial.print(F("Lumina: "));
  Serial.println(ValoareSenzorLumina); 
  ValoareSenzorLumina = map(ValoareSenzorLumina, 1, 1000, 0, 53);

  //senzor fum//senzor fum//senzor fum//senzor fum//senzor fum//senzor fum//senzor fum//senzor fum//senzor fum//senzor fum//
  int ValoareSenzorGazFum = analogRead(smokeA0);
  string=string+ValoareSenzorGazFum+","; 

  Serial.print(F("Gaz si fum: "));
  Serial.println(ValoareSenzorGazFum);
  ValoareSenzorGazFum = map(ValoareSenzorGazFum, 1, 1000, 60, 106);



  //senzor temperatura digital//senzor temperatura digital//senzor temperatura digital//senzor temperatura digital//senzor temperatura digital
  ValoareSenzorTemperatura.requestTemperatures();
  string=string+ValoareSenzorTemperatura.getTempCByIndex(0); 
  
  Serial.print(F("Temperatura: ")); 
  int ValoareSenzorTemperatura2 = map(ValoareSenzorTemperatura.getTempCByIndex(0), 1, 130, 106, 159);
  Serial.println(ValoareSenzorTemperatura.getTempCByIndex(0));
  Serial.println(F(""));


  if(myFile) {
      myFile.println(string);
      myFile.flush();
  }
  
  // shiftare
  if(count<=63) {
    lumina[count] = ValoareSenzorLumina;
    gazfum[count] = ValoareSenzorGazFum;
    temp[count] = ValoareSenzorTemperatura2;
    
    tft.drawLine((count-1)*2,lumina[count-1],count*2,lumina[count],ST77XX_GREEN);
    tft.drawLine((count-1)*2,gazfum[count-1],count*2,gazfum[count],ST77XX_RED);
    tft.drawLine((count-1)*2,temp[count-1],count*2,temp[count],ST77XX_BLUE);
    
    count++;
  } 
  else {
    for(int i=1;i<=63;i++) {
      lumina[i-1] = lumina[i];
      gazfum[i-1] = gazfum[i];
      temp[i-1] = temp[i];
    }
    
    lumina[63] = ValoareSenzorLumina;
    gazfum[63] = ValoareSenzorGazFum;
    temp[63] = ValoareSenzorTemperatura2;
    //tft.initR(INITR_BLACKTAB);
    tft.fillScreen(ST77XX_BLACK);

    for(int i=1;i<=63;i++){
          tft.drawLine((i-1)*2,lumina[i-1],i*2,lumina[i],ST77XX_GREEN);
          tft.drawLine((i-1)*2,gazfum[i-1],i*2,gazfum[i],ST77XX_RED);
          tft.drawLine((i-1)*2,temp[i-1],i*2,temp[i],ST77XX_BLUE);
    }
  }



  delay(1);
  
}
