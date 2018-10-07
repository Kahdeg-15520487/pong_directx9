#pragma once
#include "Utility.h"
#include "Command.h"
#include "Object.h"
class MoveCommand2 :
	public Command
{
private:
	Direction dir;
public:
	MoveCommand2(Direction dir) {
		this->dir = dir;
	}
	~MoveCommand2() {}

	virtual void execute(Object& obj) {
		if (dir == left || dir == right) {
			obj.Dir = dir;
		}
		Vector2 vt = GetMovementVector(dir);
		obj.Coll->Move(vt);
	}
};
