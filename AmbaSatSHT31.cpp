/*******************************************************************************
* AmbaSat-1 
* Filename: AmbaSatSHT31.c
*
* This library is designed specifically for AmbaSat-1 and the 
* SHT31 Digital Temperature and Humidity Sensor
* 
* Copyright (c) 2021 AmbaSat Ltd
* https://ambasat.com
*
* licensed under Creative Commons Attribution-ShareAlike 3.0
* ******************************************************************************/

#include "AmbaSatSHT31.h"

#define SHT31_MEASURE_HIGHREP 0x2400 // Measurement High Repeatability with Clock Stretch Disabled

// ======================================================================================

AmbaSatSHT31::AmbaSatSHT31() 
{
    humidity = NAN;
    temperature = NAN;

    if (!i2cInitialised) 
    {
        PRINTLN_DEBUG(F("Starting I2C interface."));
        Wire.begin(); 
        delay(300); // Wait for sensor to start
        
        // Global I2C reset
        PRINTLN_DEBUG(F("Global I2C reset"));
        Wire.beginTransmission(0x00); // global i2c reset address
        Wire.write(0x06);
        Wire.endTransmission(); 
        
        delay(50); // wait for everything to reboot
        PRINTLN_DEBUG(F("I2C Wire has been set up."));
        i2cInitialised = true;
    }
}

// ======================================================================================

bool AmbaSatSHT31::getSensorReading(float *temp, float *hum) 
{
    if (readSensorValues()) 
    {
        *temp = temperature;
        *hum = humidity;

        return true;
    }
    else 
    {
        return false;
    }
}

// ======================================================================================

bool AmbaSatSHT31::readSensorValues(void) 
{
    if (!writeSensorCommand(SHT31_MEASURE_HIGHREP)) 
    {
        PRINTLN_DEBUG(F("ERROR: Could not send sensor command"));
        return false;
    }

    // wait to complete
    delay(20);

    uint8_t localBuffer[6];

    if (!readSensorData(localBuffer, 6)) 
    {
        PRINTLN_DEBUG(F("ERROR: Failed to read measurement data"));
        return false;
    }
  
    uint16_t temperatureReading = (uint16_t)localBuffer[0]*256 + localBuffer[1];
    uint16_t humidityReading = (uint16_t)localBuffer[3]*256 + localBuffer[4];
   
    temperature = -45.0 + 175.0*((float)temperatureReading)/65535.0;
    humidity = 100.0*((float)humidityReading)/65535.0;
    
   return true;
}

// ======================================================================================

bool AmbaSatSHT31::writeSensorCommand(uint16_t command) 
{
    uint8_t buffer[2];
    buffer[0] = command >> 8;
    buffer[1] = command & 0xFF;

    Wire.beginTransmission(AMBASAT_SHT31_ADDR);

    for (size_t i = 0; i < 2; i++)
    {
        if (Wire.write(buffer[i]) != 1) 
        {
            PRINTLN_DEBUG(F("ERROR writing sensor command"));           
            Wire.endTransmission(true);
            return false;
        }

        delay(1);
    }

    Wire.endTransmission(true);
    return true;
}

// ======================================================================================
bool AmbaSatSHT31::readSensorData(uint8_t* data, uint8_t length, bool sendStop)
{
    Wire.requestFrom(AMBASAT_SHT31_ADDR, length, (uint8_t)sendStop);
    uint32_t startTime = millis();

    while ((Wire.available() < (int)length) && (millis() - startTime < 100)) {
        // waitng for the data
        delay(1);
    }

    if (Wire.available() != length) 
    {
       
        PRINTLN_DEBUG(F("ERROR requesting data from device address 0x"));
        PRINT_DEBUG(F(", data length = "));
        PRINT_DEBUG(length);
        PRINTLN_DEBUG(F(", received "));
        PRINTLN_DEBUG(Wire.available());
        
        return false;
    }

    for (size_t i = 0; i < length; i++) 
    {
        *data++ = Wire.read();
    }

    Wire.endTransmission();

    return true;
}