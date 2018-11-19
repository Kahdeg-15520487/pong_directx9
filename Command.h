#pragma once
#include "Utility.h"
#include "Object.h"
class Command
{
public:
	Command() {}
	~Command() {}
	virtual void execute(Object& obj) {}
	virtual void executeE() {}
};
