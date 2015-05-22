#include <string>
#include <map>
#include <Arduino.h>

#pragma once

class ErrorController
{
public:
	ErrorController();
	~ErrorController(void);
	bool check(int id);
private:
	bool output(std::map<String, String> outputs);
	bool runMethod(String method);
	bool lcdMessage(String message);
	bool ensure(int id);
protected:
	std::map< int, std::map< String, std::map<String, String> > > error_container;
};

