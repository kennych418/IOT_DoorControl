struct DEV_DOOR : Service::Door {                                                    
  SpanCharacteristic *CurrentPosition;                        
  SpanCharacteristic *TargetPosition;
  SpanCharacteristic *PositionState;
  SpanCharacteristic *ObstructionDetected;
  
  int ENABLE_PIN;
  int DIR_PIN;
  int PULSE_PIN;
  
  float TargetAngle;
  float CurrentAngle;
  bool updated;
  volatile int* counter;
  unsigned long refreshTime;

  DEV_DOOR(int PULSE, int PULSE_FREQ, int PULSE_RES, int ENABLE, int DIR, volatile int* encodercounter) : Service::Door(){
    CurrentPosition=new Characteristic::CurrentPosition();              
    TargetPosition=new Characteristic::TargetPosition();
    PositionState=new Characteristic::PositionState();
    ObstructionDetected=new Characteristic::ObstructionDetected();   
    
    ENABLE_PIN = ENABLE;
    DIR_PIN = DIR;
    PULSE_PIN = PULSE;
    ledcSetup(0, PULSE_FREQ, PULSE_RES);         
    ledcAttachPin(PULSE_PIN, 0);
    
    TargetAngle = 0;
    CurrentAngle = 0;
    updated = false;
    counter = encodercounter;   
    refreshTime = millis();         
  } 

  boolean update(){
    //Open = 100, Closed = 0
    TargetAngle = TargetPosition->getNewVal() * 0.85;
    CurrentAngle = *counter * 360/1200;
    updated = true;
    return(true);        
  }

void loop(){
  //Report position every 10 minutes after update
  if(millis() - refreshTime > 600000 && CurrentPosition->timeVal() > 600000 && TargetPosition->timeVal() > 600000){
    int refresh = (*counter * 360/1200) * 100 / 85;
    if (refresh > 100)
      refresh = 100;
    else if (refresh < 0)
      refresh = 0;
    CurrentPosition->setVal((uint8_t)refresh);
    TargetPosition->setVal((uint8_t)refresh);
    refreshTime = millis();
  }
  //Timer Overflow Case
  if((signed long)(millis() - refreshTime) < -1) //timer overflow case
        refreshTime = millis();

  if(updated){
    bool obstructed = false;
    float previousAngle = CurrentAngle;
    bool dir = 0;
    if(TargetAngle == 0)
      dir = true;  //Always swing towards CurrentAngle > 0
    else
      dir = CurrentAngle > TargetAngle;

    digitalWrite(DIR_PIN, dir);
    digitalWrite(ENABLE_PIN,LOW);
    ledcWrite(0, 127); //PULSE ON 50% duty cycle

    //wait for moving
    unsigned long startTime = millis();
    while(abs(CurrentAngle - previousAngle) < 4){
      CurrentAngle = *counter * 360/1200;
      if((millis() - startTime) > 7000) //timer cutoff
        break;
      if((signed long)(millis() - startTime) < -1) //timer overflow case
        startTime = millis();
    }

    int stopcounter = 0;
    startTime = millis();
    while(!obstructed && ((CurrentAngle > TargetAngle) == dir || TargetAngle == 0)){
      delay(600);
      previousAngle = CurrentAngle;
      CurrentAngle = *counter * 360/1200;

      if(abs(CurrentAngle - previousAngle) < 1.8) 
        stopcounter++;
      if(stopcounter > 2)
        obstructed = true;

      if((millis() - startTime) > 20000) //timer cutoff
        break;
      if((signed long)(millis() - startTime) < -1) //timer overflow case
        startTime = millis();
    }

    if(TargetAngle == 0)
      *counter = 0;
    
    ledcWrite(0, 0);   //PULSE OFF
    digitalWrite(ENABLE_PIN,HIGH);
    CurrentPosition->setVal(TargetPosition->getNewVal());
    updated = false;
  }
}
};
      