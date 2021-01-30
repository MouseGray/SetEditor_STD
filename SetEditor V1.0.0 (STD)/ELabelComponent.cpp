#include "ELabelComponent.h"

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

void ELabelComponent::erased(size_t begin, size_t end)
{
	::erased(m_labels, begin, end);
}

void ELabelComponent::inserted(size_t pos, int count)
{
	::inserted(m_labels, pos, count);
}

void ELabelComponent::eraseLines(size_t begin, size_t end)
{
	m_lineNumers.erase(m_lineNumers.begin() + begin, m_lineNumers.begin() + end);
}

void ELabelComponent::insertLines(size_t pos, std::vector<int>& values)
{
	m_lineNumers.insert(m_lineNumers.begin() + pos, values.begin(), values.end());
}

void ELabelComponent::insertLines(size_t pos, int count)
{
	m_lineNumers.insert(m_lineNumers.begin() + pos, count, -1);
}
