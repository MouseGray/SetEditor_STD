#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <cassert>

class ETextComponent
{
public:
	std::string erase(int count);

	int insert(std::string text);

	void setCursorPos(size_t pos) noexcept;

	void toCorrectText(std::string& text);

	static bool isNotCorrectChar(const char c) noexcept;
	static bool isCorrectChar(const char c) noexcept;
public:
	int m_cursor = 0;
	std::string m_buffer;
};