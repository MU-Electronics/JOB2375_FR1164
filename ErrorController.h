#include <string>
#include <map>
#include <Arduino.h>
#include "LcdDisplay.h"

#pragma once
extern LcdDisplay* Lcd;

class ErrorController
{
public:
	ErrorController();
	~ErrorController(void);
	bool check();
private:
	bool output(int id, int direction);
	bool runMethod(String method);
	bool lcdMessage(std::map<int, String> message, int direction);
	bool ensure(int id);
	bool condtionFailed(int id);
	bool condtionSuccess(int id);
protected:
	std::vector< std::map< String, std::map<int, String> > > error_container;
	std::vector<int> triggeredErrors;
};

