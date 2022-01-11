#include "HomeSpan.h" 
#include "DEV_HAP.h"

#define PULSE 4
#define PULSE_FREQ 5
#define PULSE_RES 8
#define ENABLE 17
#define DIR 16

void setup()
{
  Serial.begin(115200);  

  //Initialize Big Stepper Driver
  pinMode(PULSE, OUTPUT);
  pinMode(ENABLE, OUTPUT);
  pinMode(DIR, OUTPUT);
  digitalWrite(ENABLE, HIGH);
  digitalWrite(DIR, HIGH);

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

    new DEV_DOOR(PULSE, PULSE_FREQ, PULSE_RES, ENABLE, DIR);
}

// HOMESPAN IMPORTANT: send 'F' in terminal to factory reset
void loop()
{
  homeSpan.poll();
}
