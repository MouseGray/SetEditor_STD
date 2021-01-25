#include "ESelectComponent.h"

inline void EControlComponent::removeSelect()
{
    m_sCursor = m_cursor;
}

bool EControlComponent::isSelected()
{
    return m_cursor != m_sCursor;
}

void EControlComponent::setSelectCursorPos(size_t pos) noexcept
{
    m_sCursor = pos;
}