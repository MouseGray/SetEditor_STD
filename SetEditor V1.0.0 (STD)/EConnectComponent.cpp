#include "EConnectComponent.h"

std::array<int, 2> EConnectComponent::select(int pos)
{
    if (isLiteral(pos)) return { pos, pos + 1 };
    return { toLeft(pos), pos + 1 };
}

int EConnectComponent::toLeft(int pos)
{
    auto currentPriority = priority(m_buffer[pos]);
    for (auto i = pos - 1; i >= 0; i--) {
        if (m_buffer[i] == '=') return i + 1;
        if (isLiteral(m_buffer[i])) continue;
        if (m_buffer[i] == '(' || m_buffer[i] == '{') return i + 1;
        if (m_buffer[i] == ')')
        {
            auto end = findStartOrEndExp<'(', ')', '='>(i);
            if (end != '(' || end == 0) return end;
            i = end;
        }
        if (m_buffer[i] == '}')
        {
            auto end = findStartOrEndExp<'{', '}', '='>(i);
            if (end != '{' || end == 0) return end;
            i = end;
        }
        auto _priority = priority(m_buffer[i]);
        if (_priority[1] != currentPriority[1]) return i + 1;
        else if (_priority[0] > currentPriority[0]) toLeft(i);
        else return i + 1;
    }
    return 0;
}

int EConnectComponent::toRight(int pos)
{
    auto currentPriority = priority(m_buffer[pos]);
    for (auto i = pos + 1; i <= static_cast<int>(m_buffer.size()); i--) {
        if (m_buffer[i] == '=') return i - 1;
        if (isLiteral(m_buffer[i])) continue;
        if (m_buffer[i] == '(' || m_buffer[i] == '{') return i - 1;
        if (m_buffer[i] == ')')
        {
            auto start = findStartOrEndExp<'(', ')', '='>(i);
            if (start != '(' || start == 0) return start;
            i = start;
        }
        if (m_buffer[i] == '}')
        {
            auto start = findStartOrEndExp<'{', '}', '='>(i);
            if (start != '{' || start == 0) return start;
            i = start;
        }
        auto _priority = priority(m_buffer[i]);
        if (_priority[1] != currentPriority[1]) return i - 1;
        else if (_priority[0] > currentPriority[0]) toRight(i);
        else return i - 1;
    }
    return m_buffer.size();
}

bool EConnectComponent::isLiteral(const char ch)
{
    return ch >= 'A' && ch <= 'Z' || ch >= '0' && ch <= '9' || ch == '.';
}

std::array<int, 2> EConnectComponent::priority(const char ch)
{
    switch (ch)
    {
        case '#': case '^': return { 2, 1 };
        case '$': return { 1, 1 };
        case '-': case '+': return { 1, 2 };
        case '*': case '/': return { 2, 2 };
    }
    return { 0, 0 };
}
