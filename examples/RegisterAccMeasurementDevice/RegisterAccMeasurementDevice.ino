#include <connection4g.h>
#include <telstraiot.h>
#include <iotshield.h>
#include <shieldinterface.h>

ShieldInterface shieldif;
IoTShield shield(&shieldif);
Connection4G conn(true,&shieldif);

#define DeviceName ""

const char host[] = "XXXXX.iot.telstra.com";
const char tenant[] = "XXXXX";
const char username[] = "";
const char password[] = "";
const char appkey[] = ""; 

TelstraIoT iotPlatform(host, tenant, username, password, appkey, &conn, &shield);

void setup() {
  Serial.begin(115200);
  delay(5000); 

  Serial.print(F("[START] Starting Registration Script using device name: "));
  Serial.println(DeviceName);

  while(!Serial); // Wait for serial character before starting

  Serial.println("******* Waiting for shield *********");
  shield.waitUntilShieldIsReady(); 
  Serial.println(F("[    ] ******* Shield ready *********"));


  // Check if shield is connected to 4G network
  if(shield.isPDPContextActive()) {
        Serial.println(F("[ OK ] ******* Connected to network *********"));
      
        // Open secure TCP connection to IoT Platform host
        Serial.println("############################ OPENING TCP CONNECTION #########################");
        conn.openTCP(host,443);  
    
        // Register device on IoT Platform
        Serial.println("############################ REGISTERING DEVICE #############################");
        char id[9];
        const char* supportedMeasurements[3];
        supportedMeasurements[0] = "XAcc";
        supportedMeasurements[1] = "YAcc";
        supportedMeasurements[1] = "ZAcc";
    
        int result = iotPlatform.registerDevice(DeviceName, id, 9, supportedMeasurements, 1);
    
        if(result<0) {
          Serial.println(F("[FAIL] Registration error. Please retry."));
          while(true);
        } else {
          Serial.print(F("Arduino registered with id: "));
          Serial.println(id);
        }
      
        // Close the TCP connection to the server
        Serial.println("############################ REGISTRATION COMPLETE #########################");
        Serial.println("Please wait while connection closes...");
        conn.closeTCP();
        Serial.println("Done!");
        delay(1000);


        
  
  } else {
        Serial.println(F("Shield is not connected to 4G network!"));
        while(true);
        {
          delay(5000);
          Serial.println(F("[FAIL] Shield is not connected to 4G network!"));
        }
  }
}

void loop() {

}

