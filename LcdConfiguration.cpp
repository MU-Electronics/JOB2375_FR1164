/**
 * Include relivent C++ Libraires
 */

#include <vector>
#include <map>
#include <Arduino.h>


namespace LcdConfiguration
{
	/**
	 * Setup all system error configurations
	 * @note funny vector setup because ardunio does not like to return maps 
	 * @note the order of the below setting is IMPORTANT due to forced to using vector
	 */
	static std::vector< std::vector< int > > setupLcdDisplay()
	{
		std::vector< std::vector< int > > lcd_config_container;
		std::vector< int > temp_container;


		// Set LCD one pin numbers ID=0
		int LCD_ONE_PINS [] = {27, 23, 30, 32, 34, 36};
		temp_container.insert( temp_container.begin() , LCD_ONE_PINS , LCD_ONE_PINS + 6 ) ; 
		lcd_config_container.push_back(temp_container); temp_container.clear();


		// Set LCD two pin numbers ID=1
		int LCD_TWO_PINS [] = {27, 25, 30, 32, 34, 36};
		temp_container.insert( temp_container.begin() , LCD_TWO_PINS , LCD_TWO_PINS + 6 ) ; 
		lcd_config_container.push_back(temp_container); temp_container.clear();


		// Setup Lcd Size ID=2
		int LCD_SIZE [] = {40, 2};
		temp_container.insert( temp_container.begin() , LCD_SIZE , LCD_SIZE + 2 ) ; 
		lcd_config_container.push_back(temp_container); temp_container.clear();


		// Set LCD ids for blocks
		int BLOCK_LCD_IDS [] = {
			1,1,1,1,
			1,1,1,1,
			2,2,2,2,
			2,2,2,2
		};
		temp_container.insert( temp_container.begin() , BLOCK_LCD_IDS , BLOCK_LCD_IDS + 16 ) ; 
		lcd_config_container.push_back(temp_container); temp_container.clear();
		

		// Set LCD row for blocks
		int BLOCK_ROWS [] = {
			0,0,0,0,
			1,1,1,1,
			0,0,0,0,
			1,1,1,1
		};
		temp_container.insert( temp_container.begin() , BLOCK_ROWS , BLOCK_ROWS + 16 ) ; 
		lcd_config_container.push_back(temp_container); temp_container.clear();


		// Set LCD col for blocks
		int BLOCK_COLS [] = {
			0,10,20,30,
			0,10,20,30,
			0,10,20,30,
			0,7,14,30
		};
		temp_container.insert( temp_container.begin() , BLOCK_COLS , BLOCK_COLS + 16 ) ; 
		lcd_config_container.push_back(temp_container); temp_container.clear();

		
		// Return setup data
		return lcd_config_container;


	}


	/**
	 * Update screen every Xms
	 */
	static int refreshRate()
	{
		return 50;
	}


	/**
	 * Which pin to check the power supply level
	 *
	 * This is required to ensure the lcd is not wrote to when there is a supply blip (EMI).
	 */
	static int supplyCheck()
	{
		return 4;
	}


	/**
	 * Voltage level at which the lcd can be wrote at
	 *
	 * Bare in mind that the voltage level is being read by a 10 bit ADC
	 */
	static int supplyCheckLevel()
	{
		return 1020;
	}
	

	/**
	 * Information to show on the welcome screen on boot
	 */
	static std::map< int, String > welcome()
	{
		// Create container
		 std::map< int, String > welcome;

		// Line one
		 welcome[0] = "Mass Spec Lens Power Supply";

		// Line two
		welcome[4] = "Setting up system... Please wait";

		// Line three
		welcome[8] = "Electronics Section, School of Chemistry";

		// Line four
		welcome[12] = "Tel: 0161 275 4641";
		
		return welcome;
	}
}