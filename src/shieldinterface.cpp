#include "shieldinterface.h"

/*
 * Initialise IoT Shield
 */
 
 ShieldInterface::ShieldInterface()
{
	SPI.begin();
	this->chipSelectPin = 3;
	this->dataReadyPin = 2;
	
	pinMode(chipSelectPin, OUTPUT);
	digitalWrite(chipSelectPin, HIGH);
	pinMode(dataReadyPin, INPUT);
}

/*
 * Close connection to G55
 */
ShieldInterface::~ShieldInterface()
{
	// Close connection to G55
	SPI.end();
}

/*
 * Wait until the G55 is ready to send data, then read the data into a buffer
 * Returns STATUS_ERROR if timeout occurs
 */
int ShieldInterface::getResponse(char buffer[], int length, int timeout)
{
	if(waitResponse(timeout)==TELSTRAIOT_STATUS_OK)
	{
		return spiRead(buffer, length);
	} else {
		return 0; // Time out
	}
}


/*
 * Send data to the G55
 */
int ShieldInterface::sendCommand(char buffer[], int length)
{
	return spiWrite(buffer,length);
}


/*
 * Wait until the G55 has data ready to send
 */
int ShieldInterface::waitResponse(int timeout)
{
	int startTime = millis();
	int elapsedTime = 0;
	
	while(elapsedTime<timeout)
	{
		elapsedTime = millis()-startTime;
		if(elapsedTime<0)
		{
			startTime = 0;
		}
		
		if(digitalRead(dataReadyPin) == HIGH)
		{
			return TELSTRAIOT_STATUS_OK;
		}
	}
	return TELSTRAIOT_STATUS_TIMEOUT;
}


/*
 * Read data from SPI, if it is available
 */
int ShieldInterface::spiRead(char buf[], uint16_t len)
{
	int bytesRead = 0;

	//First byte is ignored. It is always 0.
	if (digitalRead(dataReadyPin) == HIGH) {
		digitalWrite(chipSelectPin, LOW);
		SPI.transfer(0x00);
		digitalWrite(chipSelectPin, HIGH);	
	}
	
	while(digitalRead(dataReadyPin) == HIGH)
	{
		digitalWrite(chipSelectPin, LOW);
		buf[bytesRead] = SPI.transfer(0x00); // send zeroes, save what slave sends back

		bytesRead++;
		if(bytesRead == len)
		{
			digitalWrite(chipSelectPin, HIGH);	
			return bytesRead;
		}
		digitalWrite(chipSelectPin, HIGH);	
	}

	return bytesRead;
}

/*
 * Send data over SPI
 */
int ShieldInterface::spiWrite(char buf[], uint16_t len)
{
	digitalWrite(chipSelectPin, LOW);
	SPI.transfer(buf, len);
	digitalWrite(chipSelectPin, HIGH);


	return len;
}


