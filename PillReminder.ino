const long A = 1000;     //Resistencia en oscuridad en KΩ
const int B = 15;        //Resistencia a la luz (10 Lux) en KΩ
const int Rc = 10;       //Resistencia calibracion en KΩ
const int LDRPin = A0;   //Pin de resistencia LDR 
const int RedLED = 9;
const long TimeToWait = 72000000; //20h
const int SensibilityLDR = 20;

int V;
int ilum;
bool uppedBlister = false;
long timeNow = 0;


void setup()
{
  Serial.begin(115200);
  pinMode(RedLED, OUTPUT);//Definimos como salida
}

void loop()
{

  while (!uppedBlister) {
    V = analogRead(LDRPin);
    ilum = ((long)V * A * 10) / ((long)B * Rc * (1024 - V)); //usar si LDR entre A0 y Vcc (como en el esquema anterior)
    
    if (ilum < SensibilityLDR) {
      digitalWrite(RedLED , HIGH);
    } else {
      digitalWrite(RedLED , LOW);
      uppedBlister = true;
    }
    Serial.println(ilum);
    //delay(500);
    timeNow = millis();

  }
  while ( millis() < timeNow + TimeToWait) {}
  uppedBlister = false;

}
