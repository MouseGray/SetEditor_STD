#include "ETextComponent.h"

void ETextComponent::addCallback(Callback callback, CallbackType type)
{
	switch (type)
	{
		case CallbackType::erase:
			m_eraseListeners.push_back(callback);
			break;
		case CallbackType::eraseLine:
			m_eraseLineListeners.push_back(callback);
			break;
		case CallbackType::insert:
			m_insertListeners.push_back(callback);
			break;
		case CallbackType::insertLine:
			m_insertLineListeners.push_back(callback);
			break;
	}
}

void ETextComponent::notify(std::vector<Callback>& listeners, size_t begin, size_t end, ERedoUndoComponent::Type ruType)
{
	if (begin == end) return;
	for (auto a : listeners) a(begin, end, ruType);
}


void ETextComponent::erase(size_t begin, size_t end, ERedoUndoComponent::Type ruType)
{
	auto count = end - begin;
	if (count == 0) return;

	auto urUnit = new ERedoUndoErase();
	urUnit->pos = begin;
	urUnit->text = m_buffer.substr(begin, count);

	auto beginLine = std::count(m_buffer.begin(), m_buffer.begin() + begin, '\n');
	auto endLine = beginLine + std::count(m_buffer.begin() + begin, m_buffer.begin() + end, '\n');

	m_buffer.erase(begin, count);

	notify(m_eraseListeners, begin, end, ruType);
	notify(m_eraseLineListeners, beginLine, endLine, ruType);

	add(urUnit, ruType);
}

void ETextComponent::insert(size_t pos, const std::string& text, ERedoUndoComponent::Type ruType)
{
	auto correctText = toCorrectText(text);
	if (text.size() == 0) return;

	auto urUnit = new ERedoUndoInsert();
	urUnit->pos = pos;
	urUnit->count = text.size();

	auto beginLine = std::count(m_buffer.begin(), m_buffer.begin() + pos, '\n');
	auto endLine = beginLine + std::count(correctText.begin(), correctText.end(), '\n');

	m_buffer.insert(pos, text);

	add(urUnit, ruType);

	notify(m_insertListeners, pos, pos + correctText.size(), ruType);
	notify(m_insertLineListeners, beginLine, endLine, ruType);
}

std::string ETextComponent::toCorrectText(const std::string& text)
{
	std::string result;
	std::copy_if(text.begin(), text.end(), std::back_inserter(result), isCorrectChar);
	return result;
}

bool ETextComponent::isCorrectChar(const char c) noexcept
{
	if (c >= 'A' && c <= 'Z') return true;
	if (c >= '0' && c <= '9') return true;
	if (c == '.') return true;

	if (c == '\n') return true;
//	if (c == '\r') return true;

	if (c == '(') return true;
	if (c == ')') return true;
	if (c == '{') return true;
	if (c == '}') return true;
	if (c == '#') return true;
	if (c == '$') return true;
	if (c == '^') return true;
	if (c == '-') return true;
	if (c == '+') return true;
	if (c == '*') return true;
	if (c == '/') return true;
	if (c == '=') return true;
	return false;
}
