#include "ETextComponent.h"

void ETextComponent::erase(size_t end, ERedoUndoComponent::Type redoUndoType)
{
	switch (redoUndoType)
	{
		case ERedoUndoComponent::Type::Undo:
			addUndo(ERedoUndoUnit(*this, &ETextComponent::insert, begin, std::string(m_buffer.substr(begin, end - begin)), ERedoUndoComponent::Type::Redo));
			break;
		case ERedoUndoComponent::Type::Redo:
			addRedo(ERedoUndoUnit(*this, &ETextComponent::insert, begin, m_buffer.substr(begin, end - begin), ERedoUndoComponent::Type::Undo));
			break;
	}

	if (begin == end) return;
	m_buffer.erase(m_buffer.begin() + begin, m_buffer.begin() + end);
}

void ETextComponent::insert(std::string text, ERedoUndoComponent::Type redoUndoType)
{
	toCorrectText(text);

	switch (redoUndoType)
	{
		case ERedoUndoComponent::Type::Undo:
			addUndo(ERedoUndoUnit(*this, &ETextComponent::erase, offset, offset + text.size(), ERedoUndoComponent::Type::Redo));
			break;
		case ERedoUndoComponent::Type::Redo:
			addRedo(ERedoUndoUnit(*this, &ETextComponent::erase, offset, offset + text.size(), ERedoUndoComponent::Type::Undo));
			break;
	}

	m_buffer.insert(offset, text);
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
