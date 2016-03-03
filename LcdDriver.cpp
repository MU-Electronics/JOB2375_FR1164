/**
 * Include libraries 
 */
#include <vector>
#include <algorithm>
#include <map>
#include <string>
#include "LcdDriver.h"
#include "LcdConfiguration.cpp"
#include "VoltageConfiguration.cpp"




/**
 * Set up 
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 * @ NOTE should really DI the settings
 */
LcdDriver::LcdDriver(void)
{
	// Get Lcd setup data from configuration file
	lcdSetup = LcdConfiguration::setupLcdDisplay();

	// Setup Lcds
	std::vector< int > LcdOne = lcdSetup[0];
	std::vector< int > LcdTwo = lcdSetup[1];
	lcd[1] = new LiquidCrystal(LcdOne[0], LcdOne[1], LcdOne[2], LcdOne[3], LcdOne[4], LcdOne[5]);
	lcd[2] = new LiquidCrystal(LcdTwo[0], LcdTwo[1], LcdTwo[2], LcdTwo[3], LcdTwo[4], LcdTwo[5]);	

	// Set up the LCD's number of columns and rows: 
	lcd[1]->begin(lcdSetup[2][0], lcdSetup[2][1]);
    lcd[2]->begin(lcdSetup[2][0], lcdSetup[2][1]);

	// Set LCD ids for blocks
	this->BLOCK_LCD_IDS = lcdSetup[3];

	// Set LCD row for blocks
	this->BLOCK_ROWS = lcdSetup[4];

	// Set LCD col for blocks
	this->BLOCK_COLS = lcdSetup[5];

	// voltage check port
	voltageCheckPort = LcdConfiguration::supplyCheck();

	// Allowed voltage level
	voltageCheckLevel = LcdConfiguration::supplyCheckLevel();
}






/**
 * On class destruction 
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 * @NOTE using smart pointers with object can be a bit weird on ardunio for some reason
 *       for thius reason C pointer is used
 */
LcdDriver::~LcdDriver(void)
{
	// Smart pointer should really be used
	delete lcd;
}






/**
 * PROTECTED Move curser to X, Y on LCD Z
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 * @pram int lcdId The Identification of the array where the LCD 
 */
bool LcdDriver::move(int lcdId, int x, int y)
{
	// Set the cursor postion
	this->lcd[lcdId]->setCursor(x, y);

	// Task completed 
	return true;
}


/**
 * PROTECTED Set curser to block location
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 */
bool LcdDriver::setBlock(int block)
{
	//Init varables 
	int lcdId, col, row;
	//Set LCD ID
	lcdId = this->BLOCK_LCD_IDS[block];
	//Set col 
	col = this->BLOCK_COLS[block];
	//Set row
	row = this->BLOCK_ROWS[block];
	//Move curser to calculated postion
	this->move(lcdId, col, row);

	// Success
	return true;
}


/**
 * PROTECTED Print string to LCD where ever the curser is
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 */
bool LcdDriver::print(String string, int block)
{
	// Check the voltage level
	int level = ::analogRead(voltageCheckPort);
	if(level > voltageCheckLevel){
		// Print string to LCD
		if(block <= 7) {
			// If block less than 10 clear blocks previous content
			if(string.length() < 10) { this->clearBlock(block); }
			// Printing to LCD 1
			this->lcd[1]->print(string);
		}
		else
		{
			// If block less than 10 clear blocks previous content
			if(string.length() < 10) { this->clearBlock(block); }
			// Printing to LCD 2
			this->lcd[2]->print(string);
		}

		//Task completed
		return true;
	}

	// Print error to serial to ensure capture
	::Serial.print("ERROR: VCC was not high enough to write to the lcd display; "); ::Serial.print(level); ::Serial.println(" out of 1024 is not within the accepted range.");

	// Return task failed
	return false;
	
}






/**
 * PUBLIC Show a string at X, Y
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 */
bool LcdDriver::show(String toShow, int block)
{

	// Set block
	bool blockSet = this->setBlock(block);

	// If block set successfully 
	if(blockSet)
	{
		// Show string
		if(this->print(toShow, block))	
			return true;
	}
	
	
	// Something went wrong throw exception??
	return false;
}


/**
 * PUBLIC Show a string on row
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 */
bool LcdDriver::showRow(String toShow, int row)
{
	// Set lcd id
	int lcdId = 1;
	if(row >= 3){
		lcdId = 2;
		row = row - 2;
	}

	// Set row
	this->move(lcdId, 0, row-1);


	// Show string
	if(this->lcd[lcdId]->print(toShow))	
		return true;
		
}


/**
 * PUBLIC Clear a block of its content
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 */
bool LcdDriver::clearBlock(int block)
{
	// Set curser to block
	this->setBlock(block);

	// Find the number of sqaures to clear
	int toClear;
	if(block == 15 || block == 7)
	{
		toClear = 40 - this->BLOCK_COLS[block];
	}
	else
	{
		toClear = this->BLOCK_COLS[block + 1] - this->BLOCK_COLS[block];
	}

	// Clear the number of squares
	String spaces;
	for (int i=toClear; i>0; i--){
		spaces = spaces + " ";
	}

	// Clear blocks
	if(block <= 7) { this->lcd[1]->print(spaces); }else{ this->lcd[2]->print(spaces); }

	// Set curser to block
	this->setBlock(block);
}

bool LcdDriver::clearAll()
{
	// Clear display
	this->lcd[1]->clear();
	this->lcd[2]->clear();

	this->lcd[1]->home();
	this->lcd[2]->home();

	this->lcd[1]->noBlink();
	this->lcd[2]->noBlink();

	this->lcd[1]->noCursor();
	this->lcd[2]->noCursor();

	// Clear display
	this->lcd[1]->clear();
	this->lcd[2]->clear();

	this->lcd[1]->home();
	this->lcd[2]->home();

	this->lcd[1]->noBlink();
	this->lcd[2]->noBlink();

	this->lcd[1]->noCursor();
	this->lcd[2]->noCursor();

	return true;
}

