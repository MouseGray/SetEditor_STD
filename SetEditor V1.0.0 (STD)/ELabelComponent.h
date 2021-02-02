#pragma once

#include <vector>
#include <algorithm>

#include "ETextChangeTrigger.h"
#include "EConnectComponent.h"

class ELabelComponent : public EConnectComponent
{
	std::vector<std::pair<size_t, char>> m_labels;
	std::vector<int> m_lineNumbers;
public:
	ELabelComponent();

	inline const std::vector<std::pair<size_t, char>>& labels() { return m_labels; }

	inline int lineNumber(size_t n) { return m_lineNumbers[n]; }

	void addLabel(size_t pos, char value);
	void removeLabel(size_t pos);

	void insertLabels(size_t begin, std::vector<std::pair<size_t, char>>& data, ERedoUndoComponent::Type ruType);

	void erased(size_t begin, size_t end, ERedoUndoComponent::Type ruType);

	void erasedLines(size_t begin, size_t end, ERedoUndoComponent::Type ruType);
	void insertedLines(size_t begin, size_t end, ERedoUndoComponent::Type ruType);

	void setLines(size_t begin, std::vector<int>& data, ERedoUndoComponent::Type ruType);
};