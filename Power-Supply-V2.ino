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
ErrorController* ErrorHandler = new ErrorController();
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
	delay(5000);

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
	ErrorHandler->check();

	// Update Moving Voltage
	Voltages->update(setupVoltages["INTERNAL"], setupVoltages["EXTERNAL"]);

	// Update LCD
	LcdHandle->refresh(setupVoltages, setupVoltagesAccurcy, false);
}









