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

    std::cout << "dr1:\n\n";
    for (auto& a : m_dirtyData)
        for (auto& b : a)
            std::cout << b.first->toString();
    // TODO: remove

    /*for (auto& a1 : m_dirtyData)
        for (auto& a2 : a1)
            TermTool::remove(a2.first);*/
    m_dirtyData.clear();

    m_rightData.push_back(right);
    apply(right, std::vector<int>());
    merge(right, 0, std::set<std::pair<int, int>>(), false);

    std::cout << "dr2:\n\n";
    for (auto& a : m_dirtyData)
        for (auto& b : a)
            std::cout << b.first->toString();

    /*stream << "dr2:\n";
    for (auto& a : m_dirtyData)
        for (auto& b : a)
            stream << b.first->toString();*/

    /*for (auto& a1 : m_dirtyData)
        for (auto& a2 : a1)
            TermTool::remove(a2.first);*/
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
        normalization::normalize(a);

    for (auto& a : m_rightData)
        normalization::normalize(a);

    std::cout << "Normalized left:\n";
    for (auto& a : m_leftData)
        std::cout << a->toString();
    std::cout << "Normalized right:\n";
    for (auto& a : m_rightData)
        std::cout << a->toString();
    std::cout << std::endl;

    for (auto& a : m_leftData)
        for (auto& b : m_rightData)
            if (TermTool::isEqual(*a, *b)) {
                res = true;
            };

    for (auto& a : m_leftData)
        TermTool::remove(a);
    for (auto& a : m_rightData)
        TermTool::remove(a);

    std::cout << "Result: " << res << std::endl;

    return res;
}

void TransformTable::apply(Term* term, std::vector<int> path)
{
    auto isFirst = true;

    std::cout << "Null:\n" << term->toString();

    auto res = transformation::complementVariable(term, Uniset, Nullset);
    //std::cout << "complementVariable:\n" << term->toString();

    if (res != nullptr) {
        if (isFirst)
            m_dirtyData.push_back(std::vector<std::pair<Term*, std::vector<int>>>());
        m_dirtyData.back().push_back(std::make_pair(res, path));
    }
    res = transformation::complementExpression(term);
    //std::cout << "complementExpression:\n" << term->toString();
    if (res != nullptr) {
        if (isFirst)
            m_dirtyData.push_back(std::vector<std::pair<Term*, std::vector<int>>>());
        m_dirtyData.back().push_back(std::make_pair(res, path));
    }
    res = transformation::parantheses(term, Union, Intersection);
    //std::cout << "parantheses Union Intersection:\n" << term->toString();
    if (res != nullptr) {
        if (isFirst)
            m_dirtyData.push_back(std::vector<std::pair<Term*, std::vector<int>>>());
        m_dirtyData.back().push_back(std::make_pair(res, path));
    }
    res = transformation::parantheses(term, Intersection, Union);
    //std::cout << "parantheses Intersection Union:\n" << term->toString();
    if (res != nullptr) {
        if (isFirst)
            m_dirtyData.push_back(std::vector<std::pair<Term*, std::vector<int>>>());
        m_dirtyData.back().push_back(std::make_pair(res, path));
    }
    res = transformation::parantheses(term, Multiplication, Addition);
    //std::cout << "parantheses Multiplication Addition:\n" << term->toString();
    if (res != nullptr) {
        if (isFirst)
            m_dirtyData.push_back(std::vector<std::pair<Term*, std::vector<int>>>());
        m_dirtyData.back().push_back(std::make_pair(res, path));
    }
    res = transformation::unisetComplement(term);
    //std::cout << "unisetComplement:\n" << term->toString();
    if (res != nullptr) {
        if (isFirst)
            m_dirtyData.push_back(std::vector<std::pair<Term*, std::vector<int>>>());
        m_dirtyData.back().push_back(std::make_pair(res, path));
    }
    res = transformation::minus(term);
    //std::cout << "minus:\n" << term->toString();
    if (res != nullptr) {
        if (isFirst)
            m_dirtyData.push_back(std::vector<std::pair<Term*, std::vector<int>>>());
        m_dirtyData.back().push_back(std::make_pair(res, path));
    }
    res = transformation::opposite(term, Intersection, Nullset);
    //std::cout << "opposite Intersection:\n" << term->toString();
    if (res != nullptr) {
        if (isFirst)
            m_dirtyData.push_back(std::vector<std::pair<Term*, std::vector<int>>>());
        m_dirtyData.back().push_back(std::make_pair(res, path));
    }
    res = transformation::opposite(term, Union, Uniset);
    //std::cout << "opposite Union:\n" << term->toString();
    if (res != nullptr) {
        if (isFirst)
            m_dirtyData.push_back(std::vector<std::pair<Term*, std::vector<int>>>());
        m_dirtyData.back().push_back(std::make_pair(res, path));
    }
    res = transformation::indentical(term);
    //std::cout << "indentical:\n" << term->toString();
    if (res != nullptr) {
        if (isFirst)
            m_dirtyData.push_back(std::vector<std::pair<Term*, std::vector<int>>>());
        m_dirtyData.back().push_back(std::make_pair(res, path));
    }
    auto vres = transformation::formulaExcIncX2(term);
    //std::cout << "formulaExcIncX2:\n" << term->toString();
    if (!vres.empty()) {
        if (isFirst)
            m_dirtyData.push_back(std::vector<std::pair<Term*, std::vector<int>>>());
        m_dirtyData.back().push_back(std::make_pair(res, path));
    }
    vres = transformation::formulaExcIncX3(term);
    //std::cout << "formulaExcIncX3:\n" << term->toString();
    if (!vres.empty()) {
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


