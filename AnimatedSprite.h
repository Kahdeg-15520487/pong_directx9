#pragma once

#include <d3dx9.h>
#include <list>
#include <iostream>
#include <fstream>
#include "json.hpp"
#include "Utility.h"
using json = nlohmann::json;

struct Sprite
{
private:
	int x;
	int y;
	int width;
	int height;
public:
	Sprite(int _x, int _y, int _w, int _h) :x(_x), y(_y), width(_w), height(_h) {}
	int X() { return x; }
	int Y() { return y; }
	int Width() { return width; }
	int Height() { return height; }
};

class AnimatedSprite
{
private:
	// sprite list
	std::list<Sprite> frameList;
	float frameRate;
	bool isLoaded;

	// texture store sprite sheet
	LPDIRECT3DTEXTURE9 _Texture;
	// handler to sprite
	LPD3DXSPRITE _SpriteHandler;
	// sotre infomation sprite
	D3DXIMAGE_INFO _Info;
public:
	int currentFrame;
	AnimatedSprite() { isLoaded = false; }
	~AnimatedSprite() {
		SAFE_DELETE(_Texture);

		frameList.clear();
	}

	bool IsLoaded() { return this->isLoaded; }

	void Render(float DeltaTime, float X, float Y, D3DXVECTOR2& trans, float ScaleSize, float AnimationRate, float FlipX)
	{
		auto l_front = frameList.begin();
		advance(l_front, currentFrame);

		D3DXMATRIX Combined;

		D3DXMATRIX Scale;
		D3DXMATRIX Translate;

		// Initialize the Combined matrix.
		D3DXMatrixIdentity(&Combined);

		// set location
		D3DXVECTOR3 position((float)X, (float)Y, 0);
		// Scale the sprite.
		D3DXMatrixScaling(&Scale, FlipX * ScaleSize, ScaleSize, ScaleSize);
		Combined *= Scale;
		// Translate the sprite
		D3DXMatrixTranslation(&Translate, (FlipX > 0 ? X : X + l_front->Width()*ScaleSize) + trans.x, Y + trans.y, 0.0f);
		Combined *= Translate;
		// Apply the transform.
		_SpriteHandler->SetTransform(&Combined);

		// set position tile
		RECT srect;
		srect.left = l_front->X();
		srect.top = l_front->Y();
		srect.right = srect.left + l_front->Width();
		srect.bottom = srect.top + l_front->Height();

		// get anchor point of sprite
		//D3DXVECTOR3 center = D3DXVECTOR3((srect.right - srect.left) / 2, (srect.bottom - srect.top) / 2, 0);
		D3DXVECTOR3 center = D3DXVECTOR3(0, 0, 0);

		_SpriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		_SpriteHandler->Draw(
			_Texture,
			&srect,
			&center,
			NULL,
			D3DCOLOR_XRGB(255, 255, 255)
		);

		_SpriteHandler->End();

		if (frameRate > AnimationRate)
		{
			frameRate = 0;
			currentFrame = ((currentFrame + 1) % frameList.size());
		}

		frameRate += DeltaTime;
	}

	static bool Load(AnimatedSprite& aniSprite, std::string& jsonPath, std::wstring& ImagePath, D3DCOLOR Transcolor, LPDIRECT3DDEVICE9 _d3ddv) {
		aniSprite.currentFrame = 0;
		aniSprite.frameRate = 0;

		std::ifstream ifs(jsonPath, std::ios::in);
		bool exist = ifs.good();
		std::string jsonData = ReadFileStream(ifs);
		json j = json::parse(jsonData);

		int spriteCount = j["spriteCount"];

		//char msgbuf[100];
		//sprintf_s(msgbuf, "spriteCount= %d\n", spriteCount);
		//OutputDebugStringA(msgbuf);

		aniSprite.frameList = {};

		auto arr = j["spriteRects"];
		auto it = arr.begin();

		while (it != arr.end())
		{
			int x = it.value()["x"];
			int y = it.value()["y"];
			int w = it.value()["w"];
			int h = it.value()["h"];

			Sprite sprite(x, y, w, h);
			aniSprite.frameList.push_back(sprite);

			it++;
		}

		HRESULT res = D3DXCreateSprite(_d3ddv, &aniSprite._SpriteHandler);

		if (res != D3D_OK)
		{
			aniSprite.isLoaded = false;
			return false;
		}

		HRESULT result = D3DXGetImageInfoFromFile(ImagePath.c_str(), &aniSprite._Info);

		if (result != D3D_OK)
		{
			aniSprite.isLoaded = false;
			return false;
		}

		// create texture from sprite sheet
		result = D3DXCreateTextureFromFileEx(
			_d3ddv,
			ImagePath.c_str(),
			aniSprite._Info.Width,
			aniSprite._Info.Height,
			1,
			D3DUSAGE_DYNAMIC,
			D3DFMT_UNKNOWN,
			D3DPOOL_DEFAULT,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			Transcolor,
			&aniSprite._Info,
			NULL,
			&aniSprite._Texture
		);

		if (result != D3D_OK)
		{
			aniSprite.isLoaded = false;
			return false;
		}

		aniSprite.isLoaded = true;
		aniSprite.currentFrame = 0;
		return true;
	}
};