#include "ETextComponent.h"

std::string ETextComponent::erase(int pos, int count)
{
	std::string result = m_buffer.substr(pos, count);
	m_buffer.erase(pos, count);
	return result;
}

int ETextComponent::insert(int pos, std::string text)
{
	toCorrectText(text);
	if (text.size() == 0) return 0;
	m_buffer.insert(pos, text);
	return text.size();
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
