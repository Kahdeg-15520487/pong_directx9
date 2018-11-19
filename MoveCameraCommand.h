#pragma once
#include "Command.h"

#include "Camera.h"

class MoveCameraCommand :
	public Command
{
private:
	Camera* camera;
	Direction direction;
public:
	MoveCameraCommand(Camera* cam, Direction dir) {
		camera = cam;
		direction = dir;
	}
	~MoveCameraCommand() {}

	virtual void execute() {
		float x = 0, y = 0;
		switch (direction)
		{
		case leftup:
			x = -5;
			y = -5;
			break;
		case up:
			y = -5;
			break;
		case rightup:
			x = 5;
			y = -5;
			break;
		case left:
			x = -5;
			break;
		case none:
			x = 0;
			y = 0;
			break;
		case right:
			x = 5;
			break;
		case leftdown:
			x = -5;
			y = 5;
			break;
		case down:
			y = 5;
			break;
		case rightdown:
			x = 5;
			y = 5;
			break;
		}

		camera->SetPosition(camera->GetPosition() + D3DXVECTOR3(x, y, 0));
	}
};
