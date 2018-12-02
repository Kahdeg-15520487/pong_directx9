#pragma once
#include "Utility.h"
#include "Command.h"
#include "Object.h"

class DashCommand :
	public Command
{
private:
	Direction dir;
public:
	DashCommand(Direction dir) {
		this->dir = dir;
	}
	~DashCommand() {}

	virtual void execute(Object& obj) {
		//todo move these to StateManager
		if (dir == none)
		{
			obj.moveState = idle;
			return;
		}

		if (dir == left || dir == right) {
			obj.Dir = dir;
		}
		Vector2 vt = GetMovementVector(dir) * 2;
		obj.Coll->Move(vt);

		switch (obj.moveState)
		{
		case idle:
			obj.moveState = startDash;
			break;
		case run:
		case startDash:
			obj.moveState = dash;
			break;
		}
	}
};