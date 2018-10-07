#pragma once
#include <d3d9.h>
#include <vector>

#include "RawInput.h"
#include "World.h"
#include "Object.h"
#include "Command.h"
#include "CommandManager.h"

enum MoveType {
	manual,
	around
};

class Game
{
public:
	Game(LPDIRECT3DDEVICE9);
	~Game();

	Object* player2;
	Object* ball;

	Object* player;
	std::vector<Object*> objs;
	World* world;

	std::vector<Command*> cmds;

	Object* AddObject(float, float, float, float, bool, const std::string&);
	Object* AddDynamicObject(float, float, float, float, const std::string&);
	Object* AddStaticObject(float, float, float, float, const std::string&);

	void Update(DWORD, RawInput*);
	void Render(DWORD, LPDIRECT3D9, LPDIRECT3DDEVICE9, LPDIRECT3DSURFACE9, LPDIRECT3DSURFACE9);
};
