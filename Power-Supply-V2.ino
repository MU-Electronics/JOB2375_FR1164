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
#include "LcdDriver.h"
#include "Voltage.h"
#include "ErrorController.h"
#include "VoltageConfiguration.cpp"
#include "LcdController.h"


/**
 * Set required varables 
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 */
std::map< String, std::map<int, float> > setupVoltages = VoltageConfiguration::setupVoltages();
std::map< String, std::map<int, int> > setupVoltagesAccurcy = VoltageConfiguration::setupVoltagesAccurcy();




/**
 * Init classes
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 * @note The below should really be contained with an container
 */
LcdDriver* Lcd = new LcdDriver();
LcdController* LcdHandle = new LcdController();
VoltageMeasure* Voltages = new VoltageMeasure();
bool error = false;
bool errorSet = false;
bool errorSet2 = false;
bool errorSet3 = false;
int prevModeState = 0;
//ErrorController* ErrorHandler = new ErrorController();
//std::auto_ptr<LcdDisplay> Lcd(new LcdDisplay()); << This format should be used for the above but ardunio no like atm





/**
 * Run setup procedures
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 */
void setup()
{
	// Setup serial communication
	Serial.begin(9600);

	// Show welcome message
	LcdHandle->welcome();

	// Get some values into moving average for voltage readings
	for(int i = 0; i<=10; i++)
		Voltages->update(setupVoltages["INTERNAL"], setupVoltages["EXTERNAL"]);

	// Wait for screen to be read
	//delay(5000);

	// Clear LCD
	Lcd->clearAll();

	// Init voltages
	LcdHandle->initVoltages(setupVoltages, setupVoltagesAccurcy);
}







/**
 * Run general loop
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 */
void loop()
{
	// Check for error condtions
	error = errors();

	if(error){
		// Update Moving Voltage
		Voltages->update(setupVoltages["INTERNAL"], setupVoltages["EXTERNAL"]);

		// Update LCD
		LcdHandle->refresh(setupVoltages, setupVoltagesAccurcy, false);
	}
}




/**
 *!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *!!!!!!!!!!!!!!!!!!!!!!!!!!!!!                 DISCLAIMER                  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *
 * Shit arduino is running out of dynamic memory therefore the error checking will be implimented using 
 * C only functions / varables. Therefore nothing fancy can be achived like the configuration used for 
 * the other aspects of this program
 */
bool errors()
{
	//Perfrom the power siwtch check
	pinMode(48, INPUT);
	if(digitalRead(48) == HIGH)
	{
		if(errorSet == false){
			pinMode(31, OUTPUT);
			digitalWrite(31, LOW);
			errorSet = true;
			LcdHandle->errorCondition("Please turn on the HV power to procced", "or select the relivent mode (-/+)", "", "For help contact the electronics section", 1, 1, 1);
			return false;
		}
	}else if(errorSet == true){
		pinMode(31, OUTPUT);
		digitalWrite(31, HIGH);
		errorSet = false;
		LcdHandle->errorCondition("", "", "", "", 0, 0, 0);
		return true;
	}

	//Perfrom ERROR on mode select when hv on
	pinMode(50, INPUT);
	pinMode(48, INPUT);
	if(digitalRead(48) == LOW && digitalRead(50) != prevModeState){
		if(errorSet2 == false){
			errorSet2 = true;
			LcdHandle->errorCondition("Do not change mode with HV on", "", "", "", 1, 1, 1);
			return false;
		}
	}else if(errorSet2 == true && digitalRead(48) == LOW && digitalRead(50) == prevModeState){
		errorSet2 = false;
		LcdHandle->errorCondition("", "", "", "", 0, 0, 0);
		return true;
	}

	//Perfrom the mode select
	pinMode(50, INPUT);
	pinMode(48, INPUT);
	pinMode(46, INPUT);
	pinMode(44, INPUT);
	if(digitalRead(48) == HIGH && digitalRead(50) == HIGH && digitalRead(46) == LOW){
		pinMode(29, OUTPUT);
		digitalWrite(29, HIGH);
		prevModeState = HIGH;
		if(errorSet3 == true){
			LcdHandle->errorCondition("", "", "", "", 0, 0, 0);
			errorSet3 = false;
		}
	}else if(digitalRead(48) == HIGH && digitalRead(50) == LOW && digitalRead(44) == LOW){
		pinMode(29, OUTPUT);
		digitalWrite(29, LOW);
		prevModeState = LOW;
		if(errorSet3 == true){
			LcdHandle->errorCondition("", "", "", "", 0, 0, 0);
			errorSet3 = false;
		}
	}else if(digitalRead(48) == HIGH && digitalRead(50) == HIGH && digitalRead(46) == HIGH && digitalRead(44) == LOW){
		// ERROR PLUG POLATITY
		if(errorSet3 == false){
			LcdHandle->errorCondition("Plug wrong", "", "", "", 1, 1, 1);
			errorSet3 = true;
		}
	}else if(digitalRead(48) == HIGH && digitalRead(50) == LOW && digitalRead(44) == HIGH && digitalRead(46) == LOW){
		// ERROR PLUG POLATITY
		if(errorSet3 == false){
			LcdHandle->errorCondition("Plug wrong", "", "", "", 1, 1, 1);
			errorSet3 = true;
		}
	}



	return true;
}



