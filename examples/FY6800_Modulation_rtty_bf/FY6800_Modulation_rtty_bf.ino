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


#include <SoftwareSerial.h>

#define modPin A0     //fsk/psk output
#define vcoPin 11     //vco out output


char txString[] = "...F4GOH F4GOH Beacon JN07DV JN07DV please report F4GOH at orange.fr tks sk...\n";


#define TRX_RXD 6   // We *receive* Serial from the TX FY6800 generator on this pin.
#define TRX_TXD 7   // We *send* serial to the RX FY6800 generator on this pin.

SoftwareSerial fySerial(TRX_RXD, TRX_TXD);    //for CAT system

int stateTX = 0;


void setup() {
  Serial.begin(115200);
  fySerial.begin(115200);

  pinMode(modPin, OUTPUT);
  digitalWrite(modPin, 0);


  // Mod.rsidToggle(1);


  //  majRtc();    //uncomment to set RTC

}




void loop() {
  char c;
  if (Serial.available() > 0)
  {
    c = Serial.read();
    switch (c) {
      case '1' : hf_config();
        break;
      case '2' : bf_config();
        break;
      case 'R' : stateTX = 1;
        //sendCmd("WMN1");
        break;
      case 'S' : stateTX = 0;
        sendCmd("WMN0");
        break;

    }
  }
  if (stateTX == 1) {
    rttyTx(txString);
  }
}



void hf_config()
{
  String f = "07041500000000";
  sendCmd("WMF" + f);
  sendCmd("WMN1");
  sendCmd("WPF0");
  sendCmd("WPM3");
  f = "07041670000000";
  sendCmd("WFK" + f);
}

void bf_config()
{
  String f = "00001500000000";
  sendCmd("WMF" + f);
  sendCmd("WMN1");
  sendCmd("WPF0");
  sendCmd("WPM3");
  f = "00001670000000";
  sendCmd("WFK" + f);
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

/********************************************
   RTTY  ajouter la config du ch2
 ********************************************/
void rttyTx(char * stringRtty)
{
  const static int TableRtty[59] PROGMEM = {4, 22, 17, 5, 18, 0, 11, 26, 30, 9, 0, 0, 6, 24, 7, 23, 13, 29, 25, 16, 10, 1, 21, 28, 12, 3, 14, 15, 0, 0, 0, 19, 0, 24, 19, 14, 18, 16, 22, 11, 5, 12, 26, 30, 9, 7, 6, 3, 13, 29, 10, 20, 1, 28, 15, 25, 23, 21, 17};

  int signlett = 1;  // RTTY Baudot signs/letters tables toggle
  char c;
  c = *stringRtty++;


  while ( c != '\0')
  {
    //Serial.print(c);
    c = toupper(int(c)); // Uppercase
    if (c == 10) // Line Feed
    {
      rttyTxByte(8);
    }
    else if (c == 13) // Carriage Return
    {
      rttyTxByte(2);
    }
    else if (c == 32) // Space
    {
      rttyTxByte(4);
    }
    else if (c > 32 && c < 91)
    {
      c = c - 32;
      if (c < 33)
      {
        if (signlett == 1)
        {
          signlett = 0;      // toggle form signs to letters table
          rttyTxByte(27);  //
        }
      }
      else if (signlett == 0)
      {
        signlett = 1;          // toggle form letters to signs table
        rttyTxByte(31);      //
      }
      rttyTxByte(int(pgm_read_word(&TableRtty[int(c)])));  // Send the 5 bits word
    }
    c = *stringRtty++;  // Next character in string
  }

}



void rttyTxByte(char c) { //shift 170Hz
  int val;
  c = (c << 2) + 3;
  for (int b = 7; b >= 0; b--) // MSB first
  {
    val = bitRead(c, b); // Read 1 bit
    if (val == 0) digitalWrite(modPin, LOW); else digitalWrite(modPin, HIGH); // Let's transmit (bit 1 is shifted)
    delay(22); // Gives the baud rate
  }
}

void sendCmd(String cmd)
{
  char c = 0;
  fySerial.println(cmd);
  Serial.println(cmd);
  do
  {
    if (fySerial.available() > 0)
    {
      c = fySerial.read();
    }
  }
  while (c != 0xa);
}
