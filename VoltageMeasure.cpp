/**
 * Include libraries
 */
#include <StandardCplusplus.h>
#include <vector>
#include <list>
#include <map>
#include <string>
#include "Arduino.h"
#include "VoltageMeasure.h"


/**
 * Setup class
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 */
VoltageMeasure::VoltageMeasure(std::vector<int>& averageChannelsInt, std::vector<int>& averageChannelsExt)//: channel_container()
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


	// Below sets ups the moving average array
	// @todo move the moving average to a seperate class
	//std::vector< std::map< int, std::vector<float> > >* channel_container;
	std::map< int, std::vector<float> >  channelInternal;
	std::map< int, std::vector<float> >  channelExternal;
	// Create default values
	std::vector<float> values(4, float(1));

	// Created INTERNAL channels
	for(int i=0; i>=averageChannelsInt.size(); i++){
		// Create channel witn values		
		channelInternal[averageChannelsInt[i]] = values;
	}

	// Created EXTERNAL channels
	for(int i=0; i>=averageChannelsExt.size(); i++){
		// Create channel witn values		
		channelExternal[averageChannelsExt[i]] = values;
	}

	//Push channels types onto channel container
	channel_container->push_back(channelInternal);
	channel_container->push_back(channelExternal);

	
}





/**
 * Destroy class
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 */
VoltageMeasure::~VoltageMeasure(void)
{
}





/**
 * PRIVATE Get voltage from either onboard A/D converter or external converter
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 *
 * @pram channel Channel to be read relative to the ADC
 * @pram type Type of ADC. External = 1; Internal = 0;
 */
int VoltageMeasure::acquire(int channel, int type)
{
	return (type == 1)? this->average(channel, 1, this->external(channel)) : this->average(channel, 1,  this->internal(channel));
}


/**
 * PRIVATE Add a value to a moving average per input
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 */
float VoltageMeasure::average(int channel, int type, int value)
{
	if(type == 1)
	{
		// Set a value
		
		//this->channel_container[1];

		//channel_container[1].find(1)->second.assign(myints,myints+4);

		//std::vector<float> test2 = channel_container[1].find(1)->second;
		//float value = test[1];

		// Serial print value
		
		//::Serial.print("hellow");
		//::Serial.print(test2[1]);

		// Re call values
		//std::vector<float> test2 = channel_container[1].find(1)->second;

		return value;
	}
	else if(type == 0)
	{
		return value;
	}
	
	return value;
}

/**
 * PRIVATE Convert a digital value to a voltage
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 */
float VoltageMeasure::digitalToVoltage(int value)
{
	return false;
}

/**
 * PRIVATE Retrieve the voltage from an the internal ADC (on chip)
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 *
 * @pram channel Channel to be read relative to the interal ADC
 */
int VoltageMeasure::internal(int channel)
{
	return ::analogRead(channel);
}

/**
 * PRIVATE Conect and retrieve the voltage from an external ADC
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 *
 * @pram channel Channel to be read relative to the external ADC
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
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 *
 * @pram input The channel to be read relative to the type
 * @pram type The type of ADC, Extenal = 1; Internal = 0;
 */
int VoltageMeasure::get(int channel, int type)
{
	return this->acquire(channel, type);
}



