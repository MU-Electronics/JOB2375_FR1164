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
	bool lcdError(int id, int totalErrors);
	bool lcdErrorClear();
	bool ensure(int id);
	bool conditionsMet(int id, bool lcd);
	bool condtionsNotMet(int id, bool lcd);
	int errorSize;
protected:
	std::vector< std::map< String, std::map<int, String> > > error_container;
	int lcd_error_on;
	int displayed_error;
	std::map<int, int> outputChanges;
};

