#pragma once

#include "ETextComponent.h"

class EControlComponent
{
public:
	inline void removeSelect();
	bool isSelected();

	int moveTo(int pos);

	void setSelectCursorPos(size_t pos) noexcept;

public:
	int m_cursor  = 0;
	int m_sCursor = 0;
};