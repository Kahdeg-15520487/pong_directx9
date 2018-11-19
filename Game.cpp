#include "Utility.h"
#include "Game.h"
#include "World.h"
#include "Object.h"

void DefaultCollision(Box* box, Box* other, c2Manifold& m) {
	c2v v = c2V(box->Velocity.X, box->Velocity.Y);
	c2v n = m.n;
	c2v walln = c2V(-n.x, -n.y);
	// "slide along wall"
	// remove all velocity in the direction of the wall's normal
	// v -= n * dot(v, n)
	// check coll direction

	v = c2Sub(v, c2Mulvs(walln, c2Dot(v, walln)));

	box->Velocity.X = v.x;
	box->Velocity.Y = v.y;
}

void BounceCollision(Box* box, Box* other, c2Manifold& m) {
	Vector2 comp = Vector2(other->Velocity.X, other->Velocity.Y);
	comp = comp * VELOCITY_TRANSFER;
	comp = comp + box->Velocity;
	c2v v = c2V(comp.X, comp.Y);
	c2v n = m.n;

	if (n.x > 0)
	{
		//DebugPrint("left");
	}
	else {

		//DebugPrint("right");
	}

	v = c2Sub(v, c2Mulvs(v, 2));

	box->Velocity.X = v.x;
	box->Velocity.Y = v.y;
	box->ConstVelocity.X = v.x;
	box->ConstVelocity.Y = v.y;
}

void BallUpdate(Object* self) {

}

void PlayerUpdate(Object* self) {

}

Game::Game() {}

