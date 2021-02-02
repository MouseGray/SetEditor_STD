#pragma once

#include <vector>
#include <cstddef>
#include <algorithm>

#include "ETextChangeTrigger.h"
#include "ELabelComponent.h"

class EErrorComponent : public ELabelComponent
{
	enum class ErrorType : char {
		Fixed,
		Error
	};
	std::vector<std::pair<size_t, char>> m_textErrors;
public:
	void erased(size_t begin, size_t end, ERedoUndoComponent::Type ruType);

	void setErrors(size_t begin, std::vector<std::pair<size_t, char>>& data, ERedoUndoComponent::Type ruType);

	inline const std::vector<std::pair<size_t, char>>& errors() { return m_textErrors; }
};

