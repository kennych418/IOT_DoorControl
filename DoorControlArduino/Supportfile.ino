// Support functions.

#define _MAXSPEED 1

void boardConfig(void)
{
  boardA.SPIPortConnect(&SPI);     

  boardA.configSyncPin(BUSY_PIN, 0);// BUSY pin low during operations;
                                    //  second paramter ignored.
  boardA.configStepMode(STEP_FS_8);   // 0 microsteps per step
  boardA.setMaxSpeed(_MAXSPEED);        // _MAXSPEED steps/s max
  boardA.setFullSpeed(_MAXSPEED);       // microstep below _MAXSPEED steps/s
  boardA.setAcc(_MAXSPEED);             // accelerate at _MAXSPEED steps/s/s
  boardA.setDec(_MAXSPEED);
  boardA.setSlewRate(SR_530V_us);   // Upping the edge speed increases torque.
  boardA.setOCThreshold(OC_4875mA);  // OC threshold 2250mA
  boardA.setPWMFreq(PWM_DIV_2, PWM_MUL_2); // 31.25kHz PWM freq
  boardA.setOCShutdown(OC_SD_DISABLE); // shutdown on OC
  boardA.setVoltageComp(VS_COMP_DISABLE); // don't compensate for motor V
  boardA.setSwitchMode(SW_USER);    // Switch is not hard stop
  boardA.setOscMode(INT_16MHZ_OSCOUT_16MHZ); // for boardA, we want 16MHz
                                    //  internal osc, 16MHz out.
  boardA.setAccKVAL(255);           // We'll tinker with these later, if needed.
  boardA.setDecKVAL(255);
  boardA.setRunKVAL(255);
  boardA.setHoldKVAL(32);           // This controls the holding current; keep it low.
}
