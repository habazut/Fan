
#include <Arduino.h>
#include <TimerOne.h>
#include <EEPROM.h>
#include <string.h>
#define TIMER1_A_PIN   9
#define TIMER1_B_PIN   10

struct MyEE {
    char magic[8];
    byte pwm;
};
struct MyEE myee;
long int dutyCycle = 0;

void setup(void) {

    Serial.begin(115200);
    EEPROM.get(0, myee);

    Serial.println("");
    Serial.print("EEPROM read ");
    Serial.print(myee.magic);
    Serial.print(" speed=");
    if (strcmp(myee.magic, "Fan1.0") == 0) {
	dutyCycle = myee.pwm;
	Serial.print(myee.pwm);
	Serial.println("%");
    } else {
	Serial.println(" ignored");
    }
    strcpy(myee.magic,"Fan1.0");
    myee.pwm=dutyCycle;

    Timer1.initialize(500);  // 500us = 2kHz, 40 us = 25 kHz
}


void loop(void)
{
    Serial.setTimeout(2000);
    long int n = Serial.parseInt();
    if (n != 0 && n <= 70) {

	if (n < 0)
	    dutyCycle=0;
	else
	    dutyCycle=n;
	
	myee.pwm=dutyCycle;
	EEPROM.put(0,myee);

	Serial.print("PWM Fan, Duty Cycle = ");
	Serial.println(dutyCycle);
    }
    Timer1.pwm(TIMER1_A_PIN, dutyCycle * 1024 / 100);
    delay(100);
//  }
}
