struct DEV_DOOR : Service::Door {                                                    
  SpanCharacteristic *CurrentPosition;                        
  SpanCharacteristic *TargetPosition;
  SpanCharacteristic *PositionState;
  SpanCharacteristic *ObstructionDetected;
  int ENABLE_PIN = 0;
  int DIR_PIN = 0;
  int PULSE_PIN = 0;
  volatile int* counter;

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
    counter = encodercounter;            
  } 

  boolean update(){
    //Open = 100, Closed = 0
    float TargetAngle = TargetPosition->getNewVal() * 0.85;
    float CurrentAngle = *counter * 360/1200;

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
      if((millis() - startTime) < -1) //timer overflow case
        startTime = millis();
    }

    int stopcounter = 0;
    startTime = millis();
    while(!obstructed && ((CurrentAngle > TargetAngle) == dir || TargetAngle == 0)){
      delay(600);
      previousAngle = CurrentAngle;
      CurrentAngle = *counter * 360/1200;

      if(abs(CurrentAngle - previousAngle) < 2) 
        stopcounter++;
      if(stopcounter > 2)
        obstructed = true;

      if((millis() - startTime) > 20000) //timer cutoff
        break;
      if((millis() - startTime) < -1) //timer overflow case
        startTime = millis();
    }

    if(TargetAngle == 0)
      *counter = 0;
    
    ledcWrite(0, 0);   //PULSE OFF
    digitalWrite(ENABLE_PIN,HIGH);

    CurrentPosition->setVal(TargetPosition->getNewVal());
    return(true);        
  }
};
      