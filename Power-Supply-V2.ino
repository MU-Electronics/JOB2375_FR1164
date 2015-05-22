/**
 * Include exteneral libraries
 */
#include <LiquidCrystal.h>
#include <StandardCplusplus.h>


/**
 * Require standard C++ external libraries
 */
#include <map>
#include <string>
#include <vector>
#include <iterator>
#include <memory>


/**
 * Require internal project files
 */
#include "LcdDisplay.h"
#include "Voltage.h"
#include "ErrorController.h"
#include "VoltageConfiguration.cpp"


/**
 * Set required varables 
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 */
std::map< String, std::map<int, float> > setupVoltages = VoltageConfiguration::setupVoltages();
std::map< String, std::map<int, int> > setupVoltagesAccurcy = VoltageConfiguration::setupVoltagesAccurcy();
long previousMillis = 0;
long previousMillis2 = 0;
long updateEvery = 50;




/**
 * Init singleton classes
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 */
LcdDisplay* Lcd = new LcdDisplay();

//std::auto_ptr<LcdDisplay> Lcd(new LcdDisplay());
VoltageMeasure* Voltages = new VoltageMeasure();
ErrorController* ErrorHandler = new ErrorController();




/**
 * Run setup procedures
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 */
void setup()
{
	// Setup serial communication
	Serial.begin(9600);
}







/**
 * Run general loop
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 */
void loop()
{
	// Check for error condtions
	ErrorHandler->check();

	// Update Moving Voltage
	Voltages->update(setupVoltages["INTERNAL"], setupVoltages["EXTERNAL"]);

	// Only update LCD every set interval
	unsigned long currentMillis = millis(); 
	if(currentMillis - previousMillis > updateEvery) 
	{
		// Set lasted updated
		previousMillis = currentMillis; 
		// Update LCD
		LcdUpdate();
	}
}





/**
 * Update LCD Display
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 */
void LcdUpdate()
{
	// Set block
	int i = 0;

	// Update internal channel's moving average value
	for (std::map<int, float>::iterator channelI=setupVoltages["INTERNAL"].begin(); channelI!=setupVoltages["INTERNAL"].end(); ++channelI){
	    // Create channel with values		
		Lcd->show("V"+numberToLetter(i+1)+":"+FloatToString(Voltages->get(channelI->first, 0), setupVoltagesAccurcy["INTERNAL"][channelI->first])+"v ", i);
		// Increment i
		i++;
	}

	// Update external channel's moving average values
	for (std::map<int, float>::iterator channelE=setupVoltages["EXTERNAL"].begin(); channelE!=setupVoltages["EXTERNAL"].end(); ++channelE){
	    // Create channel witn values		
		Lcd->show("V"+numberToLetter(i+1)+":"+FloatToString(Voltages->get(channelE->first, 1), setupVoltagesAccurcy["EXTERNAL"][channelE->first])+"v ", i);
		// Increment i
		i++;
	}
}









/**
 * HELPER Convert a float value to a string 
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 */
String FloatToString(float value, int accurcy)
{
	static char string[15];
	dtostrf(value, 5, accurcy, string);

	return String(string);
}


/**
 * HELPER Convert a number into a letter
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 */
String numberToLetter(int x)
{
    if(x >= 1 && x <= 26)
		return String("abcdefghijklmnopqrstuvwxyz"[x-1]);

	return "A";
}


/*
Lcd->show("HV ON ", 12);
Lcd->show("+MODE ", 13);
Lcd->show("|| ERROR PLUGS POLARITY", 14);
*/