/**
 * Include libraries
 */
#include <LiquidCrystal.h>
#include <StandardCplusplus.h>

#include <serstream>
#include <string>
#include <vector>
/**
 * Require standard C++ libraries
 */
#include <iterator>
#include <iostream>
#include <algorithm>
#include <map>

#include <string.h>

/**
 * Require projects classes etc
 */
#include "LcdDisplay.h"
#include "VoltageMeasure.h"







/**
 * Set required varables 
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 */
// Internal ADCs that required a moving average
int internalADC[] = {1,2,3,4,5}; 
std::vector<int> averageChannelsInt(internalADC, internalADC + 5);
// Internal ADCs that required a moving average
int externalADC[] = {0,1,2,3,4,5,6,7}; 
std::vector<int> averageChannelsExt(externalADC, externalADC + 8);

//Set Voltages
float DigitalVoltagesInt[] = {200,200,200,200,200}; 
std::vector<float> DigitalVoltagesInternal(DigitalVoltagesInt, DigitalVoltagesInt + 5);
float DigitalVoltagesExt[] = {200,4.096,700,200,200,200,200,200}; 
std::vector<float> DigitalVoltagesExternal(DigitalVoltagesExt, DigitalVoltagesExt + 8);





/**
 * Init singleton classes
 * @author Sam Mottley sam.mottley@manchester.ac.uk
 */
LcdDisplay* Lcd = new LcdDisplay();
VoltageMeasure* Voltages = new VoltageMeasure(averageChannelsInt, averageChannelsExt, DigitalVoltagesInternal, DigitalVoltagesExternal);
long previousMillis = 0;
long previousMillis2 = 0;
long updateEvery = 200;






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
	
	//unsigned long currentMillis2 = millis(); 
	//if(currentMillis2 - previousMillis2 > 1) 
	//{
		//Set lasted updated
		//previousMillis2 = currentMillis2; 

		//float currentMillisCount = micros(); 
		//Update LCD
		Voltages->update(averageChannelsInt, averageChannelsExt);
		//float currentMillisCount2 = micros();

		//Serial.println(currentMillisCount2-currentMillisCount);
		//delay(200);
	//}

	unsigned long currentMillis = millis(); 
	if(currentMillis - previousMillis > updateEvery) 
	{
		//Set lasted updated
		previousMillis = currentMillis; 
		//Update LCD
		LcdUpdate();
	}
	
}






/**
 * Update LCD Display
 */
void LcdUpdate()
{
	Lcd->show("VA:"+FloatToString(Voltages->get(averageChannelsExt[1], 0))+"v ", 0);
	Lcd->show("VB:"+FloatToString(Voltages->get(averageChannelsExt[2], 0))+"v ", 1);
	Lcd->show("VC:"+FloatToString(Voltages->get(averageChannelsExt[3], 0))+"v ", 2);
	Lcd->show("VD:"+FloatToString(Voltages->get(averageChannelsExt[4], 1))+"v ", 3);
	Lcd->show("VE:"+FloatToString(Voltages->get(averageChannelsExt[0], 1))+"v ", 4);
	Lcd->show("VF:"+FloatToString(Voltages->get(averageChannelsExt[1], 1))+"v ", 5);
	Lcd->show("VG:"+FloatToString(Voltages->get(averageChannelsExt[2], 1))+"v ", 6);
	Lcd->show("VH:"+FloatToString(Voltages->get(averageChannelsExt[3], 1))+"v ", 7);
	Lcd->show("VI:"+FloatToString(Voltages->get(averageChannelsExt[4], 1))+"v ", 8);
	Lcd->show("VJ:"+FloatToString(Voltages->get(averageChannelsExt[5], 1))+"v ", 9);
	Lcd->show("VK:"+FloatToString(Voltages->get(averageChannelsExt[6], 1))+"v ", 10);
	Lcd->show("VL:"+FloatToString(Voltages->get(averageChannelsExt[7], 1))+"v ", 11);
}


/**
 * Convert a float value to a string 
 */
String FloatToString(float value)
{
	static char string[15];
	dtostrf(value, 5, 3, string);

	return String(string);
}

/*
Lcd->show("HV ON ", 12);
Lcd->show("+MODE ", 13);
Lcd->show("|| ERROR PLUGS POLARITY", 14);
*/