#include <LiquidCrystal_I2C.h>

#define sda_pin 15
#define scl_pin 16

/*
 * Set the LCD address to 0x27 (0x20 ~ 0x27)
 * for a 16 chars
 * 2 line display
 */
LiquidCrystal_I2C lcd(0x27, 16, 2);

void Init_LCD1602(){
  lcd.begin(sda_pin, scl_pin);
  lcd.backlight();
}

void Print_LCD(String s){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(s);
}

void Print_LCD(String s, String s2){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(s);
  lcd.setCursor(0, 1);
  lcd.print(s2);
}