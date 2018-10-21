#FY 6800 Generator as Beacon #
F4GOH Anthony f4goh@orange.fr <br>

October 2018

Arduino 1.8.5

## Update protocols ##
- 	LF, HF full band.
-	Power : 19dbm on 40 meters.
-	Modes : BPSK , RTTY, Hellschreiber, CW, WSPR.
-	Trigger input for FSK modulation.
-	VCO input for FM modulation.
-	Arduino UNO
-	LCD 4×20 display.
-	RTC (for WSPR) 
-	Breadboard wiring.
https://github.com/f4goh/FY6800Beacon

## Installation ##
To use the FY6800Beacon library:  
- Go to https://github.com/f4goh/FY6800Beacon, click the [Download ZIP](https://github.com/f4goh/FY6800Beacon/archive/master.zip) button and save the ZIP file to a convenient location on your PC.
- Uncompress the downloaded file.  This will result in a folder containing all the files for the library, that has a name that includes the branch name, usually FY6800Beacon-master.
- Rename the folder to  FY6800Beacon.
- Copy the renamed folder to the Arduino sketchbook\libraries folder.

- you must add Arduino RTC library : <br>
  Go to http://github.com/JChristensen/DS3232RTC

## Usage notes ##


To use 4x20 characters LCD Display, the LiquidCrystal_I2C and WIRE libraries must also be included.


```c++
#include <MODULATIONFY.h>
#include <SoftwareSerial.h>
#include <DS3232RTC.h>
#include <Wire.h>
#include <Time.h>
#include <LiquidCrystal_I2C.h>
```

Change your i/o pin

```c++
#define modPin A0     //fsk/psk output can be change
#define vcoPin 11     //vco out output pin 11 only pwm
```
change serial pin i need

```c++
#define TRX_RXD 6   // We *receive* Serial from the TX FY6800 generator on this pin.
#define TRX_TXD 7   // We *send* serial to the RX FY6800 generator on this pin.
```
Set frequency and active RSID or not

```c++
 Mod.begin(7041500, modPin, vcoPin, &fySerial);
 Mod.rsidToggle(1);
```

set RTC if need

```c++
 //  majRtc();    //uncomment to set RTC
```

choose mode by select one of these functions

```c++
void loop() {
  //bridge();
  //test();
  syncTime();
}
```
