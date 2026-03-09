#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

LiquidCrystal_I2C lcd(0x27,16,2);
SoftwareSerial gsm(10,11);

const int buzzer = 12;

/*--------------- KEYPAD ----------------*/
const byte ROWS = 4;
const byte COLS = 3;

char keys[ROWS][COLS] = {
{'1','2','3'},
{'4','5','6'},
{'7','8','9'},
{'*','0','#'}
};

byte rowPins[ROWS] = {9,8,7,6};
byte colPins[COLS] = {5,4,3};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

/*------------ PATIENT STRUCT -----------*/
struct Patient{
  int token;
  String phone;
  int triage;
};

/*------------ QUEUE ARRAYS -------------*/
Patient triage1[20];
Patient triage2[20];
Patient triage3[20];

int t1Count=0;
int t2Count=0;
int t3Count=0;

int token=1;
int serving=0;

/*---------------------------------------*/

void setup(){

lcd.init();
lcd.backlight();

Serial.begin(9600);
gsm.begin(9600);

pinMode(buzzer,OUTPUT);

lcd.setCursor(0,0);
lcd.print("SMART HOSPITAL");
lcd.setCursor(0,1);
lcd.print("QUEUE SYSTEM");
delay(3000);
lcd.clear();

showHome();
}

/*---------------------------------------*/

void loop(){

char key = keypad.getKey();

if(key=='*'){
registerPatient();
}

checkSerial();
}

/*---------------------------------------*/

void showHome(){

lcd.clear();
lcd.setCursor(0,0);
lcd.print("* New Patient");
lcd.setCursor(0,1);
lcd.print("Queue:");
lcd.print(t1Count+t2Count+t3Count);
}

/*---------------------------------------*/

void registerPatient(){

int triage = selectTriage();
String phone = enterPhone();

storePatient(triage,phone);

sendTokenSMS(phone,token);

lcd.clear();
lcd.print("Token:");
lcd.print(token);
lcd.setCursor(0,1);
lcd.print("Registered");

delay(3000);

token++;

showHome();
}

/*---------------------------------------*/

int selectTriage(){

lcd.clear();
lcd.print("Select Triage");
lcd.setCursor(0,1);
lcd.print("1 2 3");

while(true){

char key = keypad.getKey();

if(key=='1'||key=='2'||key=='3'){

lcd.clear();
lcd.print("Triage:");
lcd.print(key);
delay(1500);

return key-'0';
}
}
}

/*---------------------------------------*/

String enterPhone(){

String phone="";

lcd.clear();
lcd.print("Phone:");
lcd.setCursor(0,1);

while(true){

char key = keypad.getKey();

if(key){

if(key=='#') break;

phone+=key;
lcd.print(key);
}
}

return phone;
}

/*---------------------------------------*/

void storePatient(int triage,String phone){

Patient p;
p.token=token;
p.phone=phone;
p.triage=triage;

if(triage==1){
triage1[t1Count++]=p;
}

else if(triage==2){
triage2[t2Count++]=p;
}

else{
triage3[t3Count++]=p;
}

Serial.print("Token ");
Serial.print(token);
Serial.print(" Triage ");
Serial.print(triage);
Serial.print(" Phone ");
Serial.println(phone);
}

/*---------------------------------------*/

void checkSerial(){

if(Serial.available()){

char c = Serial.read();

if(c=='N'||c=='n'){
callNextPatient();
}
}
}

/*---------------------------------------*/

void callNextPatient(){

Patient next;

if(t1Count>0){

next = triage1[0];
shiftQueue(triage1,t1Count);
t1Count--;

}

else if(t2Count>0){

next = triage2[0];
shiftQueue(triage2,t2Count);
t2Count--;

}

else if(t3Count>0){

next = triage3[0];
shiftQueue(triage3,t3Count);
t3Count--;

}

else{

lcd.clear();
lcd.print("Queue Empty");
delay(2000);
showHome();
return;
}

serving = next.token;

lcd.clear();
lcd.print("Now Serving");
lcd.setCursor(0,1);
lcd.print("Token ");
lcd.print(serving);

Serial.print("Serving Token ");
Serial.println(serving);

digitalWrite(buzzer,HIGH);
delay(1500);
digitalWrite(buzzer,LOW);

sendSMS(next.phone,"Your turn now. Token "+String(serving));

checkAlerts();

delay(3000);

showHome();
}

/*---------------------------------------*/

void shiftQueue(Patient arr[],int size){

for(int i=0;i<size-1;i++){
arr[i]=arr[i+1];
}
}

/*---------------------------------------*/

void checkAlerts(){

for(int i=0;i<t1Count;i++){

if(triage1[i].token-serving==2)
sendSMS(triage1[i].phone,"2 patients before you.");
}

for(int i=0;i<t2Count;i++){

if(triage2[i].token-serving==2)
sendSMS(triage2[i].phone,"2 patients before you.");
}

for(int i=0;i<t3Count;i++){

if(triage3[i].token-serving==2)
sendSMS(triage3[i].phone,"2 patients before you.");
}
}

/*---------------------------------------*/

void sendTokenSMS(String phone,int tok){

String msg="Hospital Queue Token ";
msg+=String(tok);

sendSMS(phone,msg);
}

/*---------------------------------------*/

void sendSMS(String number,String message){

gsm.println("AT+CMGF=1");
delay(1000);

gsm.print("AT+CMGS=\"");
gsm.print(number);
gsm.println("\"");

delay(1000);

gsm.print(message);

delay(500);

gsm.write(26);

delay(4000);
}
