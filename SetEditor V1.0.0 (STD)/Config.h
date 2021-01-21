#pragma once
#include <Windows.h>
#include <string>

using namespace std;

class Config
{
public:
	bool Init = false;

	string FileName;

	HFONT Font = nullptr;
};