#include <string>
#include <map>
#include <Arduino.h>

#pragma once

class ErrorController
{
public:
	ErrorController();
	~ErrorController(void);
	bool check();
private:
	bool output(int id, int direction);
	bool runMethod(String method);
	bool lcdMessage(String message, int direction);
	bool ensure(int id);
	bool condtionFailed(int id);
	bool condtionSuccess(int id);
protected:
	std::vector< std::map< String, std::map<int, String> > > error_container;
	std::vector<int> triggeredErrors;
};

