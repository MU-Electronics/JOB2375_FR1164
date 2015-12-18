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
		// HV off posative mode                //
		/////////////////////////////////////////
		// Define sub containers
		error_config_container["conditions"][48] = "HIGH"; // HV on swicth
		error_config_container["action_outputs"][31] = "LOW"; // HV mode
		error_config_container["action_message"][0] = "High Voltage Off";
		error_config_container["action_message"][1] = "Please turn on to continue";
		error_config_container["action_message"][2] = "or configure mode here";
		error_config_container["action_message"][2] = "";
		error_config_container["action_method"][0] = "0";
		error_config_container["type"][0] = "1";
		output.push_back(error_config_container);  error_config_container.clear();

		/*/////////////////////////////////////////
		// Check for mode change with power on //
		/////////////////////////////////////////
		// Define sub containers
		error_config_container["conditions"][48] = "HIGH"; // HV on swicth
		error_config_container["conditions"][50] = "CHANGE"; // Mode select swicth
		error_config_container["action_message"][0] = "Turn high voltage off";
		error_config_container["action_message"][1] = "to configure mode";
		error_config_container["type"][0] = "1";
		output.push_back(error_config_container);  error_config_container.clear();

		/////////////////////////////////////////
		// Check for mode change with power on //
		/////////////////////////////////////////
		// Define sub containers
		error_config_container["conditions"][48] = "LOW"; // HV on swicth
		error_config_container["conditions"][50] = "HIGH"; // Mode select swicth
		error_config_container["action_outputs"][29] = "HIGH_L"; // HV mode
		error_config_container["type"][0] = "3";
		output.push_back(error_config_container);  error_config_container.clear();
		error_config_container["conditions"][48] = "LOW"; // HV on swicth
		error_config_container["conditions"][50] = "LOW"; // Mode select swicth
		error_config_container["action_outputs"][29] = "LOW_L"; // HV mode
		error_config_container["type"][0] = "3";
		output.push_back(error_config_container);  error_config_container.clear();
		*/
		
		/*
		/////////////////////////////////////////
		// Set up interlock switches           //
		/////////////////////////////////////////
		// Define sub containers
		error_config_container["conditions"][40] = "HIGH"; //When HIGH throw error
		error_config_container["conditions"][42] = "HIGH"; //When HIGH throw error
		error_config_container["action_outputs"][33] = "HIGH";
		error_config_container["action_outputs"][34] = "HIGH";
		error_config_container["action_message"][0] = "Error: Internal interlocks";
		error_config_container["action_message"][1] = "Check encloser's panels are secure and";
		error_config_container["action_message"][2] = "in place correctly";
		error_config_container["action_method"][0] = "0";
		output.push_back(error_config_container);  error_config_container.clear();
		*/

		// Return the error condtions
		return output;
	}

}