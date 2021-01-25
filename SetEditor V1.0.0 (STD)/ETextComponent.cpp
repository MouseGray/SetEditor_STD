#include "ETextComponent.h"

int ETextComponent::packIntoBorders(int pos) noexcept
{
	return std::max(std::min(pos, static_cast<int>(m_buffer.size())), 0);
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
