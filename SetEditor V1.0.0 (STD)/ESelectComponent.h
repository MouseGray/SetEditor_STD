#pragma once

#include "ETextComponent.h"

class ESelectComponent : public ETextComponent
{
public:
	int sInsert(const std::string& text);
	std::string sErase();
	std::string sErase(int count);

	inline void removeSelect();
	bool isSelected();

	void setSelectCursorPos(size_t pos) noexcept;
public:
	int m_sCursor = 0;
};