#include <LiquidCrystal.h>

#define button_up 2
#define button_down 3
#define motorPin 5

LiquidCrystal lcd(12, NULL, 11, 10, 9, 8, 7);
byte intreg[8] = {

  B00000,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B00000,
  
};  //segment de bara plin (2 unitati)

byte gol[8] = {

  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
};  //segment de bara gol

byte inceput[8] = {


  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
};  //segmentul de inceput de bara gol

byte jumate[8] = {


  B00000,
  B00011,
  B00011,
  B00011,
  B00011,
  B00011,
  B00000,
};  //segmentul de inceput de bara numai jumatate

byte sfarsit[8] = {


  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
};  //segmentul de sfarsit de bara gol

byte sfarsitjumatate[8] = {

  B00000,
  B00110,
  B00110,
  B00110,
  B00110,
  B00110,
  B00000,
};  //segmentul de sfarsit de bara jumatate

volatile int valoare = 0;
float tensiune = 0;
int last_buttonStateUp = LOW;
int last_buttonStatedown =LOW;
void setup()
{
  
  pinMode(button_up,INPUT_PULLUP);  //activez pull-up intern pentru a nu mai folosi rezistenta pe placuta
  pinMode(button_down,INPUT_PULLUP);

  pinMode(motorPin,OUTPUT);
  

  lcd.begin(16, 2);
  lcd.noCursor();
  lcd.createChar(0, inceput);
  lcd.createChar(1, sfarsit);
  lcd.createChar(2, jumate);
  lcd.createChar(3, intreg);
  lcd.createChar(4, gol);
  lcd.createChar(5, sfarsitjumatate);
}

void loop()
{
  lcd.setCursor(0, 0);
  lcd.print("Command: ");
  if(valoare < 10)   lcd.print(" ");
  if(valoare < 100)  lcd.print(" ");

  lcd.print(valoare);  //PWM-ul curent
  lcd.print(" ");
  tensiune = (valoare * 5.0) / 256.0;
  lcd.setCursor(0, 1);
  lcd.print(tensiune);  //tensiunea curenta
  lcd.print("V");
  lcd.print(" ");
  if(tensiune >= 0.55)   lcd.write((byte)3);
  else
  {
    if(tensiune >= 0.30)  lcd.write((byte)2);
    else lcd.write((byte)0); 
  }

  for(int i = 0; i < 7; i++)
  {
    if(tensiune >= 1.1 + i * 0.55)  lcd.write((byte)3);
    else
    {
      if(tensiune >= 30 + i * 0.55)  lcd.write((byte)2);
      else lcd.write((byte)4);
    }
  }

  if(tensiune >= 4.95)  lcd.write((byte)1);
  else
  {
    if(tensiune >= 4.7)  lcd.write((byte)5);
    else lcd.write((byte)1);
  }

  delay(500);

//Handle button_up
  byte buttonStateUp = digitalRead(button_up);

  if (buttonStateUp != last_buttonStateUp) {
    last_buttonStateUp = buttonStateUp;

    if (buttonStateUp == LOW && valoare < 255) {
      valoare = valoare + 10;
      analogWrite(motorPin, valoare);
      delay(50);
    }
  }

  // Handle button_down
  byte buttonStatedown = digitalRead(button_down);

  if (buttonStatedown != last_buttonStatedown) {
    last_buttonStatedown = buttonStatedown;

    if (buttonStatedown == LOW && valoare > 0) {
      valoare = valoare - 10;
      analogWrite(motorPin, valoare);
      delay(50);
    }
  }
}