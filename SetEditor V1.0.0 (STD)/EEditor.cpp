#include "EEditor.h"

void EEditor::eraseSelectedText()
{
	if (m_cursor == m_sCursor) return;
	erase();
}

void EEditor::eraseChar()
{
	if (m_cursor == data().size()) return;
	if (data()[m_cursor]);
}


