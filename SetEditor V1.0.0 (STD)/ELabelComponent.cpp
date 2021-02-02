#include "ELabelComponent.h"

ELabelComponent::ELabelComponent()
{
	m_lineNumbers.push_back(-1);
}

void ELabelComponent::addLabel(size_t pos, char value)
{
	auto front = std::lower_bound(m_labels.begin(), m_labels.end(), pos, ::pair_comparator<char>);
	if (front == m_labels.end()) {
		m_labels.push_back(std::make_pair(pos, value));
		return;
	}
	if (front->first == pos) {
		front->second = value;
		return;
	}
	m_labels.insert(front, std::make_pair(pos, value));
}

void ELabelComponent::removeLabel(size_t pos)
{
	auto el = std::find_if(m_labels.begin(), m_labels.end(), 
		[pos](const std::pair<size_t, char>& el) {
			return el.first == pos;
		});
	m_labels.erase(el);
}

void ELabelComponent::insertLabels(size_t begin, std::vector<std::pair<size_t, char>>& data, ERedoUndoComponent::Type ruType)
{
	m_labels.insert(m_labels.begin() + begin, data.begin(), data.end());

	auto unit = new ERedoUndoInsertLabel();
	unit->begin = begin;
	unit->end = begin + data.size();
	add(unit, ruType);
}

void ELabelComponent::erased(size_t begin, size_t end, ERedoUndoComponent::Type ruType)
{
	auto unit = ::erased<ERedoUndoEraseLabel>(m_labels, begin, end);
	if (unit == nullptr) return;
	add(unit, ruType);
}

void ELabelComponent::erasedLines(size_t begin, size_t end, ERedoUndoComponent::Type ruType)
{
	auto unit = new ERedoUndoEraseLineNumber();
	unit->begin = begin;
	unit->data.insert(unit->data.begin(), m_lineNumbers.begin() + begin, m_lineNumbers.begin() + end);

	m_lineNumbers.erase(m_lineNumbers.begin() + begin, m_lineNumbers.begin() + end);
	add(unit, ruType);
}

void ELabelComponent::insertedLines(size_t begin, size_t end, ERedoUndoComponent::Type ruType)
{
	m_lineNumbers.insert(m_lineNumbers.begin() + begin, end - begin, -1);
}

void ELabelComponent::setLines(size_t begin, std::vector<int>& data, ERedoUndoComponent::Type ruType)
{
	std::copy(data.begin(), data.end(), m_lineNumbers.begin() + begin);

	auto unit = new ERedoUndoInsertLineNumber();
	unit->begin = begin;
	unit->end = begin + data.size();
	add(unit, ruType);
}
