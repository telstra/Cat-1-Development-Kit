#include "iotshield.h"

/*
 * Initialise IoT Shield
 */
IoTShield::IoTShield(ShieldInterface *shield)
{
	this->shield = shield;
}

IoTShield::~IoTShield()
{

}

int IoTShield::getStatus(char* resultBuffer, int length)
{
	char commandBuffer[TX_BUFFER_SIZE];	
	char c_checksum = 0;
	uint16_t c_size = 0;

	packet_create_cmd(CMD_GET_STATUS, commandBuffer, &c_checksum, &c_size);
	packet_append_ending(commandBuffer, &c_checksum, &c_size);
	
	shield->sendCommand(commandBuffer, c_size);
	
	return shield->getResponse(resultBuffer,length,5000);
}

void IoTShield::getIMEI(char* resultBuffer)
{
	char status[100];
	getStatus(status,100);

	//char imei[15];
	packet_get_param(resultBuffer, status, 3);
}

void IoTShield::getIP(char* resultBuffer)
{
	char status[100];
	getStatus(status,100);

	//char imei[15];
	packet_get_param(resultBuffer, status, 4);	
}


void IoTShield::getTime(char* resultBuffer)
{
	char status[100];
	getStatus(status,100);

	//char imei[15];
	packet_get_param(resultBuffer, status, 5);	
}
bool IoTShield::isEC21Connected()
{
	char status[100];
	getStatus(status,100);

	char tempBuffer[1];
	packet_get_param(tempBuffer, status, 0);
	if(tempBuffer[0]==1)
	{
		return true;
	} else {
		return false;
	}
}
bool IoTShield::isSIMReady()
{
	char status[100];
	getStatus(status,100);

	char tempBuffer[1];
	packet_get_param(tempBuffer, status, 1);
	if(tempBuffer[0]==1)
	{
		return true;
	} else {
		return false;
	}
}
bool IoTShield::isPDPContextActive()
{
	char status[100];
	getStatus(status,100);

	char tempBuffer[1];
	packet_get_param(tempBuffer, status, 2);
	if(tempBuffer[0]==1)
	{
		return true;
	} else {
		return false;
	}
}

bool IoTShield::isShieldReady()
{
	char sharedBuffer[TX_BUFFER_SIZE];	
	char c_checksum = 0;
	uint16_t c_size = 0;

	packet_create_cmd(CMD_GET_STATUS, sharedBuffer, &c_checksum, &c_size);
	packet_append_ending(sharedBuffer, &c_checksum, &c_size);
	
	shield->sendCommand(sharedBuffer, c_size);
	
	shield->getResponse(sharedBuffer,TX_BUFFER_SIZE,500);

	if(packet_get_command(sharedBuffer)==100)
	{
		return true;
	} else {
		return false;
	}
}

void IoTShield::waitUntilShieldIsReady()
{
	while(!isShieldReady())
	{
		delay(500);
	}
	while(!isPDPContextActive())
	{
		delay(500);
	}

}

void IoTShield::writeCredentials(const char *deviceID,const char *deviceTenant,const char *deviceUsername,const char *devicePassword)
{
	char sharedBuffer[TX_BUFFER_SIZE];	
	char c_checksum = 0;
	uint16_t c_size = 0;

	packet_create_cmd(CMD_WRITE_CRED, sharedBuffer, &c_checksum, &c_size);
	packet_append_param(deviceID, sharedBuffer, &c_checksum, &c_size);
	packet_append_param(deviceTenant, sharedBuffer, &c_checksum, &c_size);
	packet_append_param(deviceUsername, sharedBuffer, &c_checksum, &c_size);
	packet_append_param(devicePassword, sharedBuffer, &c_checksum, &c_size);
	packet_append_ending(sharedBuffer, &c_checksum, &c_size);
	shield->sendCommand(sharedBuffer, c_size);

	shield->getResponse(sharedBuffer,RX_BUFFER_SIZE,5000);

}


