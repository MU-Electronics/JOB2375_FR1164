/**
 * Include libraries
 */
#include <LiquidCrystal.h>
#include <StandardCplusplus.h>
#include <serstream>
#include <string>
#include <vector>
#include <iterator>
#include <iostream>
#include "LcdDisplay.h"




/**
 * Init singleton classes
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 */
LcdDisplay* Lcd = new LcdDisplay();




/**
 * Run setup procedures
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 */
void setup()
{
  Serial.begin(9600);
}




/**
 * Run general loop
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 */
void loop()
{
	String test = "100";
	Lcd->show("VA:"+test+"V ", 0);
	Lcd->show("VB:100.7V ", 1);
	Lcd->show("VD:100.7V ", 2);
	Lcd->show("VE:100.7V ", 3);
	Lcd->show("VF:100.7V ", 4);
	Lcd->show("VG:100.7V ", 5);
	Lcd->show("VH:100.7V ", 6);
	Lcd->show("VI:100.7V ", 7);
	Lcd->show("VJ:100.7V ", 8);
	Lcd->show("VK:100.7V ", 9);
	Lcd->show("VL:100.7V ", 10);
	Lcd->show("VM:100.7V ", 11);
	
	Lcd->show("HV ON ", 12);
	Lcd->show("+MODE ", 13);
	Lcd->show("|| ERROR PLUGS POLARITY", 14);

	delay(4000); 
	Lcd->show("VM:500.7V ", 11);
	delay(4000); 
}
