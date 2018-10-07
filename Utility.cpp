#include "Utility.h"
#include <Windows.h>

Vector2 GetMovementVector(Direction dir) {
	Vector2 vt;
	switch (dir)
	{
	case leftup:
		vt.X = -HOR_SPEED;
		vt.Y = -VER_SPEED;
		break;
	case up:
		vt.Y = -VER_SPEED;
		break;
	case rightup:
		vt.X = HOR_SPEED;
		vt.Y = -VER_SPEED;
		break;
	case left:
		vt.X = -HOR_SPEED;
		break;
	case right:
		vt.X = HOR_SPEED;
		break;
	case leftdown:
		vt.X = -HOR_SPEED;
		vt.Y = VER_SPEED;
		break;
	case down:
		vt.Y = VER_SPEED;
		break;
	case rightdown:
		vt.X = HOR_SPEED;
		vt.Y = VER_SPEED;
		break;
	case none:
		return vt;
	}

	return vt;
}

std::string ReadFileStream(std::ifstream& in) {
	return static_cast<std::stringstream const&>(std::stringstream() << in.rdbuf()).str();
}

std::string ExePath() {
	wchar_t buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::wstring ws(buffer);
	std::string s(ws.begin(), ws.end());
	std::string::size_type pos = s.find_last_of("\\/");
	return s.substr(0, pos);
}

std::wstring ExePathUnicode() {
	std::string s = ExePath();
	std::wstring ws(s.begin(), s.end());
	return ws;
}

void DebugPrint(const Vector2& vt, const std::string &p) {
	char msgbuf[200];
	sprintf_s(msgbuf, "%sx= %f,y= %f\n", p.c_str(), vt.X, vt.Y);
	OutputDebugStringA(msgbuf);
}

void DebugPrint(const bool& b, const std::string &p) {
	char msgbuf[200];
	sprintf_s(msgbuf, "%s%s\n", p.c_str(), b ? "true" : "false");
	OutputDebugStringA(msgbuf);
}

void DebugPrint(const std::string& s) {
	char msgbuf[200];
	sprintf_s(msgbuf, "%s\n", s.c_str());
	OutputDebugStringA(msgbuf);
}

void DebugPrint(const float& f, const std::string &p) {
	char msgbuf[200];
	sprintf_s(msgbuf, "%s%f\n", p.c_str(), f);
	OutputDebugStringA(msgbuf);
}