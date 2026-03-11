#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <SoftwareSerial.h>

LiquidCrystal_I2C lcd(0x27,16,2);
SoftwareSerial gsm(10,11);

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

int buzzer = 12;
int token = 1;
int serving = 0;

String phoneList[30];
int tokenList[30];
int queueSize = 0;

// Map keys to symptom names
String symptomNames[7] = {"", "Headache", "Flu", "Accident", "Heart Attack", "Fever", "Review"};

// Send SMS
void sendSMS(String number, String text) {
  gsm.println("AT");
  delay(500);
  gsm.println("AT+CMGF=1"); // SMS text mode
  delay(500);
  gsm.print("AT+CMGS=\"");
  gsm.print(number);
  gsm.println("\"");
  delay(1000);
  gsm.print(text);
  delay(500);
  gsm.write(26); // CTRL+Z
  delay(5000);
}

// Smooth scrolling function for LCD
void displayScrolling(String text) {
  int len = text.length();
  if(len <= 16) {
    lcd.setCursor(0,1);
    lcd.print(text);
    return;
  }
  for(int i=0; i<=len-16; i++) {
    lcd.setCursor(0,1);
    lcd.print(text.substring(i,i+16));
    delay(500); // scroll speed (500ms per shift)
  }
}

void registerPatient() {
  lcd.clear();
  lcd.print("Enter Phone:");
  lcd.setCursor(0,1);

  String phone = "";
  while(true) {
    char key = keypad.getKey();
    if(key) {
      if(key=='#') break;
      phone += key;
      lcd.print(key);
    }
  }

  String symptoms = "";
  bool confirmed = false;

  while(!confirmed) {
    symptoms = ""; // reset each attempt

    lcd.clear();
    lcd.print("Select Symptoms");
    lcd.setCursor(0,1);
    lcd.print("#Finish");

    lcd.clear();
    lcd.print("1Head 2Flu 3Acc");
    lcd.setCursor(0,1);
    lcd.print("4Heart 5Fever 6Rev");

    String symptomDisplay = "";
    char key = 0;

    while(true) {
      key = keypad.getKey();
      if(key) {
        if(key=='#') break;
        if(symptoms.indexOf(key) == -1 && key>='1' && key<='6') {
          symptoms += key;
          symptomDisplay += symptomNames[key-'0'] + " ";
          lcd.clear();
          lcd.print("Selected:");
          displayScrolling(symptomDisplay); // smooth scroll
        }
      }
    }

    // Confirmation
    lcd.clear();
    lcd.print("Confirm? 1=Yes");
    lcd.setCursor(0,1);
    lcd.print("2=No");

    char confirmKey = 0;
    while(!confirmKey) {
      confirmKey = keypad.getKey();
      if(confirmKey=='1') {
        confirmed = true;
        break;
      } else if(confirmKey=='2') {
        confirmed = false;
        lcd.clear();
        lcd.print("Reselect Symptoms");
        delay(1000);
        break;
      }
    }
  }

  // Determine triage
  String triage = "NORMAL";
  if(symptoms.indexOf('3') != -1 || symptoms.indexOf('4') != -1)
    triage = "EMERGENCY";
  else if(symptoms.indexOf('6') != -1)
    triage = "REVIEW";
  else
    triage = "NORMAL";

  lcd.clear();
  lcd.print(triage);
  delay(1500);

  phoneList[queueSize] = phone;
  tokenList[queueSize] = token;

  lcd.clear();
  lcd.print("Token No:");
  lcd.setCursor(0,1);
  lcd.print(token);

  digitalWrite(buzzer,HIGH);
  delay(200);
  digitalWrite(buzzer,LOW);

  String message =
    "Hospital Queue System\n"
    "Token: " + String(token) +
    "\nTriage: " + triage +
    "\nPlease wait.";

  sendSMS(phone,message);

  queueSize++;
  token++;
  delay(3000);
}

// Call next patient
void callNext() {
  serving++;

  for(int i=0;i<queueSize;i++) {
    if(tokenList[i]==serving+2) {
      sendSMS(phoneList[i],
        "Queue Update:\nOnly 2 patients ahead. Please prepare.");
    }

    if(tokenList[i]==serving) {
      sendSMS(phoneList[i],
        "Queue Alert:\nIt is now your turn.");
    }
  }

  lcd.clear();
  lcd.print("Serving:");
  lcd.setCursor(0,1);
  lcd.print(serving);

  digitalWrite(buzzer,HIGH);
  delay(500);
  digitalWrite(buzzer,LOW);

  delay(2000);
}

void setup() {
  gsm.begin(9600);
  lcd.init();
  lcd.backlight();
  pinMode(buzzer,OUTPUT);

  lcd.print("Smart Queue");
  delay(2000);
}

void loop() {
  lcd.clear();
  lcd.print("1:Register");
  lcd.setCursor(0,1);
  lcd.print("2:Next");

  char key = keypad.getKey();
  if(key=='1') registerPatient();
  if(key=='2') callNext();
}
