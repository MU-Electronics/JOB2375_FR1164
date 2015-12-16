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
		voltage_config_container["INTERNAL"][1] = 100;
		voltage_config_container["INTERNAL"][2] = 100;
		voltage_config_container["INTERNAL"][3] = 100;
		voltage_config_container["INTERNAL"][4] = 100;
		voltage_config_container["EXTERNAL"][0] = 100;
		voltage_config_container["EXTERNAL"][1] = 100;
		voltage_config_container["EXTERNAL"][2] = 100;
		voltage_config_container["EXTERNAL"][3] = 100;
		voltage_config_container["EXTERNAL"][4] = 100;
		voltage_config_container["EXTERNAL"][5] = 100;
		voltage_config_container["EXTERNAL"][6] = 100;
		voltage_config_container["EXTERNAL"][7] = 100;

		return voltage_config_container;
	}

	/**
	 * Setup voltage display blocks
	 */
	static std::map< int, int > setupVoltagesBlock()
	{
		//Setup container
		std::map< int, int > blocks;

		//Set voltage block postions (External then internal)
			//Internal
			blocks[1] = 5; 
			blocks[2] = 2; 
			blocks[3] = 3; 
			blocks[4] = 4;

			//External
			blocks[5] = 1;
			blocks[6] = 6; 
			blocks[7] = 7; 
			blocks[8] = 8; 
			blocks[9] = 9; 
			blocks[10] = 10; 
			blocks[11] = 11; 
			blocks[12] = 12; 

		return blocks;
	}

	/**
	 * Setup voltage accurcy
	 */
	static std::map< String, std::map<int, int> > setupVoltagesAccurcy(  )
	{
		//Setup container
		std::map< String, std::map<int, int> > voltage_config_container;

		//Set channel with their voltages
		voltage_config_container["INTERNAL"][1] = 1;
		voltage_config_container["INTERNAL"][2] = 1;
		voltage_config_container["INTERNAL"][3] = 1;
		voltage_config_container["INTERNAL"][4] = 1;
		voltage_config_container["EXTERNAL"][0] = 1;
		voltage_config_container["EXTERNAL"][1] = 1;
		voltage_config_container["EXTERNAL"][2] = 1;
		voltage_config_container["EXTERNAL"][3] = 1;
		voltage_config_container["EXTERNAL"][4] = 1;
		voltage_config_container["EXTERNAL"][5] = 1;
		voltage_config_container["EXTERNAL"][6] = 1;
		voltage_config_container["EXTERNAL"][7] = 1;

		return voltage_config_container;
	}


	/**
	 * Setup moving average configurations
	 */
	static int setupMovingAverage(  )
	{
		return 3;
	}


	/**
	 * Setup ADC jitter filter with respect to the END VOLTAGE
	 */
	static std::map< String, std::map<int, float> > setupVoltagesJitter(  )
	{
		//Setup container
		std::map< String, std::map<int, float> > voltage_config_container;

		//Set channel with their voltages
		voltage_config_container["INTERNAL"][1] = 0;
		voltage_config_container["INTERNAL"][2] = 0;
		voltage_config_container["INTERNAL"][3] = 0;
		voltage_config_container["INTERNAL"][4] = 0;
		voltage_config_container["EXTERNAL"][0] = 0;
		voltage_config_container["EXTERNAL"][1] = 0;
		voltage_config_container["EXTERNAL"][2] = 0;
		voltage_config_container["EXTERNAL"][3] = 0;
		voltage_config_container["EXTERNAL"][4] = 0;
		voltage_config_container["EXTERNAL"][5] = 0;
		voltage_config_container["EXTERNAL"][6] = 0;
		voltage_config_container["EXTERNAL"][7] = 0;

		return voltage_config_container;
	}


}