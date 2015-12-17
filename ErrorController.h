#include <string>
#include <map>
#include <Arduino.h>
#include "LcdController.h"

#pragma once
// This should really be DI'ed
extern LcdController* LcdHandle;

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
	int errorSize;
protected:
	std::vector< std::map< String, std::map<int, String> > > error_container;
	std::vector<int> triggeredErrors;
};

