#pragma once
#include "Command.h"
#include "Game.h"
#include "MoveCommand.h"
#include "ShootCommand.h"
#include "RawInput.h"
#include "DashCommand.h"

#include "MoveCommand2.h"
#include "MoveCameraCommand.h"

class CommandManager
{
private:
	static int GetMovementCommand(RawInput& ri, std::vector<Command*>& cmds) {

		if (ri.IsKeyDown(VK_SHIFT)) {
			return 0;
		}

		Direction dir = none;
		if (ri.IsKeyDown(VK_LEFT)) {
			dir = left;
		}

		if (ri.IsKeyDown(VK_RIGHT))
		{
			if (dir == left) {
				dir = none;
			}
			else {
				dir = right;
			}
		}

		if (ri.IsKeyDown(VK_UP))
		{
			switch (dir)
			{
			case left:
				dir = leftup;
				break;
			case right:
				dir = rightup;
				break;
			default:
				dir = up;
				break;
			}
		}

		if (ri.IsKeyDown(VK_DOWN))
		{
			switch (dir)
			{
			case left:
				dir = leftdown;
				break;
			case right:
				dir = rightdown;
				break;
			case none:
				dir = down;
				break;
			default:
				dir = none;
				break;
			}
		}
		//if (dir == none)
		//{
		//	return 0;
		//}

		//restrict up down movement
		if (dir != left && dir != right)
		{
			Command* cmd = new MoveCommand(none);
			cmds.push_back(cmd);
			return 1;
		}

		Command* cmd = new MoveCommand(dir);
		cmds.push_back(cmd);
		return 1;
	}
	static int GetDashCommand(RawInput& ri, std::vector<Command*>& cmds) {

		if (ri.IsKeyDown(VK_SHIFT)) {

			Direction dir = none;
			if (ri.IsKeyDown(VK_LEFT)) {
				dir = left;
			}

			if (ri.IsKeyDown(VK_RIGHT))
			{
				if (dir == left) {
					dir = none;
				}
				else {
					dir = right;
				}
			}

			if (ri.IsKeyDown(VK_UP))
			{
				switch (dir)
				{
				case left:
					dir = leftup;
					break;
				case right:
					dir = rightup;
					break;
				default:
					dir = up;
					break;
				}
			}

			if (ri.IsKeyDown(VK_DOWN))
			{
				switch (dir)
				{
				case left:
					dir = leftdown;
					break;
				case right:
					dir = rightdown;
					break;
				case none:
					dir = down;
					break;
				default:
					dir = none;
					break;
				}
			}
			//if (dir == none)
			//{
			//	return 0;
			//}

			//restrict up down movement
			if (dir != left && dir != right)
			{
				Command* cmd = new DashCommand(none);
				cmds.push_back(cmd);
				return 1;
			}

			Command* cmd = new DashCommand(dir);
			cmds.push_back(cmd);
			return 1;
		}
		return 0;
	}
	static int GetShootCommand(RawInput& ri, std::vector<Command*>& cmds) {

	}
	static int GetMovementCommand2(RawInput& ri, std::vector<Command*>& cmds) {
		Command* cmd = nullptr;
		if (ri.myr > 0)
		{
			//down
			cmd = new MoveCommand2(down);
		}
		if (ri.myr < 0)
		{
			//up
			cmd = new MoveCommand2(up);
		}

		if (cmd)
		{
			cmds.push_back(cmd);
			return 1;
		}
		return 0;
	}
	static int GetCameraMovementCommand(RawInput& ri, std::vector<Command*>& cmds, Camera* camera) {

		Direction dir = none;
		if (ri.IsKeyDown(VK_J)) {
			dir = left;
		}

		if (ri.IsKeyDown(VK_L))
		{
			if (dir == left) {
				dir = none;
			}
			else {
				dir = right;
			}
		}

		if (ri.IsKeyDown(VK_I))
		{
			switch (dir)
			{
			case left:
				dir = leftup;
				break;
			case right:
				dir = rightup;
				break;
			default:
				dir = up;
				break;
			}
		}

		if (ri.IsKeyDown(VK_K))
		{
			switch (dir)
			{
			case left:
				dir = leftdown;
				break;
			case right:
				dir = rightdown;
				break;
			case none:
				dir = down;
				break;
			default:
				dir = none;
				break;
			}
		}
		if (dir == none)
		{
			return 0;
		}

		Command* cmd = new MoveCameraCommand(camera, dir);
		cmds.push_back(cmd);
		return 1;
	}
public:
	static int GetCommand(RawInput& ri, std::vector<Command*>& cmds, Camera* camera) {
		int cmdCount = 0;
		cmdCount += GetMovementCommand(ri, cmds);
		//cmdCount += GetMovementCommand2(ri, cmds);
		cmdCount += GetDashCommand(ri, cmds);
		cmdCount += GetCameraMovementCommand(ri, cmds, camera);
		return cmdCount;
	}
};