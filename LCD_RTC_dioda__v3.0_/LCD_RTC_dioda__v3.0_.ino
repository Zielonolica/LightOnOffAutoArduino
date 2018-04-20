//wyswietlacz LCD 2x16, podejscie 1. :P
//v3.0 -> z diodką
//proba gita

#include <Wire.h>
//#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <DS1302.h>
#include <stdio.h>
#include <time.h>

#define I2C_ADDR         0x27
#define BACKLIGHT_PIN     3
#define En_pin            2
#define Rw_pin            1
#define Rs_pin            0
#define D4_pin            4
#define D5_pin            5
#define D6_pin            6
#define D7_pin            7
short int dioda = 3;
constexpr int BRIGHTMAX = 2000;
int brightness = BRIGHTMAX;
int deltaB = 10;

//initialize the LCD -> lcd(RS, En, d4, d5, d6, d7)
LiquidCrystal_I2C lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);

//initialize DS1302 RTC - stworzenie obiektu; parametry to piny RST, DAT, CLK
DS1302 zegarek(5, 6, 7);

void print2digits(int n)
{
  char buf[3] = {0};
  buf[0] = '0' + n / 10;
  buf[1] = '0' + n % 10;
  lcd.print(buf);
}

void setup() {
  lcd.begin(16,2);
  pinMode(dioda, OUTPUT);

  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH);

  zegarek.writeProtect(false);
  zegarek.halt(false);

  lcd.setCursor(5,0);
  lcd.print("R T C");
  lcd.setCursor(3,1);
  lcd.print("activated");
  delay(2000);
  lcd.clear();
}

void loop() {
  
  if (brightness >= deltaB)
  {
    brightness -= deltaB;
    lcd.setCursor(0,1);
    lcd.print(brightness);
  }  
  else //(brightness < delta)
  {
    brightness = 0;
    lcd.setCursor(0,1);
    lcd.print("off");
  }
  
  analogWrite(dioda, map(brightness, 0, BRIGHTMAX, 0, 255));
  
  Time t = zegarek.time();

  lcd.setCursor(3,0);
  print2digits(t.date);
  lcd.print(".");
  lcd.setCursor(6,0);
  print2digits(t.mon);
  lcd.print(".");
  lcd.setCursor(9,0);
  lcd.print(t.yr);

  lcd.setCursor(4,1);
  print2digits(t.hr);
  lcd.print(":");
  lcd.setCursor(7,1);
  print2digits(t.min);
  lcd.print(":");
  lcd.setCursor(10,1);
  print2digits(t.sec);
  
  delay(100);
}
