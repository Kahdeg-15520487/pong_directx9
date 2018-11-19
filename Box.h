#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

#include "Vector2.h"
#include "cute_c2.h"

class Box
{
private:
	int GetTagIndex(int tag) {
		return std::distance(Tags.begin(), std::find(Tags.begin(), Tags.end(), tag));
	}
public:
	Box(float x, float y, float width, float height, bool isStatic, const std::string& name)
		: Position(x, y), Size(width, height), IsStatic(isStatic), Name(name)
	{
		IsDirty = false;
	}
	~Box() {}

	Vector2 Position;
	Vector2 Size;

	Vector2 Center() { return Position + HalfSize(); }
	Vector2 HalfSize() { return Size / 2; }

	float X() { return Position.X; }
	float Y() { return Position.Y; }
	float Width() { return Size.X; }
	float Height() { return Size.Y; }

	bool IsDirty;
	bool IsStatic;

	std::string Name;

	std::vector<int> Tags;
	void AddTag(int tag) {
		Tags.push_back(tag);
	}
	void RemoveTag(int tag) {

	}

	Vector2 Velocity;
	void Move(const Vector2& velocity) {
		Velocity = velocity;
	}

	Vector2 ConstVelocity;

	void ApplyMovement()
	{
		Position = Position + Velocity;
		Velocity = Vector2();
	}

	void(*OnHit)(Box* box, Box* other, c2Manifold& m) = nullptr;
};