void IoTShield::readCredentials(char *deviceID,char *deviceTenant,char *deviceUsername,char *devicePassword)
{
	char sharedBuffer[TX_BUFFER_SIZE];	
	char c_checksum = 0;
	uint16_t c_size = 0;

	packet_create_cmd(CMD_READ_CRED, sharedBuffer, &c_checksum, &c_size);
	packet_append_ending(sharedBuffer, &c_checksum, &c_size);

	shield->sendCommand(sharedBuffer, c_size);
	shield->getResponse(sharedBuffer,RX_BUFFER_SIZE,5000);
	packet_get_param(deviceID,sharedBuffer,0);
	packet_get_param(deviceTenant,sharedBuffer,1);
	packet_get_param(deviceUsername,sharedBuffer,2);
	packet_get_param(devicePassword,sharedBuffer,3);
}

void IoTShield::clearCredentials()
{
	char sharedBuffer[TX_BUFFER_SIZE];	
	char c_checksum = 0;
	uint16_t c_size = 0;

	packet_create_cmd(CMD_CLEAR_CRED, sharedBuffer, &c_checksum, &c_size);
	packet_append_ending(sharedBuffer, &c_checksum, &c_size);
	shield->sendCommand(sharedBuffer, c_size);
	shield->getResponse(sharedBuffer,RX_BUFFER_SIZE,5000);

}

void IoTShield::resetModem()
{
	char sharedBuffer[TX_BUFFER_SIZE];	
	char c_checksum = 0;
	uint16_t c_size = 0;

	packet_create_cmd(CMD_RESET_MODEM, sharedBuffer, &c_checksum, &c_size);
	packet_append_ending(sharedBuffer, &c_checksum, &c_size);
	shield->sendCommand(sharedBuffer, c_size);
	shield->getResponse(sharedBuffer,RX_BUFFER_SIZE,5000);

}

int IoTShield::getBatteryStatus()
{
	char sharedBuffer[TX_BUFFER_SIZE];	
	char c_checksum = 0;
	uint16_t c_size = 0;

	packet_create_cmd(CMD_GET_BATTERY_STATUS, sharedBuffer, &c_checksum, &c_size);
	packet_append_ending(sharedBuffer, &c_checksum, &c_size);

	shield->sendCommand(sharedBuffer, c_size);
	shield->getResponse(sharedBuffer,RX_BUFFER_SIZE,5000);
	int resp = (int)packet_get_numeric_param(sharedBuffer,0);
	return resp;
}

int IoTShield::getBatteryStateOfCharge()
{
	char sharedBuffer[TX_BUFFER_SIZE];	
	char c_checksum = 0;
	uint16_t c_size = 0;

	packet_create_cmd(CMD_GET_BATTERY_SOC, sharedBuffer, &c_checksum, &c_size);
	packet_append_ending(sharedBuffer, &c_checksum, &c_size);

	shield->sendCommand(sharedBuffer, c_size);
	shield->getResponse(sharedBuffer,RX_BUFFER_SIZE,5000);
	int resp = (int)packet_get_numeric_param(sharedBuffer,0);
	return resp;
}

void IoTShield::getTemperature(char *temperature)
{
	char sharedBuffer[TX_BUFFER_SIZE];	
	char c_checksum = 0;
	uint16_t c_size = 0;

	packet_create_cmd(CMD_GET_TEMPERATURE, sharedBuffer, &c_checksum, &c_size);
	packet_append_ending(sharedBuffer, &c_checksum, &c_size);

	shield->sendCommand(sharedBuffer, c_size);
	shield->getResponse(sharedBuffer,RX_BUFFER_SIZE,5000);
	packet_get_param(temperature,sharedBuffer,0);

}

void IoTShield::getLightLevel(char *lightLevel)
{
	char sharedBuffer[TX_BUFFER_SIZE];	
	char c_checksum = 0;
	uint16_t c_size = 0;

	packet_create_cmd(CMD_GET_LIGHT_LEVEL, sharedBuffer, &c_checksum, &c_size);
	packet_append_ending(sharedBuffer, &c_checksum, &c_size);

	shield->sendCommand(sharedBuffer, c_size);
	shield->getResponse(sharedBuffer,RX_BUFFER_SIZE,5000);
	packet_get_param(lightLevel,sharedBuffer,0);

}

