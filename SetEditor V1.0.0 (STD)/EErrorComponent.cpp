#include "EErrorComponent.h"

void EErrorComponent::erased(size_t begin, size_t end, ERedoUndoComponent::Type ruType)
{
	auto unit = ::erased<ERedoUndoEraseError>(m_textErrors, begin, end);
	if (unit == nullptr) return;
	add(unit, ruType);
}

void EErrorComponent::setErrors(size_t begin, std::vector<std::pair<size_t, char>>& data, ERedoUndoComponent::Type ruType)
{
	m_textErrors.insert(m_textErrors.begin() + begin, data.begin(), data.end());

	auto unit = new ERedoUndoInsertError();
	unit->begin = begin;
	unit->end = begin + data.size();
	add(unit, ruType);
}

void EErrorComponent::pushError(const std::pair<size_t, char>& error)
{
	m_textErrors.push_back(error);
}
