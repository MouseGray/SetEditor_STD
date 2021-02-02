#pragma once

#include <ranges>
#include <cassert>
#include <algorithm>

#include "ETextComponent.h"

class EOverlineComponent : public ETextComponent
{
public:
	static std::string toCorrectOverline(const std::string& text);

	int findOverlineEnd(int pos);

	template<typename T>
	T findOverlineEnd(T begin, T end)
	{
		auto overlineLvl = 0;
		for (; begin != end; begin++)
		{
			if (*begin == '{') overlineLvl++;
			if (*begin == '}') {
				overlineLvl--;
				if (overlineLvl == 0) break;
			}
		}
		return begin;
	}

	template<ERedoUndoComponent::Type T>
	void erase_(size_t startPos, size_t endPos)
	{		
		while (true)
		{
			auto overlineStart = std::find(begin() + startPos, begin() + endPos, '{');
			if (overlineStart == data().begin() + endPos) break;
			auto overlineEnd = findOverlineEnd(overlineStart, end());
			assert(overlineEnd != data().end());
			if (std::distance(begin(), overlineEnd) < endPos) endPos--;
			startPos = std::distance(begin(), overlineStart);
			auto local_end = std::distance(begin(), overlineEnd);
			/*ru_erase*/
			eraseOverline<T>(startPos, local_end);
			endPos--;
		}
		auto nCloseOverline = std::count(data().begin() + startPos, data().begin() + endPos, '}');
		if (nCloseOverline == endPos - startPos) return;

		erase(startPos, endPos, T);

		insert(startPos, std::string(nCloseOverline, '}'), T);
	}

	template<ERedoUndoComponent::Type T>
	void insertOverline(size_t startPos, size_t endPos)
	{
		insert(endPos, std::string("}"), T);
		insert(startPos, std::string("{"), T);
	}

	template<ERedoUndoComponent::Type T>
	void eraseOverline(size_t startPos, size_t endPos)
	{
		erase(endPos, T);
		erase(startPos, T);
	}
};

