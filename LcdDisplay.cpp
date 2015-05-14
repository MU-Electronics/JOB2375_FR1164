/**
 * Include libraries 
 */
#include "LcdDisplay.h"
#include <vector>
#include <algorithm>






/**
 * Set up 
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 */
LcdDisplay::LcdDisplay(void)
{
	//Setup Lcds
	lcd[1] = new LiquidCrystal(27, 23, 30, 32, 34, 36);
	lcd[2] = new LiquidCrystal(27, 25, 30, 32, 34, 36);	

	// Set up the LCD's number of columns and rows: 
    lcd[1]->begin(40, 2);
    lcd[2]->begin(40, 2);

	//Set LCD ids for blocks
	int BLOCK_LCD_IDS [] = {
		1,1,1,1,
		1,1,1,1,
		2,2,2,2,
		2,2,2,2
	};

	//Set LCD row for blocks
	int BLOCK_ROWS [] = {
		0,0,0,0,
		1,1,1,1,
		0,0,0,0,
		1,1,1,1
	};

	//Set LCD col for blocks
	int BLOCK_COLS [] = {
		0,10,20,30,
		0,10,20,30,
		0,10,20,30,
		0,7,14,30
	};

	//Assign the above block identifiers to class variables
	std::copy(BLOCK_LCD_IDS, BLOCK_LCD_IDS + 16, this->BLOCK_LCD_IDS);
	std::copy(BLOCK_ROWS, BLOCK_ROWS + 16, this->BLOCK_ROWS);
	std::copy(BLOCK_COLS, BLOCK_COLS + 16, this->BLOCK_COLS);
}






/**
 * On class destruction 
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 */
LcdDisplay::~LcdDisplay(void)
{
	// Smart pointer should really be used
	delete lcd;
}






/**
 * PROTECTED Move curser to X, Y on LCD Z
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 * @pram int lcdId The Identification of the array where the LCD 
 */
bool LcdDisplay::move(int lcdId, int x, int y)
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
bool LcdDisplay::setBlock(int block)
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
bool LcdDisplay::print(String string, int block)
{
	// Print string to LCD
	if(block <= 7) {
		// Printing to LCD 1
		this->lcd[1]->print(string);
	}else{
		// Printing to LCD 2
		this->lcd[2]->print(string);
	}

	//Task completed
	return true;
}






/**
 * PUBLIC Show a string at X, Y
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 */
bool LcdDisplay::show(String toShow, int block)
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



