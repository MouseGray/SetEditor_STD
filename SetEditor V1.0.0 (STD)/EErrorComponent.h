#pragma once

#include <vector>
#include <cstddef>
#include <algorithm>

#include "ETextChangeTrigger.h"
#include "ELabelComponent.h"

class EErrorComponent : public ELabelComponent
{
	using error = std::tuple<size_t, size_t, int>;
	enum class ErrorType : char {
		Fixed,
		Error
	};
	std::vector<std::pair<size_t, char>> m_textErrors;
public:
	void erased(size_t begin, size_t end, ERedoUndoComponent::Type ruType);

	void setErrors(size_t begin, std::vector<std::pair<size_t, char>>& data, ERedoUndoComponent::Type ruType);

	void pushError(const std::pair<size_t, char>& error);

	inline const std::vector<std::pair<size_t, char>>& errors() { return m_textErrors; }
};

