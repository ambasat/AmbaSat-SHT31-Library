/*******************************************************************************
* AmbaSat-1 
* Filename: AmbaSatSHT31.h
*
* This library is designed specifically for AmbaSat-1 and the 
* SHT31 Digital Temperature and Humidity Sensor
* 
* Copyright (c) 2021 AmbaSat Ltd
* https://ambasat.com
*
* licensed under Creative Commons Attribution-ShareAlike 3.0
* ******************************************************************************/

#ifndef __AmbaSatSHT31Sensor__
#define __AmbaSatSHT31Sensor__

#define AMBASAT_SHT31_ADDR 0x44 // AmbaSat SHT31 Address 

#include <Arduino.h>
#include <Wire.h>
#include <Debugging.h>

class AmbaSatSHT31 
{
    public:
        AmbaSatSHT31(void);    
        bool getSensorReading(float *temp, float *hum);
    private:
        float humidity;
        float temperature;
        bool i2cInitialised = false;

        bool readSensorValues(void);
        bool writeSensorCommand(uint16_t command);
        bool readSensorData(uint8_t* data, uint8_t length, bool sendStop = true ) ;
};

#endif

