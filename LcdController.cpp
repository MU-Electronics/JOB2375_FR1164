/**
 * Include libraries 
 */
#include "LcdController.h"
#include <map>
#include <string>
#include <Arduino.h>
#include "LcdConfiguration.cpp"
#include "VoltageConfiguration.cpp"






/**
 * Set up 
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 * @ NOTE should really DI the settings
 */
LcdController::LcdController()
{
	previousMillis = 0;
	refreshRate = LcdConfiguration::refreshRate();
	voltagesJitter = VoltageConfiguration::setupVoltagesJitter();
	VoltageBlocks = VoltageConfiguration::setupVoltagesBlock();
}





/**
 * On class destruction 
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 */
LcdController::~LcdController(void)
{
}






/**
 * PRIVATE HELPER Convert a float value to a string 
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 */
String LcdController::floatToStringss(float value, int accurcy)
{
	static char string[15];
	dtostrf(value, 5, accurcy, string);

	return String(string);
}


/**
 * PRIVATE HELPER Convert a number into a letter
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 */
String LcdController::numberToLetter(int x)
{
    if(x >= 1 && x <= 26)
		return String("abcdefghijklmnopqrstuvwxyz"[x-1]);

	return "A";
}






/**
 * PUBLIC Check all voltages that need to be updated and updated them
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 */
int LcdController::refresh(std::map< String, std::map<int, float> > setupVoltages, std::map< String, std::map<int, int> > setupVoltagesAccurcy)
{

	// Only update LCD every set interval
	unsigned long currentMillis = millis(); 
	if(currentMillis - previousMillis > refreshRate) 
	{
		// Set lasted updated
		previousMillis = currentMillis; 
		
		// Set block
		int i = 1;

		//Set voltages and accurcy
		voltages = setupVoltages;
		voltagesAccurcy = setupVoltagesAccurcy;

		// Update internal channel's moving average value
		for (std::map<int, float>::iterator channelI=voltages["INTERNAL"].begin(); channelI!=voltages["INTERNAL"].end(); ++channelI){
			this->update(channelI->first, 1, VoltageBlocks[i] - 1);
			// Increment i
			i++;
		}

		// Update external channel's moving average values
		for (std::map<int, float>::iterator channelE=voltages["EXTERNAL"].begin(); channelE!=voltages["EXTERNAL"].end(); ++channelE){
			// Create channel witn values		
			this->update(channelE->first, 2, VoltageBlocks[i] - 1);
			// Increment i
			i++;
		}
	}
}


/**
 * PUBLIC update the display if the new voltage is different from the pervious
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 */
int LcdController::update(int channel, int type, int id)
{
	String types; int typen;
	if(type == 1) { types = "INTERNAL"; typen = 0; }else{ types = "EXTERNAL"; typen=1; }

	// Get current voltage
	float v = Voltages->get(channel, typen);
	String voltage = this->floatToStringss(v, voltagesAccurcy[types][channel]);
	
	// Jitter filter
	float difference = fabsf(v - lastUpdated[types][channel]);
	

	//If voltage has chnaged update
	if(difference > voltagesJitter[types][channel] || lastUpdated[types].find(channel)  == lastUpdated[types].end())
	{
		// Create channel with values		
		Lcd->show("V"+this->numberToLetter(id+1)+":"+voltage+"v ", id);
		// Update last value
		lastUpdated[types][channel] = v;
	}
}


/**
 * PUBLIC Setup a dafult voltage of 0V
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 */
int LcdController::initVoltages(std::map< String, std::map<int, float> > setupVoltages, std::map< String, std::map<int, int> > setupVoltagesAccurcy)
{
	//Set voltages and accurcy
	voltages = setupVoltages;
	voltagesAccurcy = setupVoltagesAccurcy;

	int i = 0;
	// Update internal channel's moving average value
	for (std::map<int, float>::iterator channelI=voltages["INTERNAL"].begin(); channelI!=voltages["INTERNAL"].end(); ++channelI){
		Lcd->show("V"+this->numberToLetter(i+1)+":"+this->floatToStringss(0, voltagesAccurcy["INTERNAL"][channelI->first])+"v ", i);
		// Increment i
		i++;
	}

	// Update external channel's moving average values
	for (std::map<int, float>::iterator channelE=voltages["EXTERNAL"].begin(); channelE!=voltages["EXTERNAL"].end(); ++channelE){
		// Create channel witn values		
		Lcd->show("V"+this->numberToLetter(i+1)+":"+this->floatToStringss(0, voltagesAccurcy["EXTERNAL"][channelE->first])+"v ", i);
		// Increment i
		i++;
	}
}


/**
 * PUBLIC Welcome message
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 */
void LcdController::welcome()
{
	std::map< int, String > welcome;
	welcome = LcdConfiguration::welcome();

	// Show messages
	for (std::map<int, String>::iterator message=welcome.begin(); message!=welcome.end(); ++message){
		Lcd->show(message->second,message->first); 
	}
}


/**
 * PUBLIC Controlls what is contained on the message bar
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 */
bool LcdController::messageBar()
{
	/*
	Lcd->show("HV ON ", 12);
	Lcd->show("+MODE ", 13);
	Lcd->show("|| ERROR PLUGS POLARITY", 14);
	*/
	return false;
}
