#include "World.h"

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <limits>
#include <iterator>

#include "Vector2.h"
#include "Box.h"
#include "Collision.h"
#include "cute_c2.h"
#include "Utility.h"

World::World() {
	OnCreate = nullptr;
}

World::~World() {

}

Box* World::Create(float x, float y, float width, float height, bool isStatic, const std::string& name) {
	Box* box = new Box(x, y, width, height, isStatic, name);
	Boxes.insert(std::make_pair(name, box));

	if (OnCreate)
	{
		OnCreate(box);
	}

	return box;
}
void World::Destroy(const std::string& name) {
	auto box = Boxes.find(name);
	if (box != Boxes.end())
	{
		//todo beware of leakage
		Boxes.erase(box);
	}
}

//bool Collide(Box& a, Box& b, Vector2& velocity)
//{
//	Vector2 a1 = a.Position + velocity;
//	Vector2 b1 = b.Position;
//	Vector2 a2 = a1 + a.Size;
//	Vector2 b2 = b.Position + b.Size;

//	Vector2 I1(max(a1.X, b1.X), max(a1.Y, b1.Y));
//	Vector2 I2(min(a2.X, b2.X), min(a2.Y, b2.Y));

//	bool result = (I1.X < I2.X) && (I1.Y < I2.Y);

//	//DebugPrint(I1, "TopLeft:");
//	//DebugPrint(I2, "BottomRight:");
//	//DebugPrint(result, "intersect? ");

//	if (result) {
//		Vector2 depth(0, 0);

//		if (I1.X < I2.X) {
//			depth.X = a2.X - b1.X;
//			depth.Y = 0;

//			depth.X = CLAMP(depth.X, HOR_SPEED, -HOR_SPEED);

//			DebugPrint(depth, "intersect depth: ");
//			DebugPrint(velocity, "before: ");
//			velocity = velocity - depth;
//			DebugPrint(velocity, "after: ");
//			goto end;
//		}


//		if (I1.Y < I2.Y) {
//			depth.Y = a2.Y - b1.Y;
//			depth.X = 0;

//			depth.Y = CLAMP(depth.Y, VER_SPEED, -VER_SPEED);

//			DebugPrint(depth, "intersect depth: ");
//			DebugPrint(velocity, "before: ");
//			velocity = velocity - depth;
//			DebugPrint(velocity, "after: ");
//		}

//	}
//	end:
//	return result;
//}

//Box* StepBox(Box* box) {
//	auto it = Boxes.begin();
//	while (it != Boxes.end())
//	{
//		if ((box->Name != it->second->Name))
//		{
//			Vector2 vel = box->Velocity;
//			if (Collide(*box, *it->second, box->Velocity))
//			{
//				//char msgbuf[100];
//				//sprintf_s(msgbuf, "before x= %f,y= %f\nafter x= %f,y= %f\n", vel.X, vel.Y, box->Velocity.X, box->Velocity.Y);
//				//OutputDebugStringA(msgbuf);

//				return it->second;
//			}
//		}

//		it++;
//	}
//	return nullptr;
//}

#pragma region broadphase
Box* World::GetSweptBroadphaseBox(Box& b)
{
	float x = b.Velocity.X > 0 ? b.X() : b.X() + b.Velocity.X;
	float y = b.Velocity.Y > 0 ? b.Y() : b.Y() + b.Velocity.Y;
	float w = b.Velocity.X > 0 ? b.Velocity.X + b.Width() : b.Width() - b.Velocity.X;
	float h = b.Velocity.Y > 0 ? b.Velocity.Y + b.Height() : b.Height() - b.Velocity.Y;
	return new Box(x, y, w, h, true, "");
}
bool World::AABBCheck(Box& b1, Box& b2)
{
	return !(b1.X() + b1.Width() < b2.X() || b1.X() > b2.X() + b2.Width() || b1.Y() + b1.Height() < b2.Y() || b1.Y() > b2.Y() + b2.Height());
}
#pragma endregion

void World::DefaultCollision(Box* box, Box* other, c2Manifold& m) {
	c2v v = c2V(box->Velocity.X, box->Velocity.Y);
	c2v n = m.n;

	// "slide along wall"
	// remove all velocity in the direction of the wall's normal
	// v -= n * dot(v, n)
	v = c2Sub(v, c2Mulvs(n, c2Dot(v, n)));

	box->Velocity.X = v.x;
	box->Velocity.Y = v.y;
}

Direction World::StepBox(Box* box) {
	Direction colDir = none;
	//todo assign const velocity to velocity
	box->Velocity = box->Velocity + box->ConstVelocity;
	// box is the moving box
	// block is the static box
	Box* broadphasebox = GetSweptBroadphaseBox(*box);
	auto it = Boxes.begin();
	while (it != Boxes.end())
	{
		if ((box->Name != it->second->Name))
		{
			Vector2 vel = box->Velocity;
			auto block = it->second;
			if (AABBCheck(*broadphasebox, *block))
			{
				//perform detail collision
				c2AABB b1;
				auto boxMin = box->Position;
				auto boxMax = boxMin + box->Size;
				b1.min = c2V(boxMin.X, boxMin.Y);
				b1.max = c2V(boxMax.X, boxMax.Y);

				c2AABB b2;
				auto blockMin = block->Position;
				auto blockMax = blockMin + block->Size;
				b2.min = c2V(blockMin.X, blockMin.Y);
				b2.max = c2V(blockMax.X, blockMax.Y);

				c2Manifold m;
				c2AABBtoAABBManifold(b1, b2, &m);
				if (m.count)
				{
					// move quote out of colliding configuration
					float depth = -m.depths[0];
					c2v n = m.n;
					box->Position.X += n.x * depth;
					box->Position.Y += n.y * depth;
					colDir = VectorToDirection(n.x, n.y);

					if (box->OnHit)
					{
						box->OnHit(box, block, m);
					}
					else {
						DefaultCollision(box, block, m);
					}

					// "deflect from wall"
					// flip the velocity away from the wall
					// v = 2 * (v . n) * n - v
					//c2v vXnXn = c2Mulvs(n, c2Dot(v, n));
					//v = c2Sub(c2V(2 * vXnXn.x, 2 * vXnXn.y), v);

				}
			}
		}

		it++;
	}

	delete broadphasebox;
	return colDir;
}

int World::StepSimulation(std::vector<Collision*>& colls) {
	int collCount = 0;
	auto it = Boxes.begin();
	while (it != Boxes.end()) {
		Box* box = it->second;

		//if (box->IsDirty)
		//{
		//	box->IsDirty = false;
		//	it++;
		//	continue;
		//}

		if (box->IsStatic)
		{
			it++;
			continue;
		}
		//DebugPrint(box->Name);
		//Box* collidedBox = StepBox(box);
		//if (collidedBox) {
		//	collCount++;
		//	collidedBox->IsDirty = true;
		//	Collision* col = new Collision(box, collidedBox, up);
		//	colls.push_back(col);
		//}

		Direction colDir = StepBox(box);
		if (colDir != none) {
			collCount++;
			Collision* coll = new Collision(box, nullptr, colDir);
			colls.push_back(coll);
		}

		box->ApplyMovement();

		it++;
	}
	return collCount;
}