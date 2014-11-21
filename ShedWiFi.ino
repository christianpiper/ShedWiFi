#include <stdlib.h>
#include <SoftwareSerial.h>
#include <JsonParser.h>

#define SSID "x"
#define PASS "x"
SoftwareSerial monitor(10, 11); // RX, TX
int led = 13;
int enableWifi = 2;
String content = "";
char character;

using namespace ArduinoJson::Parser;

void setup(){
  delay(5000);
  monitor.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(enableWifi, OUTPUT);
  resetWifi();
}


void loop(){
  while(Serial.available()) {
    character = Serial.read();
    content.concat(character);
  }
  if (content != "") {
    monitor.println(content);
    content = "";
  }
}

void resetWifi() {
  digitalWrite(enableWifi, LOW);
  delay(3000);
  digitalWrite(enableWifi, HIGH);
  delay(500);
  Serial.begin(9600);
  sendDebug("AT");
  delay(5000);
  if(Serial.find("OK")){
    connectWiFi();
    delay(1000);
    enableMultiConn();
    delay(1000);
    startTCPServer();
  }
}

boolean connectWiFi(){
  Serial.println("AT+CWMODE=1");
  delay(2000);
  String cmd="AT+CWJAP=\"";
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  sendDebug(cmd);
  delay(5000);
  if(Serial.find("OK")){
    monitor.println("Wifi Connected");
    return true;
  } else {
    monitor.println("Error");
    return false;
  }
}

boolean enableMultiConn(){
  sendDebug("AT+CIPMUX=1");
  delay(5000);
  if(Serial.find("OK")){
    monitor.println("Multiple Connections Enabled");
    return true;
  } else {
    monitor.println("Error");
    return false;
  }
}

boolean startTCPServer(){
  sendDebug("AT+CIPSERVER=1,9999");
  delay(5000);
  if(Serial.find("OK")){
    monitor.println("TCP Server Enabled");
    return true;
  } else {
    monitor.println("Error");
    return false;
  }
}

void sendDebug(String cmd){
  monitor.print("SEND: ");
  monitor.println(cmd);
  Serial.println(cmd);
}

