#include<FirebaseArduino.h>
//#include <LCD.h>
#include<MQ2.h>
#include <ESP8266WiFi.h>
#include <LiquidCrystal_SR.h>
//#include <LiquidCrystal.h>
#include<DHT.h>
#define HOST "gasleakage-b6005.firebaseio.com"
#define AUTH "oafrzUMv1vrm4NhZnqN2BPTUqB5jfkh4r0l1xP6m"
#define DHTPIN 13
#define DHTTYPE DHT11
long MQ_PIN=A0;
int lpg, co, smoke;
float h, t;

DHT dht(DHTPIN, DHTTYPE);
MQ2 mq2(MQ_PIN);
LiquidCrystal_SR lcd(16, 5, 4);
//Data,clock,Letch



char ssid[] = "4pp37";   //network SSID (name)
char pass[] = "appelakib121****";   //network password
int keyIndex = 0;            // network key index number (needed only for WEP)
WiFiClient  client;

void setup() {
  //pinMode(2, OUTPUT);
  Serial.begin(115200);
  //delay(100);
  //WiFi.disconnect();
  WiFi.mode(WIFI_STA);

  Firebase.begin(HOST, AUTH);
  dht.begin();
  mq2.begin();

  lcd.begin(16, 2); // set up the LCD's number of columns and rows:
  lcd.clear(); // clear the screen
}

void loop() {

  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println();
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(500);
      readingData();
      delay(500);
    }
    Serial.println("\nConnected.");

  }
  delay(500);
  readingData();
  uploadData();
  delay(1500);




  // Wait 20 seconds to uodate the channel again
  //delay(1000);
}
//Reading the Data
void readingData() {
  //delay(2000);
  h = dht.readHumidity();
  t = dht.readTemperature();
  float* values = mq2.read(true);
  lpg = mq2.readLPG();
  co = mq2.readCO();
  smoke = mq2.readSmoke();
  delay(1000);
  /*digitalWrite(2, LOW);
  if (lpg >= 1000) {
    digitalWrite(2, HIGH);
  }*/
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print((String)"LPG: " + lpg + "  CO: " + co);
  lcd.setCursor(0, 1);
  lcd.print((String)"SMOKE: " + smoke);
  delay(3000);

  //delay(3000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print((String)"Temp: " + t);
  lcd.setCursor(0, 1);
  lcd.print((String)"Hum: " + h);
  delay(3000);
}
void uploadData() {
  Serial.println((String)"Lpg: " + lpg);
  Firebase.setInt("LPG", lpg);
  if (Firebase.failed()) {
    Serial.println("Failed to write the data");
    Serial.println(Firebase.error());
  }
  Serial.println((String)"CO: " + co);
  Firebase.setInt("CO", co);
  if (Firebase.failed()) {
    Serial.println("Failed to write the data");
    Serial.println(Firebase.error());

  }
  Serial.println((String)"Smoke: " + smoke);
  Firebase.setInt("SMOKE", smoke);
  if (Firebase.failed()) {
    Serial.println("Failed to write the data");
    Serial.println(Firebase.error());

  }
  Serial.println((String)"Temp: " + t);
  Firebase.setFloat("Temp", t);
  if (Firebase.failed()) {
    Serial.println("Failed to write the data");
    Serial.println(Firebase.error());

  }
  Serial.println((String)"HUm: " + h);
  Firebase.setFloat("Hum", h);
  if (Firebase.failed()) {
    Serial.println("Failed to write the data");
    Serial.println(Firebase.error());

  }
  delay(3000);

}
