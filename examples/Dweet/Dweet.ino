#include <connection4g.h>
#include <telstraiot.h>
#include <iotshield.h>
#include <shieldinterface.h>

ShieldInterface shieldif;
IoTShield shield(&shieldif);
Connection4G conn(false,&shieldif);

const char host[] = "dweet.io";
String deviceName = "";
const char apn[] = "telstra.m2m";

void setup() {
  Serial.begin(115200);
  Serial.println("Starting....");
  
  delay(500);

  conn.activatePDP(apn, "", "");  
  
  if(!shield.isShieldReady())
  {
    Serial.println("Waiting for network");
    shield.waitUntilShieldIsReady();
  } else {
    Serial.println("Network ready!");
  }
  //while(Serial.available()==0); // Wait for serial character before starting
  
  conn.openTCP(host,80);
}


void loop() {
  char lightString[15];
  char tempString[15];

  shield.getLightLevel(lightString);
  shield.getTemperature(tempString);

  String url = "GET /dweet/for/";
  url += deviceName;
  url += "?temperature=";
  url += tempString;
  url += "&light=";
  url += lightString;
  url += " HTTP/1.1\r\nHost: ";
  url += host;
  url += "\r\n\r\n";

  Serial.print("Light: ");
  Serial.print(lightString);
  Serial.print(" | Temperature: ");
  Serial.println(tempString);

  char c_url[url.length()];
  url.toCharArray(c_url, url.length());

  conn.TCPWrite(c_url, 0);
    
  delay(5000);
}
