#include <connection4g.h>
#include <telstraiot.h>
#include <iotshield.h>
#include <shieldinterface.h>

ShieldInterface shieldif;
IoTShield shield(&shieldif);
Connection4G conn(false,&shieldif);

const char host[] = "google.com";
const char apn[] = "telstra.m2m";

void setup() {
  Serial.begin(115200);
  Serial.println("Starting....");

  conn.activatePDP(apn, "", "");
  
  delay(500);
  
  if(!shield.isShieldReady())
  {
    Serial.println("Waiting for network");
    shield.waitUntilShieldIsReady();
  } else {
    Serial.println("Network ready!");
  }
  
  conn.openTCP(host,80);
}


void loop() {

}
