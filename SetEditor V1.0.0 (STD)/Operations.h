#pragma once
#include <functional>
#include <ranges>

#include "CombinationGenerator.h"
#include "DATLib.h"

#define ERR_T_NO_ERROR 0x0000
#define ERR_T_NULL_PTR 0x0001
#define ERR_T_NOT_FOUND_LEFT 0x0002
#define ERR_T_NOT_FOUND_RIGHT 0x0003
#define ERR_T_NO_CHANGE 0x0004
#define ERR_T_INCORRECT_MARKER 0x0005
#define ERR_T_INCORRECT_EXPRESSION 0x0006
#define ERR_T_NOT_FOUND_TOFIND 0x00007

namespace infinity 
{
	void doubleComplement(Term* term);

    /*
    * Union Nullset
    * Intersect Uniset
    * Add Zero
    * Multiply by One
    */
	template<typename T>
	void indifferentOperation(Term* term, action act, T value)
	{
        if (*term == act)
        {
            for (auto i = term->size() - 1; i >= 0; i--)
                if (term->get_ref(i) == value) *term >> i;
            if (term->size() == 1)
                TermTool::collapse(term, term->get(0));
        }

        for (auto i = 0; i < term->size(); i++)
            indifferentOperation(term->get(i), act, value);
	}

    /*
    * Union A and A
    * Intersect A and A
    */
    void duplicates(Term* term, action act);
}

namespace normalization
{
    void normalize(Term* term);

    void removeOneSubterm(Term* term);

    void removeReiteration(Term* term);

    void eraseSubtraction(Term* term);

    void eraseDivision(Term* term);

    void quantityNullset(Term* term);

    /*
    * Intersect Nullset
    * Union Uniset
    * Multiply by Zero
    */
    template<typename T>
    void collapse(Term* term, action act, T value)
    {
        if (*term == act)
        {
            for (auto i = 0; i < term->size(); i++)
                if (term->get_ref(i) == value) {
                    TermTool::removeChildren(term);
                    *term = value;
                    break;
                }
        }

        for (auto i = 0; i < term->size(); i++)
            collapse(term->get(i), act, value);
    }


    void coefficients(Term* term);
}

namespace transformation
{
    /*
    * Complement Nullset
    * Complement Uniset
    */
    Term* complementVariable(const Term* term, char oldVar, char newVar);

    Term* complementExpression(const Term* term);

    /*
    * Union
    * Intersect
    * Multiply
    */
    Term* parantheses(const Term* term, action top, action bottom);

    /*
    * Union A and Complement A
    * Intersect A and Complement A
    */
    Term* opposite(const Term* term, action act, char value);

    Term* minus(const Term* term);

    std::vector<Term*> formulaExcIncX2(const Term* term);

    std::vector<Term*> formulaExcIncX3(const Term* term);

    Term* indentical(const Term* term);

    Term* unisetComplement(const Term* term);
}

#if 0
int T_ChangEqual(term_ptr pTerm_1_1, term_ptr pTerm_1_2, term_ptr pTerm_2_1, term_ptr pTerm_2_2);

term_ptr _ChangeEqualTo(term_ptr pReceiver, term_ptr pSource);

term_ptr _ChangeOfSign(term_ptr pTerm);

int T_Substitution(vector<term_ptr>* pMarkeredTerms_1, vector<term_ptr>* pMarkeredTerms_2, vector<term_ptr>* pTerms_1, vector<term_ptr>* pTerms_2);

int T_Conclusion(const CTable* table, int lineID, const string& pTerm_1, const string& pTerm_2);

int T_Given(UserInfo& UI, term_ptr pTerm_1, term_ptr pTerm_2);

int T_ToFind(UserInfo& UI, term_ptr pTerm_1, term_ptr pTerm_2);

int T_Answer(const CTable* table, UserInfo& UI, int lineID, term_ptr pTerm_1, term_ptr pTerm_2);
#endif
