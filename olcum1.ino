#include <Servo.h>
#include "Arduino.h"
#include "Wire.h"
#define I2Caddress 0x48
#define UVLED 8
#define IRLED 9


//void servoCevir(int )


unsigned int readings[20] = {0};
unsigned char readCnt = 0;
Servo myServo;

int degerOku(int girisNo, int maxVolt, int ornekleme)
{
  Wire.beginTransmission(I2Caddress);
  Wire.write(0b00000001);
  int girisBit;
  int kazancBit;
  int SPSBit;
  int MSB;
  int LSB;
  switch (girisNo)
  {
    case 0:
      girisBit = 64;
    break;
    case 1:
      girisBit = 80;
    break;
    case 2:
      girisBit = 96;
    break;
    case 3:
      girisBit = 112;
    break;
    default:
      girisBit = 64;
  }

  switch (maxVolt)
  {
    case 6144:
      kazancBit = 0;
    break;
    case 4096:
      kazancBit = 2;
    break;
    case 2048:
      kazancBit = 4;
    break;
    case 1024:
      kazancBit = 6;
    break;
    case 512:
      kazancBit = 8;
    break;
    case 256:
      kazancBit = 10;
    break;
    default:
      kazancBit = 0;
  }

  switch (ornekleme)
  {
    case 8:
      SPSBit = 0;
    break;
    case 16:
      SPSBit = 32;
    break;
    case 32:
      SPSBit = 64;
    break;
    case 64:
      SPSBit = 96;
    break;
    case 128:
      SPSBit = 128;
    break;
    case 250:
      SPSBit = 160;
    break;
    case 475:
      SPSBit = 192;
    break;
    case 860:
      SPSBit = 224;
    break;
    default:
      SPSBit = 128;
  }

  MSB = girisBit + kazancBit;
  LSB = SPSBit + 3;
  Wire.write(MSB);
  Wire.write(LSB);
  Wire.endTransmission();
  //Serial.println(MSB);
  //Serial.println(LSB);
  Wire.beginTransmission(I2Caddress);

  Wire.write(0);

  Wire.endTransmission();

  // =======================================

  Wire.requestFrom(I2Caddress, 2);

  uint16_t convertedValue;

  convertedValue = (Wire.read() << 8 | Wire.read());
  int sonuc = map(convertedValue, 0, 32767, 0, maxVolt);
  
  return sonuc;
}


int UVOlcum()
{
  Serial.println("UV olcumu yapılıyor.");
  myServo.attach(10);
  myServo.write(0);
  delay(1000);
  myServo.detach();
  delay(100);
  digitalWrite(UVLED, HIGH);
  int i = 0;
  int sonuc;
  delay(50);
  /*for(i=0; i<=1000; i++)
  {
    sonuc = degerOku(0,4096,128);
    Serial.println(sonuc);
    delay(20);
  }*/
  sonuc = degerOku(0,4096,128);
  digitalWrite(UVLED, LOW);
  myServo.attach(10);
  myServo.write(75);
  return sonuc;
}


int IROlcum()
{
  Serial.println("IR olcumu yapılıyor.");
  myServo.attach(10);
  myServo.write(0);
  delay(1000);
  myServo.detach();
  delay(100);
  digitalWrite(IRLED, HIGH);
  delay(500);
  int sonuc = degerOku(2,4096,128);
  delay(50);
  digitalWrite(IRLED, LOW);
  myServo.attach(10);
  myServo.write(75);
  return sonuc;
}



void setup()
{
  Serial.begin(9600);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);

  // Join the I2C bus as a master (call this only once)
  Wire.begin();

  myServo.attach(10);
  myServo.write(75);
  delay(500);
  Serial.println("Setup completed.");
  myServo.detach();
  digitalWrite(UVLED, HIGH);
  delay(500);
  digitalWrite(UVLED, LOW);
}

void loop()
{
  int data;
  if (Serial.available() > 0) 
  {
    // read the incoming byte:
    data = Serial.read();
    
    switch(data)
    {
      case '0':
        Serial.println(UVOlcum());
        delay(300);
        myServo.detach();
      break;
      case '2':
        Serial.println(IROlcum());
        delay(300);
        myServo.detach();
      break;
    }
  }

  //Serial.println(UVOlcum());

  
}
