//This code is written by MADUGULA SAI VENKATA SARAN

int device1 = 0;
int device2 = 2;
int device3 = 12;
int device4 = 13;

// Switch
const int BUTTON1_PIN = 5;
const int BUTTON2_PIN = 4;
const int BUTTON3_PIN = 14;
const int BUTTON4_PIN = 10;


#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsClient.h> // get it from https://github.com/Links2004/arduinoWebSockets/releases 
#include <ArduinoJson.h> // get it from https://arduinojson.org/ or install via Arduino library manager
#include <StreamString.h>
#include <AceButton.h> // https://github.com/bxparks/AceButton
using namespace ace_button;

ESP8266WiFiMulti WiFiMulti;
WebSocketsClient webSocket;
WiFiClient client;

#define MyApiKey "Your Sinric API Key" // Change to your sinric API Key. Your API Key is displayed on sinric.com dashboard
#define MySSID "Your WiFi Network SSID" // Change to your Wifi network SSID
#define MyWifiPassword "Your WiFi Password" // Change to your Wifi network password

#define HEARTBEAT_INTERVAL 300000 // 5 Minutes

uint64_t heartbeatTimestamp = 0;
bool isConnected = false;

ButtonConfig config1;
AceButton button1(&config1);
ButtonConfig config2;
AceButton button2(&config2);
ButtonConfig config3;
AceButton button3(&config3);
ButtonConfig config4;
AceButton button4(&config4);




void handleEvent1(AceButton*, uint8_t, uint8_t);
void handleEvent2(AceButton*, uint8_t, uint8_t);
void handleEvent3(AceButton*, uint8_t, uint8_t);
void handleEvent4(AceButton*, uint8_t, uint8_t);




String device_ID_1 = "Paste your Switch One deviceID Here ";
String device_ID_2 = "Paste your Switch Two deviceID Here";
String device_ID_3 = "Paste your Switch Three deviceID Here";
String device_ID_4 = "Paste your Switch Four deviceID Here";

//This code is written by MADUGULA SAI VENKATA SARAN

// If you are going to use a push button to on/off the switch manually, use this function to update the status on the server
// so it will reflect on Alexa app.
// eg: setPowerStateOnServer("deviceid", "ON")

void setPowerStateOnServer(String deviceId, String value);
void setTargetTemperatureOnServer(String deviceId, String value, String scale);

void button1Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT1");
  switch (eventType) {
    case AceButton::kEventPressed:
      Serial.println("kEventPressed");
      setPowerStateOnServer(device_ID_1, "OFF");
      digitalWrite(device1, LOW);
      break;
    case AceButton::kEventReleased:
      Serial.println("kEventReleased");
      setPowerStateOnServer(device_ID_1, "ON");
      digitalWrite(device1, HIGH);
      break;
  }                        


}

void button2Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {                                                    

  Serial.println("EVENT2");
  switch (eventType) {
    case AceButton::kEventPressed:
      Serial.println("kEventPressed");
      setPowerStateOnServer(device_ID_2, "OFF");
      digitalWrite(device2, LOW);
      break;
    case AceButton::kEventReleased:
      Serial.println("kEventReleased");
      setPowerStateOnServer(device_ID_2, "ON");
      digitalWrite(device2, HIGH);
      break;
  }
}

void button3Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT3");
  switch (eventType) {
    case AceButton::kEventPressed:
      Serial.println("kEventPressed");
      setPowerStateOnServer(device_ID_3, "OFF");
      digitalWrite(device3, LOW);
      break;
    case AceButton::kEventReleased:
      Serial.println("kEventReleased");
      setPowerStateOnServer(device_ID_3, "ON");
      digitalWrite(device3, HIGH);
      break;
  }
}

void button4Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT4");
  switch (eventType) {
    case AceButton::kEventPressed:
      Serial.println("kEventPressed");
      setPowerStateOnServer(device_ID_4, "OFF");
      digitalWrite(device4, LOW);
      break;
    case AceButton::kEventReleased:
      Serial.println("kEventReleased");
      setPowerStateOnServer(device_ID_4, "ON");
      digitalWrite(device4, HIGH);
      break;
  }
}

