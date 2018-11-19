#pragma once
#include "Box.h"
#include "Vector2.h"
#include "AnimatedSprite.h"
#include "StateManager.h"
#include "Game.h"

class Game;

class Object
{
private:
	void GetRect(RECT* rect) {

		rect->left = Coll->X();
		rect->top = Coll->Y();
		rect->right = rect->left + Coll->Width();
		rect->bottom = rect->top + Coll->Height();
	}
public:
	Object(Game* game, Box* box) {
		GameR = game;
		Coll = box;
		moveState = idle;
		isAnimationLoaded = false;
		Dir = right;
		IsBulletFree = true;
		Bullet = nullptr;

		Update = nullptr;
		stateManager = new StateManager();
	}
	~Object() {
		SAFE_DELETE(Coll);
		SAFE_DELETE(stateManager);
		//SAFE_DELETE(AniSprite);
	}

	Game* GameR;
	Box* Coll;
	std::map<int, AnimatedSprite*> AniSprites;
	bool isAnimationLoaded;
	Direction Dir;
	bool IsBulletFree;
	Object* Bullet;

	MoveState moveState;
	StateManager* stateManager;

	void LoadAniSprite(std::string sprdef, std::wstring sprsht, int state, LPDIRECT3DDEVICE9 d3ddv) {
		AnimatedSprite* anispr = new AnimatedSprite();
		AnimatedSprite::Load(*anispr, sprdef, sprsht, D3DCOLOR_XRGB(255, 0, 255), d3ddv);
		anispr->currentFrame = 0;
		AniSprites.insert(std::pair<int, AnimatedSprite*>(state, anispr));
		isAnimationLoaded = true;
	}

	void(*Update)(Object* self);

	AnimatedSprite* GetAniSprite() {
		return AniSprites[moveState];
	}

	void Render(DWORD deltaTime, LPDIRECT3D9 d3d, LPDIRECT3DDEVICE9 d3ddv, LPDIRECT3DSURFACE9 back_buffer, LPDIRECT3DSURFACE9 surface, D3DXVECTOR2 &trans) {

		if (isAnimationLoaded)
		{
			auto AniSprite = GetAniSprite();
			AniSprite->Render(deltaTime, Coll->X(), Coll->Y(), trans, 1, 50, Dir == right ? 1 : -1);
			//return;
		}

		float x = Coll->X();
		float y = Coll->Y();
		RECT rectTop, rectRight, rectBottom, rectLeft;

		//translation for primitive
		if (trans != D3DXVECTOR2())
		{
			x += trans.x;
			y += trans.y;
		}

		rectTop.left = x;
		rectTop.top = y;
		rectTop.right = rectTop.left + Coll->Width();
		rectTop.bottom = rectTop.top + RECT_DENSE;

		rectRight.left = x + Coll->Width() - RECT_DENSE;
		rectRight.top = y;
		rectRight.right = rectRight.left + RECT_DENSE;
		rectRight.bottom = rectRight.top + Coll->Height();

		rectBottom.left = x;
		rectBottom.top = y + Coll->Height() - RECT_DENSE;
		rectBottom.right = rectBottom.left + Coll->Width();
		rectBottom.bottom = rectBottom.top + RECT_DENSE;

		rectLeft.left = x;
		rectLeft.top = y;
		rectLeft.right = rectLeft.left + RECT_DENSE;
		rectLeft.bottom = rectLeft.top + Coll->Height();

		// get rect
		//GetRect(&rect);
		// Draw the surface onto the back buffer
		d3ddv->StretchRect(
			surface,			// from 
			NULL,				// which portion?
			back_buffer,		// to 
			&rectTop,				// which portion?
			D3DTEXF_NONE);
		d3ddv->StretchRect(
			surface,			// from 
			NULL,				// which portion?
			back_buffer,		// to 
			&rectRight,				// which portion?
			D3DTEXF_NONE);
		d3ddv->StretchRect(
			surface,			// from 
			NULL,				// which portion?
			back_buffer,		// to 
			&rectBottom,				// which portion?
			D3DTEXF_NONE);
		d3ddv->StretchRect(
			surface,			// from 
			NULL,				// which portion?
			back_buffer,		// to 
			&rectLeft,				// which portion?
			D3DTEXF_NONE);
	}
};

