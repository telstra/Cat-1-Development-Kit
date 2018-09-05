#ifndef SHIELDINTERFACE_H
#define SHIELDINTERFACE_H

#include <stdint.h>
#include <string.h>
#include "Arduino.h"
#include "packet_utils.h"
#include <SPI.h>

#define TX_BUFFER_SIZE 2048
#define RX_BUFFER_SIZE 2048

#define TELSTRAIOT_STATUS_OK 0
#define TELSTRAIOT_STATUS_ERROR -1
#define TELSTRAIOT_STATUS_TIMEOUT -2

class ShieldInterface
{
public:
	ShieldInterface();
	~ShieldInterface();
	
	int sendCommand(char buffer[], int length);
	int getResponse(char buffer[], int length, int timeout);

private:
	int chipSelectPin;
	int dataReadyPin;
	
	int waitResponse(int timeout);
	int spiRead(char buf[], uint16_t len);
	int spiWrite(char buf[], uint16_t len);

};

#endif // SHIELDINTERFACE_H
