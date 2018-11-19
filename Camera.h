#pragma once

#include <d3dx9.h>
#include <d3d9.h>
#include "Utility.h"

class Camera
{
public:
	Camera(int width, int height);

	//center of camera
	void SetPosition(float x, float y);
	void SetPosition(D3DXVECTOR3 pos);

	int GetWidth();
	int GetHeight();

	D3DXVECTOR3 GetPosition();
	RECT GetBound();

	LPCSTR ToString();

	~Camera();

private:
	int             mWidth, mHeight;

	D3DXVECTOR3     mPosition;
};

