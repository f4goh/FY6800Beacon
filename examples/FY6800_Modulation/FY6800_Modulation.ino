/*
   F4GOH Anthony JN07DV
   Use BY CC NA
   hellschreiber from http://brainwagon.org/2012/01/11/hellduino-sending-hellschreiber-from-an-arduino/
   Set the date and time by entering the following on the Arduino

   serial monitor:
      year,month,day,hour,minute,second,

   Where
      year can be two or four digits,
      month is 1-12,
      day is 1-31,
      hour is 0-23, and
      minute and second are 0-59.
      2016,6,18,16,32,00,

      freq wspr
      80 m : 3.592600
      40 m : 7.038600
      30 m : 10.138700
      20 m : 14.095600
      15 m : 21.094600

rsid
WMF07041500000000
WMN1
WPF5
WPM1
WFM00000180000000
WMN0

rtty
WMF07041500000000
WMN1
WPF0
WPM3
WFK07041670000000
WMN0

hell
WMF07041500000000
WMN1
WPF0
WPM3
WFK00000000,000000
WMN0

psk
WMF07041500000000
WMN1
WPF2
WPM3
WMN0

cw
WMF07041500000000
WMN1
WPF0
WPM3
WFK00000000,000000

wspr
WMF07040000000000
WMN1
WPF5
WPM1
WFM00000005000000

*/



#include <MODULATIONFY.h>
#include <SoftwareSerial.h>
#include <DS3232RTC.h> //http://github.com/JChristensen/DS3232RTC
#include <Wire.h>
#include <Time.h>
#include <LiquidCrystal_I2C.h>

tmElements_t tm;

#define modPin A0     //fsk/psk output
#define vcoPin 11     //vco out output


char txString[] = "...F4GOH F4GOH Beacon JN07DV JN07DV please report F4GOH@orange.fr F4GOH@orange.fr tks sk...";
char txCall[] = "F4GOH";
char txLoc[] = "JN07";
char txPow[] = "00";


#define TRX_RXD 6   // We *receive* Serial from the TX FY6800 generator on this pin.

#define TRX_TXD 7   // We *send* serial to the RX FY6800 generator on this pin.

SoftwareSerial fySerial(TRX_RXD, TRX_TXD);    //for CAT system
LiquidCrystal_I2C lcd(0x27, 20, 4); //4 lines *20 columns lcd char


int pskSpeed = 31;

void setup() {
  Serial.begin(115200);
  lcd.begin();
  lcd.setBacklight(HIGH);
  lcd.clear();
  lcd.print(F("FY6800  F4GOH 2018")); //intro

  fySerial.begin(115200);


  Mod.begin(7041500, modPin, vcoPin, &fySerial);

  Mod.rsidToggle(1);


  //  majRtc();    //uncomment to set RTC

}




void loop() {
  //bridge();
  //test();
  syncTime();

}

void syncTime()
{
  RTC.read(tm);

  if ((tm.Minute & 0x01) == 0 && tm.Second == 0) {
    txing(3);   //1 : rtty, 2: hellschreiber, 3: wspr, 4: cw, 5: psk 31 63 125
  }
  Serial.print(tm.Hour);
  Serial.print(":");
  Serial.print(tm.Minute);
  Serial.print(":");
  Serial.println(tm.Second);
  lcd.setCursor(0, 2);
  lcd.print(tm.Hour);
  lcd.print(":");
  lcd.print(tm.Minute);
  lcd.print(":");
  lcd.print(tm.Second);
  lcd.print("   ");
}


void bridge()
{
  char c;
  if (Serial.available() > 0)
  {
    fySerial.write(Serial.read());
  }
  if (fySerial.available() > 0)
  {
    Serial.write(fySerial.read());
  }

}

void test()
{
  char car;
  if (Serial.available() > 0) {
    car = Serial.read();
    if (car > '0' && car < '6') {
      txing(car - '0');
    }
    else {
      Serial.println("error");
    }
  }
}


void txing(byte mod)
{
  switch (mod)
  {
    case  1: Serial.println("rtty");
      lcd.setCursor(0, 3);
      lcd.print("tx rtty");
      Mod.rttyTx(txString);
      break;
    case  2: Serial.println("hell");
      lcd.setCursor(0, 3);
      lcd.print("tx hell");
      Mod.hellTx(txString);
      break;
    case  3 : Serial.println("wspr");
      lcd.setCursor(0, 3);
      lcd.print("tx wspr");
      Mod.centerFreq=7038600+1400;
      Mod.wsprTx(txCall, txLoc, txPow);
      break;
    case  4: Serial.println("cw");
      lcd.setCursor(0, 3);
      lcd.print("tx cw");
      Mod.cwTx(txString, 18);
      break;
    case  5: Serial.println("psk");
      lcd.setCursor(0, 3);
      lcd.print("tx psk");
      Mod.pskTx(txString, pskSpeed);
      break;
  }
  lcd.setCursor(0, 3);
  lcd.print("tx off    ");
}



void majRtc()
{
  time_t t;
  //check for input to set the RTC, minimum length is 12, i.e. yy,m,d,h,m,s
  Serial.println(F("update time : format yy,m,d,h,m,s"));
  Serial.println(F("exemple : 2016,6,18,16,32,30, "));

  while (Serial.available () <= 12) {
  }
  //note that the tmElements_t Year member is an offset from 1970,
  //but the RTC wants the last two digits of the calendar year.
  //use the convenience macros from Time.h to do the conversions.
  int y = Serial.parseInt();
  if (y >= 100 && y < 1000)
    Serial.println(F("Error: Year must be two digits or four digits!"));
  else {
    if (y >= 1000)
      tm.Year = CalendarYrToTm(y);
    else    //(y < 100)
      tm.Year = y2kYearToTm(y);
    tm.Month = Serial.parseInt();
    tm.Day = Serial.parseInt();
    tm.Hour = Serial.parseInt();
    tm.Minute = Serial.parseInt();
    tm.Second = Serial.parseInt();
    t = makeTime(tm);
    RTC.set(t);        //use the time_t value to ensure correct weekday is set
    setTime(t);
    while (Serial.available () > 0) Serial.read();
  }
}


