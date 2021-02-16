#include "EOverlineComponent.h"

size_t EOverlineComponent::endOfOverline(size_t start)
{
	auto overlineLvl = 0;
	auto size = static_cast<int>(data().size());
	for (; start < size; start++) {
		if (data()[start] == static_cast<char>(Complement)) overlineLvl++;
		if (data()[start] == static_cast<char>(ComplementEnd))
			if (--overlineLvl) return start;
	}
	return std::string::npos;
}
