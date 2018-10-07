#include "Utility.h"
#include "Game.h"
#include "World.h"
#include "Object.h"

void DefaultCollision(Box* box, Box* other, c2Manifold& m) {
	c2v v = c2V(box->Velocity.X, box->Velocity.Y);
	c2v n = m.n;

	// "slide along wall"
	// remove all velocity in the direction of the wall's normal
	// v -= n * dot(v, n)
	v = c2Sub(v, c2Mulvs(n, c2Dot(v, n)));

	box->Velocity.X = v.x;
	box->Velocity.Y = v.y;
}

void BallUpdate(Object* self) {

}

Game::Game(LPDIRECT3DDEVICE9 d3ddv)
{
	world = new World();

	player = AddDynamicObject(0, 0, 25, 75, "player1");
	player->Coll->OnHit = DefaultCollision;

	player2 = AddDynamicObject(SCREEN_WIDTH - 26, 0, 25, 75, "player2");
	player2->Coll->OnHit = DefaultCollision;

	//auto spriteSheetDef = (ExePath() + "\\sprite.json");
	//auto imagePath = (ExePathUnicode() + L"\\x_sprite.png");
	//AnimatedSprite::Load(*player->AniSprite, spriteSheetDef, imagePath, D3DCOLOR_XRGB(255, 0, 255), d3ddv);

	//AddStaticObject(100, 100, 100, 100, "obstacle");
	ball = AddDynamicObject(90, 90, 20, 20, "ball");
	ball->Update = BallUpdate;
}

Game::~Game()
{
	delete world;
}

Object* Game::AddObject(float x, float y, float width, float height, bool isStatic, const std::string& name) {
	Box* box = (world->Create(x, y, width, height, isStatic, name));
	Object* obj = new Object(this, box);
	objs.push_back(obj);
	return obj;
}

Object* Game::AddStaticObject(float x, float y, float width, float height, const std::string& name) {
	return AddObject(x, y, width, height, true, name);
}

Object* Game::AddDynamicObject(float x, float y, float width, float height, const std::string& name) {
	return AddObject(x, y, width, height, false, name);
}

Direction MoveAround(Object* obj, Direction currentDirection) {
	Direction lastDirection = currentDirection;
	Vector2 velocity;
	switch (currentDirection)
	{
	case right:
		velocity.X = HOR_SPEED;
		velocity.Y = 0;
		if ((obj->Coll->X() + obj->Coll->Size.X + velocity.X) > SCREEN_WIDTH)
		{
			currentDirection = down;
		}
		break;
	case down:
		velocity.X = 0;
		velocity.Y = VER_SPEED;
		if ((obj->Coll->Y() + obj->Coll->Size.Y + velocity.Y) > SCREEN_HEIGHT)
		{
			currentDirection = left;
		}
		break;
	case left:
		velocity.X = -HOR_SPEED;
		velocity.Y = 0;
		if (obj->Coll->X() + velocity.X < 0)
		{
			currentDirection = up;
		}
		break;
	case up:
		velocity.X = 0;
		velocity.Y = -VER_SPEED;
		if (obj->Coll->Y() + velocity.Y < 0)
		{
			currentDirection = right;
		}
		break;
	}
	if (currentDirection == lastDirection)
	{
		obj->Coll->Move(velocity);
	}
	//char msgbuf[100];
	//sprintf_s(msgbuf, "state= %d,x= %f,y= %f,vel.x= %f,vel.y= %f\n", currentDirection, obj->Box->X(), obj->Box->Y(), velocity.X, velocity.Y);
	//OutputDebugStringA(msgbuf);
	return currentDirection;
}

void Game::Update(DWORD deltaTime, RawInput* ri) {

	cmds.clear();
	CommandManager::GetCommand(*ri, cmds);

	for (std::vector<Command*>::iterator it = cmds.begin(); it != cmds.end(); it++) {
		//cmds[it]->execute(obj);
		Command* cmd = nullptr;

		cmd = dynamic_cast<MoveCommand*>(*it);
		if (cmd) {
			cmd->execute(*player);
			continue;
		}

		cmd = dynamic_cast<MoveCommand2*>(*it);
		if (cmd) {
			cmd->execute(*player2);
			continue;
		}
	}

	ri->Refresh();

	for (auto it = objs.begin(); it != objs.end(); it++) {
		if ((*it)->Update) {
			(*it)->Update(*it);
		}
	}

	std::vector<Collision*> colls = {};
	int colCount = world->StepSimulation(colls);
	if (colCount != 0)
	{
		//there is some collission !!
	}

	if (ri->IsKeyDown(VK_SPACE))
	{
		//char msgbuf[100];
		//sprintf_s(msgbuf, "upKeyDown= %d,upKeyUp= %d,downKeyDown = %d,downKeyUp = %d,x= %f,y= %f\n", ri->IsKeyDown(VK_UP), ri->IsKeyUp(VK_UP), ri->IsKeyDown(VK_DOWN), ri->IsKeyUp(VK_DOWN), player->Box->X(), player->Box->Y());
		//OutputDebugStringA(msgbuf);

		//sprintf_s(msgbuf, "colCount= %d\n", colCount);
		//OutputDebugStringA(msgbuf);
	}
}

void Game::Render(DWORD deltaTime, LPDIRECT3D9 d3d, LPDIRECT3DDEVICE9 d3ddv, LPDIRECT3DSURFACE9 back_buffer, LPDIRECT3DSURFACE9 surface)
{
	// Clear back buffer with BLACK
	d3ddv->ColorFill(back_buffer, NULL, D3DCOLOR_XRGB(0, 0, 0));

	// Random color for the surface
	int r = 0;
	int g = 0;
	int b = 255;

	// Fill the bitmap
	d3ddv->ColorFill(surface, NULL, D3DCOLOR_XRGB(r, g, b));

	RECT rect;

	auto it = objs.begin();
	while (it != objs.end()) {
		(*it)->Render(deltaTime, d3d, d3ddv, back_buffer, surface);

		auto pos = player->Coll->Position;
		auto pos2 = (*it)->Coll->Position;
		LPD3DXLINE lineSprite;
		D3DXVECTOR2 points[2];
		points[0] = D3DXVECTOR2(pos.X, pos.Y);
		points[1] = D3DXVECTOR2(pos2.X, pos2.Y);
		D3DXCreateLine(d3ddv, &lineSprite);
		lineSprite->Begin();
		lineSprite->Draw(points, 2, 0xffff8080);
		lineSprite->End();
		lineSprite->Release();

		it++;
	}
}