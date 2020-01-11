//FirebaseESP8266.h must be included before ESP8266WiFi.h
#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>

#define WIFI_SSID "Brodi"
#define WIFI_PASSWORD "Beni1234"
#define FIREBASE_HOST "https://iotterrarium.firebaseio.com"
#define FIREBASE_AUTH "cNsAC9mT9mBpLRIMGGsjBvf6o12g4LBVGOcL61Qa"

//Define FirebaseESP8266 data object
FirebaseData firebaseData;

// Display
#include "TM1637.h"
#define CLK 5
#define DIO 4
TM1637 tm1637(CLK, DIO);
int measuredHumidity = 0;
int desiredHumidity = 0;
int checkDistance = 30; //default 30cm
#include "Ultrasonic.h"
Ultrasonic ultrasonic(16);

// rotatory sensor
int analogPin = A0;
String rotatVal = "";

int startTimeInMinute = 0;

void setup() {
  Serial.begin(115200);

  delay(300);

  wifiConnect();
  delay(1000);

  initFirebase();
  measuredHumidity = getFirebaseIntValue("measuredHumidity");

  tm1637.init();
  tm1637.set(BRIGHT_TYPICAL);//BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;  

  // Red-LED-Light (Alarm Light)
  // -------------- IMPORTANT ----------------
  // First after boot and the internet is connected
  // then connect the Red-Led via GROVE to D2
  pinMode(2, OUTPUT);
}

void loop() {
  rotatVal = getRotateValue();

  if (rotatVal != "middle") {
    setDesiredHumidity();
    setFirebaseIntValue("desiredHumidity", desiredHumidity);
    tm1637.point(POINT_ON);
  } else {
    tm1637.point(POINT_OFF);
  }
  
  setDisplay();
  
  if (WiFi.status() != WL_CONNECTED) {
    wifiConnect();
    initFirebase();
  }


  measuringDistanceAndAlarm();
  desiredHumidity = getFirebaseIntValue("desiredHumidity");

  delay(300);
}

void measuringDistanceAndAlarm(){
  int measuringChanges;
  measuringChanges = (int) ultrasonic.MeasureInCentimeters(); // two measurements should keep an interval
  Serial.print(measuringChanges);//0~400cm
  Serial.println(" cm");
  
  checkDistance = getFirebaseIntValue("checkDistance");
  int currentTimeInMinute = ( millis()/1000 ) / 60;
  if( measuringChanges < checkDistance ){
    Serial.println("-> Reset Timer ");
    startTimeInMinute = currentTimeInMinute;
  }

  int minutesWhenAlarm = getFirebaseIntValue("reactionAlarm");
  
  if( currentTimeInMinute - startTimeInMinute >= minutesWhenAlarm ) {
      Serial.println(" ALAARM ");
      digitalWrite(2, HIGH);   
      setFirebaseStringValue("alarm", "on");
  } else {
      digitalWrite(2, LOW);
      setFirebaseStringValue("alarm", "off");
  }
  Serial.print("Zeit: ");
  Serial.print(currentTimeInMinute);
  Serial.print("Minute seit keine veränderung: ");
  Serial.print(startTimeInMinute);
}

void setFirebaseStringValue(String type, String value) {
  Serial.println("------------------------------------");
  if (Firebase.setString(firebaseData, type, value)) {
    Serial.println("PASSED");
    Serial.println(firebaseData.dataPath());
    Serial.println(firebaseData.intData());
  } else {
    Serial.println("FAILED");
    Serial.println(firebaseData.errorReason());
  }
  Serial.println("------------------------------------");
  Serial.println();
}

void setFirebaseIntValue(String type, int value) {
  Serial.println("------------------------------------");
  if (Firebase.setInt(firebaseData, type, value)) {
    Serial.println("PASSED");
    Serial.println(firebaseData.dataPath());
    Serial.println(firebaseData.intData());
  } else {
    Serial.println("FAILED");
    Serial.println(firebaseData.errorReason());
  }
  Serial.println("------------------------------------");
  Serial.println();
}

void setDesiredHumidity() {
  if (rotatVal == "left") {
    if (desiredHumidity > 0) {
      desiredHumidity--;
    }
  } else {
    if (desiredHumidity < 99) {
      desiredHumidity++;
    }
  }
}

String getRotateValue() {
  int sensor_value = analogRead(analogPin);
  int degress = sensor_value / 64;
  String rotateValue = "";

  if (degress > 13) {
    rotateValue = "left";
  } else if (degress < 3) {
    rotateValue = "right";
  } else {
    rotateValue = "middle";
  }

  return rotateValue;
}

void setDisplay() {
  int mesHumi = getFirebaseIntValue("measuredHumidity");
  int mesHumiOne = mesHumi / 10;
  int mesHumiTwo = mesHumi % 10;
  measuredHumidity = mesHumi;

  int desHumi = desiredHumidity;
  int desHumiOne = desHumi / 10;
  int desHumiTwo = desHumi % 10;

  tm1637.display(0, desHumiOne);
  tm1637.display(1, desHumiTwo);
  tm1637.display(2, mesHumiOne);
  tm1637.display(3, mesHumiTwo);
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

void initFirebase() {
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

  Serial.println('\n');
  Serial.println("Connection established!");
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer
}
