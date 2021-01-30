#pragma once

#include <vector>
#include <algorithm>

template<typename T>
auto pair_comparator = [](std::pair<size_t, T>& el, size_t val)
{
    return std::get<0>(el) < val;
};

template<typename T>
void erased(std::vector<std::pair<size_t, T>>&vec, size_t begin, size_t end)
{
    auto count = end - begin;

    auto front = std::lower_bound(vec.begin(), vec.end(), begin, pair_comparator<T>);
    if (front == vec.end()) return;

    auto back = std::lower_bound(vec.begin(), vec.end(), end, pair_comparator<T>);

    std::for_each(back, vec.end(),
        [count](std::pair<size_t, T>& el) {
        std::get<0>(el) -= count;
    });

    vec.erase(front, back);
}

template<typename T>
void inserted(std::vector<std::pair<size_t, T>>& vec, size_t pos, int count)
{
    auto start = pos + count;
    auto front = std::lower_bound(vec.begin(), vec.end(), pos, pair_comparator<T>);
    if (front == vec.end()) return;

    std::for_each(front, vec.end(),
        [count](std::pair<size_t, T>& el) {
        el.first += count;
    });
}