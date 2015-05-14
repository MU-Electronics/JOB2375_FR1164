/**
 * Include libraries
 */
#include "VoltageMeasure.h"
#include "Arduino.h"




/**
 * Setup class
 */
VoltageMeasure::VoltageMeasure(void)
{
	//Set External SPI port pins
	SEL_PIN = 10;
	DATAOUT_PIN = 11;
	DATAIN_PIN = 13;
	SPICLOCK_PIN = 12;

	 // Set pin modes 
    ::pinMode(SEL_PIN, OUTPUT); 
    ::pinMode(DATAOUT_PIN, OUTPUT); 
    ::pinMode(DATAIN_PIN, INPUT); 
    ::pinMode(SPICLOCK_PIN, OUTPUT); 
    
	// Disable device to start with 
    ::digitalWrite(SEL_PIN, HIGH); 
    ::digitalWrite(DATAOUT_PIN, LOW); 
    ::digitalWrite(SPICLOCK_PIN, LOW);
}





/**
 * Destroy class
 */
VoltageMeasure::~VoltageMeasure(void)
{
}





/**
 * PRIVATE Get voltage from either onboard A/D converter or external converter
 */
int VoltageMeasure::acquire(int input, int type)
{
	return 0;
}


/**
 * PRIVATE Add a value to a moving average per input
 */
bool VoltageMeasure::average(int input, int value)
{
	return false;
}

/**
 * PRIVATE Convert a digital value to a voltage
 */
float VoltageMeasure::digitalToVoltage(int value)
{
	return false;
}

/**
 * PRIVATE Conect and retrieve the voltage from an external ADC
 */
int VoltageMeasure::external(int channel)
{
	// Set default value for ADC reading
	int adcvalue = 0;

	// Set command bits for ADC - start, mode, chn (3), dont care (3)
	::byte commandbits = B11000000; 
	//unsigned char commandbits = B11000000; 

	// Select channel to read
	commandbits|=channel<<3; //commandbits|=((channel-1)<<3); We'll start from zero for now

	// Select adc
	::digitalWrite(SEL_PIN, LOW); 

	// Ready bits to be wrote
	for (int i=7; i>=3; i--){
		::digitalWrite(DATAOUT_PIN, commandbits&1<<i);
		//cycle clock
		::digitalWrite(SPICLOCK_PIN, HIGH);
		::digitalWrite(SPICLOCK_PIN, LOW);    
	}

	// Ignores 2 null bits
	::digitalWrite(SPICLOCK_PIN, HIGH);    
	::digitalWrite(SPICLOCK_PIN, LOW);
	::digitalWrite(SPICLOCK_PIN, HIGH);  
	::digitalWrite(SPICLOCK_PIN, LOW);

	// Read bits from ADC
	for (int i=11; i>=0; i--)
	{
		adcvalue+=::digitalRead(DATAIN_PIN)<<i;
		//cycle clock
		::digitalWrite(SPICLOCK_PIN, HIGH);
		::digitalWrite(SPICLOCK_PIN, LOW);
	}

	// Turn off device ADC
	::digitalWrite(SEL_PIN, HIGH); 
	
	//Return value
	return adcvalue;
}






/**
 * PUBLIC Get voltage public facing
 */
int VoltageMeasure::get(int input, int type)
{
	return this->external(input);
}



