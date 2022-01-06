#include <SparkFunAutoDriver.h>
#include <SPI.h>

#include "HomeSpan.h" 
#include "DEV_HAP.h"

#define RESET 4 //Designated by user
#define CS 5    //CS of the SPI channel
#define SCLK 18 //SCLK of the SPI channel

AutoDriver boardA(0, CS, RESET);

void setup()
{
  Serial.begin(115200);  

  //Initialize SPI and AutoDriver Library
  pinMode(RESET, OUTPUT);
  pinMode(MOSI, OUTPUT);
  pinMode(MISO, INPUT);
  pinMode(SCLK, OUTPUT);
  pinMode(CS, OUTPUT);
  
  digitalWrite(CS, HIGH);
  digitalWrite(RESET, LOW);       
  digitalWrite(RESET, HIGH);      
  
  SPI.begin();
  SPI.setDataMode(SPI_MODE3);
  boardConfig();  

  //Initialize HomeSpan
  homeSpan.begin(Category::Doors,"Smart Door");

  new SpanAccessory(); 
  
    new Service::AccessoryInformation(); 
      new Characteristic::Name("Servo"); 
      new Characteristic::Manufacturer("Kenny"); 
      new Characteristic::SerialNumber("123-ABC"); 
      new Characteristic::Model("BingBong"); 
      new Characteristic::FirmwareRevision("1.0"); 
      new Characteristic::Identify();            
      
    new Service::HAPProtocolInformation();      
      new Characteristic::Version("1.1.0");   

    new DEV_DOOR(&boardA);
}

// HOMESPAN IMPORTANT: send 'F' in terminal to factory reset
void loop()
{
  homeSpan.poll();
}
