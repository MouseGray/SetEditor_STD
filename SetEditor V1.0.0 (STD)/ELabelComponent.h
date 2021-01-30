#pragma once

#include <vector>
#include <algorithm>

#include "ETextChangeTrigger.h"

class ELabelComponent
{
	std::vector<std::pair<size_t, char>> m_labels;
	std::vector<int> m_lineNumers;
public:
	inline std::vector<std::pair<size_t, char>> labels() const { return m_labels; }

	void addLabel(size_t pos, char value);
	void removeLabel(size_t pos);

	void erased(size_t begin, size_t end);
	void inserted(size_t pos, int count);

	void eraseLines(size_t begin, size_t end);
	void insertLines(size_t pos, std::vector<int>& values);
	void insertLines(size_t pos, int count = 1);
};