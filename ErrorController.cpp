/**
 * Include libraries
 */
#include "ErrorController.h"
#include "ErrorConfiguration.cpp"



/**
 * Setup class
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 */
ErrorController::ErrorController()
{
	// Get error setups
	std::map< int, std::map< String, std::map<String, String> > > error_config_container;

	// Setup system error condtions
	error_config_container = ErrorConfiguration::setupErrors();

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
	return false;
}

/**
 * PRIVATE Set outputs to correct value when error condtion is met
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 *
 * @pram outputs Map of outputs to there digital value
 */
bool ErrorController::output(std::map<String, String> outputs)
{
	return false;
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
bool ErrorController::check(int id)
{
	return false;
}
