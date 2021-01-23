#pragma once

#include <string>
#include <vector>

#include "ERedoUndoComponent.h"

class ETextComponent : public ERedoUndoComponent
{
public:
	void erase(size_t begin, size_t end, ERedoUndoComponent::Type redoUndoType);

	void insert(size_t offset, std::string text, ERedoUndoComponent::Type redoUndoType);

	void toCorrectText(std::string& text);
	static bool isNotCorrectChar(const char c) noexcept;
	static bool isCorrectChar(const char c) noexcept;
public:
	std::string m_buffer;
};