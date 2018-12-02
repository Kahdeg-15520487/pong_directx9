#pragma once
#include "Utility.h"
#include "Command.h"
#include "Object.h"
class MoveCommand :
	public Command
{
private:
	Direction dir;
public:
	MoveCommand(Direction dir) {
		this->dir = dir;
	}
	~MoveCommand() {}

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
		Vector2 vt = GetMovementVector(dir);
		obj.Coll->Move(vt);

		switch (obj.moveState)
		{
		case idle:
			obj.moveState = startRun;
			break;
		case dash:
		case startRun:
			obj.moveState = run;
			break;
		}
	}
};
