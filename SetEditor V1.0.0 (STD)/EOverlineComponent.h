#pragma once

#include <ranges>
#include <cassert>
#include <algorithm>

#include "ETextComponent.h"
#include "Operation.h"

class EOverlineComponent : public ETextComponent
{
public:
	// [revised 14.02.21 16:35]
	size_t endOfOverline(size_t start);

	// [revised 14.02.21 16:34]
	template<ERedoUndoComponent::Type T>
	void erase_(size_t startPos, size_t endPos)
	{		
		while (true)
		{
			auto overlineStart = data().find(static_cast<char>(Complement), startPos);
			if (overlineStart >= endPos) break; // include overlineStart == std::string::npos

			auto overlineEnd = endOfOverline(overlineStart);
			assert(overlineEnd == std::string::npos);

			if (overlineEnd < endPos) endPos--;
			startPos = overlineStart;

			eraseOverline<T>(overlineStart, overlineEnd);
			endPos--;
		}
		auto nCloseOverline = std::count(data().begin() + startPos, data().begin() + endPos, static_cast<char>(ComplementEnd));
		if (nCloseOverline == endPos - startPos) return;

		erase(startPos, endPos, T);

		insert(startPos, std::string(nCloseOverline, static_cast<char>(ComplementEnd)), T);
	}

	// [revised 14.02.21 16:36]
	template<ERedoUndoComponent::Type T>
	void insertOverline(size_t startPos, size_t endPos)
	{
		insert(endPos, std::string({ static_cast<char>(ComplementEnd) }), T);
		insert(startPos, std::string({ static_cast<char>(Complement) }), T);
	}

	// [revised 14.02.21 16:36]
	template<ERedoUndoComponent::Type T>
	void eraseOverline(size_t startPos, size_t endPos)
	{
		erase(endPos, T);
		erase(startPos, T);
	}
};

