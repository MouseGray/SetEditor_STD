#pragma once
#include "TermTool.h"
#include "Header.h"

#define ERR_T_NO_ERROR 0x0000
#define ERR_T_NULL_PTR 0x0001
#define ERR_T_NOT_FOUND_LEFT 0x0002
#define ERR_T_NOT_FOUND_RIGHT 0x0003
#define ERR_T_NO_CHANGE 0x0004
#define ERR_T_INCORRECT_MARKER 0x0005
#define ERR_T_INCORRECT_EXPRESSION 0x0006
#define ERR_T_NOT_FOUND_TOFIND 0x00007
int T_ComplNullset(term_ptr pTerm_1, term_ptr pTerm_2);

int T_ComplUniset_R1(term_ptr pTerm_1, term_ptr pTerm_2);

int T_ComplUniset_R2(term_ptr pTerm_1, term_ptr pTerm_2);

int T_ComplUniset_R3M(term_ptr pTerm_1, term_ptr pTerm_2);

int T_ComplUniset_R4(term_ptr pTerm_1, term_ptr pTerm_2);

int T_ComplUniset_R5(term_ptr pTerm_1, term_ptr pTerm_2);

bool ___ComplExp(const term_ptr& pterm_1, const term_ptr& pterm_2);

bool ___UnionParantheses(const term_ptr& pterm_1, const term_ptr& pterm_2);

bool ___IntersectParantheses(const term_ptr& pterm_1, const term_ptr& pterm_2);

bool ___MultiplyParantheses(const term_ptr& pterm_1, const term_ptr& pterm_2);

int T_DoubleCompl(term_ptr pTerm_1, term_ptr pTerm_2);

int T_InterNullset_R1(term_ptr pTerm_1, term_ptr pTerm_2);

int T_InterNullset_R2(term_ptr pTerm_1, term_ptr pTerm_2);

int T_InterNullset_R3M(term_ptr pTerm_1, term_ptr pTerm_2);

int T_InterNullset_R4(term_ptr pTerm_1, term_ptr pTerm_2);

int T_InterNullset_R5(term_ptr pTerm_1, term_ptr pTerm_2);

int T_UnionNullset(term_ptr pTerm_1, term_ptr pTerm_2);

int T_InterUniset(term_ptr pTerm_1, term_ptr pTerm_2);

int T_UnionUniset(term_ptr pTerm_1, term_ptr pTerm_2);

bool ___InterExpAndExp(const term_ptr& pterm_1, const term_ptr& pterm_2);

bool ___UnionExpAndExp(const term_ptr& pterm_1, const term_ptr& pterm_2);

int T_InterExpAndComplExp_R1(term_ptr pTerm_1, term_ptr pTerm_2);

int T_InterExpAndComplExp_R2(term_ptr pTerm_1, term_ptr pTerm_2);

int T_InterExpAndComplExp_R3M(term_ptr pTerm_1, term_ptr pTerm_2);

int T_InterExpAndComplExp_R4(term_ptr pTerm_1, term_ptr pTerm_2);

int T_InterExpAndComplExp_R5(term_ptr pTerm_1, term_ptr pTerm_2);

int T_UnionExpAndComplExp(term_ptr pTerm_1, term_ptr pTerm_2);

bool ___DiffUnisetAndExp(const term_ptr& pterm_1, const term_ptr& pterm_2);

int T_DiffExpAndExp(term_ptr pTerm_1, term_ptr pTerm_2);

bool ___ExcIncFormulaX2(const term_ptr& pterm_1, const term_ptr& pterm_2);

bool ___ExcIncFormulaX3(const term_ptr& pterm_1, const term_ptr& pterm_2);

int T_DoubleNeg(term_ptr pTerm_1, term_ptr pTerm_2);

int T_NegExp(term_ptr pTerm_1, term_ptr pTerm_2);

int T_MultiplyZero_R1(term_ptr pTerm_1, term_ptr pTerm_2);

int T_MultiplyZero_R3(term_ptr pTerm_1, term_ptr pTerm_2);

int T_AddExpAndNegExp_R1(term_ptr pTerm_1, term_ptr pTerm_2);

int T_AddExpAndNegExp_R3(term_ptr pTerm_1, term_ptr pTerm_2);

int T_AddExpAndNegExp_R4(term_ptr pTerm_1, term_ptr pTerm_2);

int T_AddExpAndNegExp_R5(term_ptr pTerm_1, term_ptr pTerm_2);

int T_AddZero_M(term_ptr pTerm_1, term_ptr pTerm_2);

int T_Comp(term_ptr pTerm_1, term_ptr pTerm_2);

double _Complite(term_ptr pTerm) noexcept(false);

int T_CardialityNullset_R1(term_ptr pTerm_1, term_ptr pTerm_2);

int T_CardialityNullset_R3(term_ptr pTerm_1, term_ptr pTerm_2);

int T_CardialityNullset_R4(term_ptr pTerm_1, term_ptr pTerm_2);

int T_CardialityNullset_R5(term_ptr pTerm_1, term_ptr pTerm_2);

bool ___CardComplExp(const term_ptr& pterm_1, const term_ptr& pterm_2);

bool ___CardDiffUnisetAndExp(const term_ptr& pterm_1, const term_ptr& pterm_2);

bool ___IndenticalExp(const term_ptr& pterm_1, const term_ptr& pterm_2);

int T_MultiplyOne_M(term_ptr pTerm_1, term_ptr pTerm_2);

int T_MultiplyMinusOne(term_ptr pTerm_1, term_ptr pTerm_2);

int T_DivideByOne(term_ptr pTerm_1, term_ptr pTerm_2);

int T_ChangEqual(term_ptr pTerm_1_1, term_ptr pTerm_1_2, term_ptr pTerm_2_1, term_ptr pTerm_2_2);

term_ptr _ChangeEqualTo(term_ptr pReceiver, term_ptr pSource);

term_ptr _ChangeOfSign(term_ptr pTerm);

int T_Substitution(vector<term_ptr>* pMarkeredTerms_1, vector<term_ptr>* pMarkeredTerms_2, vector<term_ptr>* pTerms_1, vector<term_ptr>* pTerms_2);

int T_Transposition(term_ptr pTerm_1, term_ptr pTerm_2);

int T_Conclusion(const CTable* table, int lineID, const string& pTerm_1, const string& pTerm_2);

int T_Given(UserInfo& UI, term_ptr pTerm_1, term_ptr pTerm_2);

int T_ToFind(UserInfo& UI, term_ptr pTerm_1, term_ptr pTerm_2);

int T_Answer(const CTable* table, UserInfo& UI, int lineID, term_ptr pTerm_1, term_ptr pTerm_2);