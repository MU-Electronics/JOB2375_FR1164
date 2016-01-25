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
bool errorSet4 = false;
bool errorSet5 = false;
bool errorSet6 = false;
bool errorSet7 = false;
bool errorSet8 = false;
bool errorSet9 = false;
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

	// Ensure HV switch is off
	setupErrors();

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
bool setupErrors()
{
	pinMode(48, INPUT);
	if(digitalRead(48) == LOW)
	{
		bool i = false;
		bool first = true;
		while(!i){
			if(first){ LcdHandle->errorCondition("Please place the HV power switch to the", "off postion to proccedd", "", "For help contact the electronics section", 1, 1, 1); first = false; };
			if(digitalRead(48) == HIGH)
				i = true;
		}
	}
}


bool errors()
{
	// Setup pins
	pinMode(48, INPUT); pinMode(31, OUTPUT); pinMode(50, INPUT); pinMode(46, INPUT); pinMode(44, INPUT); pinMode(29, OUTPUT);

	// Check interlock
	if(digitalRead(52) == HIGH && errorSet8 == false){
		digitalWrite(31, LOW);
		LcdHandle->errorCondition("Please check the interlock", "", "", "For help contact the electronics section", 1, 1, 1);
		errorSet8 = true;
		return false;
	}else if(digitalRead(52) == LOW && errorSet8 == true && errorSet9 == false && digitalRead(48) == LOW){
		errorSet9 = true;
		errorSet8 = false;
		LcdHandle->errorCondition("Please turn HV off,", "to comfirm you have check the" , "interlock system.", "For help contact the electronics section", 1, 1, 1);
		return false;
	}else if(errorSet9 == true && digitalRead(48) == HIGH){
		errorSet9 = false;
	}else if(errorSet8 == true && digitalRead(48) == HIGH && digitalRead(52) == LOW){
		errorSet8 = false;
		errorSet = false;
	}


	//Perfrom the power switch check
	if(digitalRead(48) == HIGH)
	{
		if((digitalRead(46) == LOW && digitalRead(44) == HIGH ) || (digitalRead(46) == HIGH && digitalRead(44) == LOW )){  // Error: Welcoming screen (always HV off)
			if(errorSet == false){
				digitalWrite(31, LOW);
				errorSet = true;
				LcdHandle->errorCondition("Please turn on the HV power to procced", "or select the relivent mode (-/+)", "", "For help contact the electronics section", 1, 1, 1);
				return false;
			}
			errorSet4 = false;
		}else if(errorSet4 == false){ // Error: No plugs are plugged into the back
			digitalWrite(31, LOW);
			errorSet4 = true;
			errorSet = false;
			LcdHandle->errorCondition("Please ensure etheir the posative or negative", "plug is plugged in, to the rear of the", "power supply to continue.", "", 1, 1, 1);
			//return false;
		}
	}else if(errorSet == true && errorSet3 == false){ // Action: clear screen turn HV on
		digitalWrite(31, HIGH);
		errorSet = false;
		LcdHandle->errorCondition("", "", "", "", 0, 0, 0);
		return true;
	}


	if(digitalRead(48) == HIGH && digitalRead(50) == LOW && digitalRead(46) == LOW){ // Condition: Switch mode
		digitalWrite(29, HIGH);
		prevModeState = LOW;
		if(errorSet3 == true){
			errorSet3 = false;
			errorSet = false;
		}
	}else if(digitalRead(48) == HIGH && digitalRead(50) == HIGH && digitalRead(44) == LOW){ // Condition: Switch mode
		digitalWrite(29, LOW);
		prevModeState = HIGH;
		if(errorSet3 == true){
			errorSet3 = false;
			errorSet = false;
		}
	}else if(digitalRead(48) == LOW && digitalRead(44) == HIGH && digitalRead(46) == HIGH){ // Error: No plugs are plugged into the back
		if(errorSet3 == false){
			LcdHandle->errorCondition("Please ensure ethier the posative or", "negative plug is plugged in, then", "turn the HV supply off and the back on", "to confirm this error", 1, 1, 1);
			errorSet3 = true;
		}
		digitalWrite(31, HIGH);
		return false;
	}else if(digitalRead(50) == LOW && digitalRead(46) == HIGH && digitalRead(44) == LOW){ // Error: plug polarity
		if(errorSet3 == false){
			LcdHandle->errorCondition("Plug wrongs", "", "", "", 1, 1, 1);
			errorSet3 = true;
		}
	}else if(digitalRead(50) == HIGH && digitalRead(44) == HIGH && digitalRead(46) == LOW){ // Error: plug polarity
		if(errorSet3 == false){
			LcdHandle->errorCondition("Plug wrong", "", "", "", 1, 1, 1);
			errorSet3 = true;
		}
	}else{
		errorSet3 = false;
	}

	if(digitalRead(48) == LOW && digitalRead(50) != prevModeState){ // Error: Changing mode when HV on
		if(errorSet2 == false){
			errorSet2 = true;
			LcdHandle->errorCondition("Do not change mode with HV on", "", "", "", 1, 1, 1);
			return false;
		}
	}else if(errorSet2 == true && digitalRead(50) == prevModeState && errorSet3 == false){ // Action: clear screen
		errorSet2 = false;
		LcdHandle->errorCondition("", "", "", "", 0, 0, 0);
		return true;
	}else if(errorSet2 == true && digitalRead(48) == HIGH){
		errorSet2 = false;
		errorSet = true;
		errorSet3 = false;
		LcdHandle->errorCondition("", "", "", "", 0, 0, 0);
	}

	return true;
}



