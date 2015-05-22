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
	static std::vector< std::map< String, std::map<int, String> > > setupErrors()
	{
		//Config container
		std::map< String, std::map<int, String> > error_config_container;
		std::vector< std::map< String, std::map<int, String> > > output;
		
		/////////////////////////////////////////
		// Setup Plug and Swicth               //
		/////////////////////////////////////////
		// Define sub containers
		error_config_container["conditions"][52] = "HIGH";
		error_config_container["conditions"][50] = "HIGH";
		error_config_container["action_outputs"][46] = "HIGH";
		error_config_container["action_outputs"][44] = "HIGH";
		error_config_container["action_message"][0] = "Error with plug and swicth";
		error_config_container["action_method"][0] = "0";
		output.push_back(error_config_container);  error_config_container.clear();

		

		/////////////////////////////////////////
		// Set up interlock switches           //
		/////////////////////////////////////////
		// Define sub containers
		error_config_container["conditions"][46] = "LOW";
		error_config_container["conditions"][44] = "HIGH";
		error_config_container["action_outputs"][33] = "HIGH";
		error_config_container["action_outputs"][34] = "HIGH";
		error_config_container["action_message"][0] = "Error with plug and swicth";
		error_config_container["action_method"][0] = "0";
		output.push_back(error_config_container);  error_config_container.clear();

		return output;
	}

}