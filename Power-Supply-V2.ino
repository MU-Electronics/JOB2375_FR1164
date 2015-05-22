/**
 * Include libraries
 */
#include <LiquidCrystal.h>
#include <StandardCplusplus.h>

/**
 * Require standard C++ libraries
 */
#include <iterator>
#include <iostream>
#include <algorithm>
#include <map>
#include <serstream>
#include <string>
#include <vector>
#include <iterator>



/**
 * Require projects classes etc
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
long previousMillis = 0;
long previousMillis2 = 0;
long updateEvery = 200;




/**
 * Init singleton classes
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 */
LcdDisplay* Lcd = new LcdDisplay();
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
 */
void LcdUpdate()
{
	// Set block
	int i = 0;

	// Update internal channel's moving average value
	for (std::map<int, float>::iterator channelI=setupVoltages["INTERNAL"].begin(); channelI!=setupVoltages["INTERNAL"].end(); ++channelI){
	    // Create channel witn values		
		Lcd->show("V"+numberToLetter(i+1)+":"+FloatToString(Voltages->get(channelI->first, 0))+"v ", i);
		i++;
	}

	// Update external channel's moving average values
	for (std::map<int, float>::iterator channelE=setupVoltages["EXTERNAL"].begin(); channelE!=setupVoltages["EXTERNAL"].end(); ++channelE){
	    // Create channel witn values		
		Lcd->show("V"+numberToLetter(i+1)+":"+FloatToString(Voltages->get(channelE->first, 1))+"v ", i);
		i++;
	}
}









/**
 * HELPER Convert a float value to a string 
 */
String FloatToString(float value)
{
	static char string[15];
	dtostrf(value, 5, 3, string);

	return String(string);
}

/**
 * HELPER Convert a number into a letter
 */
String numberToLetter(int n)
{
    if(n >= 1 && n <= 26)
		return String("abcdefghijklmnopqrstuvwxyz"[n-1]);

	return "A";
}


/*
Lcd->show("HV ON ", 12);
Lcd->show("+MODE ", 13);
Lcd->show("|| ERROR PLUGS POLARITY", 14);
*/