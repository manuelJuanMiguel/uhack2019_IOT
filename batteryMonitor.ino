#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

int batteryPin = A0;
int batteryStatus = 0;
int inputVoltage = 0;

const char* cSSID="Dev";
const char* cPASS="1234567890";

String sPostAddr = "http://0832434a.ngrok.io/api/device/updateDeviceStatus/MAC-123";
String sPostValue = "";

int led30 = 4;
int led60 = 5;
int led100 = 16;

void setup() {
  Serial.begin(115200);
  Serial.println("Battery Monitor Initialize");
  WiFi.begin(cSSID, cPASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.print("CONNECTED SUCCESSFULLY : "); Serial.println(cSSID);
  pinMode(batteryPin, INPUT);
  pinMode(batteryStatus, INPUT);
  pinMode(led30, OUTPUT); 
  pinMode(led60, OUTPUT); 
  pinMode(led100, OUTPUT); 
  delay(500);
}

String getStatusBattery() {
 String sBattStatus = "";
 int iBatteryStatusRead = digitalRead(batteryStatus);
 Serial.println(iBatteryStatusRead);
 if (iBatteryStatusRead == LOW) {
    sBattStatus = "0";
 } else {
    sBattStatus = "1";
 }
 
 return sBattStatus;
}

String getPercentageBattery() {
  String sBattPercent = "";
  int batteryRead = analogRead(batteryPin);
  inputVoltage = (batteryRead/1023) * 100;
  Serial.println(inputVoltage);
  
  if (inputVoltage >= 70 && inputVoltage <= 100){
      digitalWrite(led30, HIGH);
      digitalWrite(led60, HIGH);
      digitalWrite(led100, HIGH);
  } else if (inputVoltage >= 40 && inputVoltage <= 69) {
      digitalWrite(led30, HIGH);
      digitalWrite(led60, HIGH);
      digitalWrite(led100, LOW);
  } else if (inputVoltage > 0 && inputVoltage <= 39) {
      digitalWrite(led30, HIGH);
      digitalWrite(led60, LOW);
      digitalWrite(led100, LOW);
  } else {
      digitalWrite(led30, LOW);
      digitalWrite(led60, LOW);
      digitalWrite(led100, LOW);
  }

  sBattPercent = String(inputVoltage);
  return sBattPercent;
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) { 
    HTTPClient http;
    http.begin(sPostAddr);  
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String sGetStatus = getStatusBattery();
    String sGetPercentage = getPercentageBattery();
    sPostValue = "status=" + sGetStatus + "&percentage=" + sGetPercentage; 
    
    int httpResponseCode = http.POST(sPostValue);
    http.writeToStream(&Serial);
    if (httpResponseCode > 0) {
     
    String payload = http.getString();   
    Serial.println(payload);             
     
    }
    http.end(); 
  }
   
  delay(5000);
}



