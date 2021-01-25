#include "EOverlineComponent.h"

std::string EOverlineComponent::toCorrectOverline(const std::string& text)
{
    auto overlineLvl = 0;
    std::string result;
    for (auto ch : text)
    {
        if (ch == '{') overlineLvl++;
        if (ch == '}') {
            overlineLvl--;
            if (overlineLvl < 0) {
                overlineLvl = 0;
                continue;
            }
        }
        result += ch;
    }
    if (overlineLvl) 
        result.insert(result.size(), overlineLvl, '}');
    return result;
}

int EOverlineComponent::findOverlineEnd(int pos)
{
    auto overlineLvl = 0;
    auto size = static_cast<int>(m_buffer.size());
    for (int i = pos; i < size; i++) {
        if (m_buffer[i] == '{') overlineLvl++;
        if (m_buffer[i] == '}') {
            overlineLvl--;
            if (overlineLvl == 0) return i; 
        }
    }
    return m_buffer.size();
}
