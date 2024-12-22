// C++ code
//
#include <SoftwareSerial.h>
SoftwareSerial mySerial(10,9); //SIM800L Tx & Rx is connected to Arduino #3 & #2
int led = 13;
int buzzer = 7;
double Gassensor;
void setup()
{
  pinMode(A0, INPUT);
  pinMode(led, OUTPUT);
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
  mySerial.begin(9600);

}
void setupSIM()
{
  Serial.println("Initializing..."); 
  delay(1000);

  mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();

  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  mySerial.println("AT+CMGS=\"+84961089406\"");
  //change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
  mySerial.print("Gas leaked"); //text content
  updateSerial();
  mySerial.write(26);
}
void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}

void loop()
{
  digitalWrite(led,LOW);
  digitalWrite(buzzer,LOW);
  Gassensor = analogRead(A0);
  if(Gassensor > 300){
	  digitalWrite(led,HIGH);
    digitalWrite(buzzer,HIGH);
    setupSIM();
    delay(200000);
  }
  else{
  	digitalWrite(led,LOW);
    digitalWrite(buzzer,LOW);
  }
  Serial.println(Gassensor);
  delay(1000);
}