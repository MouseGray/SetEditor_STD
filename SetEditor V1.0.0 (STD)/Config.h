#pragma once
#define byte win_byte
#include <Windows.h>
#undef byte
#include <string>

using namespace std;

class Config
{
public:
	bool Init = false;

	string FileName;

	HFONT Font = nullptr;
};