#include <connection4g.h>
#include <telstraiot.h>
#include <iotshield.h>
#include <shieldinterface.h>

ShieldInterface shieldif;
IoTShield shield(&shieldif);
Connection4G conn(true,&shieldif);

#define DeviceName "device1"

const char host[] = "XXXXX.cumulocity.com";
const char tenant[] = "XXXXX";
const char username[] = "username from cumulocity";
const char password[] = "password from cumulocity";
const char appkey[] = "needs to be empty"; 
const char apn[] = "telstra.m2m";
char id[9];


TelstraIoT iotPlatform(host, tenant, username, password, appkey, &conn, &shield);

void setup() {
  Serial.begin(115200);
  delay(5000);  

  conn.activatePDP(apn, "", "");
  
  Serial.print(F("[START] Starting Registration Script using device name: "));
  Serial.println(DeviceName);

  while(!Serial); // Wait for serial character before starting

  Serial.println(F("[    ] ******* Waiting for shield *********"));
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
        
        const char* supportedMeasurements[1];
        supportedMeasurements[0] = "LightMeasurement";
    
        int result = iotPlatform.registerDevice(DeviceName, id, 9, supportedMeasurements, 1);
    
        if(result<0) {
          Serial.println(F("[FAIL] Registration error. Please retry."));
          while(true);
        } else {
          Serial.print(F("[    ] Arduino registered with id: "));
          Serial.println(id);
        }        
  
  } else {
        Serial.println(F("[FAIL] Shield is not connected to 4G network!"));
        while(true);
        {
          delay(5000);
          Serial.println(F("[FAIL] Shield is not connected to 4G network!"));
        }
  }
}

void loop() {

  if(id>0){
    
    char lightString[15];

    shield.getLightLevel(lightString);

    Serial.print(F("[    ] Light: "));
    Serial.println(lightString);

    Serial.println("############################ Preparing to send MEASUREMENTS #############################");  
    iotPlatform.sendMeasurement("LightMeasurement", "LightMeasurement", "Light level (lux)", lightString, "lux");
  
    delay(5000);
          
  } else {
    Serial.println("Something went wrong and an id isnt assigned, please check the number and try again");
  }

}

