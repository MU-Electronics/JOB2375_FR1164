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
bool errorSet10 = false;
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
			if(first){ LcdHandle->errorCondition("Please place the HV power switch to the", "off postion to proceed", "", "For help contact the Electronics Section", 1, 1, 1); first = false; };
			if(digitalRead(48) == HIGH)
				i = true;
		}
	}
}


bool errors()
{
	// Setup pins
	pinMode(48, INPUT); pinMode(31, OUTPUT); pinMode(50, INPUT); pinMode(46, INPUT); pinMode(44, INPUT); pinMode(29, OUTPUT); pinMode(52, INPUT);

	// Take readings from the pins
	int interlockPin = digitalRead(52); 
	int hvPin = digitalRead(48);
	int posNegPin = digitalRead(50);
	int negModePin = digitalRead(44);
	int posModePin = digitalRead(46);

	// Check interlock
	if(interlockPin == HIGH && errorSet8 == false){
		digitalWrite(31, LOW);
		LcdHandle->errorCondition("ERROR: Check system interlock", "", "", "For help contact the Electronics Section", 1, 1, 1);
		errorSet8 = true;
		return false;
	}else if(interlockPin == LOW && errorSet8 == true && errorSet9 == false && hvPin == LOW){
		errorSet9 = true;
		errorSet8 = false;
		LcdHandle->errorCondition("ERROR: Please turn HV off-on", "to reset the high voltage output" , "", "For help contact the Electronics Section", 1, 1, 1);
		return false;
	}else if(errorSet9 == true && hvPin == HIGH){
		errorSet9 = false;
	}else if(errorSet8 == true && hvPin == HIGH && interlockPin == LOW){
		errorSet8 = false;
		errorSet = false;
	}


	if(interlockPin == LOW){
		//Perfrom the power switch check
		if(hvPin == HIGH)
		{
			if((posModePin == LOW && negModePin == HIGH ) || (posModePin == HIGH && negModePin == LOW )){  // Error: Welcoming screen (always HV off)
				if(errorSet == false){
					digitalWrite(31, LOW);
					errorSet = true;
					LcdHandle->errorCondition("1. Select the relevant mode (+/-)", "2. Switch HV on", "", "For help contact the Electronics Section", 1, 1, 1);
					return false;
				}
				errorSet4 = false;
			}else if(errorSet4 == false){ // Error: No plugs are plugged into the back
				digitalWrite(31, LOW);
				errorSet4 = true;
				errorSet = false;
				LcdHandle->errorCondition("ERROR: Check pos/neg mode plugs are", "connected", "", "For help contact the Electronics Section", 1, 1, 1);
				//return false;
			}
		}else if(errorSet == true && errorSet3 == false){ // Action: clear screen turn HV on
			digitalWrite(31, HIGH);
			errorSet = false;
			LcdHandle->errorCondition("", "", "", "", 0, 0, 0);
			return true;
		}


		if(hvPin == HIGH && posNegPin == LOW && posModePin == LOW){ // Condition: Switch mode
			digitalWrite(29, HIGH);
			prevModeState = LOW;
			if(errorSet3 == true){
				errorSet3 = false;
				errorSet = false;
			}
		}else if(hvPin == HIGH && posNegPin == HIGH && negModePin == LOW){ // Condition: Switch mode
			digitalWrite(29, LOW);
			prevModeState = HIGH;
			if(errorSet3 == true){
				errorSet3 = false;
				errorSet = false;
			}
		}else if(hvPin == LOW && negModePin == HIGH && posModePin == HIGH && errorSet10 == false){ // Error: No plugs are plugged into the back
			if(errorSet3 == false){
				LcdHandle->errorCondition("ERROR: Check pos/neg mode plugs are", "connected", "", "For help contact the Electronics Section", 1, 1, 1);
				errorSet3 = true;
			}
			digitalWrite(31, LOW);
			return false;
		}else if(hvPin == LOW && ((posNegPin == HIGH && negModePin == LOW) || (posNegPin == LOW && posModePin == LOW)) && errorSet3 == true){ // Error: Plugs are pluged in but hv need to be turned off
			if(errorSet10 == false){
				LcdHandle->errorCondition("ERROR: Please turn HV off-on", "to reset the high voltage output" , "", "For help contact the Electronics Section", 1, 1, 1);
				errorSet10 = true;
			}
			digitalWrite(31, LOW);
			return false;
		}else if(posNegPin == LOW && posModePin == HIGH && negModePin == LOW){ // Error: plug polarity
			if(errorSet3 == false){
				LcdHandle->errorCondition("ERROR: Incorrect HV mode selected.", "Check rear HV plugs or change mode", "", "For help contact the Electronics Section", 1, 1, 1);
				errorSet3 = true;
			}
		}else if(posNegPin == HIGH && negModePin == HIGH && posModePin == LOW){ // Error: plug polarity
			if(errorSet3 == false){
				LcdHandle->errorCondition("ERROR: Incorrect HV mode selected.", "Check rear HV plugs or change mode", "", "For help contact the Electronics Section", 1, 1, 1);
				errorSet3 = true;
			}
		}else{
			errorSet3 = false;
			errorSet10 = false;
		}

		if(hvPin == LOW && posNegPin != prevModeState){ // Error: Changing mode when HV on
			if(errorSet2 == false){
				errorSet2 = true;
				LcdHandle->errorCondition("ERROR: Do not change mode with HV on", "", "", "For help contact the Electronics Section", 1, 1, 1);
				return false;
			}
		}else if(errorSet2 == true && posNegPin == prevModeState && errorSet3 == false){ // Action: clear screen
			errorSet2 = false;
			LcdHandle->errorCondition("", "", "", "", 0, 0, 0);
			return true;
		}else if(errorSet2 == true && hvPin == HIGH){
			errorSet2 = false;
			errorSet = true;
			errorSet3 = false;
			LcdHandle->errorCondition("", "", "", "", 0, 0, 0);
		}
	
		return true;
	}
}



