#include "HomeSpan.h" 
#include "DEV_HAP.h"

#define PULSE 4
#define PULSE_FREQ 20
#define PULSE_RES 8
#define ENABLE 17
#define DIR 16

#define PHASE_A 26
#define PHASE_B 27
volatile unsigned int temp, counter = 0;

void setup()
{
  Serial.begin(115200);  

  //Initialize Big Stepper Driver
  pinMode(ENABLE, OUTPUT);
  digitalWrite(ENABLE, HIGH);
  pinMode(DIR, OUTPUT);
  digitalWrite(DIR, HIGH);
  pinMode(PULSE, OUTPUT);

  //Initialize Encoder
  pinMode(PHASE_A, INPUT_PULLUP);
  pinMode(PHASE_B, INPUT_PULLUP);
  attachInterrupt(PHASE_A, ai0, RISING);
  attachInterrupt(PHASE_B, ai1, RISING);

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
  if( counter != temp ){
  Serial.print("Angle: ");
  float angle = 360*counter/1200;
  Serial.println (angle);
  temp = counter;
  }
}

void ai0() {
  // ai0 is activated if DigitalPin nr 2 is going from LOW to HIGH
  // Check pin 3 to determine the direction
  if(digitalRead(PHASE_B)==LOW) {
  counter++;
  }else{
  counter--;
  }
  }
   
  void ai1() {
  // ai0 is activated if DigitalPin nr 3 is going from LOW to HIGH
  // Check with pin 2 to determine the direction
  if(digitalRead(PHASE_A)==LOW) {
  counter--;
  }else{
  counter++;
  }
  }
