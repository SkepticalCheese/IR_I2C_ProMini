/*
   

   
   Thanks to:
   - Ken Shirriff (http://arcfn.com) for IRsendDemo (Arduino-IRRemote)
   - Nicholas Zambetti (http://www.zambetti.com) for Wire Slave Receiver (Wire library)

   An IR LED must be connected to Arduino PWM pin 3.
   
*/

#include <IRremote.h>
#include <Wire.h>  

IRsend irsend;

unsigned long OutgoingCode;
byte incomingCode;
int codeOnBuffer = 0;

// Initialize Wire library
void setup()
{
  Wire.begin(8);                // join i2c bus with address #8, defaults rate to 100 kHz
  Wire.setClock (100000L);
  Wire.onReceive(receiveEvent); // register event

  Serial.begin(9600);
  Serial.println(F("Every time you press a key is a serial monitor we will send."));
}

// Loop waiting for an I2C message from the master
void loop() {
  delay(10);
  // Used to be 100

  if (codeOnBuffer) {
    sendIR (incomingCode);
    codeOnBuffer = 0;
    }
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  while (Wire.available()) {     // loop through all but the last
    byte c = Wire.read();        

    if (!codeOnBuffer) {
      codeOnBuffer = 1;
      incomingCode = c;
    }
//    Serial.print(F("Received:"));
//    Serial.println((int) c);
  }
}

void sendIR(byte IncomingByte) {

//    Serial.print(F("Parsing:"));
//    Serial.println((int) IncomingByte);

  switch (IncomingByte) {
    case 10 : OutgoingCode = 0xA2AD43BC;    break; // Blu ray play/pause
    case 11 : OutgoingCode = 0xA2AD03FC;    break; // Blu ray ON/OFF
    case 12 : OutgoingCode = 0xA2AD24DB;    break; // Blu ray Chapter +
    case 13 : OutgoingCode = 0xA2ADC43B;    break; // Blu ray Chapter -
    case 14 : OutgoingCode = 0xA2AD837C;    break; // Blu ray Eject
    case 15 : OutgoingCode = 0xA2AD14EB;    break; // Blu ray Sub
    /* Blu Ray
      Play/Pause  A2AD43BC
      ON/OFF      A2AD03FC
      Chapter +   A2AD24DB
      Chapter -   A2ADC43B
      Eject       A2AD837C
      Sub         A2AD14EB
    */

    case 20 : OutgoingCode = 0x20DFF00F;    break; // TV - TV
    case 21 : OutgoingCode = 0x20DF40BF;    break; // TV - Vol +
    case 22 : OutgoingCode = 0x20DFC03F;    break; // TV - Vol -
    case 23 : OutgoingCode = 0x20DF00FF;    break; // CH +
    case 24 : OutgoingCode = 0x20DF807F;    break; // CH -
    case 25 : OutgoingCode = 0x20DF906F;    break; // TV - Mute
    case 26 : OutgoingCode = 0x20DF23DC;    break; // TV - ON
    case 27 : OutgoingCode = 0x20DFA35C;    break; // TV - OFF
    case 28 : OutgoingCode = 0x20DF738C;    break; // HDMI 1
    default:  OutgoingCode = 0;             break;
      /* TV (prefix: 20DF)
        Key     LG  Flipped + Comp
        POWER   08  10EF
        MUTE    09  906F
        VOL +   02  40BF
        VOL -   03  C03F
        CH +    00  00FF
        CH -    01  807F
        TV      0F  F00F
        PWR ON  C4  23DC
        PWR OFF C5  A35C
        HDMI 1  CE  738C
        HDMI 2  CC  33CC
      */
  }

//    Serial.print(F("Outgoing:"));
//    Serial.println(OutgoingCode);
    
  if (OutgoingCode != 0) {
    irsend.sendNEC (OutgoingCode, 32);
//    Serial.println(F("Sent signal."));
  }

}
