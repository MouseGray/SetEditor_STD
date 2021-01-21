#include "Transforms.h"


bool TReleaseParantheses(term_ptr pterm, char curOperation, char newOperation)
{
	if (!pterm->is(curOperation, term_type::operation)) return false;

	int count = 1; // Кол-во новых выражений
	int work_var = 1;
	// Найти кол-во скобок после раскрытия
	for (size_t i = 0; i < pterm->size(); i++)
		if (pterm->get(i)->is(newOperation, term_type::operation)) count *= pterm->get(i)->size();

	if (count == 1) return false;

	vector<term_ptr> M; // Массив новых скобок
	for (size_t i = 0; i < count; i++) M.push_back(make_shared<Term>(curOperation, term_type::operation));

	for (size_t i = 0; i < pterm->size(); i++)
	{
		if (pterm->get(i)->is(newOperation, term_type::operation))
		{
			for (size_t m = 0; m < count;)
			{
				for (size_t j = 0; j < pterm->get(i)->size(); j++)
				{
					for (size_t l = 0; l < work_var; l++)
						M[m++]->add(shared_ptr<Term>(pterm->get(i)->get(j)));
				}
			}
			work_var *= pterm->get(i)->size();
			continue;
		}
		for (size_t m = 0; m < count; m++) M[m]->add(shared_ptr<Term>(pterm->get(i)));
	}

	pterm->set(newOperation, term_type::operation);
	pterm->remove_all();
	for (size_t m = 0; m < count; m++) pterm->add(M[m]);
}

bool T____Parantheses(term_ptr pterm_1, term_ptr pterm_2, char topOperation, char bottomOperation)
{
	// Правая: topOperation bottomOperation`s
	// Левая: bottomOperation topOperation`s
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pterm_1 == nullptr || pterm_2 == nullptr) return false;

	// 1. 
	// UNION(UNION_1(a11, a12, ..., a1n), ..., UNION_m(am1, am2, ..., amk), UNION_m+1(bm+11, bm+12, ..., bm+1l), ..., UNION_m+k(bm+k1, bm+k2, ..., bm+kj), c1, ..., ct) =>
	// UNION(a11, a12, ..., a1n, ..., am1, am2, ..., amk, bm + 11, bm + 12, ..., bm + 1l, ..., bm + k1, bm + k2, ..., bm + kj, c1, ..., ct) => Collect =>
	// UNION(a11, a12, ..., a1n, ..., am1, am2, ..., amk)
	// INTERSECT(UNION(INTERSECT_1(...), ..., INTERSECT_n(...))) = INTERSECT(UNION_1(...), ..., UNION_k(...))
	if (pterm_1->is(topOperation, term_type::operation)) {
		if (pterm_2->is(bottomOperation, term_type::operation)) {
			TReleaseParantheses(pterm_1, topOperation, bottomOperation);
			TermTool::ToNormalForm(pterm_1);
			if (TermTool::Equal(pterm_1, pterm_2)) return true;

			for (size_t i = 0; i < pterm_2->size(); i++) {
				TReleaseParantheses(pterm_2->get(i), topOperation, bottomOperation);
			}
			TermTool::ToNormalForm(pterm_1);
			TermTool::ToNormalForm(pterm_2);
			return TermTool::Equal(pterm_1, pterm_2);
		}
		return false;
	}
	if (pterm_2->is(topOperation, term_type::operation)) {
		if (pterm_1->is(bottomOperation, term_type::operation)) {
			TReleaseParantheses(pterm_2, topOperation, bottomOperation);
			TermTool::ToNormalForm(pterm_2);
			if (TermTool::Equal(pterm_1, pterm_2)) return true;

			for (size_t i = 0; i < pterm_1->size(); i++) {
				TReleaseParantheses(pterm_1->get(i), topOperation, bottomOperation);
			}
			TermTool::ToNormalForm(pterm_1);
			return TermTool::Equal(pterm_1, pterm_2);
		}
		return false;
	}
	if (pterm_1->is(bottomOperation, term_type::operation)) {
		if (pterm_2->is(bottomOperation, term_type::operation)) {
			TermTool::Collect(pterm_1, pterm_2);
			for (size_t i = 0; i < pterm_1->size(); i++) {
				TReleaseParantheses(pterm_1->get(i), topOperation, bottomOperation);
			}
			for (size_t i = 0; i < pterm_2->size(); i++) {
				TReleaseParantheses(pterm_2->get(i), topOperation, bottomOperation);
			}
			TermTool::ToNormalForm(pterm_1);
			TermTool::ToNormalForm(pterm_2);
			return TermTool::Equal(pterm_1, pterm_2);
		}
		return false;
	}

	return false;
}

bool T___IntersectParantheses(const term_ptr& pterm_1, const term_ptr& pterm_2)
{
	return T____Parantheses(pterm_1, pterm_2, IntersectionSign, UnionSign);
}