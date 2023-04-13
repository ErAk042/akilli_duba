#include <Servo.h>
#include "Arduino.h"
#include "Wire.h"
#include "DHT.h"
#define I2Caddress 0x48
#define UVLED 8
#define IRLED 9
#define ISI A0
#define EN 7
#define DHTPIN 5   
#define SIM_EN 12
#define DHTTYPE DHT11 


String device_name = "Fenlisesi_Duba"; // Name your station
String dataout;


unsigned int readings[20] = {0};
unsigned char readCnt = 0;
Servo myServo;
DHT dht(DHTPIN, DHTTYPE);


//----------------------------------------------------------------------------------------------------**
//----------------------------------------------------------------------------------------------------**


void updateSerial()
{
  delay(20);
  while (Serial.available())
  {
    Serial1.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while (Serial1.available())
  {
    Serial.write(Serial1.read());//Forward what Software Serial received to Serial Port
  }
}


void UplinkData(String dataName, String dataType, float dataval)
{
  Serial.flush(); // Make sure that serial is flushed

  Serial1.flush(); // Make sure software serial is flushed

  delay(2000);

  
  //String json = "{\"key\":\"" + device_name + "\",\"measurement_name\":\"" + "Humidity" + "\",\"unit\":\"" + "Percentage" + "\",\"value\":" + String(dataval) + ",\"timestamp\":" + 456 + ",\"latitude\":" + 70.0 + ",\"longtitude\":" + 30.0 + ",\"hardware\":\"None\"}";
  
  String json = "{\"key\":\"" + device_name + "\",\"measurement_name\":\"" + dataName + "\",\"unit\":\"" + dataType + "\",\"value\":" + String(dataval) + ",\"timestamp\":\""+ "1673818606" + "\",\"lat\":" + 70.0 + ",\"lon\":" + 30.0 + ",\"hardware\":\"None\"}";
  
  //String json = "{\"key\":\"" + device_name + "\",\"measurement_name\":\"" + "Humidity" + "\",\"unit\":\"" + "Percentage" + "\",\"value\":" + String(dataval) + ",\"timestamp\":" + 1 + ",\"latitude\":\"30\",\"longtitude\":\"30\"}";
  //  const char *fulljson = json.c_str(); // Because SoftwareSerial truncates pure strings, save json as a char

  // How many bytes the server should expect , how many seconds/1000 the server should wait for those bytes. Set to 120 bytes for now, trouble calculating sizeof(json)
  // Set to 89 if no date time
  // Set to 120 if date time
  Serial1.println("AT+HTTPDATA=" + String(json.length()) + ",10000");

  delay(2000);

  updateSerial();

  delay(2000);
  updateSerial();
  Serial1.println(json);
  delay(5000);
  updateSerial();
  Serial1.println("AT+HTTPACTION=1"); // 1 for post, 0 for get

  delay(5000);

  updateSerial();

  Serial1.println("AT+HTTPREAD"); // Read response, should be 1, 500, 21 and also will say Internal server error even though data has uploaded

  delay(5000);

  updateSerial();

  delay(1000);

}


void intBaglan()
{
  Serial1.println("AT"); //Once the handshake test is successful, it will back to OK

  updateSerial(); // Return the response to the serial port for display purposes, and print anything from either serial or SoftwareSerial. May be able to remove in actual deployment pending testing

  Serial1.println("AT+CSQ"); //Signal quality test, value range is 0-31 , 31 is the best

  delay(3000);

  updateSerial();

  Serial1.println("AT+CCID"); //Read SIM information to confirm whether the SIM is plugged in

  delay(1000);

  updateSerial();

  Serial1.println("AT+CREG?"); //Check whether it has registered in the network

  delay(1000);

  updateSerial();

  Serial1.println("AT+CGREG?"); // Check to see if the device is registered again

  delay(1000);

  updateSerial();

  Serial1.println("AT+COPS?"); // Returns the mobile network name i.e. T-mobile

  delay(1000);

  updateSerial();

  Serial1.println("AT+cgatt=1");

  delay(1000);

  updateSerial();

  Serial1.println("AT+CMGF=1"); // Set to SMS text mode

  delay(2000);

  updateSerial();

  Serial1.println("AT+SAPBR=3,1,Contype,GPRS"); // Set connection type to GPRS

  delay(2000);

  updateSerial();

  Serial1.println("AT+SAPBR=3,1,APN,internet"); // Change "TM" to the APN provided by your carrier

  delay(2000);

  updateSerial();

  Serial1.println("AT+CGACT=1,1"); // Perform GPRS attach, which must occur befpre PDP context can be established

  delay(2000);

  updateSerial();

  Serial1.println("AT+SAPBR=1,1"); // Enable bearer 1

  delay(5000);

  updateSerial();

  Serial1.println("AT+SAPBR=2,1"); // Check whether bearer 1 is open

  delay(3000);

  updateSerial();

  Serial1.println("AT+HTTPINIT"); // Initialize HTTP service

  delay(3000);

  updateSerial();

  Serial1.println("AT+HTTPPARA=CID,1"); // Set bearer profile ID

  delay(3000);

  updateSerial();

  Serial1.println("AT+HTTPPARA=URL,http://13.59.161.20:8080/api/v0p2/sensor"); // Server address

  delay(3000);

  updateSerial();

  Serial1.println("AT+HTTPPARA=\"CONTENT\",\"application/json\"");

  delay(2000);

  updateSerial();

  delay(1000);
}


//----------------------------------------------------------------------------------------------------**
//----------------------------------------------------------------------------------------------------**


/*int isiOku()
{
  float sonuc;
  int olcum = analogRead(ISI);
  float voltage = olcum * (5000.0 / 1023.0);
  sonuc = (voltage-500)/10;
  return sonuc;
}*/

float nemOlc()
{
  float sonuc = dht.readHumidity();
  return sonuc;
}


float isiOlc()
{
  float sonuc = dht.readTemperature();
  return sonuc;
}


void servoCevir(int sonPos)
{
  myServo.attach(10);
  int konum = myServo.read();
  int fark = sonPos - konum;
  int i;
  if (fark > 0)
  { 
    for (i = konum; i <= sonPos; i += 1)
    {  
      myServo.write(i);             
      delay(15);
    }
  }
  else if (fark < 0)
  { 
    for (i = konum; i >= sonPos; i -= 1)
    {  
      myServo.write(i);             
      delay(15);
    }
  }
  myServo.detach();
}


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
  int sonuc;
  sonuc = degerOku(0,4096,128);
  Serial.println("UV olcumu yapılıyor.");
  servoCevir(0);
  delay(1000);
  delay(100);
  digitalWrite(UVLED, HIGH);
  int i = 0;
  delay(50);
  /*for(i=0; i<=1000; i++)
  {
    sonuc = degerOku(0,4096,128);
    Serial.println(sonuc);
    delay(20);
  }*/
  sonuc = degerOku(0,4096,128);
  digitalWrite(UVLED, LOW);
  servoCevir(75);
  return sonuc;
}


