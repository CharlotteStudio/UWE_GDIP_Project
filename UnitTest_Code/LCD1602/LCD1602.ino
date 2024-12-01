#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define sda_pin 15
#define scl_pin 16

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{
  Serial.begin(115200);
  
  lcd.begin(sda_pin, scl_pin);
  lcd.backlight();

  Serial.println("Start LiquidCrystal I2C");
}

void loop()
{
  PrintHello();
  delay(5000);
}

void PrintHello(){
  lcd.clear();
  lcd.print("Hello, world!");
  delay(3000);

  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Hello?");
  delay(3000);

  lcd.rightToLeft();
  lcd.setCursor(5, 1);
  lcd.print("Hello?");
  delay(3000);

  lcd.leftToRight();
}
