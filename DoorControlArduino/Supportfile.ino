// Support functions.

#define _MAXSPEED 10000

void boardConfig(void)
{
  boardA.SPIPortConnect(&SPI);     

  boardA.configSyncPin(BUSY_PIN, 0);// BUSY pin low during operations;
                                    //  second paramter ignored.
  boardA.configStepMode(STEP_FS);   // 0 microsteps per step
  boardA.setMaxSpeed(_MAXSPEED);        // _MAXSPEED steps/s max
  boardA.setFullSpeed(_MAXSPEED);       // microstep below _MAXSPEED steps/s
  boardA.setAcc(_MAXSPEED);             // accelerate at _MAXSPEED steps/s/s
  boardA.setDec(_MAXSPEED);
  boardA.setSlewRate(SR_530V_us);   // Upping the edge speed increases torque.
  boardA.setOCThreshold(OC_2250mA);  // OC threshold 2250mA
  boardA.setPWMFreq(PWM_DIV_2, PWM_MUL_2); // 31.25kHz PWM freq
  boardA.setOCShutdown(OC_SD_DISABLE); // shutdown on OC
  boardA.setVoltageComp(VS_COMP_DISABLE); // don't compensate for motor V
  boardA.setSwitchMode(SW_USER);    // Switch is not hard stop
  boardA.setOscMode(INT_16MHZ_OSCOUT_16MHZ); // for boardA, we want 16MHz
                                    //  internal osc, 16MHz out.
  boardA.setAccKVAL(128);           // We'll tinker with these later, if needed.
  boardA.setDecKVAL(128);
  boardA.setRunKVAL(128);
  boardA.setHoldKVAL(32);           // This controls the holding current; keep it low.
}
