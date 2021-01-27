#pragma once

#include "ETextComponent.h"

class EOverlineComponent : public ETextComponent
{
public:
	static std::string toCorrectOverline(const std::string& text);

	int findOverlineEnd(int pos);

	template<ERedoUndoComponent::Type T>
	bool erase_(int pos, int count)
	{
		auto end = pos + count;
		auto start = pos;
		auto result = false;
		for (auto i = pos; i < end; i++) {
			if (m_buffer[i] == '{') 
			{
				auto _end = findOverlineEnd(i);
				eraseOverline<T>(i, _end);
				if (_end < end) end--;
				end--;
				i--;
				result = true;
				continue;
			}
			if (m_buffer[i] == '}')
			{
				auto count = i - start;
				if (count > 0) {
					result = true;
					erase<T>(start++, count);
					i -= count;
					end -= count;
				}
				continue;
			}
		}
		auto _count = end - start;
		if (_count > 0) {
			result = true;
			erase<T>(start, _count);
		}
		return result;
	}

	template<ERedoUndoComponent::Type T>
	void insertOverline(int startPos, int endPos)
	{
		auto urUnit = new ERedoUndoInsertOverline();
		urUnit->startPos = startPos;
		urUnit->endPos = endPos;
		m_buffer.insert(endPos, std::string("}"));
		m_buffer.insert(startPos, std::string("{"));
		add<T>(urUnit);
	}

	template<ERedoUndoComponent::Type T>
	bool eraseOverline(int startPos, int endPos)
	{
		auto urUnit = new ERedoUndoEraseOverline();
		urUnit->startPos = startPos;
		urUnit->endPos = endPos - 1;
		m_buffer.erase(endPos, 1);
		m_buffer.erase(startPos, 1);
		add<T>(urUnit);
		return true;
	}
};

