#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include "Vector2.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 680

#define RECT_WIDTH 50
#define RECT_HEIGHT 30
#define HOR_SPEED 5
#define VER_SPEED 5

#ifndef SAFE_DELETE 
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p) = nullptr; } } 
#endif 

#define CLAMP(x, upper, lower) (min(upper, max(x, lower)))

enum Direction {
	leftup, up, rightup,
	left, none, right,
	leftdown, down, rightdown
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
