#ifndef ErrorConfiguration
#define ErrorConfiguration

/**
 * Include relivent C++ Libraires
 */
#include <StandardCplusplus.h>
#include <map>
#include <string>
#include <algorithm>
#include <sstream>

/**
 * Define the error configuration container
 */
static std::map< int, std::map< String, std::map<String, String> > > error_config_container;









/**
 * Build error conditions empty
 */
static int buildError( int id )
{
	// Define error boxes
	std::map< String, std::map<String, String> > error_conditions;
	std::map<String, String> error_values;

	// Set three condtions
	error_conditions["conditions"] = error_values;
	error_conditions["message"] = error_values;
	error_conditions["action_outputs"] = error_values;
	error_conditions["action_method"] = error_values;

	error_config_container[id] = error_conditions;

	return id;
}



/**
 * Constuct and define an error condtions
 */
static int construct(int id, String* lowCon, String* highCon, String message, String* actionHigh, String* actionLow, String actionMethod)
{
	int build = buildError( id );

	// Define conditions
    for (String *ptr = lowCon; *ptr; ptr++) {
        error_config_container[id]["conditions"][*ptr] = "0";
    }
	for (String *ptr = highCon; *ptr; ptr++) {
        error_config_container[id]["conditions"][*ptr] = "1";
    }

	// Define message
	error_config_container[id]["message"]["1"] = message;
	
	// Define output actions
	for (String *ptr = actionLow; *ptr; ptr++) {
        error_config_container[id]["action_outputs"][*ptr] = "0";
    }
	for (String *ptr = actionHigh; *ptr; ptr++) {
        error_config_container[id]["action_outputs"][*ptr] = "1";
    }

	// Define action method
	error_config_container[id]["action_method"]["0"] = actionMethod;

	// Return the error condition id
	return build;
}









/**
 * Define conditions
 */
//Plug and switch
static String plug_switch_high_cond[] = {"30"};
static String plug_switch_low_cond[] = {"32"};

//Interlocks
static String interlock_high_cond[] = {"22","23","24","25"};
static String interlock_low_cond[] = {"0"};




/**
 * Define messages
 */
//Plug and switch
static String plug_switch_message = "There is an error with the plug and switch";

//Interlocks
static String interlock_message = "There is an error with the interlock switches";




/**
 * Define action pointer functions
 */
//Plug and switch
static String plug_switch_function = "none";

//Interlocks
static String interlock_function = "interlocks";



/**
 * Define action outputs
 */
//Plug and switch
static String plug_switch_high_action[] = {"19"};
static String plug_switch_low_action[] = {"0"};

//Interlocks
static String interlock_high_action[] = {"0"};
static String interlock_low_action[] = {"0"};




/**
 * Define errors
 */
// Plug and switch error condition
static int plug_switch_error_id = construct(1, plug_switch_low_cond, plug_switch_high_cond, plug_switch_message, plug_switch_high_action, plug_switch_low_action, plug_switch_function);

//Interlocks error condition
static int interlock_error_id = construct(2, interlock_low_cond, interlock_high_cond, interlock_message, interlock_high_action, interlock_low_action, interlock_function);



#endif