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
#include "VoltageConfiguration.cpp"
#include <iterator>

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


/**
 * Setup class
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 */
VoltageMeasure::VoltageMeasure()
{
	//Get configuration for ADC (internal and external)
	std::map< String, std::map<String, int> > setupAdcs;
	setupAdcs = VoltageConfiguration::setupAdcs();

	//Get configuration for equivilent voltages (internal and external)
	std::map< String, std::map<int, float> > setupVoltages;
	setupVoltages = VoltageConfiguration::setupVoltages();

	//Set External SPI port pins
	SEL_PIN = setupAdcs["EXTERNAL_ADC"]["SEL_PIN"];
	DATAOUT_PIN = setupAdcs["EXTERNAL_ADC"]["DATAOUT_PIN"];
	DATAIN_PIN = setupAdcs["EXTERNAL_ADC"]["DATAIN_PIN"];
	SPICLOCK_PIN = setupAdcs["EXTERNAL_ADC"]["SPICLOCK_PIN"];

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
	INTERNAL_ADC_BIT_SIZE = setupAdcs["INTERNAL_ADC"]["BIT_SIZE"];
	EXTERNAL_ADC_BIT_SIZE = setupAdcs["EXTERNAL_ADC"]["BIT_SIZE"];

	// Set Voltages
	voltages_internal = setupVoltages["INTERNAL"]; 
	voltages_external = setupVoltages["EXTERNAL"];

	// Define the number of samples
	MOVING_AVERAGE_SAMPLES = VoltageConfiguration::setupMovingAverage();

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
	float voltage = 0;
	if(type == 1){
		int voltageRead = this->external(channel);
		if(this->validateVoltage(voltageRead))
			voltage = this->average(channel, 2, voltageRead);
	}else{ 
		int voltageRead = this->internal(channel);
		if(this->validateVoltage(voltageRead))
			voltage = this->average(channel, 1,  voltageRead);
	}

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
	float toreturn;
	if(type == 2){
		stepSize = voltages_external[channel] / EXTERNAL_ADC_BIT_SIZE;
		toreturn = stepSize * value;
	}else{
		stepSize =  voltages_internal[channel] / INTERNAL_ADC_BIT_SIZE;
		// apply some scaling (Arduino 5V ref and high voltage boards 4.096 ref)
		toreturn = (stepSize * value) * 1.220703125;
	}
	
	return toreturn;
}

/**
 * PRIVATE Retrieve the voltage from an the internal ADC (on chip)
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 *
 * @pram channel Channel to be read relative to the interal ADC
 */
int VoltageMeasure::internal(int channel)
{
	// Read the value
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
 * PRIVATE Validate a voltage that has been read
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 *
 * @pram voltage, The voltage to be validated 
 */
bool VoltageMeasure::validateVoltage(int voltage)
{

	//regex regex_pattern("-?[0-9]+.?[0-9]+");
	if ((voltage >= 0) && (voltage <= 4096)) //&& (voltage >= 0) && (voltage <= 4096)
		return true;
	
	::Serial.print("ERROR: Read voltage is not a int: ");
	::Serial.println(voltage);

	return false;
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
bool VoltageMeasure::update(std::map<int, float>& averageChannelsInt, std::map<int, float>& averageChannelsExt)
{
	// Update internal channel's moving average value
	for (std::map<int, float>::iterator channelI=averageChannelsInt.begin(); channelI!=averageChannelsInt.end(); ++channelI){
		// Create channel witn values		
		this->get(channelI->first, 0);
	}

	// Update external channel's moving average values
	for (std::map<int, float>::iterator channelE=averageChannelsExt.begin(); channelE!=averageChannelsExt.end(); ++channelE){
		// Create channel witn values		
		this->get(channelE->first, 1);
	}

	return true;
}