int pilVoltajOku()
{
  degerOku(1,4096,128);
  delay(50);
  int sonuc = degerOku(1,4096,128);
  return 11*sonuc;
}


int IROlcum()
{
  int sonuc;
  sonuc = degerOku(2,4096,128);
  Serial.println("IR olcumu yapılıyor.");
  servoCevir(0);
  delay(200);
  digitalWrite(IRLED, HIGH);
  delay(500);
  sonuc = degerOku(2,4096,128);
  delay(50);
  digitalWrite(IRLED, LOW);
  servoCevir(75);
  return sonuc;
}


void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(EN, OUTPUT);
  pinMode(SIM_EN, OUTPUT);
  digitalWrite(SIM_EN, LOW);
  digitalWrite(EN, HIGH);
  delay(5000);
  dht.begin();
  // Join the I2C bus as a master (call this only once)
  Wire.begin();

  servoCevir(75);
  delay(200);
  Serial.println("Setup completed.");
  digitalWrite(UVLED, HIGH);
  delay(500);
  digitalWrite(UVLED, LOW);
}


void loop()
{
  
  Serial.println("w");
  delay(50);
  Serial.println("c");
  //digitalWrite(EN, HIGH);
  //delay(7000);
  Serial.println("d");
  int UVVoltaj = UVOlcum();
  delay(2000);
  Serial.println("e");
  int IRVoltaj = IROlcum();
  delay(2000);
  int pilVolt = pilVoltajOku();
  delay(500);
  int isi = isiOlc();
  delay(500);
  int nem = nemOlc();
  delay(500);
  //digitalWrite(EN, LOW);
  

  Serial1.println("AT+CFUN?");
  updateSerial();
  Serial1.println("AT+CFUN=1");
  updateSerial();
  //digitalWrite(SIM_EN, HIGH);
  delay(10000);
  intBaglan();
  UplinkData("klorofil", "mV", UVVoltaj);
  UplinkData("bulanıklık", "mV", IRVoltaj);
  UplinkData("pil_voltaj", "mV", pilVolt);
  UplinkData("ısı", "°C", isi);
  UplinkData("nem", "%", nem);
  delay(1000);
  Serial.println("a");
  Serial1.println("AT+CFUN=0");
  updateSerial();
  Serial.println("b");
  //digitalWrite(SIM_EN, LOW);
  delay(3600000);
  
  /*int data;
  if (Serial.available() > 0) 
  {
    // read the incoming byte:
    data = Serial.read();
    
    switch(data)
    {
      case '0':
        Serial.println(UVOlcum());
        delay(300);
      break;
      case '1':
        Serial.println(pilVoltajOku());
      break;
      case '2':
        Serial.println(IROlcum());
        delay(300);
      break;
      case '3':
        Serial.println(isiOlc());
        Serial.println(nemOlc());
      break;
    }
  }*/

  //Serial.println(UVOlcum());
  
}
