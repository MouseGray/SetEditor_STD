#pragma once

#include <string>
#include <vector>

#include "ERedoUndoComponent.h"

class ETextComponent : public ERedoUndoComponent
{
public:
	template<ERedoUndoComponent::Type T>
	int erase(int pos, int count)
	{
		auto border_1 = packIntoBorders(pos);
		auto border_2 = packIntoBorders(pos + count);
		auto leftBorder = std::min(border_1, border_2);
		auto rightBorder = std::max(border_1, border_2);
		auto realCount = std::min(std::abs(count), rightBorder - leftBorder);
		if (realCount == 0) return 0;

		auto urUnit = new ERedoUndoErase();
		urUnit->pos = leftBorder;
		urUnit->text = m_buffer.substr(leftBorder, realCount);
		m_buffer.erase(pos, count);
		add<T>(urUnit);

		return realCount;
	}

	template<ERedoUndoComponent::Type T>
	int insert(int pos, std::string text)
	{
		auto realPos = packIntoBorders(pos);
		toCorrectText(text);
		if (text.size() == 0) return 0;

		auto urUnit = new ERedoUndoInsert();
		urUnit->pos = realPos;
		urUnit->count = text.size();
		m_buffer.insert(realPos, text);
		add<T>(urUnit);

		return text.size();
	}

	int packIntoBorders(int pos) noexcept;

	void toCorrectText(std::string& text);

	static bool isNotCorrectChar(const char c) noexcept;
	static bool isCorrectChar(const char c) noexcept;
public:
	std::string m_buffer;
};