void Game::Init(LPDIRECT3DDEVICE9 d3ddv) {
	objs = {};
	cmds = {};
	world = new World();

	WhoWin = none;

	player = AddDynamicObject(3, (SCREEN_HEIGHT / 2) - (75 / 2), 36, 36, "player1");
	//player->Update = PlayerUpdate;
	//p1Score = 0;
	//player->Coll->OnHit = DefaultCollision;

	//player2 = AddStaticObject(SCREEN_WIDTH - 26, (SCREEN_HEIGHT / 2) - (75 / 2), 25, 75, "player2");
	//player2->Update = PlayerUpdate;
	//p2Score = 0;
	//player2->Coll->OnHit = DefaultCollision;

	auto spriteSheetDef = (ExePath() + "\\idle.json");
	auto imagePath = (ExePathUnicode() + L"\\idle.png");
	player->LoadAniSprite(spriteSheetDef, imagePath, idle, d3ddv);
	spriteSheetDef = (ExePath() + "\\start_run.json");
	imagePath = (ExePathUnicode() + L"\\start_running.png");
	player->LoadAniSprite(spriteSheetDef, imagePath, startRun, d3ddv);
	spriteSheetDef = (ExePath() + "\\run.json");
	imagePath = (ExePathUnicode() + L"\\running_cycle.png");
	player->LoadAniSprite(spriteSheetDef, imagePath, run, d3ddv);
	spriteSheetDef = (ExePath() + "\\dash.json");
	imagePath = (ExePathUnicode() + L"\\dashing.png");
	player->LoadAniSprite(spriteSheetDef, imagePath, dash, d3ddv);
	spriteSheetDef = (ExePath() + "\\start_dash.json");
	imagePath = (ExePathUnicode() + L"\\dashing.png");
	player->LoadAniSprite(spriteSheetDef, imagePath, startDash, d3ddv);

	AddStaticObject(5, SCREEN_HEIGHT / 2, SCREEN_WIDTH - 10, SCREEN_HEIGHT / 2 - 10, "obstacle");
	//ball = AddDynamicObject((SCREEN_WIDTH / 2) - 10, (SCREEN_HEIGHT / 2) - 10, 20, 20, "ball");
	//ball->Coll->OnHit = BounceCollision;
	//ball->Coll->ConstVelocity = Vector2(10, 0);
	//ball->Update = BallUpdate;

	D3DXCreateFont(d3ddv, 17, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &DefaultFont);

	camera = new Camera(SCREEN_WIDTH, SCREEN_HEIGHT);
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

	//if (ball->Coll->X() > SCREEN_WIDTH)
	//{
	//	WhoWin = right;
	//}
	//else if (ball->Coll->X() < -10) {
	//	WhoWin = left;
	//}

	cmds.clear();
	CommandManager::GetCommand(*ri, cmds, camera);

	for (std::vector<Command*>::iterator it = cmds.begin(); it != cmds.end(); it++) {
		//cmds[it]->execute(obj);
		Command* cmd = nullptr;

		cmd = dynamic_cast<MoveCommand*>(*it);
		if (cmd) {
			cmd->execute(*player);
			continue;
		}

		//cmd = dynamic_cast<MoveCommand2*>(*it);
		//if (cmd) {
		//	cmd->execute(*player2);
		//	continue;
		//}

		auto camera_cmd = dynamic_cast<MoveCameraCommand*>(*it);
		if (camera_cmd)
		{
			camera_cmd->execute();
		}

		cmd = dynamic_cast<DashCommand*>(*it);
		if (cmd)
		{
			cmd->execute(*player);
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
		for (auto it = colls.begin(); it != colls.end(); it++) {
			char msgbuf[100];
			sprintf_s(msgbuf, "%d\n", (*it)->CollideDirection);
			OutputDebugStringA(msgbuf);
		}
	}

	//if (ri->IsKeyDown(VK_SPACE))
	//{
	//	//char msgbuf[100];
	//	//sprintf_s(msgbuf, "upKeyDown= %d,upKeyUp= %d,downKeyDown = %d,downKeyUp = %d,x= %f,y= %f\n", ri->IsKeyDown(VK_UP), ri->IsKeyUp(VK_UP), ri->IsKeyDown(VK_DOWN), ri->IsKeyUp(VK_DOWN), player->Box->X(), player->Box->Y());
	//	//OutputDebugStringA(msgbuf);

	//	//sprintf_s(msgbuf, "colCount= %d\n", colCount);
	//	//OutputDebugStringA(msgbuf);
	//}
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
	D3DXVECTOR2 trans =
		//D3DXVECTOR2(SCREEN_WIDTH / 2 - camera->GetPosition().x,
		//			SCREEN_HEIGHT / 2 - camera->GetPosition().y);
		D3DXVECTOR2(camera->GetPosition().x,
			camera->GetPosition().y);

	auto it = objs.begin();
	while (it != objs.end()) {
		(*it)->Render(deltaTime, d3d, d3ddv, back_buffer, surface, trans);

		//auto pos = player->Coll->Position;
		//auto pos2 = (*it)->Coll->Position;
		//LPD3DXLINE lineSprite;
		//D3DXVECTOR2 points[2];
		//points[0] = D3DXVECTOR2(pos.X, pos.Y);
		//points[1] = D3DXVECTOR2(pos2.X, pos2.Y);
		//D3DXCreateLine(d3ddv, &lineSprite);
		//lineSprite->Begin();
		//lineSprite->Draw(points, 2, 0xffff8080);
		//lineSprite->End();
		//lineSprite->Release();

		it++;
	}


	RECT tr;
	tr.left = SCREEN_WIDTH / 2 - 90;
	tr.top = 5;


	char msgbuf[100];
	sprintf_s(msgbuf, "camera %fx%f", camera->GetPosition().x, camera->GetPosition().y);
	//sprintf_s(msgbuf, "x= %f,y= %f", trans.x, trans.y);

	tr.right = tr.left + 180;
	tr.bottom = tr.top + 40;
	DefaultFont->DrawTextA(NULL, msgbuf, 30, &tr, DT_LEFT, D3DCOLOR_XRGB(255, 0, 0));

	sprintf_s(msgbuf, "player %fx%f", player->Coll->X(), player->Coll->Y());
	tr.top = tr.bottom;
	tr.bottom = tr.top + 40;
	DefaultFont->DrawTextA(NULL, msgbuf, 30, &tr, DT_LEFT, D3DCOLOR_XRGB(255, 0, 0));

	//tr.left = SCREEN_WIDTH / 2 - 40;
	//tr.right = tr.left + 20;
	//DefaultFont->DrawTextA(NULL, "" + p1Score, 2, &tr, DT_RIGHT, D3DCOLOR_XRGB(255, 0, 0));

	//tr.left = SCREEN_WIDTH / 2 + 20;
	//tr.right = tr.left + 20;
	//DefaultFont->DrawTextA(NULL, "" + p2Score, 2, &tr, DT_LEFT, D3DCOLOR_XRGB(255, 0, 0));
}