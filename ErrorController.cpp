/**
 * Include libraries
 */
#include "ErrorController.h"
#include "ErrorConfiguration.cpp"
#include <iterator>
#include <Arduino.h>
#include <algorithm>
#include "LcdDriver.h"


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
	int o = error_container.size();
	for(int i = 0; i < o; i++){
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

	displayed_error = -1;
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
	//std::map<int, String> conditions = error_container[id]["conditions"];
	std::map<int, int> triggeredErrorsNexted;

	//Loop though the error conditions that should not be meet
	for (std::map<int, String>::iterator condition=error_container[id]["conditions"].begin(); condition!=error_container[id]["conditions"].end(); ++condition)
	{
		// Get value of input
		int val = ::digitalRead(condition->first); 

		// Error for high and low
		if( (condition->second == "HIGH" && val == HIGH) || (condition->second == "LOW" && val == LOW)){
			if( triggeredErrorsNexted.find(id) == triggeredErrorsNexted.end() ){ triggeredErrorsNexted[id] = 1; }else{ triggeredErrorsNexted[id] = triggeredErrorsNexted[id] + 1; }
		}
		
		// Error for switched state
		if(condition->second == "CHANGE" && outputChanges[id] != val){
			//triggeredErrorsNexted[id]++;
		}
	}
	
	
	bool met = false;
	// Are there any triggered condtions
	if( triggeredErrorsNexted.find(id) != triggeredErrorsNexted.end() ){
		// All condtions are triggered
		int c = error_container[id]["conditions"].size();
		if(triggeredErrorsNexted[id] == c){
			met = true;
		}
	}

	return met;
	
}


/**
 * PRIVATE Runs when a condtion is has meat therefore triggers an error alert but has now been fixed
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 *
 * @pram id ID of the error condition
 */
bool ErrorController::condtionsNotMet(int id, bool lcd)
{
	// Set desired outputs
	this->output(id, 2);

	return true;
}


/**
 * PRIVATE Runs when a condtion is meet therefore triggers an error alert
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 *
 * @pram id ID of the error condition
 */
bool ErrorController::conditionsMet(int id, bool lcd)
{
	// Set desired outputs for met condtions 
	this->output(id, 1);

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
		//std::map<int, String> actionoutputs = error_container[id]["action_outputs"];
		for (std::map<int, String>::iterator output=error_container[id]["action_outputs"].begin(); output!=error_container[id]["action_outputs"].end(); ++output){
			// Double check pin is output
			::pinMode(output->first, OUTPUT); 
						//outputChanges[output->first] = output->second;

			//Decide the output
			if(output->second == "HIGH" && direction == 1){
				::digitalWrite(output->first, HIGH); 
			}else if(output->second == "HIGH" && direction == 2){
				::digitalWrite(output->first, LOW); 
			}else if(output->second == "LOW" && direction == 1){
				::digitalWrite(output->first, LOW); 
			}else if(output->second == "LOW" && direction == 2){
				::digitalWrite(output->first, HIGH);
			}else if(output->second == "LOW_L" && direction == 2){
				::digitalWrite(output->first, LOW); 
				outputChanges[output->first] = 0;
			}else if(output->second == "HIGH_L" && direction == 2){
				::digitalWrite(output->first, HIGH); 
				outputChanges[output->first] = 1;
			}else{
				//::digitalWrite(output->first, LOW);
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
bool ErrorController::lcdError(int id, int totalErrors)
{
	// Has the error message changed?
	if(displayed_error == -1 || displayed_error != id){
		displayed_error = id;
		// Check there are errors that required lcd message
		//if(error_container[id]["type"][0] != "3"){
		
			// Set line three to show addtions errors
			//if(totalErrors > 1 && error_container[id]["type"][0] == "2")
				//error_container[id]["action_message"][3] = "+" + String(totalErrors-1) + " error/s; Solve current to view.";

			// Refresh LCD
			//LcdHandle->errorCondition(id,totalErrors,1, 1);

			//lcd_error_on = true;
		//}
	}

	return true;
}


bool ErrorController::lcdErrorClear(){
	// Clear error
	//LcdHandle->errorCondition(0, 0);

	lcd_error_on = false;

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
	// Loop though error condtions and get the trigged condtions ids
	std::map<int, int> triggered;
	int triggered_id;
	int c = error_container.size();
	for(int i = 0; i < c; i++){
		triggered_id = this->ensure(i);
		if(triggered_id == 1)
			triggered[i] = triggered_id;
	}
	::Serial.println("hellow");
	// If there is an error
	if(!triggered.empty()){
		this->lcdError(triggered[0], triggered.size());
	}else if(lcd_error_on == true){
		this->lcdErrorClear();
	}
	::Serial.println("hellow");
	return true;
}
