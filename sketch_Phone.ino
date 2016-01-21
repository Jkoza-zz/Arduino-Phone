#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "SIM900.h"
#include <SoftwareSerial.h>
#include "sms.h"
#include "call.h"
#include <Keypad.h>
SMSGSM sms;
CallGSM call;
char number[11];
char message[] = "SMS sent from Arduino Phone";
bool selected = false;
String mode;
int count = 0;
const byte rows = 4; //four rows
const byte cols = 4; //three columns
char keys[rows][cols] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'0','F','E','D'}
};
byte rowPins[rows] = {5,4,3,2}; //connect to the row pinouts of the keypad
byte colPins[cols] = {9,8,7,6}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );
LiquidCrystal_I2C lcd(0x27,16,2);  
void setup(){ 
  count = 0;
  mode = "";
  selected = false;
  lcd.init();       
  lcd.backlight();
  lcd.setCursor(2,0); 
  lcd.print("ArduinoPhone");
  delay(1000);
  lcd.setCursor(1,1);
  lcd.print("1: Call 2: SMS");
  Serial.begin(9600);
  gsm.begin(2400);
}
void loop(){
  if (count == 11){
     phone();
  }
  char input = keypad.getKey();
  if (input != NO_KEY){
    Serial.print(input);
    if(selected == true){
      number[count] = input;
      lcd.setCursor(count + 1,1);
      lcd.print(input);
      count++;
    }
    else if(selected == false && input == '1'){
      lcd.clear();
      lcd.setCursor(2,0);
      lcd.print("Enter Number");
      lcd.setCursor(0,1);
      lcd.print("+");
      selected = true;
      mode = "Call"; 
    }
    else if(selected == false && input == '2'){
      lcd.clear();
      lcd.setCursor(2,0);
      lcd.print("Enter Number");
      lcd.setCursor(0,1);
      lcd.print("+");
      selected = true;
      mode = "SMS"; 
    }
  }
}
void phone(){
  if (mode == "Call"){
        lcd.clear();
        lcd.setCursor(2,0);
        lcd.print("Calling.....");
        call.Call(number);
        lcd.setCursor(2,1);
        delay(10000);
        setup();
  }
  else if (mode == "SMS"){
        lcd.clear();
        lcd.setCursor(2,0);
        lcd.print("Sending.....");
        if (sms.SendSMS(number, message)){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Sent Succesfully");
          lcd.setCursor(0,1);
          lcd.print("Restarting...");
        }
        else{
          lcd.clear();
          lcd.setCursor(3,0);
          lcd.print("Error");
          lcd.setCursor(0,1);
          lcd.print("Restarting...");
        }
        delay(3000);
        setup();
  }
}

