/********************************************************************************************
 * MODULATIONFY Arduino library
 * Created 12/10/2018
 * Anthony LE CREN f4goh@orange.fr 
 * thanks to Christophe Caiveau f4goj@free.fr 
 * BY NC SA
 *
 * Instance :
 *
 * Functions :
 *
 *******************************************************************************************/
 
 
 
#ifndef MODULATIONFY_H
#define MODULATIONFY_H
#include <Arduino.h>
#include <SoftwareSerial.h>


class MODULATIONFY
{
  public:
    MODULATIONFY();

    byte mod_pin;
    byte vco_pin;


    void begin(float freq, byte mod, byte vco, SoftwareSerial * ss);
    String convFreq(float fmod);
    void rsidToggle(boolean rsidEnable);
    void cwTx(char * stringCw, int cwWpm);
    void rttyTx(char * stringRtty);
    void hellTx(char * stringHell);
    void wsprTx(char * callWsprTx, char * locWsprTx, char * powWsprTx);
    void wsprEncode(char * callWsprProc, char * locWsprProc, char * powWsprProc);
    void pskTx(char * stringPsk, int baudsPsk);

    SoftwareSerial * fySerial;

    byte wsprSymb[162];
    int wsprSymbGen;
    void rttyTxByte (char c);
    void sendCmd(String cmd);

    void rsidTx(int modeRsid);
    float centerFreq;
	
  private:
    void pskIdle(unsigned int bauds);
    byte parity(unsigned long tempo);
    byte rsidTxEnable;

};

extern MODULATIONFY Mod;

#endif
