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
    auto size = static_cast<int>(data().size());
    for (int i = pos; i < size; i++) {
        if (data()[i] == '{') overlineLvl++;
        if (data()[i] == '}') {
            overlineLvl--;
            if (overlineLvl == 0) return i; 
        }
    }
    return data().size();
}
