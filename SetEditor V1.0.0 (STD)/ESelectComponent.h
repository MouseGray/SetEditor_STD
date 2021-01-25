#pragma once

#include "EOverlineComponent.h"

class EControlComponent : public EOverlineComponent
{
public:
	inline void removeSelect();
	bool isSelected();

	template <ERedoUndoComponent::Type T>
	void moveTo(int cursor, int sCursor)
	{
		auto urUnit = new ERedoUndoCursor();
		urUnit->cursor = m_cursor;
		urUnit->sCursor = m_sCursor;
		m_cursor = cursor;
		m_sCursor = sCursor;
		add<T>(urUnit);
	}

	void setSelectCursorPos(size_t pos) noexcept;

public:
	int m_cursor  = 0;
	int m_sCursor = 0;
};