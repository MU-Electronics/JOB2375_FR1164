/**
 * Include relivent C++ Libraires
 */
#include <map>
#include <string>
#include <Arduino.h>


namespace VoltageConfiguration
{


	/**
	 * Setup ADC configurations
	 */
	static std::map< String, std::map<String, int> > setupAdcs(  )
	{
		//Setup container
		std::map< String, std::map<String, int> > adc_config_container;

		//Set voltage ADC pins
		adc_config_container["EXTERNAL_ADC"]["SEL_PIN"] = 10;
		adc_config_container["EXTERNAL_ADC"]["DATAOUT_PIN"] = 11;
		adc_config_container["EXTERNAL_ADC"]["DATAIN_PIN"] = 13;
		adc_config_container["EXTERNAL_ADC"]["SPICLOCK_PIN"] = 12;

		//Set ADC bit sizes
		adc_config_container["INTERNAL_ADC"]["BIT_SIZE"] = 1024;
		adc_config_container["EXTERNAL_ADC"]["BIT_SIZE"] = 4096;

		return adc_config_container;
	}


	/**
	 * Setup voltage configurations
	 */
	static std::map< String, std::map<int, float> > setupVoltages(  )
	{
		//Setup container
		std::map< String, std::map<int, float> > voltage_config_container;

		//Set channel with their voltages
		voltage_config_container["INTERNAL"][1] = 200;
		voltage_config_container["INTERNAL"][2] = 200;
		voltage_config_container["INTERNAL"][3] = 200;
		voltage_config_container["INTERNAL"][4] = 200;
		//voltage_config_container["INTERNAL"][5] = 200;
		voltage_config_container["EXTERNAL"][0] = 200;
		voltage_config_container["EXTERNAL"][1] = 4.096;
		voltage_config_container["EXTERNAL"][2] = 200;
		voltage_config_container["EXTERNAL"][3] = 200;
		voltage_config_container["EXTERNAL"][4] = 200;
		voltage_config_container["EXTERNAL"][5] = 200;
		voltage_config_container["EXTERNAL"][6] = 200;
		voltage_config_container["EXTERNAL"][7] = 200;

		return voltage_config_container;
	}


	/**
	 * Setup moving average configurations
	 */
	static int setupMovingAverage(  )
	{
		return 5;
	}

}