#include "ESelectComponent.h"

inline void EControlComponent::removeSelect()
{
    m_sCursor = m_cursor;
}

bool EControlComponent::isSelected()
{
    return m_cursor != m_sCursor;
}

int EControlComponent::moveTo(int pos)
{
    auto res = m_cursor;
    m_cursor = m_sCursor = pos;
    return res;
}

void EControlComponent::setSelectCursorPos(size_t pos) noexcept
{
    m_sCursor = pos;
}