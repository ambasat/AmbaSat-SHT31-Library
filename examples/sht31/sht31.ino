#include <AmbaSatSHT31.h>
#include <Debugging.h>

/*******************************************************************************
* AmbaSat-1 
* Filename: sht31.ino
*
* Example program to test the AmbaSat-1 library for the 
* SHT31 Digital Temperature and Humidity Sensor
* 
* Copyright (c) 2021 AmbaSat Ltd
* https://ambasat.com
*
* licensed under Creative Commons Attribution-ShareAlike 3.0
* ******************************************************************************/

AmbaSatSHT31 *ambasatSHT31;

// =============================================================================

void setup()
{
    Serial.begin(9600);

    while (!Serial)
	{
        delay(10);    
	}
		
    ambasatSHT31 = new AmbaSatSHT31();

    PRINTLN_DEBUG("Setup complete");  		
}

// =============================================================================

void loop()
{
  float temperature, humidity;
 
  if (ambasatSHT31->getSensorReading(&temperature, &humidity)) 
  {  
      PRINT_DEBUG(F("temperature: "));
      PRINT_DEBUG(temperature);   
      PRINT_DEBUG(F("  -- humidity: "));
      PRINT_DEBUG(humidity);   
      PRINTLN_DEBUG("");
  } 
  else 
  { 
      PRINTLN_DEBUG("Failed to read sensor");
  }  

  delay(1000);
}
