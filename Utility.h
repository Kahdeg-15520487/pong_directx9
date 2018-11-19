#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include "Vector2.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 680

#define RECT_WIDTH 50
#define RECT_HEIGHT 30
#define HOR_SPEED 2
#define VER_SPEED 5
#define VELOCITY_TRANSFER 0.5
#define RECT_DENSE 2

#ifndef SAFE_DELETE 
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p) = nullptr; } } 
#endif 

#define CLAMP(x, upper, lower) (min(upper, max(x, lower)))

enum Direction {
	leftup, up, rightup,
	left, none, right,
	leftdown, down, rightdown
};

enum MoveState {
	idle,
	startRun,
	run,
	startDash,
	dash,
	startJump,
	jump,
	endJump,
	fall
};

Vector2 GetMovementVector(Direction dir);
std::string ReadFileStream(std::ifstream& in);
std::string ExePath();
std::wstring ExePathUnicode();

const std::string empty = std::string();

void DebugPrint(const float& f, const std::string &p = empty);
void DebugPrint(const Vector2& vt, const std::string &p = empty);
void DebugPrint(const std::string& s);
void DebugPrint(const bool& b, const std::string &p = empty);

Direction VectorToDirection(float x, float y);

#define VK_0 0x30 //0 key
#define VK_1 0x31 //1 key
#define VK_2 0x32 //2 key
#define VK_3 0x33 //3 key
#define VK_4 0x34 //4 key
#define VK_5 0x35 //5 key
#define VK_6 0x36 //6 key
#define VK_7 0x37 //7 key
#define VK_8 0x38 //8 key
#define VK_9 0x39 //9 key
#define VK_A 0x41 //A key
#define VK_B 0x42 //B key
#define VK_C 0x43 //C key
#define VK_D 0x44 //D key
#define VK_E 0x45 //E key
#define VK_F 0x46 //F key
#define VK_G 0x47 //G key
#define VK_H 0x48 //H key
#define VK_I 0x49 //I key
#define VK_J 0x4A //J key
#define VK_K 0x4B //K key
#define VK_L 0x4C //L key
#define VK_M 0x4D //M key
#define VK_N 0x4E //N key
#define VK_O 0x4F //O key
#define VK_P 0x50 //P key
#define VK_Q 0x51 //Q key
#define VK_R 0x52 //R key
#define VK_S 0x53 //S key
#define VK_T 0x54 //T key
#define VK_U 0x55 //U key
#define VK_V 0x56 //V key
#define VK_W 0x57 //W key
#define VK_X 0x58 //X key
#define VK_Y 0x59 //Y key
#define VK_Z 0x5A //Z key

#define SSTR( x ) ( std::ostringstream() << std::dec << x ).str()