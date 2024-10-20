
#include <Arduino.h>
#include <TimerOne.h>
#include <EEPROM.h>
#include <string.h>
#define TIMER1_A_PIN   9
#define TIMER1_B_PIN   10
#define MAGIC "Fan1.0"

struct MyEE {
    char magic[8];
    byte pwm;
};
struct MyEE myee;
long int dutyCycle = 0;
byte led;

void setSpeed() {
  Timer1.pwm(TIMER1_A_PIN, dutyCycle * 1024 / 100);
}

void writeEEPROM() {
  strcpy(myee.magic, MAGIC);
  myee.pwm=dutyCycle;
  EEPROM.put(0,myee);
  for(int n=0; n<10 ; n++) {
    delay(100);
    digitalWrite(LED_BUILTIN, led = !led);
  }
}

void setup(void) {

    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    Timer1.initialize(500);  // 500us = 2kHz, 40 us = 25 kHz

    EEPROM.get(0, myee);
    Serial.println("");
    Serial.print("EEPROM read ");
    Serial.print(myee.magic);
    Serial.print(" speed=");
    if (strcmp(myee.magic, MAGIC) == 0) {
        dutyCycle = myee.pwm;
	Serial.print(myee.pwm);
        Serial.println("%");
    } else {
        Serial.println(" ignored");
        dutyCycle = 10;
	writeEEPROM();
    }
    Serial.print("Start PWM Fan, Duty Cycle = ");
    Serial.println(dutyCycle);
    setSpeed();
}


void loop(void)
{
    Serial.setTimeout(1000);
    long int n = Serial.parseInt(); // hangs for 1sec according to timeout
    if (n != 0 && n <= 70) {
	if (n < 0)
	    dutyCycle=0;
	else
	    dutyCycle=n;
	writeEEPROM();
	Serial.print("PWM Fan, Duty Cycle = ");
	Serial.println(dutyCycle);
    }
    setSpeed();
    digitalWrite(LED_BUILTIN, led = !led);
}
