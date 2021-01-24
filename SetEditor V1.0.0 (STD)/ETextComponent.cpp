#include "ETextComponent.h"

std::string ETextComponent::erase(int count)
{
	if (count == 0) return std::string();
	auto size   = static_cast<int>(m_buffer.size());
	auto start  = std::min(std::max(std::min(m_cursor + count, m_cursor), 0), size);
	auto length = std::min(std::abs(count), size - start);
	if (length == 0) return std::string();
	std::string result = m_buffer.substr(start, length);
	m_buffer.erase(start, length);
	m_cursor = start;
	return result;
}

int ETextComponent::insert(std::string text)
{
	toCorrectText(text);
	if (text.size() == 0) return 0;
	m_buffer.insert(m_cursor, text);
	m_cursor += text.size();
	return text.size();
}

void ETextComponent::setCursorPos(size_t pos) noexcept
{
	assert(pos <= m_buffer.size());
	m_cursor = pos;
}

void ETextComponent::toCorrectText(std::string& text)
{
	text.erase(std::remove_if(text.begin(), text.end(), isNotCorrectChar), text.end());
}

bool ETextComponent::isNotCorrectChar(const char c) noexcept
{
	return !isCorrectChar(c);
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
	return false;
}
