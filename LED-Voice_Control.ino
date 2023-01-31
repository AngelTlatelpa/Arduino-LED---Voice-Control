/**

    *************
    Blue is 0
    Red is 1
    Purple is 2
    Off is 3 
    Orange is 4
    *************

    
  */
  
#include <SoftwareSerial.h>
#include "VoiceRecognitionV3.h"

    
  
VR myVR(2,3);   

uint8_t records[7]; 
uint8_t buf[64];

int redPin = 9;
int greenPin = 10;
int bluePin = 11;

#define BlueRecord (0)
#define RedRecord (1)
#define PurpleRecord (2)  
#define OffRecord (3)  
#define OrangeRecord  (4)    

void printSignature(uint8_t *buf, int len)
{
  int i;
  for(i=0; i<len; i++){
    if(buf[i]>0x19 && buf[i]<0x7F){
      Serial.write(buf[i]);
    }
    else{
      Serial.print("[");
      Serial.print(buf[i], HEX);
      Serial.print("]");
    }
  }
}


void printVR(uint8_t *buf)
{
  Serial.println("VR Index\tGroup\tRecordNum\tSignature");

  Serial.print(buf[3], DEC);
  Serial.print("\t\t");

  if(buf[0] == 0xFF){
    Serial.print("NONE");
  }
  else if(buf[0]&0x80){
    Serial.print("UG ");
    Serial.print(buf[0]&(~0x80), DEC);
  }
  else{
    Serial.print("SG ");
    Serial.print(buf[0], DEC);
  }
  Serial.print("\t");

  Serial.print(buf[1], DEC);
  Serial.print("\t\t");
  if(buf[4]>0){
    printSignature(buf+4, buf[4]);
  }
  else{
    Serial.print("NONE");
  }
  Serial.println("\r\n");
}

void setColor(int red, int green, int blue)
{
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}
void setup()
{
  /** initialize */
  myVR.begin(9600);
  
  Serial.begin(115200);
  Serial.println("Elechouse Voice Recognition V3 Module\r\nControl LED sample");
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);  
  
    
  if(myVR.clear() == 0){
    Serial.println("Recognizer cleared.");
  }else{
    Serial.println("Not find VoiceRecognitionModule.");
    Serial.println("Please check connection and restart Arduino.");
    while(1);
  }
  
  if(myVR.load((uint8_t)BlueRecord) >= 0){
    Serial.println("BlueRecord loaded");
  }
  
  if(myVR.load((uint8_t)RedRecord) >= 0){
    Serial.println("RedRecord loaded");
  }
   if(myVR.load((uint8_t)PurpleRecord) >= 0){
   Serial.println("PurpleRecord loaded");
  }
     if(myVR.load((uint8_t)OffRecord) >= 0){
   Serial.println("OffRecord loaded");
  }
     if(myVR.load((uint8_t)OrangeRecord) >= 0){
   Serial.println("OrangeRecord loaded");
  }
}

void loop()
{
  int ret;
  ret = myVR.recognize(buf, 50);
  if(ret>0){
    switch(buf[6]){
      case BlueRecord:
        setColor(0, 0, 255);  
        break;
      case RedRecord:      
        setColor(255, 0, 0);  
        break;
      case PurpleRecord:
        setColor(80, 0, 80);
        break;
      case OffRecord:
        setColor(0, 0, 0);  
        break;
      case OrangeRecord:
        setColor(80, 0, 80);
        break;
      default:
        Serial.println("Record function undefined");
        break;
    }  
    printVR(buf);
  }
}
