#pragma once

#include <vector>
#include <cstddef>
#include <algorithm>

#include "ETextChangeTrigger.h"

class EErrorComponent
{
	enum class ErrorType {
		Fixed,
		Error
	};
	std::vector<std::pair<size_t, ErrorType>> m_textErrors;
	std::vector<std::pair<size_t, ErrorType>> m_connectionErrors;
public:
	void erased(size_t begin, size_t end);
	void inserted(size_t pos, int count);
};

