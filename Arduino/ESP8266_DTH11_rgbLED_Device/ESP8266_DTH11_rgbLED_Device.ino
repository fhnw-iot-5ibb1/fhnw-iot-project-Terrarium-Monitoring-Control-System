//FirebaseESP8266.h must be included before ESP8266WiFi.h
#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>

#define WIFI_SSID "Brodi"
#define WIFI_PASSWORD "Beni1234"
#define FIREBASE_HOST "https://iotterrarium.firebaseio.com"
#define FIREBASE_AUTH "cNsAC9mT9mBpLRIMGGsjBvf6o12g4LBVGOcL61Qa"

//Define FirebaseESP8266 data object
FirebaseData firebaseData;

// Temperatur & Humidity Measuering 
#include "DHT.h"
#define DHTPIN 13 
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Display
#include "TM1637.h"
#define CLK 15
#define DIO 16
TM1637 tm1637(CLK,DIO);
int measuredHumidity = 0;
int measuredTemp = 0;
int desiredTemp = 0;


#include <ChainableLED.h>
#define NUM_LEDS 2
ChainableLED leds(5, 4, NUM_LEDS);

// rotatory sensor
int analogPin = A0; 
String rotatVal = "";  


void setup() {
    Serial.begin(115200);
    delay(300);
    
    wifiConnect();
    delay(1000);

    initFirebase();
  
    Serial.println("DHT11 start");
    dht.begin();

    desiredTemp = getTemp();

    tm1637.init();
    tm1637.set(BRIGHT_TYPICAL);//BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
    tm1637.point(POINT_ON);
}

void loop() {
  rotatVal = getRotateValue();

  if(rotatVal != "middle"){
    setDesiredTemp();
    setFirebaseIntValue("desiredTemp", desiredTemp);
    tm1637.point(POINT_ON);
  } else {
    tm1637.point(POINT_OFF);
  }
 
  setLEDLight();
  setDisplay();

  if(WiFi.status() != WL_CONNECTED){
    wifiConnect();
    initFirebase();
  }

  setFirebaseIntValue("measuredTemp", measuredTemp);
  setFirebaseIntValue("measuredHumidity", getHumi());

  desiredTemp = getFirebaseIntValue("desiredTemp");
  
  delay(300);
}


void setLEDLight(){
  if(desiredTemp > measuredTemp){
		// red 
       leds.setColorRGB(0, 255, 0, 0);
       leds.setColorRGB(1, 255, 0, 0);
  } else if(desiredTemp < measuredTemp){
		// blue
       leds.setColorRGB(0, 0, 0, 255);
       leds.setColorRGB(1, 0, 0, 255);
  } else{
		// white
       leds.setColorRGB(0, 255, 255, 255);
       leds.setColorRGB(1, 255, 255, 255);
  }
}

void setDesiredTemp(){
    if(rotatVal == "left"){
      if (desiredTemp > 0) {
         desiredTemp--;
      }
    } else {
        if (desiredTemp < 99) {
          desiredTemp++;
        }
    }
}

String getRotateValue() {
  int sensor_value = analogRead(analogPin);
  int degress = sensor_value / 64;
  String rotateValue = "";
  
  if(degress > 13){
    rotateValue = "left";
  } else if(degress < 3) {
    rotateValue = "right";
  } else {
    rotateValue = "middle";
  }
  
  return rotateValue;
}


void setDisplay(){
    int mesTemp = getTemp();
    int mesTempOne = mesTemp / 10;
    int mesTempTwo = mesTemp % 10;
    measuredTemp = mesTemp;

    int desTemp = desiredTemp;
    int desTempOne = desTemp / 10;
    int desTempTwo = desTemp % 10;
  
    tm1637.display(0, desTempOne);
    tm1637.display(1, desTempTwo);
    tm1637.display(2, mesTempOne);
    tm1637.display(3, mesTempTwo);
}

float getTemp(){
  float t = dht.readTemperature(); // Celsius

  if (isnan(t)) {
    Serial.println("Failed to read from DHT11 sensor");
  } else {
    Serial.print(t);
    Serial.println(" *C");
  }
  return (int) t;
}

float getHumi(){
  float h = dht.readHumidity(); // %

  if (isnan(h)) {
    Serial.println("Failed to read from DHT11 sensor");
  } else {
    Serial.print(h);
    Serial.println(" %");
  }
  return (int) h;
}

void setFirebaseIntValue(String type, int value){
       Serial.println("------------------------------------");
       if (Firebase.setInt(firebaseData, type, value)){
            Serial.println("PASSED: ");
            Serial.print(firebaseData.dataPath());
            Serial.println(firebaseData.intData());
        } else {
            Serial.println("FAILED: ");
            Serial.print(firebaseData.errorReason());
        }
        Serial.println("------------------------------------");
        Serial.println();
}

int getFirebaseIntValue(String datatype) {
  int result = 0;
  if (Firebase.getInt(firebaseData, datatype)) {
    Serial.println("PASSED");
    Serial.println(firebaseData.dataPath());
    Serial.println(firebaseData.intData());
    result = firebaseData.intData();
  } else {
    Serial.println("FAILED");
    Serial.println(firebaseData.errorReason());
  }
  return result;
}

void initFirebase(){
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  //Set database read timeout to 1 minute (max 15 minutes)
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  //tiny, small, medium, large and unlimited.
  //Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
}

void wifiConnect() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);             // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID); Serial.println(" ...");

  int teller = 0;
  while (WiFi.status() != WL_CONNECTED) {       
  // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(++teller); Serial.print(' ');
  }
  
  tm1637.point(POINT_OFF);
  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer
}
