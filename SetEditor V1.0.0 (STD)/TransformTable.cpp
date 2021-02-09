#include "TransformTable.h"

#include <fstream>

TransformTable::TransformTable()
{
}

bool TransformTable::start(Term* left, Term* right)
{
    std::ofstream stream("Log.txt");

    m_leftData.push_back(left);
    apply(left, std::vector<int>());
    merge(left, 0, std::set<std::pair<int, int>>(), true);

    stream << "dr1:\n";
    for (auto& a : m_dirtyData)
        for (auto& b : a)
            stream << b.first->toString();

    for (auto& a1 : m_dirtyData)
        for (auto& a2 : a1)
            delete a2.first;
    m_dirtyData.clear();

    m_rightData.push_back(right);
    apply(right, std::vector<int>());
    merge(right, 0, std::set<std::pair<int, int>>(), false);

    stream << "dr2:\n";
    for (auto& a : m_dirtyData)
        for (auto& b : a)
            stream << b.first->toString();

    for (auto& a1 : m_dirtyData)
        for (auto& a2 : a1)
            delete a2.first;
    m_dirtyData.clear();
    auto res = false;

    stream << "a:\n";
    for (auto& a : m_leftData)
        stream << a->toString();
    stream << "b:\n";
    for (auto& a : m_rightData)
        stream << a->toString();
    stream << "\n";
    stream.close();

    for (auto& a : m_leftData)
        for (auto& b : m_rightData)
            if (TermTool::isEqual(*a, *b)) {
                res = true;
            };

    for (auto& a : m_leftData)
        delete a;
    for (auto& a : m_rightData)
        delete a;

    return res;
}

void TransformTable::apply(Term* term, std::vector<int> path)
{
    auto isFirst = true;

    auto res = transformation::complementVariable(term, Nullset, Uniset);
    if (res != nullptr) {
        if (isFirst)
            m_dirtyData.push_back(std::vector<std::pair<Term*, std::vector<int>>>());
        m_dirtyData.back().push_back(std::make_pair(res, path));
    }
    res = transformation::complementVariable(term, Uniset, Nullset);
    if (res != nullptr) {
        if (isFirst)
            m_dirtyData.push_back(std::vector<std::pair<Term*, std::vector<int>>>());
        m_dirtyData.back().push_back(std::make_pair(res, path));
    }
    res = transformation::complementExpression(term);
    if (res != nullptr) {
        if (isFirst)
            m_dirtyData.push_back(std::vector<std::pair<Term*, std::vector<int>>>());
        m_dirtyData.back().push_back(std::make_pair(res, path));
    }
    res = transformation::parantheses(term, Union, Intersection);
    if (res != nullptr) {
        if (isFirst)
            m_dirtyData.push_back(std::vector<std::pair<Term*, std::vector<int>>>());
        m_dirtyData.back().push_back(std::make_pair(res, path));
    }
    res = transformation::parantheses(term, Intersection, Union);
    if (res != nullptr) {
        if (isFirst)
            m_dirtyData.push_back(std::vector<std::pair<Term*, std::vector<int>>>());
        m_dirtyData.back().push_back(std::make_pair(res, path));
    }
    res = transformation::parantheses(term, Multiplication, Addition);
    if (res != nullptr) {
        if (isFirst)
            m_dirtyData.push_back(std::vector<std::pair<Term*, std::vector<int>>>());
        m_dirtyData.back().push_back(std::make_pair(res, path));
    }
    res = transformation::unisetComplement(term);
    if (res != nullptr) {
        if (isFirst)
            m_dirtyData.push_back(std::vector<std::pair<Term*, std::vector<int>>>());
        m_dirtyData.back().push_back(std::make_pair(res, path));
    }
    res = transformation::minus(term);
    if (res != nullptr) {
        if (isFirst)
            m_dirtyData.push_back(std::vector<std::pair<Term*, std::vector<int>>>());
        m_dirtyData.back().push_back(std::make_pair(res, path));
    }
    res = transformation::opposite(term, Intersection, Nullset);
    if (res != nullptr) {
        if (isFirst)
            m_dirtyData.push_back(std::vector<std::pair<Term*, std::vector<int>>>());
        m_dirtyData.back().push_back(std::make_pair(res, path));
    }
    res = transformation::opposite(term, Union, Uniset);
    if (res != nullptr) {
        if (isFirst)
            m_dirtyData.push_back(std::vector<std::pair<Term*, std::vector<int>>>());
        m_dirtyData.back().push_back(std::make_pair(res, path));
    }
    res = transformation::indentical(term);
    if (res != nullptr) {
        if (isFirst)
            m_dirtyData.push_back(std::vector<std::pair<Term*, std::vector<int>>>());
        m_dirtyData.back().push_back(std::make_pair(res, path));
    }
    auto vres = transformation::formulaExcIncX2(term);
    if (res != nullptr) {
        if (isFirst)
            m_dirtyData.push_back(std::vector<std::pair<Term*, std::vector<int>>>());
        m_dirtyData.back().push_back(std::make_pair(res, path));
    }
    vres = transformation::formulaExcIncX3(term);
    if (res != nullptr) {
        if (isFirst)
            m_dirtyData.push_back(std::vector<std::pair<Term*, std::vector<int>>>());
        m_dirtyData.back().push_back(std::make_pair(res, path));
    }

    for (auto i = 0; i < term->size(); i++)
    {
        path.push_back(i);
        apply(term->get(i), path);
        path.pop_back();
    }
}

void TransformTable::merge(Term* term, int no, std::set<std::pair<int, int>> set, bool f)
{
    if (no == m_dirtyData.size()) return;
    merge(term, no + 1, set, f);
    for (auto i = 0; i < m_dirtyData[no].size(); i++)
    {
        if (m_dirtyData[no][i].second.empty()) {
            auto temp = TermTool::copy(term);
            TermTool::implement(temp, m_dirtyData[no][i]);
            if(f)m_leftData.push_back(temp);
            else m_rightData.push_back(temp);
            continue;
        }
        auto pair = std::make_pair(static_cast<int>(m_dirtyData[no][i].second.size()) - 1, m_dirtyData[no][i].second.back());
        if (set.find(pair) == set.end()) {
            auto temp = TermTool::copy(term);
            TermTool::implement(temp, m_dirtyData[no][i]);
            if (f)m_leftData.push_back(temp);
            else m_rightData.push_back(temp);
            auto temp_set = set;
            for (auto t = 0; t < m_dirtyData[no][i].second.size(); t++)
                temp_set.insert(std::make_pair(t, m_dirtyData[no][i].second[t]));
            merge(temp, no + 1, temp_set, f);
        }
    }
}


