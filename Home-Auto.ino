#define BLYNK_PRINT Serial // Enables Serial Monitor

#define FAN V5
#define DOOR V6
#define LED1 V4
#define LED2 V9
#define H_VAL V7
#define T_VAL V8

#define DHTPIN 18
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

#define BLYNK_TEMPLATE_ID "TMPLmeYWC5WV"
#define BLYNK_DEVICE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "VcAsoAcXs2GKDLZ-_LTxhSN2HPBxeQL_"

//#define BLYNK_TEMPLATE_ID "TMPLdSPVwRZY"
//#define BLYNK_DEVICE_NAME "Quickstart Template"
//#define BLYNK_AUTH_TOKEN "t5Cl3jYVOpC2_U3k13t9bHfrmKxRRTfm"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>
#include "DHT.h"


char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Electronics";
char pass[] = "Electronics";

//Servo objects for doors
Servo rightServo;
Servo leftServo;

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

//light pins
int led1 = 5;
int led2 = 17;
// fan motor pin
int fan = 19;

//servoPos
int rightServoPos = 90;
int leftServoPos = 0;

BlynkTimer timer;


//FAN
BLYNK_WRITE(FAN) // Executes when the value of virtual pin 0 changes
{
  if (param.asInt() == 1)
  {
    // execute this code if the switch widget is now ON
    digitalWrite(fan, HIGH);
  }
  else
  {
    // execute this code if the switch widget is now OFF
    digitalWrite(fan, LOW);
  }
}

//LED
BLYNK_WRITE(LED1) // Executes when the value of virtual pin 4 changes
{
  if (param.asInt() == 1)
  {
    // execute this code if the switch widget is now ON
    digitalWrite(led1, HIGH);
  }
  else
  {
    digitalWrite(led1, LOW);
  }
}

BLYNK_WRITE(LED2) // Executes when the value of virtual pin 4 changes
{
  if (param.asInt() == 1)
  {
    // execute this code if the switch widget is now ON
    digitalWrite(led2, HIGH);
  }
  else
  {
    digitalWrite(led2, LOW);
  }
}
//Door
BLYNK_WRITE(DOOR) // Executes when the value of virtual pin 0 changes
{
  if (param.asInt() == 1)
  {
    // execute this code if the switch widget is now ON
    for (rightServoPos = 90, leftServoPos = 0; leftServoPos <= 45; leftServoPos += 1, rightServoPos -= 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      rightServo.write(rightServoPos);             // tell servo to go to position in variable 'pos'
      leftServo.write(leftServoPos);
    }
  }
  else
  {
    // execute this code if the switch widget is now OFF
    for (rightServoPos = 0, leftServoPos = 45; rightServoPos <= 90; leftServoPos -= 1, rightServoPos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      rightServo.write(rightServoPos);             // tell servo to go to position in variable 'pos'
      leftServo.write(leftServoPos);
    }
  }
}

void DhtUpdate()
{
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  Blynk.virtualWrite(T_VAL, t);
  Blynk.virtualWrite(H_VAL, h);
}

void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, millis() / 1000);
}
void setup() {

  Serial.begin(115200);
  dht.begin();

  // put your setup code here, to run once:
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(fan, OUTPUT);
  rightServo.attach(16);
  leftServo.attach(22);

  rightServo.write(rightServoPos);
  leftServo.write(leftServoPos);
  Blynk.begin(auth, ssid, pass);

  timer.setInterval(1000L, myTimerEvent);
  timer.setInterval(2000L, DhtUpdate);
}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();
  timer.run();
  
}
