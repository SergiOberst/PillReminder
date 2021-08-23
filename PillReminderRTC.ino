#include <ThreeWire.h>
#include <RtcDS1302.h>

const long A = 1000;     //Resistance at darkness on KΩ
const int B = 15;        //Resistance at light (10 Lux) en KΩ
const int Rc = 10;       //Resistance callibration en KΩ
const int LDRPin = A0;   //LDR Resistance Pin
const int RedLED = 9;
const int SensibilityLDR = 20;
const int StartDayHour = 8; //Start medication hour


int V;
int ilum;
bool liftedBlister = false;
RtcDateTime lastLift; //Object to storage datetime when pill box is lifted


//RTC
ThreeWire myWire(4, 5, 2); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);

void setup()
{
  Serial.begin(115200);
  pinMode(RedLED, OUTPUT);

  //Start RTC
  Rtc.Begin();
  lastLift = Rtc.GetDateTime();
}

void loop()
{

  while (!liftedBlister) {
    V = analogRead(LDRPin);
    ilum = ((long)V * A * 10) / ((long)B * Rc * (1024 - V)); //usar si LDR entre A0 y Vcc (como en el esquema anterior)

    if (ilum < SensibilityLDR) {
      digitalWrite(RedLED , HIGH);
    } else {
      digitalWrite(RedLED , LOW);
      liftedBlister = true;
      lastLift = Rtc.GetDateTime();
    }
    //Serial.println(ilum);
  }

  //Waits until next day
  while (isSameDay()) {
    delay(1000);
  }

  //Waits until the specified hour and onwards
  while (!isStartDayHour()) {
    delay(1000);
  }
  liftedBlister = false;
}

bool isSameDay() {
  RtcDateTime today = Rtc.GetDateTime();
  //Serial.println(today.Hour());

  //contemplate the possibility of taking the pill between 12AM and 3AM
  if (today.Hour() >= 0 && today.Hour() <= 3) {
    return false;
  }
  return lastLift.Day() == today.Day();
}

bool isStartDayHour() {
  RtcDateTime today = Rtc.GetDateTime();
//  Serial.println(today.Hour());
//  Serial.println(lastLift.Day());
  return today.Hour() >= StartDayHour;
}
