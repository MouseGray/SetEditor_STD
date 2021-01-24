#pragma once

#include <string>
#include <vector>
#include <sstream>

#include "ERedoUndoComponent.h"

class ETextComponent : public ERedoUndoComponent
{
public:
	void erase(size_t end, ERedoUndoComponent::Type redoUndoType);

	void insert(std::string text, ERedoUndoComponent::Type redoUndoType);

	void setCursorPos(size_t pos) noexcept;

	void toCorrectText(std::string& text);

	static bool isNotCorrectChar(const char c) noexcept;
	static bool isCorrectChar(const char c) noexcept;
public:
	size_t m_cursor = 0;
	std::string m_buffer;
};