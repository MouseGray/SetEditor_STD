#pragma once

#include <set>
#include <vector>
#include "Operations.h"

class TransformTable
{
public:
    TransformTable();

    bool start(Term* left, Term* right);

    void apply(Term* node, std::vector<int> path);

    void merge(Term* node, int no, std::set<std::pair<int, int>> set, bool f);

    std::vector<int> m_path;

    std::vector<std::vector<std::pair<Term*, std::vector<int>>>> m_dirtyData;

    std::vector<Term*> m_leftData;
    std::vector<Term*> m_rightData;
};

