#pragma once
#include "Box.h"
#include "Vector2.h"
#include "AnimatedSprite.h"
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
		AniSprite = new AnimatedSprite();
		Dir = right;
		IsBulletFree = true;
		Bullet = nullptr;

		Update = nullptr;
	}
	~Object() {
		SAFE_DELETE(Coll);
		SAFE_DELETE(AniSprite);
	}

	Game* GameR;
	Box* Coll;
	AnimatedSprite* AniSprite;
	Direction Dir;
	bool IsBulletFree;
	Object* Bullet;

	void(*Update)(Object* self);

	void Render(DWORD deltaTime, LPDIRECT3D9 d3d, LPDIRECT3DDEVICE9 d3ddv, LPDIRECT3DSURFACE9 back_buffer, LPDIRECT3DSURFACE9 surface) {

		if (AniSprite->IsLoaded())
		{
			AniSprite->Render(deltaTime, Coll->X(), Coll->Y(), 1, 50, Dir == right ? 1 : -1);
			//return;
		}

		RECT rectTop, rectRight, rectBottom, rectLeft;

		rectTop.left = Coll->X();
		rectTop.top = Coll->Y();
		rectTop.right = rectTop.left + Coll->Width();
		rectTop.bottom = rectTop.top + 5;

		rectRight.left = Coll->X() + Coll->Width() - 5;
		rectRight.top = Coll->Y();
		rectRight.right = rectRight.left + 5;
		rectRight.bottom = rectRight.top + Coll->Height();

		rectBottom.left = Coll->X();
		rectBottom.top = Coll->Y() + Coll->Height() - 5;
		rectBottom.right = rectBottom.left + Coll->Width();
		rectBottom.bottom = rectBottom.top + 5;

		rectLeft.left = Coll->X();
		rectLeft.top = Coll->Y();
		rectLeft.right = rectLeft.left + 5;
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

