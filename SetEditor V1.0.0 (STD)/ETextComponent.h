#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <cassert>

class ETextComponent
{
public:
	std::string erase(int pos, int count);

	int insert(int pos, std::string text);

	void toCorrectText(std::string& text);

	static bool isNotCorrectChar(const char c) noexcept;
	static bool isCorrectChar(const char c) noexcept;
public:
	std::string m_buffer;
};