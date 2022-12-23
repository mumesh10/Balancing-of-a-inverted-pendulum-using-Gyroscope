#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <Servo.h>
Servo s1;
Servo s2;

ESP8266WebServer webServer(80);
IPAddress apIP(192,168,1,100);
const char* ap_ssid = "ESP8266Servo";  //APのSSID
const char* ap_password = "nagasawa";
DNSServer dnsServer;


void Servo_top() {
  String html = "";
  html +="<html><body><center>";
  html +="Servo [<button onclick=\"location.href='/sl'\">Left</button>] [<button onclick=\"location.href='/st'\">Top</button>]  [<button onclick=\"location.href='/sr'\">Right</button>]<br>";
  html +="Servo [<button onclick=\"location.href='/s2l'\">Left2</button>] [<button onclick=\"location.href='/s2t'\">Top2</button>]  [<button onclick=\"location.href='/s2r'\">Right2</button>]";
  html +="</center></body></html>";
  webServer.send(200, "text/html", html);
}


void Servo1_L() {
  s1.write(0);
  //s2.write(0);
  delay(400);
  // 「/」に転送
  webServer.sendHeader("Location", String("/"), true);
  webServer.send(302, "text/plain", "");
}


void Servo1_T() {
  s1.write(60);
  //s2.write(60);
  delay(400);
  // 「/」に転送
  webServer.sendHeader("Location", String("/"), true);
  webServer.send(302, "text/plain", "");
}

void Servo1_R() {
  s1.write(120);
  //s2.write(90);
  delay(400);
  // 「/」に転送
  webServer.sendHeader("Location", String("/"), true);
  webServer.send(302, "text/plain", "");
}


void Servo2_L() {
  //s1.write(0);
  s2.write(0);
  delay(400);
  // 「/」に転送
  webServer.sendHeader("Location", String("/"), true);
  webServer.send(302, "text/plain", "");
}


void Servo2_T() {
  //s1.write(60);
  s2.write(30);
  delay(400);
  // 「/」に転送
  webServer.sendHeader("Location", String("/"), true);
  webServer.send(302, "text/plain", "");
}

void Servo2_R() {
  //s1.write(120);
  s2.write(-10);
  delay(1000);

  s2.write(10);
  delay(1000);
  // 「/」に転送
  webServer.sendHeader("Location", String("/"), true);
  webServer.send(302, "text/plain", "");
}
void setup() {
  s1.attach(4);
  s2.attach(5);
  WiFi.mode(WIFI_AP);

 
  delay(200); // Important! This delay is necessary 
  WiFi.softAPConfig(apIP,apIP,IPAddress(255,255,255,0)); 
  WiFi.softAP(ap_ssid,ap_password); // no password
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(53, "*", apIP);
  
  webServer.on("/", Servo_top);
  webServer.on("/sl", HTTP_GET, Servo1_L);
  webServer.on("/st", HTTP_GET, Servo1_T);
  webServer.on("/sr", HTTP_GET, Servo1_R);
  webServer.on("/s2l", HTTP_GET, Servo2_L);
  webServer.on("/s2t", HTTP_GET, Servo2_T);
  webServer.on("/s2r", HTTP_GET, Servo2_R);
  webServer.onNotFound(Servo_top);
  webServer.begin();
}

void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();
}
