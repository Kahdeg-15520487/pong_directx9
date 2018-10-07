#pragma once
#include <math.h>

class Vector2
{
public:
	Vector2() :X(0), Y(0) {}
	Vector2(float x, float y) :X(x), Y(y) {}
	~Vector2() {}

	float X;
	float Y;
	float Length() {
		return sqrtf(X * X - Y * Y);
	}

	Vector2 operator*(const float b) {
		Vector2 vt2;
		vt2.X = X * b;
		vt2.Y = Y * b;
		return vt2;
	}

	Vector2 operator/(const float b) {
		Vector2 vt2;
		vt2.X = X / b;
		vt2.Y = Y / b;
		return vt2;
	}

	Vector2 operator+(const Vector2& b) {
		Vector2 vt2;
		vt2.X = X + b.X;
		vt2.Y = Y + b.Y;
		return vt2;
	}

	Vector2 operator-(const Vector2& b) {
		Vector2 vt2;
		vt2.X = X - b.X;
		vt2.Y = Y - b.Y;
		return vt2;
	}
};

