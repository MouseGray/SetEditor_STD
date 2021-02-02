#pragma once

#include "EOverlineComponent.h"

class EControlComponent : public EOverlineComponent
{
public:
	inline void removeSelect();
	bool isSelected();
	
	void inserted(size_t begin, size_t end, ERedoUndoComponent::Type ruType)
	{
		auto urUnit = new ERedoUndoCursor();
		urUnit->cursor = m_cursor;
		urUnit->sCursor = m_sCursor;
		m_cursor = m_sCursor = end;
		add(urUnit, ruType);
	}

	void erased(size_t begin, size_t end, ERedoUndoComponent::Type ruType)
	{
		auto urUnit = new ERedoUndoCursor();
		urUnit->cursor = m_cursor;
		urUnit->sCursor = m_sCursor;
		m_cursor = m_sCursor = begin;
		add(urUnit, ruType);
	}

	void moveTo(int cursor, int sCursor, ERedoUndoComponent::Type ruType)
	{
		auto urUnit = new ERedoUndoCursor();
		urUnit->cursor = m_cursor;
		urUnit->sCursor = m_sCursor;
		m_cursor = cursor;
		m_sCursor = sCursor;
		add(urUnit, ruType);
	}

	void setSelectCursorPos(size_t pos) noexcept;

public:
	int m_cursor  = 0;
	int m_sCursor = 0;
};