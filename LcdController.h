
#include <map>
#include <Arduino.h>
#include "Voltage.h"
#include "LcdDriver.h"



#pragma once
// These should really be DI'ed
extern LcdDriver* Lcd;
extern VoltageMeasure* Voltages;

class LcdController
{
public:
	LcdController(void);
	~LcdController(void);
	int refresh(std::map< String, std::map<int, float> > setupVoltages, std::map< String, std::map<int, int> > setupVoltagesAccurcy, bool forcefull);
	int update(int channel, int type, int id, bool forcefull);
	int initVoltages(std::map< String, std::map<int, float> > setupVoltages, std::map< String, std::map<int, int> > setupVoltagesAccurcy);
	void welcome();
	bool messageBar();
	bool errorCondition(std::map<int, String> message, int direction, int forceRefresh);
protected:
	unsigned long previousMillis; 
	int refreshRate;
	int errorEnabled;
	std::map< String, std::map< int, float> > lastUpdated;
	std::map< String, std::map<int, float> > voltages;
	std::map< String, std::map<int, int> > voltagesAccurcy;
	std::map< String, std::map<int, float> > voltagesJitter;
	std::map< int, int > VoltageBlocks;
	String floatToStringss(float value, int accurcy);
	String numberToLetter(int x);
};

