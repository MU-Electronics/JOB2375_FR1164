/**
 * Include libraries
 */
#include <StandardCplusplus.h>
#include <vector>
#include <list>
#include <map>
#include <string>
#include "Arduino.h"
#include "Voltage.h"


/**
 * Setup class
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 */
VoltageMeasure::VoltageMeasure(std::vector<int>& averageChannelsInt, std::vector<int>& averageChannelsExt, std::vector<float>& DigitalVoltagesInternal, std::vector<float>& DigitalVoltagesExternal)
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

	//Set Bit size 
	INTERNAL_ADC_BIT_SIZE = 1024;
	EXTERNAL_ADC_BIT_SIZE = 4096;

	// Set Voltages
	voltages_internal = DigitalVoltagesInternal; 
	voltages_external = DigitalVoltagesExternal;

	// Define the number of samples
	MOVING_AVERAGE_SAMPLES = 5;
	// Below sets ups the moving average array
	std::map< int, std::map<int, float> >  channelInternal;
	std::map< int, std::map<int, float> >  channelExternal;
	// Create default values
	std::map<int, float> values;

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
	channel_container[0] = channelInternal;
	channel_container[1] = channelExternal;
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
 * @pram newValue Take a new value and re caculate the moving average or take just the current value
 */
float VoltageMeasure::acquire(int channel, int type)
{
	//Get the average voltage
	float voltage = (type == 1)? this->average(channel, 2, this->external(channel)) : this->average(channel, 1,  this->internal(channel));

	//We'll change to interger for now
	return voltage;
}


/**
 * PRIVATE Add a value to a moving average per input
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 */
float VoltageMeasure::average(int channel, int type, int value)
{
	// channel_container[ internal or external ][ channel id ][ sample id ]
	// For Example      channel_container[1][1][1] = float(5);        ::Serial.print(channel_container[1][1][1]);

	//Start the moving average
	float total = 0;
	for(int i=MOVING_AVERAGE_SAMPLES; i>=1; i--)
	{

		// Circular Buffer
		if(i == 1){ 
			channel_container[type][channel][i] = float(value); 
		}else{ 
			channel_container[type][channel][i] = channel_container[type][channel][i-1];
		} 
		
		//Add total 
		total = total + channel_container[type][channel][i];
	}

	//Return total divided by number of samples
	
	return this->digitalToVoltage(channel, type, ( total / MOVING_AVERAGE_SAMPLES ));
}

/**
 * PRIVATE Convert a digital value to a voltage
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 */
float VoltageMeasure::digitalToVoltage(int channel, int type, int value)
{
	float stepSize;
	if(type == 2){
		stepSize = voltages_external[channel] / EXTERNAL_ADC_BIT_SIZE;
	}else{
		stepSize =  voltages_internal[channel] / INTERNAL_ADC_BIT_SIZE;
	}
	
	return stepSize * value;
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
float VoltageMeasure::get(int channel, int type)
{
	return this->acquire(channel, type);
}

/**
 * PUBLIC Update all voltages moving averages
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 */
bool VoltageMeasure::update(std::vector<int>& averageChannelsInt, std::vector<int>& averageChannelsExt)
{
	// Update internal channel's moving average values
	for(int i=0; i>=averageChannelsInt.size(); i++){
		// Create channel witn values		
		this->get(averageChannelsInt[i], 0);
	}

	// Update external channel's moving average values
	for(int i=0; i>=averageChannelsExt.size(); i++){
		// Create channel witn values		
		this->get(averageChannelsExt[i], 1);
	}

	return true;
}

