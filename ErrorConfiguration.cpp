/**
 * Include relivent C++ Libraires
 */
#include <map>
#include <string>
#include <Arduino.h>


namespace ErrorConfiguration
{

	/**
	 * HELPER FUNCTION: Build error conditions empty
	 */
	static std::map< String, std::map<String, String> > buildError( int id )
	{
		// Define error boxes
		std::map< String, std::map<String, String> > error_conditions;
		std::map<String, String> error_values;

		// Set three condtions
		error_conditions["conditions"] = error_values;
		error_conditions["message"] = error_values;
		error_conditions["action_outputs"] = error_values;
		error_conditions["action_method"] = error_values; 

		return error_conditions;
	}



	/**
	 * HELPER FUNCTION: Constuct and define an error condtions
	 */
	static std::map< int, std::map< String, std::map<String, String> > > construct(int id, std::map<String, String> condtions, std::map<String, String> actionOutput, String actionMessage, String actionMethod, std::map< int, std::map< String, std::map<String, String> > > error_config_container)
	{
		std::map< String, std::map<String, String> > build = buildError( id );

		// Define conditions
		error_config_container[id]["conditions"] = condtions;

		// Define message
		error_config_container[id]["message"]["0"] = actionMessage;
	
		// Define output actions
		error_config_container[id]["action_outputs"] = actionOutput;

		// Define action method
		error_config_container[id]["action_method"]["0"] = actionMethod;

		// Return the error condition id
		return error_config_container;
		
	}



	/**
	 * Setup all system error configurations
	 */
	static std::map< int, std::map< String, std::map<String, String> > > setupErrors()
	{
		std::map< int, std::map< String, std::map<String, String> > > error_config_container;

		
		/////////////////////////////////////////
		// Setup Plug and Swicth               //
		/////////////////////////////////////////
		// Define sub containers
		error_config_container[1]["conditions"]["30"] = "1";
		error_config_container[1]["conditions"]["32"] = "0";
		error_config_container[1]["conditions"]["33"] = "1";
		error_config_container[1]["conditions"]["34"] = "1";
		error_config_container[1]["message"]["lcd"] = "Error with plug and swicth";
		error_config_container[1]["action_method"]["0"] = "0";
		
	

		/////////////////////////////////////////
		// Set up interlock switches           //
		/////////////////////////////////////////
		// Define sub containers
		/*std::map<String, String> interlocks_condtions;
		std::map<String, String> interlocks_action;
		// Define error conditions
		interlocks_condtions["30"] = "1"; interlocks_condtions["32"] = "0";
		// Define outputs on error
		interlocks_action["33"] = "1"; interlocks_action["34"] = "1";
		// Define LCD error message
		String interlocks_message = "Error with interlocks";
		// Define Error funnction
		String interlocks_function = "interlocks";
		// Merge data into main container
		error_config_container = construct(2, interlocks_condtions, interlocks_action, interlocks_message, interlocks_function, error_config_container);	
		*/
		return error_config_container;
	}

}