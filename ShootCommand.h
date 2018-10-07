#pragma once
#include "Object.h"
#include "Game.h"
#include "Command.h"
#include "Utility.h"

class ShootCommand :
	public Command
{
private:
	Direction dir;
public:
	ShootCommand(Direction dir) {
		this->dir = dir;
	}
	~ShootCommand() {}

	virtual void execute(Object& obj) {
		if (!obj.IsBulletFree)
		{
			return;
		}
		Vector2 vt = GetMovementVector(dir);

		float x = obj.Coll->X() + dir == right ? obj.Coll->Width() : -10;
		float y = obj.Coll->Y() + obj.Coll->Height() / 2 - 5;

		//bullshit error here v
		//obj.GameR->AddObject(x, y, 10, 5, false, "bullet");
	}
};