void IoTShield::getLightSensorState(char* resultBuffer, int len)
{
	char commandBuffer[TX_BUFFER_SIZE];	
	char c_checksum = 0;
	uint16_t c_size = 0;

	packet_create_cmd(CMD_GET_LIGHT_SENSOR_STATE, commandBuffer, &c_checksum, &c_size);
	packet_append_ending(commandBuffer, &c_checksum, &c_size);

	shield->sendCommand(commandBuffer, c_size);
	shield->getResponse(resultBuffer, len, 5000);
}

bool IoTShield::isLightSensorDataValid()
{
	char resultBuffer[100];
	getLightSensorState(resultBuffer, 100);
	return packet_get_numeric_param(resultBuffer, 0) != 0;
}

bool IoTShield::isLightSensorDataNew()
{
	char resultBuffer[100];
	getLightSensorState(resultBuffer, 100);
	return packet_get_numeric_param(resultBuffer, 1) != 0;
}

void IoTShield::getLightSensorParams(char* resultBuffer, int len)
{
	char commandBuffer[TX_BUFFER_SIZE];	
	char c_checksum = 0;
	uint16_t c_size = 0;

	packet_create_cmd(CMD_GET_LIGHT_SENSOR_PARAMS, commandBuffer, &c_checksum, &c_size);
	packet_append_ending(commandBuffer, &c_checksum, &c_size);

	shield->sendCommand(commandBuffer, c_size);
	shield->getResponse(resultBuffer, len, 5000);
}

uint8_t IoTShield::getLightSensorGain()
{
	char resultBuffer[100];
	getLightSensorParams(resultBuffer, 100);
	return (uint8_t)packet_get_numeric_param(resultBuffer, 0);
}

uint8_t IoTShield::getLightSensorIntegrationTime()
{
	char resultBuffer[100];
	getLightSensorParams(resultBuffer, 100);
	return (uint8_t)packet_get_numeric_param(resultBuffer, 1);
}

uint8_t IoTShield::getLightSensorMeasurementRate()
{
	char resultBuffer[100];
	getLightSensorParams(resultBuffer, 100);
	return (uint8_t)packet_get_numeric_param(resultBuffer, 2);
}

void IoTShield::setLightSensorParams(uint8_t gain, uint8_t int_rate, uint8_t meas_rate)
{
	char sharedBuffer[TX_BUFFER_SIZE];	
	char c_checksum = 0;
	uint16_t c_size = 0;

	packet_create_cmd(CMD_SET_LIGHT_SENSOR_PARAMS, sharedBuffer, &c_checksum, &c_size);
	packet_append_bin_param((char *)&gain, 1, sharedBuffer, &c_checksum, &c_size);
	packet_append_bin_param((char *)&int_rate, 1, sharedBuffer, &c_checksum, &c_size);
	packet_append_bin_param((char *)&meas_rate, 1, sharedBuffer, &c_checksum, &c_size);
	packet_append_ending(sharedBuffer, &c_checksum, &c_size);

	shield->sendCommand(sharedBuffer, c_size);
	shield->getResponse(sharedBuffer, TX_BUFFER_SIZE, 5000);
}

void IoTShield::updateRTCFromNetwork(char* resultBuffer)
{
	char sharedBuffer[TX_BUFFER_SIZE];	
	char c_checksum = 0;
	uint16_t c_size = 0;

	packet_create_cmd(CMD_UPDATE_RTC, sharedBuffer, &c_checksum, &c_size);
	packet_append_ending(sharedBuffer, &c_checksum, &c_size);

	shield->sendCommand(sharedBuffer, c_size);
	shield->getResponse(sharedBuffer,RX_BUFFER_SIZE,5000);
	packet_get_param(resultBuffer,sharedBuffer,0);

}

