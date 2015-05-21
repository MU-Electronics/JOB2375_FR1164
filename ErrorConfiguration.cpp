/**
 * Include relivent C++ Libraires
 */
#include <StandardCplusplus.h>
#include <map>
#include <string>
#include <Arduino.h>

namespace ErrorConfiguration
{
	/**
	 * Define the error configuration container
	 */
	static std::map< int, std::map< String, std::map<String, String> > > error_config_container;



	/**
	 * HELPER FUNCTION: Build error conditions empty
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
	 * HELPER FUNCTION: Constuct and define an error condtions
	 */
	static int construct(int id, std::map<String, String> condtions, std::map<String, String> actionOutput, String actionMessage, String actionMethod)
	{
		int build = buildError( id );

		// Define conditions
		error_config_container[id]["conditions"] = condtions;

		// Define message
		error_config_container[id]["message"]["0"] = actionMessage;
	
		// Define output actions
		error_config_container[id]["action_outputs"] = actionOutput;

		// Define action method
		error_config_container[id]["action_method"]["0"] = actionMethod;

		// Return the error condition id
		return build;
	}



	/**
	 * Setup all system error configurations
	 */
	static void setupErrors( )
	{
		/////////////////////////////////////////
		// Setup Plug and Swicth               //
		/////////////////////////////////////////
		// Define sub containers
		std::map<String, String> plug_switch_condtions;
		std::map<String, String> plug_switch_action;
		// Define error conditions
		plug_switch_condtions["30"] = "1"; plug_switch_condtions["32"] = "0";
		// Define outputs on error
		plug_switch_action["33"] = "1"; plug_switch_action["34"] = "1";
		// Define LCD error message
		String plug_switch_message = "Error with switch and plug";
		// Define Error funnction
		String plug_switch_function = "none";
		// Merge data into main container
		int plug_switch = construct(1, plug_switch_condtions, plug_switch_action, plug_switch_message, plug_switch_function);


		/////////////////////////////////////////
		// Set up interlock switches           //
		/////////////////////////////////////////
		// Define sub containers
		std::map<String, String> interlocks_condtions;
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
		int interlock = construct(2, interlocks_condtions, interlocks_action, interlocks_message, interlocks_function);	
	}

}