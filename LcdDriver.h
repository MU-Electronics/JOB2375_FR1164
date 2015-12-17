/**
 * Include Libraries 
 */
#include "LiquidCrystal.h"
#include <Arduino.h>
#include <map>
#include <vector>
#include "Voltage.h"



#pragma once
// This should really be DI'ed
extern VoltageMeasure* Voltages;

class LcdDriver
{
	

	public:
		LcdDriver(void);
		~LcdDriver(void);
	protected:
		bool move(int lcdId, int x, int y);
		bool setBlock(int block);
		bool print(String string, int block);
		std::vector< std::vector< int > > lcdSetup;
	public:
		std::vector< int > BLOCK_LCD_IDS;
		std::vector< int > BLOCK_ROWS;
		std::vector< int > BLOCK_COLS;
		LiquidCrystal* lcd[2];
		bool show(String toShow, int block);
		bool clearBlock(int block);
		bool clearAll();
		bool showRow(String toShow, int row);
};