// deviceId is the ID assgined to your smart-home-device in sinric.com dashboard. Copy it from dashboard and paste it here
//This code is written by MADUGULA SAI VENKATA SARAN

void turnOn(String deviceId) {
 if (deviceId == "Paste your Switch One deviceID Here") // Device ID of device 1
 { 
 Serial.print("Turn on device id: ");
 Serial.println(deviceId);
 digitalWrite(device1, HIGH);
 } 
 else if (deviceId == "Paste your Switch Two deviceID Here") // Device ID of device 2
 { 
 Serial.print("Turn on device id: ");
 Serial.println(deviceId);
 digitalWrite(device2, HIGH);
 }
 else if (deviceId == "Paste your Switch Three deviceID Here") // Device ID of device 3
 { 
 Serial.print("Turn on device id: ");
 Serial.println(deviceId);
 digitalWrite(device3, HIGH);
 } 
 else if (deviceId == "Paste your Switch Four deviceID Here") // Device ID of device 4
 { 
 Serial.print("Turn on device id: ");
 Serial.println(deviceId);
 digitalWrite(device4, HIGH);
 }
 
}

void turnOff(String deviceId) {
 if (deviceId == "Paste your Switch One deviceID Here") // Device ID of device 1
 { 
 Serial.print("Turn off Device ID: ");
 Serial.println(deviceId);
 digitalWrite(device1, LOW);
 }
 else if (deviceId == "Paste your Switch Two deviceID Here") // Device ID of device 2
 { 
 Serial.print("Turn off Device ID: ");
 Serial.println(deviceId);
 digitalWrite(device2, LOW);
 }
 else if (deviceId == "Paste your Switch Three deviceID Here") // Device ID of device 3
 { 
 Serial.print("Turn off Device ID: ");
 Serial.println(deviceId);
 digitalWrite(device3, LOW);
 }
 else if (deviceId == "Paste your Switch Four deviceID Here") // Device ID of device 4
 { 
 Serial.print("Turn off Device ID: ");
 Serial.println(deviceId);
 digitalWrite(device4, LOW);
 }
 
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
 switch(type) {
 case WStype_DISCONNECTED:
 isConnected = false; 
 Serial.printf("[WSc] Webservice disconnected from sinric.com!\n");
 break;
 case WStype_CONNECTED: {
 isConnected = true;
 Serial.printf("[WSc] Service connected to sinric.com at url: %s\n", payload);
 Serial.printf("Waiting for commands from sinric.com ...\n"); 
 }
 break;
 case WStype_TEXT: {
 Serial.printf("[WSc] get text: %s\n", payload);
 // Example payloads

// For Switch or Light device types
 // {"deviceId": xxxx, "action": "setPowerState", value: "ON"} // https://developer.amazon.com/docs/device-apis/alexa-powercontroller.html
//This code is written by MADUGULA SAI VENKATA SARAN

// For Light device type
 // Look at the light example in github
 
 DynamicJsonBuffer jsonBuffer;
 JsonObject& json = jsonBuffer.parseObject((char*)payload); 
 String deviceId = json ["deviceId"]; 
 String action = json ["action"];
 
 if(action == "setPowerState") { // Switch or Light
 String value = json ["value"];
 if(value == "ON") {
 turnOn(deviceId);
 } else {
 turnOff(deviceId);
 }
 }
 else if (action == "SetTargetTemperature") {
 String deviceId = json ["deviceId"]; 
 String action = json ["action"];
 String value = json ["value"];
 }
 else if (action == "test") {
 Serial.println("[WSc] received test command from sinric.com");
 }
 }
 break;
 case WStype_BIN:
 Serial.printf("[WSc] get binary length: %u\n", length);
 break;
 }
}

