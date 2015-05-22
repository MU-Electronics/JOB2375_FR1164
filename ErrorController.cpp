/**
 * Include libraries
 */
#include "ErrorController.h"
#include "ErrorConfiguration.cpp"
#include <iterator>
#include <Arduino.h>
#include <algorithm>
#include "LcdDisplay.h"


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
	std::vector<int> triggeredErrorsNexted;

	//Loop though the error conditions that should not be meet
	for (std::map<int, String>::iterator condition=conditions.begin(); condition!=conditions.end(); ++condition){
		// Get value of input
		int val = ::digitalRead(condition->first); 

		// Is condition is met OR not
		if( (condition->second == "HIGH" && val == LOW) || (condition->second == "LOW" && val == HIGH)){
			if(std::find(triggeredErrorsNexted.begin(), triggeredErrorsNexted.end(), id) == triggeredErrorsNexted.end()){
				// Add the id to the triggered errors
				triggeredErrorsNexted.push_back(id);
			}
		}
	}
	
	// Run relivent functions
	if(std::find(triggeredErrorsNexted.begin(), triggeredErrorsNexted.end(), id) != triggeredErrorsNexted.end()){
		triggeredErrors.push_back(id);
		this->condtionFailed(id);
	}else if(std::find(triggeredErrors.begin(), triggeredErrors.end(), id) != triggeredErrors.end()){
		triggeredErrors.resize(std::remove(triggeredErrors.begin(), triggeredErrors.end(), id) - triggeredErrors.begin());
		this->condtionSuccess(id);
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
	// Set desired outputs
	this->output(id, 2);
	this->lcdMessage(error_container[id]["action_message"], 0);
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
	// Set desired outputs
	this->output(id, 1);
	// Set disired lcd message
	this->lcdMessage(error_container[id]["action_message"], 1);
	return true;
}

/**
 * PRIVATE Set outputs to correct value when error condtion is met
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 * @todo tidy the below
 *
 * @pram id ID of the error condition
 * @pram direction Is the error being disabled or enabled? 1=enabled 2 = disabled
 */
bool ErrorController::output(int id, int direction)
{
	// Is this behavour required?
	if (error_container[id].find("action_outputs") != error_container[id].end() ){
		// The the actions required
		std::map<int, String> actionoutputs = error_container[id]["action_outputs"];

		for (std::map<int, String>::iterator output=actionoutputs.begin(); output!=actionoutputs.end(); ++output){
			// Double check pin is output
			::pinMode(output->first, OUTPUT); 

			//Decide the output
			if(output->second == "HIGH" && direction == 1){
				::digitalWrite(output->first, HIGH); 
			}else if(output->second == "HIGH" && direction == 2){
				::digitalWrite(output->first, LOW); 
			}else if(output->second == "LOW" && direction == 1){
				::digitalWrite(output->first, LOW); 
			}else if(output->second == "LOW" && direction == 2){
				::digitalWrite(output->first, HIGH);
			}else{
				::digitalWrite(output->first, LOW);
			}
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
bool ErrorController::lcdMessage(std::map<int, String> message, int direction)
{
	::Lcd->errorCondition(message, direction);
	return true;
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
	for(int i = 0; i < error_container.size(); i++){
		this->ensure(i);
	}

	return false;
}
