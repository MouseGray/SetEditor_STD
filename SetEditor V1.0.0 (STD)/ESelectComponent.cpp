#include "ESelectComponent.h"

int ESelectComponent::sInsert(const std::string& text)
{
    auto res = insert(text);
    removeSelect();
    return res;
}

std::string ESelectComponent::sErase()
{
    auto res = erase(m_sCursor - m_cursor);
    removeSelect();
    return res;
}

std::string ESelectComponent::sErase(int count)
{
    auto res = erase(count);
    removeSelect();
    return res;
}

inline void ESelectComponent::removeSelect()
{
    m_sCursor = m_cursor;
}

bool ESelectComponent::isSelected()
{
    return m_cursor != m_sCursor;
}

void ESelectComponent::setSelectCursorPos(size_t pos) noexcept
{
    m_sCursor = pos;
}