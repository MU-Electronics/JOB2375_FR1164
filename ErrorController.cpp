/**
 * Include libraries
 */
#include "ErrorController.h"
#include "ErrorConfiguration.cpp"
#include <iterator>
#include <Arduino.h>
#include <algorithm>


/**
 * Setup class
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 */
ErrorController::ErrorController()
{
	// Get error setups
	std::vector< std::map< String, std::map<int, String> > > error_config_container;

	// Setup system error condtions
	error_container = ErrorConfiguration::setupErrors();

	//Setup outputs and inputs
	for(int i = 0; i < error_container.size(); i++){
		// Set inputs
		for (std::map<int, String>::iterator input=error_container[i]["conditions"].begin(); input!=error_container[i]["conditions"].end(); ++input){
			// Get value of input
			::pinMode(input->first, INPUT); 
		}
		
		// Set outputs
		for (std::map<int, String>::iterator output=error_container[i]["action_outputs"].begin(); output!=error_container[i]["action_outputs"].end(); ++output){
			// Get value of input
			::pinMode(output->first, OUTPUT); 
		}
	}
}



/**
 * Destroy class
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 */
ErrorController::~ErrorController(void)
{
}





/**
 * PRIVATE Ensure that a specific error condition is NOT met
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 *
 * @pram id Id of the error condition in the error container
 */
bool ErrorController::ensure(int id)
{
	// Get the condition settings
	std::map<int, String> conditions = error_container[id]["conditions"];

	//Loop though the error conditions that should not be meet
	for (std::map<int, String>::iterator condition=conditions.begin(); condition!=conditions.end(); ++condition){
		// Get value of input
		int val = ::digitalRead(condition->first); 

		// Is condition is met OR not
		if( (condition->second == "HIGH" && val == LOW) || (condition->second == "LOW" && val == HIGH)){
			triggeredErrors.push_back(id);
			this->condtionFailed(id);
		}else{
			//Remove error condition for errors that have been fixed
			if(std::find(triggeredErrors.begin(), triggeredErrors.end(), id) != triggeredErrors.end()){
				this->condtionSuccess(id);
			}
		}
	}
	

	
	return false;
}


/**
 * PRIVATE Runs when a condtion is has meat therefore triggers an error alert but has now been fixed
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 *
 * @pram id ID of the error condition
 */
bool ErrorController::condtionSuccess(int id)
{
	this->output(id, 2);
	::Serial.println("hellow");
	return false;
}


/**
 * PRIVATE Runs when a condtion is meet therefore triggers an error alert
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 *
 * @pram id ID of the error condition
 */
bool ErrorController::condtionFailed(int id)
{
	this->output(id, 1);
	
	return false;
}

/**
 * PRIVATE Set outputs to correct value when error condtion is met
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 *
 * @pram id ID of the error condition
 * @pram direction Is the error being disabled or enabled? 1=enabled 2 = disabled
 */
bool ErrorController::output(int id, int direction)
{
	std::map<int, String> actionoutputs = error_container[id]["action_outputs"];

	if(direction == 1){
		for (std::map<int, String>::iterator output=actionoutputs.begin(); output!=actionoutputs.end(); ++output){
			// Set the outputs
			if(output->second == "HIGH")
				::digitalWrite(output->first, HIGH); 

			if(output->second == "LOW")
				::digitalWrite(output->first, LOW);
		}
	}else if(direction == 2){
		for (std::map<int, String>::iterator output=actionoutputs.begin(); output!=actionoutputs.end(); ++output){
			// Reverse the outputs
			if(output->second == "HIGH")
				::digitalWrite(output->first, LOW); 

			if(output->second == "LOW")
				::digitalWrite(output->first, HIGH);
		}
	}
	
	return true;
}

/**
 * PRIVATE Run specific method when error condition is met
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 *
 * @pram method Method name to run
 */
bool ErrorController::runMethod(String method)
{
	return false;
}

/**
 * PRIVATE Write an error message to an LCD 
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 *
 * @pram message Message to be wrote to LCD screen
 */
bool ErrorController::lcdMessage(String message)
{
	return false;
}





/**
 * PUBLIS Ensure that a all error conditions are NOT met
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 *
 * @pram id Id of the error condition in the error container
 */
bool ErrorController::check()
{
	// @todo Run one condition for now, loop though all in end
	// Loop though error condtions
	//for(int i = 0; i < error_container.size(); i++){
		this->ensure(0);
	//}

	return false;
}
