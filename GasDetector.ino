// C++ code
//
#include <SoftwareSerial.h>
SoftwareSerial mySerial(11,10); //SIM800L Tx & Rx is connected to Arduino #3 & #2
int led = 13;
int buzzer = 7;
double Gassensor;
void setup()
{
  pinMode(A0, INPUT);
  pinMode(led, OUTPUT);
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
}

#define PHONE_NUMBER      "0965842465"              // thay đổi số điện thoại
#define SET_BAUDRATE      "AT+IPREX = 115200"         // thay đổi badrate (VD: "AT+IPREX=115200")
#define MCU_SIM_BAUDRATE  115200

void sim_at_wait(){
    delay(100);
    while (simSerial.available()) {
        Serial.write(simSerial.read());
    }
}

bool sim_at_cmd(String cmd){
    simSerial.println(cmd);
    sim_at_wait();
    return true;
}

bool sim_at_send(char c){
    simSerial.write(c);
    return true;
}

void sent_sms() {             // Hàm thực hiện gửi tin nhắn
    sim_at_cmd("AT+CMGF=1");
    String temp = "AT+CMGS=\"";
    temp += (String)PHONE_NUMBER;
    temp += "\"";
    sim_at_cmd(temp);
    sim_at_cmd("Gas leaked");
    sim_at_send(0x1A);    // End charactor for SMS
}
void setupSIM() {
    delay(200);
    simSerial.begin(MCU_SIM_BAUDRATE);
    delay(200);
    
    Serial.println("System started!!!!");
    sim_at_cmd("AT");           // Kiểm tra AT Command
    delay(200);
    sim_at_cmd("AT+IPREX?");    // Kiểm tra tốc độ baud rate
    delay(200);
    sim_at_cmd(SET_BAUDRATE);   // Cài tốc độ baudrate
    delay(200);
    pinMode(13,OUTPUT); 
    digitalWrite(13,HIGH);
    sent_sms();                 // thực hiện gửi tin nhắn
    delay(5000);                // Delay 5s 
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