void setup() 
{
 pinMode (device1, OUTPUT);
 pinMode (device2, OUTPUT);
 pinMode (device3, OUTPUT);
 pinMode (device4, OUTPUT);
 pinMode (LED_BUILTIN, OUTPUT);

 // Setup button
  // Button uses the built-in pull up register.
  pinMode(BUTTON1_PIN, INPUT_PULLUP); // INPUT_PULLUP so no need a 10K resistor
  pinMode(BUTTON2_PIN, INPUT_PULLUP); // INPUT_PULLUP so no need a 10K resistor
  pinMode(BUTTON3_PIN, INPUT_PULLUP); // INPUT_PULLUP so no need a 10K resistor
  pinMode(BUTTON4_PIN, INPUT_PULLUP); // INPUT_PULLUP so no need a 10K resistor

 

Serial.begin(115200);
 
 WiFiMulti.addAP(MySSID, MyWifiPassword);
 Serial.println();
 Serial.print("Connecting to Wifi: ");
 Serial.println(MySSID);

// Waiting for Wifi connect
 while(WiFiMulti.run() != WL_CONNECTED) {

 Serial.print(".");
 


digitalWrite(LED_BUILTIN ,LOW);
delay(500);
 }
 if(WiFiMulti.run() == WL_CONNECTED) {



 Serial.println("");
 Serial.print("WiFi connected. ");
 Serial.print("IP address: ");
 Serial.println(WiFi.localIP());

  
  
 }

 
// server address, port and URL
 webSocket.begin("iot.sinric.com", 80, "/");

// event handler
 webSocket.onEvent(webSocketEvent);
 webSocket.setAuthorization("apikey", MyApiKey);
 
 // try again every 5000ms if connection has failed 
 //This code is written by MADUGULA SAI VENKATA SARAN

 webSocket.setReconnectInterval(5000); // If you see 'class WebSocketsClient' has no member named 'setReconnectInterval' error update arduinoWebSockets


  
  
  config1.setEventHandler(button1Handler);
  config2.setEventHandler(button2Handler);
  config3.setEventHandler(button3Handler);
  config4.setEventHandler(button4Handler);

  button1.init(BUTTON1_PIN);
  button2.init(BUTTON2_PIN);
  button3.init(BUTTON3_PIN);
  button4.init(BUTTON4_PIN);

}

 

void loop() {

  button1.check();
  button2.check();
  button3.check();
  button4.check();

  
 
 webSocket.loop();
 //This code is written by MADUGULA SAI VENKATA SARAN

 if(isConnected) {
 uint64_t now = millis();
 
 // Send heartbeat in order to avoid disconnections during ISP resetting IPs over night. 
 if((now - heartbeatTimestamp) > HEARTBEAT_INTERVAL) {
 heartbeatTimestamp = now;
 webSocket.sendTXT("H"); 
 }
 } 
}

void setPowerStateOnServer(String deviceId, String value) {
 DynamicJsonBuffer jsonBuffer;
 JsonObject& root = jsonBuffer.createObject();
 root["deviceId"] = deviceId;
 root["action"] = "setPowerState";
 root["value"] = value;
 StreamString databuf;
 root.printTo(databuf);
 
 webSocket.sendTXT(databuf);
}

//eg: setPowerStateOnServer("deviceid", "CELSIUS", "25.0")
void setTargetTemperatureOnServer(String deviceId, String value, String scale) {
 DynamicJsonBuffer jsonBuffer;
 JsonObject& root = jsonBuffer.createObject();
 root["action"] = "SetTargetTemperature";
 root["deviceId"] = deviceId;
 
 JsonObject& valueObj = root.createNestedObject("value");
 JsonObject& targetSetpoint = valueObj.createNestedObject("targetSetpoint");
 targetSetpoint["value"] = value;
 targetSetpoint["scale"] = scale;
 
 StreamString databuf;
 root.printTo(databuf);
 
 webSocket.sendTXT(databuf);
}
//This code is written by MADUGULA SAI VENKATA SARAN

 
