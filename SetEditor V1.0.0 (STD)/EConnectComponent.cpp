#include "EConnectComponent.h"

void EConnectComponent::erased(size_t begin, size_t end, ERedoUndoComponent::Type ruType)
{
    auto count = end - begin;

    auto unit = new ERedoUndoEraseConnect();

    auto inErase = [begin, end](const std::tuple<size_t, size_t, int>& el)
    {
        return between(std::get<0>(el), begin, end) || between(std::get<1>(el), begin, end);
    };

    for (auto& el: m_connections)
    {
        if (inErase(el)) {
            unit->data.push_back(el);
            continue;
        }
        if (std::get<0>(el) >= end) std::get<0>(el) -= count;
        if (std::get<1>(el) >= end) std::get<1>(el) -= count;
    }

    m_connections.erase(std::remove_if(m_connections.begin(), m_connections.end(), inErase), m_connections.end());

    add(unit, ruType);
}

void EConnectComponent::inserted(size_t begin, size_t end, ERedoUndoComponent::Type ruType)
{
    auto count = end - begin;

    for (auto& el : m_connections)
    {
        if (std::get<0>(el) >= begin) std::get<0>(el) += count;
        if (std::get<1>(el) >= begin) std::get<1>(el) += count;
    }
}

void EConnectComponent::addConnects(std::vector<std::tuple<size_t, size_t, int>>& data, ERedoUndoComponent::Type ruType)
{
    m_connections.insert(m_connections.end(), data.begin(), data.end());

    auto unit = new ERedoUndoInsertConnect();
    for (auto el : data)
        unit->elements.push_back(std::get<0>(el));
    add(unit, ruType);
}

void EConnectComponent::removeConnects(std::vector<size_t>& data, ERedoUndoComponent::Type ruType)
{
    auto unit = new ERedoUndoEraseConnect();

    for (auto& val : data)
    {
        auto removeElement = std::find_if(m_connections.begin(), m_connections.end(), [val](const std::tuple<size_t, size_t, int>& el) {
                return std::get<0>(el) == val || std::get<1>(el) == val;
            });

        unit->data.push_back(*removeElement);
        m_connections.erase(removeElement);
    }

    if (unit->data.empty()) {
        delete unit;
        return;
    }

    add(unit, ruType);
}

bool EConnectComponent::addConnection(size_t pos)
{
    auto element = std::find_if(m_connections.begin(), m_connections.end(), [pos](const std::tuple<size_t, size_t, int>& el) {
        return std::get<0>(el) == pos || std::get<1>(el) == pos;
    });
    if (element != m_connections.end()) return false;

    m_temp = pos;
    m_isStarted = true;

    return true;
}

bool EConnectComponent::completeConnection(size_t pos)
{
    auto element = std::find_if(m_connections.begin(), m_connections.end(), [pos](const std::tuple<size_t, size_t, int>& el) {
        return std::get<0>(el) == pos || std::get<1>(el) == pos;
    });
    if (element != m_connections.end()) return false;

    startUndo(ERedoUndoComponent::Type::Null);
    m_isStarted = false; 
    std::vector<std::tuple<size_t, size_t, int>> vec;
    vec.push_back(std::make_tuple(m_temp, pos, 0));
    addConnects(vec, ERedoUndoComponent::Type::Null);
    finishUndo(ERedoUndoComponent::Type::Null);
    clearRedo(ERedoUndoComponent::Type::Null);
    return true;
}

void EConnectComponent::cancelConnection()
{
    m_isStarted = false;
}

bool EConnectComponent::removeConnection(size_t pos)
{
    auto element = std::find_if(m_connections.begin(), m_connections.end(), [pos](const std::tuple<size_t, size_t, int>& el) {
        return std::get<0>(el) == pos || std::get<1>(el) == pos;
    });
    if (element == m_connections.end()) return false;

    startUndo(ERedoUndoComponent::Type::Null);
    std::vector<size_t> vec;
    vec.push_back(pos);
    removeConnects(vec, ERedoUndoComponent::Type::Null);
    finishUndo(ERedoUndoComponent::Type::Null);
    clearRedo(ERedoUndoComponent::Type::Null);
    return true;
}

std::array<int, 2> EConnectComponent::select(int pos)
{
    if (isLiteral(data()[pos])) return { pos, pos + 1 };
    return { toLeft(pos), toRight(pos) };
}

int EConnectComponent::toLeft(int pos)
{
    auto currentPriority = priority(data()[pos]);
    for (auto i = pos - 1; i >= 0; i--) {
        if (data()[i] == '=') return i + 1;
        if (isLiteral(data()[i])) continue;
        if (data()[i] == '(' || data()[i] == '{') return i + 1;
        if (data()[i] == ')')
        {
            auto end = findStartOrEndExp<'(', ')', '='>(i);
            if (end != '(' || end == 0) return end;
            i = end;
        }
        if (data()[i] == '}')
        {
            auto end = findStartOrEndExp<'{', '}', '='>(i);
            if (end != '{' || end == 0) return end;
            i = end;
        }
        auto _priority = priority(data()[i]);
        if (_priority[1] != currentPriority[1]) return i + 1;
        else if (_priority[0] > currentPriority[0]) toLeft(i);
        else return i + 1;
    }
    return 0;
}

int EConnectComponent::toRight(int pos)
{
    auto currentPriority = priority(data()[pos]);
    for (auto i = pos + 1; i <= static_cast<int>(data().size()); i++) {
        if (data()[i] == '=') return i - 1;
        if (isLiteral(data()[i])) continue;
        if (data()[i] == '(' || data()[i] == '{') return i - 1;
        if (data()[i] == ')')
        {
            auto start = findStartOrEndExp<'(', ')', '='>(i);
            if (start != '(' || start == 0) return start;
            i = start;
        }
        if (data()[i] == '}')
        {
            auto start = findStartOrEndExp<'{', '}', '='>(i);
            if (start != '{' || start == 0) return start;
            i = start;
        }
        auto _priority = priority(data()[i]);
        if (_priority[1] != currentPriority[1]) return i - 1;
        else if (_priority[0] > currentPriority[0]) toRight(i);
        else return i - 1;
    }
    return data().size();
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
