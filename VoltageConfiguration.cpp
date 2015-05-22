/**
 * Include relivent C++ Libraires
 */
#include <map>
#include <string>
#include <Arduino.h>


namespace VoltageConfiguration
{

	/**
	 * HELPER FUNCTION: Build conditions empty
	 */
	static std::map< String, std::map<String, String> > build( int id )
	{
		
	}




	/**
	 * Setup all system error configurations
	 */
	static std::map< String, std::map<String, String> > setupErrors( std::map< String, std::map<String, String> > voltage_config_container )
	{
		//Set voltage ADC pins
		voltage_config_container["EXTERNAL_ADC"]["SEL_PIN"] = "10";
		voltage_config_container["EXTERNAL_ADC"]["DATAOUT_PIN"] = "11";
		voltage_config_container["EXTERNAL_ADC"]["DATAIN_PIN"] = "13";
		voltage_config_container["EXTERNAL_ADC"]["SPICLOCK_PIN"] = "12";

		//Set bit sizes
		voltage_config_container["INTERNAL_ADC"]["BIT_SIZE"] = "1024";
		voltage_config_container["EXTERNAL_ADC"]["BIT_SIZE"] = "4096";

		//Set voltages
		//voltage_config_container["INTERNAL_ADC"]["voltages"] = {"200","200","200","200","200"};
		//voltage_config_container["EXTERNAL_ADC"]["BIT_SIZE"] = "4096";
	}

}