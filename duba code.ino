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


String device_name = "Fenlisesi_Fake_Buoy2"; // Name your station
String dataout;


/*unsigned int readings[20] = {0};
unsigned char readCnt = 0;*/
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

  
  String json = "{\"key\":\"" + device_name + "\",\"measurement_name\":\"" + dataName + "\",\"unit\":\"" + dataType + "\",\"value\":" + String(dataval) + ",\"timestamp\":\""+ "1673818606" + "\",\"lat\":" + 70.0 + ",\"lon\":" + 30.0 + ",\"hardware\":\"None\"}";
  
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

  Serial1.println("AT+HTTPREAD"); // Read response
  
  delay(5000);

  updateSerial();

  delay(1000);

}


void intConnect()
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

  Serial1.println("AT+CGATT=1");

  delay(1000);

  updateSerial();

  Serial1.println("AT+CMGF=1"); // Set to SMS text mode

  delay(2000);

  updateSerial();

  Serial1.println("AT+SAPBR=3,1,Contype,GPRS"); // Set connection type to GPRS

  delay(2000);

  updateSerial();

  Serial1.println("AT+SAPBR=3,1,APN,internet"); // Change "internet" to the APN provided by your carrier

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


float getHumidity()
{
  float val = dht.readHumidity();
  return val;
}


float getTemperature()
{
  float val = dht.readTemperature();
  return val;
}


void turnServo(int targetPos)
{
  // Turns the servo motor slover since fast movements cause magnets to decouple from each other. 
  // Attaches servo at the start and detaches at the end to avoid any excessive energy consumption.
  myServo.attach(10);
  int curPos = myServo.read();
  int diff = targetPos - curPos;
  int i;
  if (diff > 0)
  { 
    for (i = curPos; i <= targetPos; i += 1)
    {  
      myServo.write(i);             
      delay(15);
    }
  }
  else if (diff < 0)
  { 
    for (i = curPos; i >= targetPos; i -= 1)
    {  
      myServo.write(i);             
      delay(15);
    }
  }
  myServo.detach();
}


int readVal(int inpNo, int maxVolt, int SPS)
{
  // Start talking to ADS1115 
  Wire.beginTransmission(I2Caddress);
  Wire.write(0b00000001);
  int inpBit;
  int gainBit;
  int SPSBit;
  int MSB;
  int LSB;
  switch (inpNo)
  {
    // In order to manipulate MUX settings, you have to manipulate bits 14-12; in this case bits 6-4 of MSB. 
    // So we set a variable according to thoose bits and later, will att this to the MSB.
    case 0:
      inpBit = 64;
    break;
    case 1:
      inpBit = 80;
    break;
    case 2:
      inpBit = 96;
    break;
    case 3:
      inpBit = 112;
    break;
    default:
      inpBit = 64;
  }

  // In order to manipulate PGA settings, you have to manipulate bits 11-9; in this case bits 3-1 of MSB. 
  // So we set a variable according to thoose bits and later, will add this to the MSB.
  switch (maxVolt)
  {
    case 6144:
      gainBit = 0;
    break;
    case 4096:
      gainBit = 2;
    break;
    case 2048:
      gainBit = 4;
    break;
    case 1024:
      gainBit = 6;
    break;
    case 512:
      gainBit = 8;
    break;
    case 256:
      gainBit = 10;
    break;
    default:
      gainBit = 0;
  }

  // In order to manipulate PGA settings, you have to manipulate bits 7-5; in this case bits 7-5 of LSB. 
  // So we set a variable according to thoose bits and later, will add this to the LSB.
  switch (SPS)
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

  // Finally adding all the choices into MSB and LSB. Since we are not going to use any other parameter other bits are not changeable.
  // In future use cases it might be possible to use other features like single-shot conversion or alert modes but in our case we don't use them.
  MSB = inpBit + gainBit;
  LSB = SPSBit + 3;
  
  Wire.write(MSB);
  Wire.write(LSB);

  Wire.endTransmission();

  Wire.beginTransmission(I2Caddress);

  Wire.write(0);

  Wire.endTransmission();

  // =======================================

  // Requesting the result from ADS1115
  Wire.requestFrom(I2Caddress, 2);

  uint16_t convertedValue;

  // Reading 2 bytes, then putting MSB in front of the LSB in order to get the 16-bit value. 
  convertedValue = (Wire.read() << 8 | Wire.read());
  // Converting the 16-bit value into voltage readings.
  int val = map(convertedValue, 0, 32767, 0, maxVolt);
  
  return val;
}


int getUV()
{
  int sonuc;
  sonuc = readVal(0,4096,128);
  Serial.println("UV olcumu yapılıyor.");
  turnServo(0);
  delay(1000);
  delay(100);
  digitalWrite(UVLED, HIGH);
  int i = 0;
  delay(50);
  /*for(i=0; i<=1000; i++)
  {
    sonuc = readVal(0,4096,128);
    Serial.println(sonuc);
    delay(20);
  }*/
  sonuc = readVal(0,4096,128);
  digitalWrite(UVLED, LOW);
  turnServo(75);
  return sonuc;
}


int getBattVoltage()
{
  readVal(1,4096,128);
  delay(50);
  int sonuc = readVal(1,4096,128);
  return 11*sonuc;
}


int getIR()
{
  int sonuc;
  sonuc = readVal(2,4096,128);
  Serial.println("IR olcumu yapılıyor.");
  turnServo(0);
  delay(200);
  digitalWrite(IRLED, HIGH);
  delay(500);
  sonuc = readVal(2,4096,128);
  delay(50);
  digitalWrite(IRLED, LOW);
  turnServo(75);
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

  turnServo(75);
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
  int UVVoltage = getUV();
  delay(2000);
  Serial.println("e");
  int IRVoltage = getIR();
  delay(2000);
  int battVoltage = getBattVoltage();
  delay(500);
  int temp = getTemperature();
  delay(500);
  int hum = getHumidity();
  delay(500);
  //digitalWrite(EN, LOW);
  

  Serial1.println("AT+CFUN?");
  updateSerial();
  Serial1.println("AT+CFUN=1");
  updateSerial();
  //digitalWrite(SIM_EN, HIGH);
  delay(10000);
  intConnect();
  UplinkData("klorofil", "mV", UVVoltage);
  UplinkData("bulanıklık", "mV", IRVoltage);
  UplinkData("pil_voltaj", "mV", battVoltage);
  UplinkData("ısı", "°C", temp);
  UplinkData("nem", "%", hum);
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
        Serial.println(getUV());
        delay(300);
      break;
      case '1':
        Serial.println(getBattVoltage());
      break;
      case '2':
        Serial.println(getIR());
        delay(300);
      break;
      case '3':
        Serial.println(getTemperature());
        Serial.println(getHumidity());
      break;
    }
  }*/

  //Serial.println(getUV());
  
}