#include "HomeSpan.h" 
#include "DEV_HAP.h"

#define PULSE 4
#define ENABLE 17
#define DIR 16

//AutoDriver boardA(0, CS, RESET);

void setup()
{
  Serial.begin(115200);  

  //Initialize SPI and AutoDriver Library
  //pinMode(RESET, OUTPUT);
  //pinMode(MOSI, OUTPUT);
  //pinMode(MISO, INPUT);
  //pinMode(SCLK, OUTPUT);
  //pinMode(CS, OUTPUT);
  
  //digitalWrite(CS, HIGH);
  //digitalWrite(RESET, LOW);       
  //digitalWrite(RESET, HIGH);      
  
  //SPI.begin();
  //SPI.setDataMode(SPI_MODE3);
  //boardConfig();  

  //Initialize Big Stepper Driver
  pinMode(PULSE, OUTPUT);
  pinMode(ENABLE, OUTPUT);
  pinMode(DIR, OUTPUT);

  digitalWrite(ENABLE, HIGH);
  digitalWrite(DIR, LOW);

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

    new DEV_DOOR();
}

// HOMESPAN IMPORTANT: send 'F' in terminal to factory reset
void loop()
{
  homeSpan.poll();
}
