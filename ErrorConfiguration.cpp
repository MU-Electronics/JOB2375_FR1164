/**
 * Include relivent C++ Libraires
 */
#include <map>
#include <string>
#include <vector>
#include <Arduino.h>


namespace ErrorConfiguration
{
	/**
	 * Setup all system error configurations
	 *
	 * Vector of maps is used are Ardunio wont upload with map of maps
	 */
	static std::vector< std::map< String, std::map<String, String> > > setupErrors()
	{
		//Config container
		std::map< String, std::map<String, String> > error_config_container;
		std::vector< std::map< String, std::map<String, String> > > output;
		
		/////////////////////////////////////////
		// Setup Plug and Swicth               //
		/////////////////////////////////////////
		// Define sub containers
		error_config_container["conditions"]["30"] = "1";
		error_config_container["conditions"]["32"] = "0";
		error_config_container["action_outputs"]["33"] = "1";
		error_config_container["action_outputs"]["34"] = "1";
		error_config_container["action_message"]["lcd"] = "Error with plug and swicth";
		error_config_container["action_method"]["0"] = "0";
		output.push_back(error_config_container);  error_config_container.clear();

		

		/////////////////////////////////////////
		// Set up interlock switches           //
		/////////////////////////////////////////
		// Define sub containers
		error_config_container["conditions"]["30"] = "1";
		error_config_container["conditions"]["32"] = "0";
		error_config_container["action_outputs"]["33"] = "1";
		error_config_container["action_outputs"]["34"] = "1";
		error_config_container["action_message"]["lcd"] = "Error with plug and swicth";
		error_config_container["action_method"]["0"] = "0";
		output.push_back(error_config_container);  error_config_container.clear();

		return output;
	}

}