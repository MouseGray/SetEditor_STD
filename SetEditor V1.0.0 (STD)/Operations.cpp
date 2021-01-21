#include "Operations.h"

int T_ComplNullset(term_ptr pTerm_1, term_ptr pTerm_2)
{
/*	// Правая: универсальное множесто
	// Левая: дополнение к пустому множеству
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pTerm_1 == nullptr || pTerm_2 == nullptr) return ERR_T_NULL_PTR;
	// ----------------------------------------
	// Нахождение сторон
	// ----------------------------------------
	term_ptr left, right;
	if (pTerm_1->is(UNISET, term_type::symbol)) {
		left = pTerm_2;
		right = pTerm_1;
	}
	else {
		if (!pTerm_2->is(UNISET, term_type::symbol)) return ERR_T_NOT_FOUND_RIGHT;
		left = pTerm_1;
		right = pTerm_2;
	}
	// ----------------------------------------
	// Проверить левую сторону
	// ----------------------------------------
	if (!left->is(OS_1, term_type::operation, 1)) return ERR_T_NOT_FOUND_LEFT;
	if (!left->get(0)->is(NULLSET, term_type::symbol)) return ERR_T_NOT_FOUND_LEFT;*/
	return ERR_T_NO_ERROR;
}

int T_ComplUniset_R1(term_ptr pTerm_1, term_ptr pTerm_2)
{
/*	// Правая: пустое множесто
	// Левая: дополнение к универсальному множеству
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pTerm_1 == nullptr || pTerm_2 == nullptr) return ERR_T_NULL_PTR;
	// ----------------------------------------
	// Нахождение сторон
	// ----------------------------------------
	term_ptr left, right;
	if (pTerm_1->is(NULLSET, term_type::symbol)) {
		left = pTerm_2;
		right = pTerm_1;
	}
	else {
		if (!pTerm_2->is(NULLSET, term_type::symbol)) return ERR_T_NOT_FOUND_RIGHT;
		left = pTerm_1;
		right = pTerm_2;
	}
	// ----------------------------------------
	// Проверить левую сторону
	// ----------------------------------------
	if (!left->is(OS_1, term_type::operation, 1)) return ERR_T_NOT_FOUND_LEFT;
	if (!left->get(0)->is(UNISET, term_type::symbol)) return ERR_T_NOT_FOUND_LEFT;*/
	return ERR_T_NO_ERROR;
}

int T_ComplUniset_R2(term_ptr pTerm_1, term_ptr pTerm_2)
{
/*	// Правая: ноль
	// Левая: мощность дополнения к универсальному множеству
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pTerm_1 == nullptr || pTerm_2 == nullptr) return ERR_T_NULL_PTR;
	// ----------------------------------------
	// Нахождение сторон
	// ----------------------------------------
	term_ptr left, right;
	if (pTerm_1->is(0, term_type::number)) {
		left = pTerm_2;
		right = pTerm_1;
	}
	else {
		if (!pTerm_2->is(0, term_type::number)) return ERR_T_NOT_FOUND_RIGHT;
		left = pTerm_1;
		right = pTerm_2;
	}
	// ----------------------------------------
	// Проверить левую сторону
	// ----------------------------------------
	if (!left->is(OB__, term_type::operation, 1)) return ERR_T_NOT_FOUND_LEFT;
	if (!left->get(0)->is(OS_1, term_type::operation, 1)) return ERR_T_NOT_FOUND_LEFT;
	if (!left->get(0)->get(0)->is(UNISET, term_type::symbol)) return ERR_T_NOT_FOUND_LEFT;*/
	return ERR_T_NO_ERROR;
}

int T_ComplUniset_R3M(term_ptr pTerm_1, term_ptr pTerm_2)
{
/*	// Правая: какое-то выражение 
	// Левая: сложение какого-то выражения и мощности дополнения к универсальному множеству
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pTerm_1 == nullptr || pTerm_2 == nullptr) return ERR_T_NULL_PTR;
	// ----------------------------------------
	// Нахождение количеств мощностей дополнения к универсальному множеству
	// в каждой стороне и удаление их
	// ----------------------------------------
	size_t amountFirstExp = 0, amountSecondExp = 0;
	if (pTerm_1->is(OA_2, term_type::operation)) {
		for (int i = pTerm_1->size() - 1; i >= 0; i--) {
			if (!pTerm_1->get(i)->is(OB__, term_type::operation, 1)) continue;
			if (!pTerm_1->get(i)->get(0)->is(OS_1, term_type::operation, 1)) continue;
			if (!pTerm_1->get(i)->get(0)->get(0)->is(UNISET, term_type::symbol)) continue;
			pTerm_1->remove(i);
			amountFirstExp++;
		}
		if (pTerm_1->size() == 1) pTerm_1 = TrimTop(pTerm_1);
	}
	if (pTerm_2->is(OA_2, term_type::operation)) {
		for (int i = pTerm_2->size() - 1; i >= 0; i--) {
			if (!pTerm_2->get(i)->is(OB__, term_type::operation, 1)) continue;
			if (!pTerm_2->get(i)->get(0)->is(OS_1, term_type::operation, 1)) continue;
			if (!pTerm_2->get(i)->get(0)->get(0)->is(UNISET, term_type::symbol)) continue;
			pTerm_2->remove(i);
			amountSecondExp++;
		}
		if (pTerm_2->size() == 1) pTerm_2 = TrimTop(pTerm_2);
	}
	// ----------------------------------------
	// Проверить равенство оставшихся частей
	// ----------------------------------------
	if (amountFirstExp == 0 && amountSecondExp == 0) return ERR_T_INCORRECT_MARKER;
	if (amountFirstExp == amountSecondExp) return ERR_T_INCORRECT_MARKER;
	if (!Equal(pTerm_1, pTerm_2)) return ERR_T_INCORRECT_MARKER;*/
	return ERR_T_NO_ERROR;
}

int T_ComplUniset_R4(term_ptr pTerm_1, term_ptr pTerm_2)
{
/*	// Правая: ноль
	// Левая: минус мощность дополнения к универсальному множеству
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pTerm_1 == nullptr || pTerm_2 == nullptr) return ERR_T_NULL_PTR;
	// ----------------------------------------
	// Нахождение сторон
	// ----------------------------------------
	term_ptr left, right;
	if (pTerm_1->is(0, term_type::number)) {
		left = pTerm_2;
		right = pTerm_1;
	}
	else {
		if (!pTerm_2->is(0, term_type::number)) return ERR_T_NOT_FOUND_RIGHT;
		left = pTerm_1;
		right = pTerm_2;
	}
	// ----------------------------------------
	// Проверить левую сторону
	// ----------------------------------------
	if (!left->is(OA_1, term_type::operation, 1)) return ERR_T_NOT_FOUND_LEFT;
	if (!left->get(0)->is(OB__, term_type::operation, 1)) return ERR_T_NOT_FOUND_LEFT;
	if (!left->get(0)->get(0)->is(OS_1, term_type::operation, 1)) return ERR_T_NOT_FOUND_LEFT;
	if (!left->get(0)->get(0)->get(0)->is(UNISET, term_type::symbol)) return ERR_T_NOT_FOUND_LEFT;*/
	return ERR_T_NO_ERROR;
}

int T_ComplUniset_R5(term_ptr pTerm_1, term_ptr pTerm_2)
{
/*	// Правая: какое-то выражение 
	// Левая: сложение какого-то выражения и минус мощности дополнения к универсальному множеству
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pTerm_1 == nullptr || pTerm_2 == nullptr) return ERR_T_NULL_PTR;
	// ----------------------------------------
	// Нахождение количеств минус мощностей дополнения к универсальному множеству
	// в каждой стороне и удаление их
	// ----------------------------------------
	size_t amountFirstExp = 0, amountSecondExp = 0;
	if (pTerm_1->is(OA_2, term_type::operation)) {
		for (int i = pTerm_1->size() - 1; i >= 0; i--) {
			if (!pTerm_1->get(i)->is(OA_1, term_type::operation, 1)) continue;
			if (!pTerm_1->get(i)->get(0)->is(OB__, term_type::operation, 1)) continue;
			if (!pTerm_1->get(i)->get(0)->get(0)->is(OS_1, term_type::operation, 1)) continue;
			if (!pTerm_1->get(i)->get(0)->get(0)->get(0)->is(UNISET, term_type::symbol)) continue;
			pTerm_1->remove(i);
			amountFirstExp++;
		}
		if (pTerm_1->size() == 1) pTerm_1 = TrimTop(pTerm_1);
	}
	if (pTerm_2->is(OA_2, term_type::operation)) {
		for (int i = pTerm_2->size() - 1; i >= 0; i--) {
			if (!pTerm_2->get(i)->is(OA_1, term_type::operation, 1)) continue;
			if (!pTerm_2->get(i)->get(0)->is(OB__, term_type::operation, 1)) continue;
			if (!pTerm_2->get(i)->get(0)->get(0)->is(OS_1, term_type::operation, 1)) continue;
			if (!pTerm_2->get(i)->get(0)->get(0)->get(0)->is(UNISET, term_type::symbol)) continue;
			pTerm_2->remove(i);
			amountSecondExp++;
		}
		if (pTerm_2->size() == 1) pTerm_2 = TrimTop(pTerm_2);
	}
	// ----------------------------------------
	// Проверить равенство оставшихся частей
	// ----------------------------------------
	if (amountFirstExp == 0 && amountSecondExp == 0) return ERR_T_INCORRECT_MARKER;
	if (amountFirstExp == amountSecondExp) return ERR_T_INCORRECT_MARKER;
	if (!Equal(pTerm_1, pTerm_2)) return ERR_T_INCORRECT_MARKER;*/
	return ERR_T_NO_ERROR;
}

bool ReleaseCompl(term_ptr pterm, char curOperation, char newOperation)
{
	if (!pterm->is(ComplementSign, term_type::operation, 1)) return false;
	if (!pterm->get(0)->is(curOperation, term_type::operation)) return false;
	term_ptr complTerm = nullptr;
	for (size_t i = 0; i < pterm->get(0)->size(); i++) {
		complTerm = make_shared<Term>(ComplementSign, term_type::operation);
		complTerm->add(pterm->get(0)->get(i));
		pterm->add(complTerm);
	}
	pterm->remove(0);
	pterm->set(newOperation, term_type::operation);
	return true;
}

bool ___ComplExp(const term_ptr& pterm_1, const term_ptr& pterm_2)
{
	// Правая: дополнение к пересечению/объединению выражений
	// Левая: объединение/пересечение дополнений выражений
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pterm_1 == nullptr || pterm_2 == nullptr) return false;
	
	// 1. UNION(COMPL(INTERSECT(A1, ..., Am)), T1, T2, ..., Tn) = UNION(COMPL(A1), ... COMPL(Am), T1, ..., Tn)
	// 1.1. INTERSECT(COMPL(UNION(A1, ..., Am)), T1, T2, ..., Tn) = INTERSECT(COMPL(A1), ... COMPL(Am), T1, ..., Tn)
	// 2. COMPL(UNION(A1, ..., Am)) = INTERSECT(COMPL(A1), ..., COMPL(Am))
	// 2.1. COMPL(INTERSECT(A1, ..., Am)) = UNION(COMPL(A1), ..., COMPL(Am))
	// Уточнение: COMPL(F(A1, ..., Am)) = F`(COMPL(A`11, ... A`1k)1, ..., COMPL(A`t, ... A`tk)t), F = UNION || INTERSECT

	term_ptr _pterm_1 = TermTool::Copy(pterm_1);
	term_ptr _pterm_2 = TermTool::Copy(pterm_2);

	double F = 0;
	double NF = 0;
	term_ptr complTerm = nullptr;
	term_ptr FTerm = nullptr;
	// Случай 1.
	if (_pterm_1->is(UnionSign, term_type::operation) && _pterm_2->is(UnionSign, term_type::operation)) {
		// Удаление одинаковых подтермов.
		TermTool::Collect(_pterm_1, _pterm_2);
		// Преобразование 
		for (size_t i = 0; i < _pterm_1->size(); i++) {
			if (_pterm_1->get(i)->is(ComplementSign, term_type::operation, 1)) {
				if (_pterm_1->get(i)->get(0)->is(IntersectionSign, term_type::operation)) {
					FTerm = make_shared<Term>(UnionSign, term_type::operation);
					for (size_t j = 0; j < _pterm_1->get(i)->get(0)->size(); j++) {
						complTerm = make_shared<Term>(ComplementSign, term_type::operation);
						complTerm->add(_pterm_1->get(i)->get(0)->get(j));
						FTerm->add(complTerm);
					}
					_pterm_1->add(FTerm);
					_pterm_1->remove(i);
					continue;
				}
			}
		}
		for (size_t i = 0; i < _pterm_2->size(); i++) {
			if (_pterm_2->get(i)->is(ComplementSign, term_type::operation, 1)) {
				if (_pterm_2->get(i)->get(0)->is(IntersectionSign, term_type::operation)) {
					FTerm = make_shared<Term>(UnionSign, term_type::operation);
					for (size_t j = 0; j < _pterm_2->get(i)->get(0)->size(); j++) {
						complTerm = make_shared<Term>(ComplementSign, term_type::operation);
						complTerm->add(_pterm_2->get(i)->get(0)->get(j));
						FTerm->add(complTerm);
					}
					_pterm_2->add(FTerm);
					_pterm_2->remove(i);
					continue;
				}
			}
		}
		TermTool::ToNormalForm(_pterm_1);
		TermTool::ToNormalForm(_pterm_2);
		return TermTool::Equal(_pterm_1, _pterm_2);
	}
	// Случай 1.1.
	if (_pterm_1->is(IntersectionSign, term_type::operation) && _pterm_2->is(IntersectionSign, term_type::operation)) {
		// Удаление одинаковых подтермов.
		TermTool::Collect(_pterm_1, _pterm_2);
		// Преобразование 
		for (size_t i = 0; i < _pterm_1->size(); i++) {
			if (_pterm_1->get(i)->is(ComplementSign, term_type::operation, 1)) {
				if (_pterm_1->get(i)->get(0)->is(UnionSign, term_type::operation)) {
					FTerm = make_shared<Term>(IntersectionSign, term_type::operation);
					for (size_t j = 0; j < _pterm_1->get(i)->get(0)->size(); j++) {
						complTerm = make_shared<Term>(ComplementSign, term_type::operation);
						complTerm->add(_pterm_1->get(i)->get(0)->get(j));
						FTerm->add(complTerm);
					}
					_pterm_1->add(FTerm);
					_pterm_1->remove(i);
					continue;
				}
			}
		}
		for (size_t i = 0; i < _pterm_2->size(); i++) {
			if (_pterm_2->get(i)->is(ComplementSign, term_type::operation, 1)) {
				if (_pterm_2->get(i)->get(0)->is(UnionSign, term_type::operation)) {
					FTerm = make_shared<Term>(IntersectionSign, term_type::operation);
					for (size_t j = 0; j < _pterm_2->get(i)->get(0)->size(); j++) {
						complTerm = make_shared<Term>(ComplementSign, term_type::operation);
						complTerm->add(_pterm_2->get(i)->get(0)->get(j));
						FTerm->add(complTerm);
					}
					_pterm_2->add(FTerm);
					_pterm_2->remove(i);
					continue;
				}
			}
		}
		TermTool::ToNormalForm(_pterm_1);
		TermTool::ToNormalForm(_pterm_2);
		return TermTool::Equal(_pterm_1, _pterm_2);
	}
	// Случай 2.
	if (_pterm_1->is(ComplementSign, term_type::operation, 1)) {
		if (_pterm_2->is(IntersectionSign, term_type::operation)) {
			if (!ReleaseCompl(_pterm_1, UnionSign, IntersectionSign)) return false;

			for (size_t i = 0; i < _pterm_2->size(); i++) {
				ReleaseCompl(_pterm_2->get(i), UnionSign, IntersectionSign);
			}
		}
		else if (_pterm_2->is(UnionSign, term_type::operation)) {
			if (!ReleaseCompl(_pterm_1, IntersectionSign, UnionSign)) return false;

			for (size_t i = 0; i < _pterm_2->size(); i++) {
				ReleaseCompl(_pterm_2->get(i), IntersectionSign, UnionSign);
			}
		}
		else return false;

		TermTool::ToNormalForm(_pterm_1);
		TermTool::ToNormalForm(_pterm_2);

		return TermTool::Equal(_pterm_1, _pterm_2);
	}
	// Случай 2.2.
	if (_pterm_2->is(ComplementSign, term_type::operation, 1)) {
		if (_pterm_1->is(IntersectionSign, term_type::operation)) {
			if (!ReleaseCompl(_pterm_2, UnionSign, IntersectionSign)) return false;

			for (size_t i = 0; i < _pterm_1->size(); i++) {
				ReleaseCompl(_pterm_1->get(i), UnionSign, IntersectionSign);
			}
		}
		else if (_pterm_1->is(UnionSign, term_type::operation)) {
			if (!ReleaseCompl(_pterm_2, IntersectionSign, UnionSign)) return false;

			for (size_t i = 0; i < _pterm_1->size(); i++) {
				ReleaseCompl(_pterm_1->get(i), IntersectionSign, UnionSign);
			}
		}
		else return false;

		TermTool::ToNormalForm(_pterm_1);
		TermTool::ToNormalForm(_pterm_2);

		return TermTool::Equal(_pterm_1, _pterm_2);
	}
	return false;
}

bool ReleaseParantheses(term_ptr pterm, char curOperation, char newOperation)
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

bool ____Parantheses(term_ptr pterm_1, term_ptr pterm_2, char topOperation, char bottomOperation)
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
			ReleaseParantheses(pterm_1, topOperation, bottomOperation);
			TermTool::ToNormalForm(pterm_1);
			if (TermTool::Equal(pterm_1, pterm_2)) return true;

			for (size_t i = 0; i < pterm_2->size(); i++) {
				ReleaseParantheses(pterm_2->get(i), topOperation, bottomOperation);
			}
			TermTool::ToNormalForm(pterm_1);
			TermTool::ToNormalForm(pterm_2);
			return TermTool::Equal(pterm_1, pterm_2);
		}
		return false;
	}
	if (pterm_2->is(topOperation, term_type::operation)) {
		if (pterm_1->is(bottomOperation, term_type::operation)) {
			ReleaseParantheses(pterm_2, topOperation, bottomOperation);
			TermTool::ToNormalForm(pterm_2);
			if (TermTool::Equal(pterm_1, pterm_2)) return true;

			for (size_t i = 0; i < pterm_1->size(); i++) {
				ReleaseParantheses(pterm_1->get(i), topOperation, bottomOperation);
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
				ReleaseParantheses(pterm_1->get(i), topOperation, bottomOperation);
			}
			for (size_t i = 0; i < pterm_2->size(); i++) {
				ReleaseParantheses(pterm_2->get(i), topOperation, bottomOperation);
			}
			TermTool::ToNormalForm(pterm_1);
			TermTool::ToNormalForm(pterm_2);
			return TermTool::Equal(pterm_1, pterm_2);
		}
		return false;
	}

	return false;
}

bool ___UnionParantheses(const term_ptr& pterm_1, const term_ptr& pterm_2)
{
	return ____Parantheses(pterm_1, pterm_2, UnionSign, IntersectionSign);
}

bool ___IntersectParantheses(const term_ptr& pterm_1, const term_ptr& pterm_2)
{
	return ____Parantheses(pterm_1, pterm_2, IntersectionSign, UnionSign);
}

bool ___MultiplyParantheses(const term_ptr& pterm_1, const term_ptr& pterm_2)
{
	return ____Parantheses(pterm_1, pterm_2, MultiplicationSign, AdditionSign);
}

int T_DoubleCompl(term_ptr pTerm_1, term_ptr pTerm_2)
{
/*	// Правая: n дополнений к выражению
	// Левая: m дополнений к выражению
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pTerm_1 == nullptr || pTerm_2 == nullptr) return ERR_T_NULL_PTR;
	// ----------------------------------------
	// Подсчитать количество дополнений первого выражения
	// ----------------------------------------
	size_t n = 0;
	while (pTerm_1->is(OS_1, term_type::operation, 1)) {
		pTerm_1 = TrimTop(pTerm_1);
		n++;
	}
	size_t m = 0;
	while (pTerm_2->is(OS_1, term_type::operation, 1)) {
		pTerm_2 = TrimTop(pTerm_2);
		m++;
	}
	// ----------------------------------------
	// Сравнить оставшиеся выражения
	// ----------------------------------------
	if (n == m) return ERR_T_INCORRECT_MARKER;
	if (!Equal(pTerm_1, pTerm_2)) return ERR_T_INCORRECT_MARKER;
	*/
	return ERR_T_NO_ERROR;
}

int T_InterNullset_R1(term_ptr pTerm_1, term_ptr pTerm_2)
{
/*	// Правая: пустое множество
	// Левая: пересечение с пустым множеством
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pTerm_1 == nullptr || pTerm_2 == nullptr) return ERR_T_NULL_PTR;
	// ----------------------------------------
	// Нахождение сторон
	// ----------------------------------------
	term_ptr left, right;
	if (pTerm_1->is(NULLSET, term_type::symbol)) {
		left = pTerm_2;
		right = pTerm_1;
	}
	else {
		if (!pTerm_2->is(NULLSET, term_type::symbol)) return ERR_T_NOT_FOUND_RIGHT;
		left = pTerm_1;
		right = pTerm_2;
	}
	// ----------------------------------------
	// Проверить левую сторону
	// ----------------------------------------
	if (!left->is(OS_2, term_type::operation)) return ERR_T_NOT_FOUND_LEFT;
	bool isFound = false;
	for (size_t i = 0; i < left->size(); i++)
		if (left->get(i)->is(NULLSET, term_type::symbol)) {
			isFound = true;
			break;
		}
	if (!isFound) return ERR_T_NOT_FOUND_LEFT;
	*/
	return ERR_T_NO_ERROR;
}

int T_InterNullset_R2(term_ptr pTerm_1, term_ptr pTerm_2)
{
/*	// Правая: ноль
	// Левая: мощность пересечения с пустым множеством
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pTerm_1 == nullptr || pTerm_2 == nullptr) return ERR_T_NULL_PTR;
	// ----------------------------------------
	// Нахождение сторон
	// ----------------------------------------
	term_ptr left, right;
	if (pTerm_1->is(0, term_type::number)) {
		left = pTerm_2;
		right = pTerm_1;
	}
	else {
		if (!pTerm_2->is(0, term_type::number)) return ERR_T_NOT_FOUND_RIGHT;
		left = pTerm_1;
		right = pTerm_2;
	}
	// ----------------------------------------
	// Проверить левую сторону
	// ----------------------------------------
	if (!left->is(OB__, term_type::operation)) return ERR_T_NOT_FOUND_LEFT;
	if (!left->get(0)->is(OS_2, term_type::operation)) return ERR_T_NOT_FOUND_LEFT;
	bool isFound = false;
	for (size_t i = 0; i < left->size(); i++)
		if (left->get(0)->get(i)->is(NULLSET, term_type::symbol)) {
			isFound = true;
			break;
		}
	if (!isFound) return ERR_T_NOT_FOUND_LEFT;
	*/
	return ERR_T_NO_ERROR;
}

int T_InterNullset_R3M(term_ptr pTerm_1, term_ptr pTerm_2)
{
/*	// Правая: какое-то выражение
	// Левая: сложение какого-то выражения с мощностью пересечения с пустым множеством
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pTerm_1 == nullptr || pTerm_2 == nullptr) return ERR_T_NULL_PTR;
	// ----------------------------------------
	// Нахождение количеств мощностей пересечения с пустым множеством
	// в каждой стороне и удаление их
	// ----------------------------------------
	size_t amountFirstExp = 0, amountSecondExp = 0;
	if (pTerm_1->is(OA_2, term_type::operation)) {
		for (int i = pTerm_1->size() - 1; i >= 0; i--) {
			if (!pTerm_1->get(i)->is(OB__, term_type::operation, 1)) continue;
			if (!pTerm_1->get(i)->get(0)->is(OS_2, term_type::operation, 1)) continue;
			if (!pTerm_1->get(i)->get(0)->get(0)->is(NULLSET, term_type::symbol)) continue;
			pTerm_1->remove(i);
			amountFirstExp++;
		}
		if (pTerm_1->size() == 1) pTerm_1 = TrimTop(pTerm_1);
	}
	if (pTerm_2->is(OA_2, term_type::operation)) {
		for (int i = pTerm_2->size() - 1; i >= 0; i--) {
			if (!pTerm_2->get(i)->is(OB__, term_type::operation, 1)) continue;
			if (!pTerm_2->get(i)->get(0)->is(OS_2, term_type::operation, 1)) continue;
			if (!pTerm_2->get(i)->get(0)->get(0)->is(NULLSET, term_type::symbol)) continue;
			pTerm_2->remove(i);
			amountSecondExp++;
		}
		if (pTerm_2->size() == 1) pTerm_2 = TrimTop(pTerm_2);
	}
	// ----------------------------------------
	// Проверить равенство оставшихся частей
	// ----------------------------------------
	if (amountFirstExp == 0 && amountSecondExp == 0) return ERR_T_INCORRECT_MARKER;
	if (amountFirstExp == amountSecondExp) return ERR_T_INCORRECT_MARKER;
	if (!Equal(pTerm_1, pTerm_2)) return ERR_T_INCORRECT_MARKER;*/
	return ERR_T_NO_ERROR;
}

int T_InterNullset_R4(term_ptr pTerm_1, term_ptr pTerm_2)
{
/*	// Правая: ноль
	// Левая: минус мощность пересечения с пустым множеством
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pTerm_1 == nullptr || pTerm_2 == nullptr) return ERR_T_NULL_PTR;
	// ----------------------------------------
	// Нахождение сторон
	// ----------------------------------------
	term_ptr left, right;
	if (pTerm_1->is(0, term_type::number)) {
		left = pTerm_2;
		right = pTerm_1;
	}
	else {
		if (!pTerm_2->is(0, term_type::number)) return ERR_T_NOT_FOUND_RIGHT;
		left = pTerm_1;
		right = pTerm_2;
	}
	// ----------------------------------------
	// Проверить левую сторону
	// ----------------------------------------
	if (!left->is(OA_1, term_type::operation, 1)) return ERR_T_NOT_FOUND_LEFT;
	if (!left->get(0)->is(OB__, term_type::operation, 1)) return ERR_T_NOT_FOUND_LEFT;
	if (!left->get(0)->get(0)->is(OS_2, term_type::operation)) return ERR_T_NOT_FOUND_LEFT;
	bool isFound = false;
	for (size_t i = 0; i < left->size(); i++)
		if (left->get(0)->get(0)->get(i)->is(NULLSET, term_type::symbol)) {
			isFound = true;
			break;
		}
	if (!isFound) return ERR_T_NOT_FOUND_LEFT;
	*/
	return ERR_T_NO_ERROR;
}

int T_InterNullset_R5(term_ptr pTerm_1, term_ptr pTerm_2)
{
/*	// Правая: какое-то выражение
	// Левая: сложение какого-то выражения с минус мощностью пересечения с пустым множеством
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pTerm_1 == nullptr || pTerm_2 == nullptr) return ERR_T_NULL_PTR;
	// ----------------------------------------
	// Нахождение количеств минус мощностей пересечения с пустым множеством
	// в каждой стороне и удаление их
	// ----------------------------------------
	size_t amountFirstExp = 0, amountSecondExp = 0;
	if (pTerm_1->is(OA_2, term_type::operation)) {
		for (int i = pTerm_1->size() - 1; i >= 0; i--) {
			if (!pTerm_1->get(i)->is(OA_1, term_type::operation, 1)) continue;
			if (!pTerm_1->get(i)->get(0)->is(OB__, term_type::operation, 1)) continue;
			if (!pTerm_1->get(i)->get(0)->get(0)->is(OS_2, term_type::operation, 1)) continue;
			if (!pTerm_1->get(i)->get(0)->get(0)->get(0)->is(NULLSET, term_type::symbol)) continue;
			pTerm_1->remove(i);
			amountFirstExp++;
		}
		if (pTerm_1->size() == 1) pTerm_1 = TrimTop(pTerm_1);
	}
	if (pTerm_2->is(OA_2, term_type::operation)) {
		for (int i = pTerm_2->size() - 1; i >= 0; i--) {
			if (!pTerm_2->get(i)->is(OA_1, term_type::operation, 1)) continue;
			if (!pTerm_2->get(i)->get(0)->is(OB__, term_type::operation, 1)) continue;
			if (!pTerm_2->get(i)->get(0)->get(0)->is(OS_2, term_type::operation, 1)) continue;
			if (!pTerm_2->get(i)->get(0)->get(0)->get(0)->is(NULLSET, term_type::symbol)) continue;
			pTerm_2->remove(i);
			amountSecondExp++;
		}
		if (pTerm_2->size() == 1) pTerm_2 = TrimTop(pTerm_2);
	}
	// ----------------------------------------
	// Проверить равенство оставшихся частей
	// ----------------------------------------
	if (amountFirstExp == 0 && amountSecondExp == 0) return ERR_T_INCORRECT_MARKER;
	if (amountFirstExp == amountSecondExp) return ERR_T_INCORRECT_MARKER;
	if (!Equal(pTerm_1, pTerm_2)) return ERR_T_INCORRECT_MARKER;*/
	return ERR_T_NO_ERROR;
}

int T_UnionNullset(term_ptr pTerm_1, term_ptr pTerm_2)
{
/*	// Правая: какое-то выражение
	// Левая: объединение какого-то выражения с пустым множеством
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pTerm_1 == nullptr || pTerm_2 == nullptr) return ERR_T_NULL_PTR;
	// ----------------------------------------
	// Нахождение количеств пустых множеств
	// в каждой стороне и удаление их
	// ----------------------------------------
	size_t amountFirstExp = 0, amountSecondExp = 0;
	if (pTerm_1->is(OS_3, term_type::operation)) {
		for (int i = pTerm_1->size() - 1; i >= 0; i--) {
			if (!pTerm_1->get(i)->is(NULLSET, term_type::symbol)) continue;
			pTerm_1->remove(i);
			amountFirstExp++;
		}
		if (pTerm_1->size() == 1) pTerm_1 = TrimTop(pTerm_1);
	}
	if (pTerm_2->is(OS_3, term_type::operation)) {
		for (int i = pTerm_2->size() - 1; i >= 0; i--) {
			if (!pTerm_2->get(i)->is(NULLSET, term_type::symbol)) continue;
			pTerm_2->remove(i);
			amountSecondExp++;
		}
		if (pTerm_2->size() == 1) pTerm_2 = TrimTop(pTerm_2);
	}
	// ----------------------------------------
	// Проверить равенство оставшихся частей
	// ----------------------------------------
	if (amountFirstExp == 0 && amountSecondExp == 0) return ERR_T_INCORRECT_MARKER;
	if (amountFirstExp == amountSecondExp) return ERR_T_INCORRECT_MARKER;
	if (!Equal(pTerm_1, pTerm_2)) return ERR_T_INCORRECT_MARKER;*/
	return ERR_T_NO_ERROR;
}

int T_InterUniset(term_ptr pTerm_1, term_ptr pTerm_2)
{
/*	// Правая: какое-то выражение
	// Левая: пересечение какого-то выражения с универсальным множеством
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pTerm_1 == nullptr || pTerm_2 == nullptr) return ERR_T_NULL_PTR;
	// ----------------------------------------
	// Нахождение количеств универсальных множеств множеств
	// в каждой стороне и удаление их
	// ----------------------------------------
	size_t amountFirstExp = 0, amountSecondExp = 0;
	if (pTerm_1->is(OS_3, term_type::operation)) {
		for (int i = pTerm_1->size() - 1; i >= 0; i--) {
			if (!pTerm_1->get(i)->is(UNISET, term_type::symbol)) continue;
			pTerm_1->remove(i);
			amountFirstExp++;
		}
	}
	if (pTerm_1->size() == 1) pTerm_1 = TrimTop(pTerm_1);
	if (pTerm_2->is(OS_3, term_type::operation)) {
		for (int i = pTerm_2->size() - 1; i >= 0; i--) {
			if (!pTerm_2->get(i)->is(UNISET, term_type::symbol)) continue;
			pTerm_2->remove(i);
			amountSecondExp++;
		}
	}
	if (pTerm_2->size() == 1) pTerm_2 = TrimTop(pTerm_2);
	// ----------------------------------------
	// Проверить равенство оставшихся частей
	// ----------------------------------------
	if (amountFirstExp == 0 || amountSecondExp == 0) return ERR_T_INCORRECT_MARKER;
	if (amountFirstExp == amountSecondExp) return ERR_T_INCORRECT_MARKER;
	if (!Equal(pTerm_1, pTerm_2)) return ERR_T_INCORRECT_MARKER;*/
	return ERR_T_NO_ERROR;
}

int T_UnionUniset(term_ptr pTerm_1, term_ptr pTerm_2)
{
/*	// Правая: универсальное множество
	// Левая: объединение с универсальным множеством
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pTerm_1 == nullptr || pTerm_2 == nullptr) return ERR_T_NULL_PTR;
	// ----------------------------------------
	// Нахождение сторон
	// ----------------------------------------
	term_ptr left, right;
	if (pTerm_1->is(UNISET, term_type::symbol)) {
		left = pTerm_2;
		right = pTerm_1;
	}
	else {
		if (!pTerm_2->is(UNISET, term_type::symbol)) return ERR_T_NOT_FOUND_RIGHT;
		left = pTerm_1;
		right = pTerm_2;
	}
	// ----------------------------------------
	// Проверить левую сторону
	// ----------------------------------------
	if (!left->is(OS_3, term_type::operation)) return ERR_T_NOT_FOUND_LEFT;
	bool isFound = false;
	for (size_t i = 0; i < left->size(); i++)
		if (left->get(i)->is(NULLSET, term_type::symbol)) {
			isFound = true;
			break;
		}
	if (!isFound) return ERR_T_NOT_FOUND_LEFT;
	*/
	return ERR_T_NO_ERROR;
}

bool ___InterExpAndExp(const term_ptr& pterm_1, const term_ptr& pterm_2)
{
	// Правая: выражение
	// Левая: пересечение одинаковых выражений
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pterm_1 == nullptr || pterm_2 == nullptr) return false;
	// ----------------------------------------
	// Нахождение количеств пересечений одинаковых множеств
	// в каждой стороне и удаление их
	// ----------------------------------------
	bool isFound = false;
	size_t amountFirstExp = 0, amountSecondExp = 0;
	term_ptr tempTerm_1 = make_shared<Term>(IntersectionSign, term_type::operation);
	term_ptr tempTerm_2 = make_shared<Term>(IntersectionSign, term_type::operation);
	if (pterm_1->is(IntersectionSign, term_type::operation)) {
		for (size_t i = 0; i < pterm_1->size(); i++) {
			isFound = false;
			for (size_t j = 0; j < tempTerm_1->size(); j++) {
				if (TermTool::Equal(pterm_1->get(i), tempTerm_1->get(j))) {
					amountFirstExp++; 
					isFound = true;
					break;
				}
			}
			if (!isFound) tempTerm_1->add(pterm_1->get(i));
		}
		if (tempTerm_1->size() == 1) tempTerm_1 = TrimTop(tempTerm_1);
	}
	if (pterm_2->is(IntersectionSign, term_type::operation)) {
		for (size_t i = 0; i < pterm_2->size(); i++) {
			isFound = false;
			for (size_t j = 0; j < tempTerm_2->size(); j++) {
				if (TermTool::Equal(pterm_2->get(i), tempTerm_2->get(j))) {
					amountSecondExp++;
					isFound = true;
					break;
				}
			}
			if (!isFound) tempTerm_2->add(pterm_2->get(i));
		}
		if (tempTerm_2->size() == 1) tempTerm_2 = TrimTop(tempTerm_2);
	}
	// ----------------------------------------
	// Проверить равенство оставшихся частей
	// ----------------------------------------
	if (amountFirstExp == 0 && amountSecondExp == 0) return false;
	if (amountFirstExp == amountSecondExp) return false;
	return Equal(tempTerm_1, tempTerm_2);
}

bool ___UnionExpAndExp(const term_ptr& pterm_1, const term_ptr& pterm_2)
{
	// Правая: выражение
	// Левая: объединение одинаковых выражений
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pterm_1 == nullptr || pterm_2 == nullptr) return false;
	// ----------------------------------------
	// Нахождение количеств объединений одинаковых множеств
	// в каждой стороне и удаление их
	// ----------------------------------------
	bool isFound = false;
	size_t amountFirstExp = 0, amountSecondExp = 0;
	term_ptr tempTerm_1 = make_shared<Term>(UnionSign, term_type::operation);
	term_ptr tempTerm_2 = make_shared<Term>(UnionSign, term_type::operation);
	if (pterm_1->is(UnionSign, term_type::operation)) {
		for (size_t i = 0; i < pterm_1->size(); i++) {
			isFound = false;
			for (size_t j = 0; j < tempTerm_1->size(); j++) {
				if (Equal(pterm_1->get(i), tempTerm_1->get(j))) {
					amountFirstExp++;
					isFound = true;
					break;
				}
			}
			if (!isFound) tempTerm_1->add(pterm_1->get(i));
		}
		if (tempTerm_1->size() == 1) tempTerm_1 = TrimTop(tempTerm_1);
	}
	if (pterm_2->is(UnionSign, term_type::operation)) {
		for (size_t i = 0; i < pterm_2->size(); i++) {
			isFound = false;
			for (size_t j = 0; j < tempTerm_2->size(); j++) {
				if (Equal(pterm_2->get(i), tempTerm_2->get(j))) {
					amountSecondExp++;
					isFound = true;
					break;
				}
			}
			if (!isFound) tempTerm_2->add(pterm_2->get(i));
		}
		if (tempTerm_2->size() == 1) tempTerm_2 = TrimTop(tempTerm_2);
	}
	// ----------------------------------------
	// Проверить равенство оставшихся частей
	// ----------------------------------------
	if (amountFirstExp == 0 && amountSecondExp == 0) return false;
	if (amountFirstExp == amountSecondExp) return false;
	return Equal(tempTerm_1, tempTerm_2);
}

int T_InterExpAndComplExp_R1(term_ptr pTerm_1, term_ptr pTerm_2)
{
	return 0;
}

bool T_IsLeft_InterExpAndComplExp(term_ptr left)
{
/*	if (!left->is(OS_2, term_type::operation)) return false;

	term_ptr exp, complExp;
	if (left->size() == 2)
	{
		// Найти самую высокую ветвь

		if (GetDeep(left->get(0)) > GetDeep(left->get(1))) {
			complExp = left->get(0);
			exp = left->get(1);
		}
		else {
			complExp = left->get(1);
			exp = left->get(0);
		}
		if (!complExp->is(OS_1, term_type::operation, 1)) return false;
		if (!Equal(complExp->get(0), exp)) return false;
	}
	else
	{
		int pMaxDeep = GetMaxDeep(left);
		complExp = left->get(pMaxDeep);
		exp = make_shared<Term>(left);
		exp->remove(pMaxDeep);
		if (!complExp->is(OS_1, term_type::operation, 1)) return false;
		if (!Equal(complExp->get(0), exp)) return false;
	}
	*/
	return true;
}

int ___InterExpAndComplExp_R1(term_ptr pTerm_1, term_ptr pTerm_2)
{
  
	return ERR_T_NO_ERROR;
}

int T_InterExpAndComplExp_R2(term_ptr pTerm_1, term_ptr pTerm_2)
{
/*	// Правая: ноль
	// Левая: мощность пересечение дополнения выражения и выражения
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pTerm_1 == nullptr || pTerm_2 == nullptr) return ERR_T_NULL_PTR;
	// ----------------------------------------
	// Нахождение сторон
	// ----------------------------------------
	term_ptr left, right;
	if (pTerm_1->is(0, term_type::number)) {
		left = pTerm_2;
		right = pTerm_1;
	}
	else {
		if (!pTerm_2->is(0, term_type::number)) return ERR_T_NOT_FOUND_RIGHT;
		left = pTerm_1;
		right = pTerm_2;
	}
	// ----------------------------------------
	// Проверка левой стороны
	// ----------------------------------------
	if (!left->is(OB__, term_type::operation)) return ERR_T_NOT_FOUND_LEFT;
	if (!T_IsLeft_InterExpAndComplExp(left->get(0))) return ERR_T_NOT_FOUND_LEFT;
	*/
	return ERR_T_NO_ERROR;
}

int T_InterExpAndComplExp_R3M(term_ptr pTerm_1, term_ptr pTerm_2)
{
	// Правая: какое-то выражение
	// Левая: сложение какого-то выражения с мощностью пересечения дополнения выражения и выражения
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pTerm_1 == nullptr || pTerm_2 == nullptr) return ERR_T_NULL_PTR;
	// ----------------------------------------
	// Нахождение количеств мощностей пересечения дополнения выражения и выражения
	// в каждой стороне и удаление их
	// ----------------------------------------
	size_t amountFirstExp = 0, amountSecondExp = 0;
	if (pTerm_1->is(OA_2, term_type::operation)) {
		for (int i = pTerm_1->size() - 1; i >= 0; i--) {
			if (!pTerm_1->get(i)->is(OB__, term_type::operation, 1)) continue;
			if (!T_IsLeft_InterExpAndComplExp(pTerm_1->get(0)->get(i))) continue;
			pTerm_1->remove(i);
			amountFirstExp++;
		}
		if (pTerm_1->size() == 1) pTerm_1 = TrimTop(pTerm_1);
	}
	if (pTerm_2->is(OA_2, term_type::operation)) {
		for (int i = pTerm_2->size() - 1; i >= 0; i--) {
			if (!pTerm_2->get(i)->is(OB__, term_type::operation, 1)) continue;
			if (!T_IsLeft_InterExpAndComplExp(pTerm_2->get(0)->get(i))) continue;
			pTerm_2->remove(i);
			amountSecondExp++;
		}
		if (pTerm_2->size() == 1) pTerm_2 = TrimTop(pTerm_2);
	}
	// ----------------------------------------
	// Проверить равенство оставшихся частей
	// ----------------------------------------
	if (amountFirstExp == 0 && amountSecondExp == 0) return ERR_T_INCORRECT_MARKER;
	if (amountFirstExp == amountSecondExp) return ERR_T_INCORRECT_MARKER;
	if (!Equal(pTerm_1, pTerm_2)) return ERR_T_INCORRECT_MARKER;
	return ERR_T_NO_ERROR;
}

int T_InterExpAndComplExp_R4(term_ptr pTerm_1, term_ptr pTerm_2)
{
	// Правая: ноль
	// Левая: минус мощность пересечение дополнения выражения и выражения
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pTerm_1 == nullptr || pTerm_2 == nullptr) return ERR_T_NULL_PTR;
	// ----------------------------------------
	// Нахождение сторон
	// ----------------------------------------
	term_ptr left, right;
	if (pTerm_1->is(0, term_type::number)) {
		left = pTerm_2;
		right = pTerm_1;
	}
	else {
		if (!pTerm_2->is(0, term_type::number)) return ERR_T_NOT_FOUND_RIGHT;
		left = pTerm_1;
		right = pTerm_2;
	}
	// ----------------------------------------
	// Проверка левой стороны
	// ----------------------------------------
	if (!left->is(OA_1, term_type::operation)) return ERR_T_NOT_FOUND_LEFT;
	if (!left->get(0)->is(OB__, term_type::operation)) return ERR_T_NOT_FOUND_LEFT;
	if (!T_IsLeft_InterExpAndComplExp(left->get(0)->get(0))) return ERR_T_NOT_FOUND_LEFT;

	return ERR_T_NO_ERROR;
}

int T_InterExpAndComplExp_R5(term_ptr pTerm_1, term_ptr pTerm_2)
{
	// Правая: какое-то выражение
	// Левая: сложение какого-то выражения с минус мощностью пересечения дополнения выражения и выражения
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pTerm_1 == nullptr || pTerm_2 == nullptr) return ERR_T_NULL_PTR;
	// ----------------------------------------
	// Нахождение количеств минус мощностей пересечения дополнения выражения и выражения
	// в каждой стороне и удаление их
	// ----------------------------------------
	size_t amountFirstExp = 0, amountSecondExp = 0;
	if (pTerm_1->is(OA_2, term_type::operation)) {
		for (int i = pTerm_1->size() - 1; i >= 0; i--) {
			if (!pTerm_1->get(i)->is(OA_1, term_type::operation, 1)) continue;
			if (!pTerm_1->get(0)->get(i)->is(OB__, term_type::operation, 1)) continue;
			if (!T_IsLeft_InterExpAndComplExp(pTerm_1->get(0)->get(i))) continue;
			pTerm_1->remove(i);
			amountFirstExp++;
		}
		if (pTerm_1->size() == 1) pTerm_1 = TrimTop(pTerm_1);
	}
	if (pTerm_2->is(OA_2, term_type::operation)) {
		for (int i = pTerm_2->size() - 1; i >= 0; i--) {
			if (!pTerm_2->get(i)->is(OA_1, term_type::operation, 1)) continue;
			if (!pTerm_2->get(0)->get(i)->is(OB__, term_type::operation, 1)) continue;
			if (!T_IsLeft_InterExpAndComplExp(pTerm_2->get(0)->get(i))) continue;
			pTerm_2->remove(i);
			amountSecondExp++;
		}
		if (pTerm_2->size() == 1) pTerm_2 = TrimTop(pTerm_2);
	}
	// ----------------------------------------
	// Проверить равенство оставшихся частей
	// ----------------------------------------
	if (amountFirstExp == 0 && amountSecondExp == 0) return ERR_T_INCORRECT_MARKER;
	if (amountFirstExp == amountSecondExp) return ERR_T_INCORRECT_MARKER;
	if (!Equal(pTerm_1, pTerm_2)) return ERR_T_INCORRECT_MARKER;
	return ERR_T_NO_ERROR;
}

int T_UnionExpAndComplExp(term_ptr pTerm_1, term_ptr pTerm_2)
{
	// Правая: универсальное множество
	// Левая: объединение дополнения выражения и выражения
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pTerm_1 == nullptr || pTerm_2 == nullptr) return ERR_T_NULL_PTR;
	// ----------------------------------------
	// Нахождение сторон
	// ----------------------------------------
	term_ptr left, right;
	if (pTerm_1->is(UNISET, term_type::symbol)) {
		left = pTerm_2;
		right = pTerm_1;
	}
	else {
		if (!pTerm_2->is(UNISET, term_type::symbol)) return ERR_T_NOT_FOUND_RIGHT;
		left = pTerm_1;
		right = pTerm_2;
	}
	// ----------------------------------------
	// Проверка левой стороны
	// ----------------------------------------
	if (!left->is(OS_3, term_type::operation)) return ERR_T_NOT_FOUND_LEFT;

	int pMaxDeep = -1;
	term_ptr tempTerm = nullptr;

	if (left->size() == 2)
	{
		// Найти самую высокую ветвь
		term_ptr exp, complExp;
		if (GetDeep(left->get(0)) > GetDeep(left->get(1))) {
			complExp = left->get(0);
			exp = left->get(1);
		}
		else {
			complExp = left->get(1);
			exp = left->get(0);
		}
		if (!complExp->is(OS_1, term_type::operation, 1)) return ERR_T_NOT_FOUND_LEFT;
		if (!Equal(complExp->get(0), exp)) return ERR_T_NOT_FOUND_LEFT;
	}
	else
	{
		pMaxDeep = GetMaxDeep(left);
		tempTerm = left->get(pMaxDeep);
		left->remove(pMaxDeep);
		if (!tempTerm->is(OS_1, term_type::operation, 1)) return ERR_T_NOT_FOUND_LEFT;
		if (!Equal(tempTerm->get(0), left)) return ERR_T_NOT_FOUND_LEFT;
	}

	return ERR_T_NO_ERROR;
}

bool ___DiffUnisetAndExp(const term_ptr& pterm_1, const term_ptr& pterm_2)
{
	// Правая: разность универсума и множества#1
	// Левая: дополнение множества#1
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pterm_1 == nullptr || pterm_2 == nullptr) return false;
	// ----------------------------------------
	// Нахождение сторон
	// ----------------------------------------
	term_ptr left, right;
	if (pterm_1->is(DifferenceSign, term_type::operation, 2)) {
		left = pterm_2;
		right = pterm_1;
	}
	else {
		if (!pterm_2->is(DifferenceSign, term_type::operation, 2)) return false;
		left = pterm_1;
		right = pterm_2;
	}
	// ----------------------------------------
	// Основная проверка
	// ----------------------------------------
	if (!left->is(ComplementSign, term_type::operation, 1)) return false;
	if (!right->get(0)->is(UniversalSet, term_type::symbol)) return false;
	return Equal(right->get(1), left->get(0));
}

int T_DiffExpAndExp(term_ptr pTerm_1, term_ptr pTerm_2)
{
	// Правая: разность множества#1 и множества#2
	// Левая: пересечение множества#1 с дополнением множества#2
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pTerm_1 == nullptr || pTerm_2 == nullptr) return ERR_T_NULL_PTR;
	// ----------------------------------------
	// Нахождение сторон
	// ----------------------------------------
	term_ptr left, right;
	if (pTerm_1->is(OS_4, term_type::operation, 2)) {
		left = pTerm_2;
		right = pTerm_1;
	}
	else {
		if (!pTerm_2->is(OS_4, term_type::operation, 2)) return ERR_T_NOT_FOUND_RIGHT;
		left = pTerm_1;
		right = pTerm_2;
	}
	// ----------------------------------------
	// Проверка левой стороны
	// ----------------------------------------
	if (!left->is(OS_2, term_type::operation)) return ERR_T_NOT_FOUND_LEFT;
	if (!Equal(right->get(0), left->get(0))) return ERR_T_NOT_FOUND_LEFT;
	if (!left->get(1)->is(OS_1, term_type::operation, 1)) return ERR_T_NOT_FOUND_LEFT;
	if (!Equal(right->get(1), left->get(1)->get(0))) return ERR_T_NOT_FOUND_LEFT;
	return ERR_T_NO_ERROR;
}

bool ___ExcIncFormulaX2(const term_ptr& pterm_1, const term_ptr& pterm_2)
{
	// Правая: -
	// Левая: -
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pterm_1 == nullptr || pterm_2 == nullptr) return false;
	// ----------------------------------------
	// Нахождение сторон
	// ----------------------------------------
	term_ptr left, right;
	if (pterm_1->is(AdditionSign, term_type::operation)) {
		left = pterm_2;
		right = pterm_1;
	}
	else {
		if (!pterm_2->is(AdditionSign, term_type::operation)) return false;
		left = pterm_1;
		right = pterm_2;
	}
	// ----------------------------------------
	// Основная проверка
	// ----------------------------------------
	if (!left->is(ModuleSign, term_type::operation, 1)) return false;
	if (!left->get(0)->is(UnionSign, term_type::operation)) return false;

	if (right->size() != 3) return false;

	term_ptr tempInterTerm = shared_ptr<Term>(new Term(IntersectionSign, term_type::operation));
	term_ptr tempUnionTerm = shared_ptr<Term>(new Term(UnionSign, term_type::operation));

	int interIndex = -1;

	for (size_t i = 0; i < right->size(); i++)
	{
		if (right->get(i)->is(SubtractionSign, term_type::operation, 1)) {
			if (!right->get(i)->get(0)->is(ModuleSign, term_type::operation, 1)) return false;
			interIndex = i;
			continue;
		}
		if (right->get(i)->is(ModuleSign, term_type::operation, 1)) {
			tempInterTerm->add(right->get(i)->get(0));
			tempUnionTerm->add(right->get(i)->get(0));
			continue;
		}
		return false;
	}
	if (interIndex == -1) return false;

	TermTool::ToNormalForm(tempInterTerm);
	TermTool::ToNormalForm(tempUnionTerm);

	if (!TermTool::Equal(tempInterTerm, right->get(interIndex)->get(0)->get(0))) return false;
	if (!TermTool::Equal(tempUnionTerm, left->get(0))) return false;

	return true;
}

bool ___ExcIncFormulaX3(const term_ptr& pterm_1, const term_ptr& pterm_2)
{
	// Правая: -
	// Левая: -
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pterm_1 == nullptr || pterm_2 == nullptr) return false;
	// ----------------------------------------
	// Нахождение сторон
	// ----------------------------------------
	term_ptr left, right;
	if (pterm_1->is(AdditionSign, term_type::operation)) {
		left = pterm_2;
		right = pterm_1;
	}
	else {
		if (!pterm_2->is(AdditionSign, term_type::operation)) return false;
		left = pterm_1;
		right = pterm_2;
	}
	// ----------------------------------------
	// Основная проверка
	// ----------------------------------------
	if (!left->is(ModuleSign, term_type::operation, 1)) return false;
	if (!left->get(0)->is(UnionSign, term_type::operation)) return false;

	if (right->size() != 7) return false;

	int interIndex = -1;
	vector<term_ptr> terms;
	vector<term_ptr> doubleTerms;

	for (size_t i = 0; i < right->size(); i++) {
		if (right->get(i)->is(SubtractionSign, term_type::operation, 1)) {
			if (!right->get(i)->get(0)->is(ModuleSign, term_type::operation, 1)) return false;
			if (!right->get(i)->get(0)->get(0)->is(IntersectionSign, term_type::operation)) return false;
			doubleTerms.push_back(right->get(i)->get(0)->get(0));
			continue;
		}
		if (right->get(i)->is(ModuleSign, term_type::operation, 1)) {
			if (!right->get(i)->get(0)->is(IntersectionSign, term_type::operation)) {
				terms.push_back(right->get(i)->get(0));
				continue;
			}
			if (interIndex == -1) {
				interIndex = i;
				continue;
			}
			if (right->get(i)->get(0)->size() > right->get(interIndex)->get(0)->size()) {
				terms.push_back(right->get(interIndex)->get(0));
				interIndex = i;
				continue;
			}
			terms.push_back(right->get(i)->get(0));
			continue;
		}
		return false;
	}
	if (interIndex == -1) return false;
	if (terms.size() != 3) return false;
	if (doubleTerms.size() != 3) return false;

	bool isFound = false;
	term_ptr tempTerm = nullptr;
	// ----------------------------------------
	// Генерация объединения
	// ----------------------------------------
	tempTerm = make_shared<Term>(UnionSign, term_type::operation);
	tempTerm->add(terms[0]);
	tempTerm->add(terms[1]);
	tempTerm->add(terms[2]);
	TermTool::ToNormalForm(tempTerm);
	if (!Equal(left->get(0), tempTerm)) return false;
	// ----------------------------------------
	// Генерация пересечения 1
	// ----------------------------------------
	tempTerm->set(IntersectionSign, term_type::operation);
	tempTerm->remove_all();
	tempTerm->add(terms[0]);
	tempTerm->add(terms[1]);
	TermTool::ToNormalForm(tempTerm);
	isFound = false;
	for (size_t i = 0; i < doubleTerms.size(); i++)
	{
		if (TermTool::Equal(doubleTerms[i], tempTerm)) {
			doubleTerms.erase(doubleTerms.begin() + i);
			isFound = true;
			break;
		}
	}
	if (!isFound) return false;
	// ----------------------------------------
	// Генерация пересечения 2
	// ----------------------------------------
	tempTerm->remove_all();
	tempTerm->add(terms[0]);
	tempTerm->add(terms[2]);
	TermTool::ToNormalForm(tempTerm);
	isFound = false;
	for (size_t i = 0; i < doubleTerms.size(); i++)
	{
		if (TermTool::Equal(doubleTerms[i], tempTerm)) {
			doubleTerms.erase(doubleTerms.begin() + i);
			isFound = true;
			break;
		}
	}
	if (!isFound) return false;
	// ----------------------------------------
	// Генерация пересечения 3
	// ----------------------------------------
	tempTerm->remove_all();
	tempTerm->add(terms[1]);
	tempTerm->add(terms[2]);
	TermTool::ToNormalForm(tempTerm);
	isFound = false;
	for (size_t i = 0; i < doubleTerms.size(); i++)
	{
		if (TermTool::Equal(doubleTerms[i], tempTerm)) {
			doubleTerms.erase(doubleTerms.begin() + i);
			isFound = true;
			break;
		}
	}
	if (!isFound) return false;
	// ----------------------------------------
	// Генерация пересечения 3
	// ----------------------------------------
	tempTerm->remove_all();
	tempTerm->add(terms[0]);
	tempTerm->add(terms[1]);
	tempTerm->add(terms[2]);
	TermTool::ToNormalForm(tempTerm);
	if (!Equal(right->get(interIndex)->get(0), tempTerm)) return false;

	return true;
}

int T_DoubleNeg(term_ptr pTerm_1, term_ptr pTerm_2)
{
	// Правая: n минусов выражения
	// Левая: m минусов выражения
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pTerm_1 == nullptr || pTerm_2 == nullptr) return ERR_T_NULL_PTR;
	// ----------------------------------------
	// Подсчитать количество дополнений первого выражения
	// ----------------------------------------
	size_t n = 0;
	while (pTerm_1->is(OA_1, term_type::operation, 1)) {
		pTerm_1 = TrimTop(pTerm_1);
		n++;
	}
	size_t m = 0;
	while (pTerm_2->is(OA_1, term_type::operation, 1)) {
		pTerm_2 = TrimTop(pTerm_2);
		m++;
	}
	// ----------------------------------------
	// Сравнить оставшиеся выражения
	// ----------------------------------------
	if (n == m) return ERR_T_INCORRECT_MARKER;
	if (!Equal(pTerm_1, pTerm_2)) return ERR_T_INCORRECT_MARKER;

	return ERR_T_NO_ERROR;
}

int T_NegExp(term_ptr pTerm_1, term_ptr pTerm_2)
{
	// Правая: минус к сумме выражений
	// Левая: сумма минус выражений
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pTerm_1 == nullptr || pTerm_2 == nullptr) return ERR_T_NULL_PTR;
	// ----------------------------------------
	// Нахождение сторон
	// ----------------------------------------
	term_ptr left, right;
	if (pTerm_1->is(OA_1, term_type::operation, 1)) {
		left = pTerm_2;
		right = pTerm_1;
	}
	else {
		if (!pTerm_2->is(OA_1, term_type::operation, 1)) return ERR_T_NOT_FOUND_RIGHT;
		left = pTerm_1;
		right = pTerm_2;
	}
	// ----------------------------------------
	// Проверка левой стороны, нахождение операций
	// ----------------------------------------
	if (!right->get(0)->is(OA_2, term_type::operation)) return ERR_T_NOT_FOUND_RIGHT;

	if (!left->is(OA_2, term_type::operation)) return ERR_T_NOT_FOUND_LEFT;

	if (left->size() != right->get(0)->size()) return ERR_T_NOT_FOUND_LEFT;
	bool isFound = false;
	for (size_t i = left->size() - 1; i >= 0; i--)
	{
		if (!left->get(i)->is(OA_1, term_type::operation, 1)) return ERR_T_NOT_FOUND_LEFT;
		isFound = false;
		for (size_t j = 0; j < right->get(0)->size(); j++) {
			if (Equal(right->get(0)->get(j), left->get(i))) {
				left->remove(i);
				right->remove(j);
				isFound = true;
				break;
			}
		}
		if (!isFound) return ERR_T_NOT_FOUND_LEFT;
	}
	return ERR_T_NO_ERROR;
}

bool T_IsLeft_MultiplyZero(term_ptr left)
{
	if (!left->is(OA_3, term_type::operation)) return false;
	bool isFound = false;
	for (size_t i = 0; i < left->size(); i++)
		if (left->get(i)->is(0, term_type::number)) {
			isFound = true;
			break;
		}
	if (!isFound) return false;

	return true;
}

int T_MultiplyZero_R1(term_ptr pTerm_1, term_ptr pTerm_2)
{
	// Правая: ноль
	// Левая: умножение выражения на ноль
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pTerm_1 == nullptr || pTerm_2 == nullptr) return ERR_T_NULL_PTR;
	// ----------------------------------------
	// Нахождение сторон
	// ----------------------------------------
	term_ptr left, right;
	if (pTerm_1->is(0, term_type::number)) {
		left = pTerm_2;
		right = pTerm_1;
	}
	else {
		if (!pTerm_2->is(0, term_type::number)) return ERR_T_NOT_FOUND_RIGHT;
		left = pTerm_1;
		right = pTerm_2;
	}
	// ----------------------------------------
	// Проверить левую сторону
	// ----------------------------------------
	if (!left->is(OA_3, term_type::operation)) return ERR_T_NOT_FOUND_LEFT;
	bool isFound = false;
	for (size_t i = 0; i < left->size(); i++)
		if (left->get(i)->is(0, term_type::number)) {
			isFound = true;
			break;
		}
	if (!isFound) return ERR_T_NOT_FOUND_LEFT;

	return ERR_T_NO_ERROR;
}

int T_MultiplyZero_R3(term_ptr pTerm_1, term_ptr pTerm_2)
{
	// Правая: какое-то выражение
	// Левая: сложение какого-то выражения с выражением умноженным на ноль
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pTerm_1 == nullptr || pTerm_2 == nullptr) return ERR_T_NULL_PTR;
	// ----------------------------------------
	// Нахождение количеств выражений умноженных на ноль
	// в каждой стороне и удаление их
	// ----------------------------------------
	size_t amountFirstExp = 0, amountSecondExp = 0;
	if (pTerm_1->is(OA_2, term_type::operation)) {
		for (int i = pTerm_1->size() - 1; i >= 0; i--) {
			if (!T_IsLeft_MultiplyZero(pTerm_1->get(i))) continue;
			pTerm_1->remove(i);
			amountFirstExp++;
		}
		if (pTerm_1->size() == 1) pTerm_1 = TrimTop(pTerm_1);
	}
	if (pTerm_2->is(OA_2, term_type::operation)) {
		for (int i = pTerm_2->size() - 1; i >= 0; i--) {
			if (!T_IsLeft_MultiplyZero(pTerm_2->get(i))) continue;
			pTerm_2->remove(i);
			amountSecondExp++;
		}
		if (pTerm_2->size() == 1) pTerm_2 = TrimTop(pTerm_2);
	}
	// ----------------------------------------
	// Проверить равенство оставшихся частей
	// ----------------------------------------
	if (amountFirstExp == 0 && amountSecondExp == 0) return ERR_T_INCORRECT_MARKER;
	if (amountFirstExp == amountSecondExp) return ERR_T_INCORRECT_MARKER;
	if (!Equal(pTerm_1, pTerm_2)) return ERR_T_INCORRECT_MARKER;
	return ERR_T_NO_ERROR;
}

bool T_IsLeft_AddExpAndNegExp(term_ptr left)
{
	if (!left->is(OA_2, term_type::operation)) return false;

	term_ptr exp, complExp;
	if (left->size() == 2)
	{
		// Найти самую высокую ветвь

		if (GetDeep(left->get(0)) > GetDeep(left->get(1))) {
			complExp = left->get(0);
			exp = left->get(1);
		}
		else {
			complExp = left->get(1);
			exp = left->get(0);
		}
		if (!complExp->is(OA_1, term_type::operation, 1)) return false;
		if (!Equal(complExp->get(0), exp)) return false;
	}
	else
	{
		int pMaxDeep = GetMaxDeep(left);
		complExp = left->get(pMaxDeep);
		exp = make_shared<Term>(left);
		exp->remove(pMaxDeep);
		if (!complExp->is(OA_1, term_type::operation, 1)) return false;
		if (!Equal(complExp->get(0), exp)) return false;
	}

	return true;
}

int T_AddExpAndNegExp_R1(term_ptr pTerm_1, term_ptr pTerm_2)
{
	// Правая: ноль
	// Левая: сумма минус выражения и выражения
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pTerm_1 == nullptr || pTerm_2 == nullptr) return ERR_T_NULL_PTR;
	// ----------------------------------------
	// Нахождение сторон
	// ----------------------------------------
	term_ptr left, right;
	if (pTerm_1->is(0, term_type::number)) {
		left = pTerm_2;
		right = pTerm_1;
	}
	else {
		if (!pTerm_2->is(0, term_type::number)) return ERR_T_NOT_FOUND_RIGHT;
		left = pTerm_1;
		right = pTerm_2;
	}
	// ----------------------------------------
	// Проверка левой стороны
	// ----------------------------------------
	if (!T_IsLeft_AddExpAndNegExp(left)) return ERR_T_NOT_FOUND_LEFT;

	return ERR_T_NO_ERROR;
}

int T_AddExpAndNegExp_R3(term_ptr pTerm_1, term_ptr pTerm_2)
{
	// Правая: какое-то выражение
	// Левая: сложение какого-то выражения с суммой минус выражения и выражения
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pTerm_1 == nullptr || pTerm_2 == nullptr) return ERR_T_NULL_PTR;
	// ----------------------------------------
	// Нахождение количеств сумм минус выражения и выражения
	// в каждой стороне и удаление их
	// ----------------------------------------
	size_t amountFirstExp = 0, amountSecondExp = 0;
	if (pTerm_1->is(OA_2, term_type::operation)) {
		for (int i = pTerm_1->size() - 1; i >= 0; i--) {
			if (!T_IsLeft_AddExpAndNegExp(pTerm_1->get(i))) continue;
			pTerm_1->remove(i);
			amountFirstExp++;
		}
		if (pTerm_1->size() == 1) pTerm_1 = TrimTop(pTerm_1);
	}
	if (pTerm_2->is(OA_2, term_type::operation)) {
		for (int i = pTerm_2->size() - 1; i >= 0; i--) {
			if (!T_IsLeft_AddExpAndNegExp(pTerm_2->get(i))) continue;
			pTerm_2->remove(i);
			amountSecondExp++;
		}
		if (pTerm_2->size() == 1) pTerm_2 = TrimTop(pTerm_2);
	}
	// ----------------------------------------
	// Проверить равенство оставшихся частей
	// ----------------------------------------
	if (amountFirstExp == 0 && amountSecondExp == 0) return ERR_T_INCORRECT_MARKER;
	if (amountFirstExp == amountSecondExp) return ERR_T_INCORRECT_MARKER;
	if (!Equal(pTerm_1, pTerm_2)) return ERR_T_INCORRECT_MARKER;
	return ERR_T_NO_ERROR;
}

int T_AddExpAndNegExp_R4(term_ptr pTerm_1, term_ptr pTerm_2)
{
	// Правая: ноль
	// Левая: сумма минус выражения и выражения
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pTerm_1 == nullptr || pTerm_2 == nullptr) return ERR_T_NULL_PTR;
	// ----------------------------------------
	// Нахождение сторон
	// ----------------------------------------
	term_ptr left, right;
	if (pTerm_1->is(0, term_type::number)) {
		left = pTerm_2;
		right = pTerm_1;
	}
	else {
		if (!pTerm_2->is(0, term_type::number)) return ERR_T_NOT_FOUND_RIGHT;
		left = pTerm_1;
		right = pTerm_2;
	}
	// ----------------------------------------
	// Проверка левой стороны
	// ----------------------------------------
	if (!left->is(OA_1, term_type::operation)) return ERR_T_NOT_FOUND_LEFT;
	if (!T_IsLeft_AddExpAndNegExp(left)) return ERR_T_NOT_FOUND_LEFT;

	return ERR_T_NO_ERROR;
}

int T_AddExpAndNegExp_R5(term_ptr pTerm_1, term_ptr pTerm_2)
{
	// Правая: какое-то выражение
	// Левая: сложение какого-то выражения с минус суммой минус выражения и выражения
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pTerm_1 == nullptr || pTerm_2 == nullptr) return ERR_T_NULL_PTR;
	// ----------------------------------------
	// Нахождение количеств минус сумм минус выражения и выражения
	// в каждой стороне и удаление их
	// ----------------------------------------
	size_t amountFirstExp = 0, amountSecondExp = 0;
	if (pTerm_1->is(OA_2, term_type::operation)) {
		for (int i = pTerm_1->size() - 1; i >= 0; i--) {
			if (!pTerm_1->is(OA_1, term_type::operation)) continue;
			if (!T_IsLeft_AddExpAndNegExp(pTerm_1->get(0)->get(i))) continue;
			pTerm_1->remove(i);
			amountFirstExp++;
		}
		if (pTerm_1->size() == 1) pTerm_1 = TrimTop(pTerm_1);
	}
	if (pTerm_2->is(OA_2, term_type::operation)) {
		for (int i = pTerm_2->size() - 1; i >= 0; i--) {
			if (!pTerm_2->is(OA_1, term_type::operation)) continue;
			if (!T_IsLeft_AddExpAndNegExp(pTerm_2->get(0)->get(i))) continue;
			pTerm_2->remove(i);
			amountSecondExp++;
		}
		if (pTerm_2->size() == 1) pTerm_2 = TrimTop(pTerm_2);
	}
	// ----------------------------------------
	// Проверить равенство оставшихся частей
	// ----------------------------------------
	if (amountFirstExp == 0 && amountSecondExp == 0) return ERR_T_INCORRECT_MARKER;
	if (amountFirstExp == amountSecondExp) return ERR_T_INCORRECT_MARKER;
	if (!Equal(pTerm_1, pTerm_2)) return ERR_T_INCORRECT_MARKER;
	return ERR_T_NO_ERROR;
}

int T_AddZero_M(term_ptr pTerm_1, term_ptr pTerm_2)
{
	// Правая: какое-то выражение
	// Левая: сложение какого-то выражения с нулем
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pTerm_1 == nullptr || pTerm_2 == nullptr) return ERR_T_NULL_PTR;
	// ----------------------------------------
	// Нахождение количеств пустых множеств
	// в каждой стороне и удаление их
	// ----------------------------------------
	size_t amountFirstExp = 0, amountSecondExp = 0;
	if (pTerm_1->is(OA_2, term_type::operation)) {
		for (int i = pTerm_1->size() - 1; i >= 0; i--) {
			if (!pTerm_1->get(i)->is(0, term_type::number)) continue;
			pTerm_1->remove(i);
			amountFirstExp++;
		}
		if (pTerm_1->size() == 1) pTerm_1 = TrimTop(pTerm_1);
	}
	if (pTerm_2->is(OS_3, term_type::operation)) {
		for (int i = pTerm_2->size() - 1; i >= 0; i--) {
			if (!pTerm_2->get(i)->is(0, term_type::number)) continue;
			pTerm_2->remove(i);
			amountSecondExp++;
		}
		if (pTerm_2->size() == 1) pTerm_2 = TrimTop(pTerm_2);
	}
	// ----------------------------------------
	// Проверить равенство оставшихся частей
	// ----------------------------------------
	if (amountFirstExp == 0 && amountSecondExp == 0) return ERR_T_INCORRECT_MARKER;
	if (amountFirstExp == amountSecondExp) return ERR_T_INCORRECT_MARKER;
	if (!Equal(pTerm_1, pTerm_2)) return ERR_T_INCORRECT_MARKER;
	return ERR_T_NO_ERROR;
}

int T_Comp(term_ptr pTerm_1, term_ptr pTerm_2)
{
	// Правая: числовое выражение#1
	// Левая: числовое выражение#2
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pTerm_1 == nullptr || pTerm_2 == nullptr) return ERR_T_NULL_PTR;
	// ----------------------------------------
	// Нахождение значений для каждой стороны
	// ----------------------------------------
	try {
		double valueFirst = _Complite(pTerm_1);
		double valueSecond = _Complite(pTerm_2);

		if (valueFirst != valueSecond) return ERR_T_INCORRECT_MARKER;
	}
	catch (int) {
		return ERR_T_INCORRECT_MARKER;
	}
	return ERR_T_NO_ERROR;
}

double _Complite(term_ptr pTerm) noexcept(false)
{
	double result;
	if (pTerm->_type == term_type::number) return pTerm->_value;
	else if (pTerm->is(OA_1, term_type::operation, 1)) {
		result = -_Complite(pTerm->get(0));
	}
	else if (pTerm->is(OA_2, term_type::operation)) {
		result = 0;
		for (size_t i = 0; i < pTerm->size(); i++)
			result += _Complite(pTerm->get(i));
	}
	else if (pTerm->is(OA_3, term_type::operation)) {
		result = 1;
		for (size_t i = 0; i < pTerm->size(); i++)
			result *= _Complite(pTerm->get(i));
	}
	else throw 1;
	return result;
}

int T_CardialityNullset_R1(term_ptr pTerm_1, term_ptr pTerm_2)
{
	// Правая: ноль
	// Левая: мощность пустого множества
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pTerm_1 == nullptr || pTerm_2 == nullptr) return ERR_T_NULL_PTR;
	// ----------------------------------------
	// Нахождение сторон
	// ----------------------------------------
	term_ptr left, right;
	if (pTerm_1->is(0, term_type::number)) {
		left = pTerm_2;
		right = pTerm_1;
	}
	else {
		if (!pTerm_2->is(0, term_type::number)) return ERR_T_NOT_FOUND_RIGHT;
		left = pTerm_1;
		right = pTerm_2;
	}
	// ----------------------------------------
	// Проверка левой стороны
	// ----------------------------------------
	if (!left->is(OB__, term_type::operation, 1)) return ERR_T_NOT_FOUND_LEFT;
	if (!left->get(0)->is(NULLSET, term_type::symbol)) return ERR_T_NOT_FOUND_LEFT;
	return ERR_T_NO_ERROR;
}

int T_CardialityNullset_R3(term_ptr pTerm_1, term_ptr pTerm_2)
{
	// Правая: какое-то выражение
	// Левая: сложение какого-то выражения с мощностью пустого множества
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pTerm_1 == nullptr || pTerm_2 == nullptr) return ERR_T_NULL_PTR;
	// ----------------------------------------
	// Нахождение количеств мощностей пустого множества
	// в каждой стороне и удаление их
	// ----------------------------------------
	size_t amountFirstExp = 0, amountSecondExp = 0;
	if (pTerm_1->is(OA_2, term_type::operation)) {
		for (int i = pTerm_1->size() - 1; i >= 0; i--) {
			if (!pTerm_1->get(i)->is(OB__, term_type::operation, 1)) continue;
			if (!pTerm_1->get(i)->get(0)->is(NULLSET, term_type::symbol)) continue;
			pTerm_1->remove(i);
			amountFirstExp++;
		}
		if (pTerm_1->size() == 1) pTerm_1 = TrimTop(pTerm_1);
	}
	if (pTerm_2->is(OA_2, term_type::operation)) {
		for (int i = pTerm_2->size() - 1; i >= 0; i--) {
			if (!pTerm_2->get(i)->is(OB__, term_type::operation, 1)) continue;
			if (!pTerm_2->get(i)->get(0)->is(NULLSET, term_type::symbol)) continue;
			pTerm_2->remove(i);
			amountSecondExp++;
		}
		if (pTerm_2->size() == 1) pTerm_2 = TrimTop(pTerm_2);
	}
	// ----------------------------------------
	// Проверить равенство оставшихся частей
	// ----------------------------------------
	if (amountFirstExp == 0 && amountSecondExp == 0) return ERR_T_INCORRECT_MARKER;
	if (amountFirstExp == amountSecondExp) return ERR_T_INCORRECT_MARKER;
	if (!Equal(pTerm_1, pTerm_2)) return ERR_T_INCORRECT_MARKER;
	return ERR_T_NO_ERROR;
}

int T_CardialityNullset_R4(term_ptr pTerm_1, term_ptr pTerm_2)
{
	// Правая: ноль
	// Левая: минус мощность пустого множества
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pTerm_1 == nullptr || pTerm_2 == nullptr) return ERR_T_NULL_PTR;
	// ----------------------------------------
	// Нахождение сторон
	// ----------------------------------------
	term_ptr left, right;
	if (pTerm_1->is(0, term_type::number)) {
		left = pTerm_2;
		right = pTerm_1;
	}
	else {
		if (!pTerm_2->is(0, term_type::number)) return ERR_T_NOT_FOUND_RIGHT;
		left = pTerm_1;
		right = pTerm_2;
	}
	// ----------------------------------------
	// Проверка левой стороны
	// ----------------------------------------
	if (!left->is(OA_1, term_type::operation, 1)) return ERR_T_NOT_FOUND_LEFT;
	if (!left->get(0)->is(OB__, term_type::operation, 1)) return ERR_T_NOT_FOUND_LEFT;
	if (!left->get(0)->get(0)->is(NULLSET, term_type::symbol)) return ERR_T_NOT_FOUND_LEFT;
	return ERR_T_NO_ERROR;
}

int T_CardialityNullset_R5(term_ptr pTerm_1, term_ptr pTerm_2)
{
	// Правая: какое-то выражение
	// Левая: сложение какого-то выражения с мощностью пустого множества
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pTerm_1 == nullptr || pTerm_2 == nullptr) return ERR_T_NULL_PTR;
	// ----------------------------------------
	// Нахождение количеств мощностей пустого множества
	// в каждой стороне и удаление их
	// ----------------------------------------
	size_t amountFirstExp = 0, amountSecondExp = 0;
	if (pTerm_1->is(OA_2, term_type::operation)) {
		for (int i = pTerm_1->size() - 1; i >= 0; i--) {
			if (!pTerm_1->get(i)->is(OA_1, term_type::operation, 1)) continue;
			if (!pTerm_1->get(i)->get(0)->is(OB__, term_type::operation, 1)) continue;
			if (!pTerm_1->get(i)->get(0)->get(0)->is(NULLSET, term_type::symbol)) continue;
			pTerm_1->remove(i);
			amountFirstExp++;
		}
		if (pTerm_1->size() == 1) pTerm_1 = TrimTop(pTerm_1);
	}
	if (pTerm_2->is(OA_2, term_type::operation)) {
		for (int i = pTerm_2->size() - 1; i >= 0; i--) {
			if (!pTerm_2->get(i)->is(OA_1, term_type::operation, 1)) continue;
			if (!pTerm_2->get(i)->get(0)->is(OB__, term_type::operation, 1)) continue;
			if (!pTerm_2->get(i)->get(0)->get(0)->is(NULLSET, term_type::symbol)) continue;
			pTerm_2->remove(i);
			amountSecondExp++;
		}
		if (pTerm_2->size() == 1) pTerm_2 = TrimTop(pTerm_2);
	}
	// ----------------------------------------
	// Проверить равенство оставшихся частей
	// ----------------------------------------
	if (amountFirstExp == 0 && amountSecondExp == 0) return ERR_T_INCORRECT_MARKER;
	if (amountFirstExp == amountSecondExp) return ERR_T_INCORRECT_MARKER;
	if (!Equal(pTerm_1, pTerm_2)) return ERR_T_INCORRECT_MARKER;
	return ERR_T_NO_ERROR;
}

bool ____CardComplExp(const term_ptr& pterm_1, const term_ptr& pterm_2)
{
	if (!pterm_1->is(ModuleSign, term_type::operation)) return false;
	if (!pterm_1->get(0)->is(ComplementSign, term_type::operation, 1)) return false;

	if (!pterm_2->is(AdditionSign, term_type::operation, 2)) return false;

	if (pterm_2->get(0)->is(ModuleSign, term_type::operation, 1)) {
		if (!pterm_2->get(0)->get(0)->is(UniversalSet, term_type::symbol)) return false;
		if (!pterm_2->get(1)->is(SubtractionSign, term_type::operation, 1)) return false;
		if (!pterm_2->get(1)->get(0)->is(ModuleSign, term_type::operation, 1)) return false;
		return Equal(pterm_1->get(0)->get(0), pterm_2->get(1)->get(0)->get(0));
	}
	else {
		if (!pterm_2->get(1)->is(ModuleSign, term_type::operation, 1)) return false;
		if (!pterm_2->get(1)->get(0)->is(UniversalSet, term_type::symbol)) return false;
		if (!pterm_2->get(0)->is(SubtractionSign, term_type::operation, 1)) return false;
		if (!pterm_2->get(0)->get(0)->is(ModuleSign, term_type::operation, 1)) return false;
		return Equal(pterm_1->get(0)->get(0), pterm_2->get(0)->get(0)->get(0));
	}
	return false;
}

bool ___CardComplExp(const term_ptr& pterm_1, const term_ptr& pterm_2)
{
	// Правая: мощность дополнения множества#1
	// Левая: разность мощности универсума и множества#1
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pterm_1 == nullptr || pterm_2 == nullptr) return false;

	if (____CardComplExp(pterm_1, pterm_2) || ____CardComplExp(pterm_2, pterm_1)) return true;

	if (!pterm_1->is(AdditionSign, term_type::operation) || !pterm_2->is(AdditionSign, term_type::operation)) return false;

	vector<term_ptr> terms_1;
	vector<term_ptr> terms_2;
	vector<bool> checked(pterm_2->size(), false);
	size_t i, j;
	for (i = 0; i < pterm_1->size(); i++) {
		for (j = 0; j < pterm_2->size(); j++) {
			if (checked[j]) continue;
			if (TermTool::Equal(pterm_1->get(i), pterm_2->get(j))) {
				checked[j] = true;
				break;
			}
		}
		if (j == pterm_2->size()) {
			terms_1.push_back(pterm_1->get(i));
		}
	}
	for (i = 0; i < pterm_2->size(); i++) {
		if (!checked[i]) terms_2.push_back(pterm_2->get(i));
	}
	if (terms_1.empty() || terms_2.empty()) return false;

	term_ptr tempTerm = make_shared<Term>(AdditionSign, term_type::operation);
	for (int i = terms_1.size() - 1; i >= 0; i--) {
		for (size_t j = 0; j < terms_2.size(); j++) {
			for (size_t k = j; k < terms_2.size(); k++) {
				tempTerm->remove_all();
				tempTerm->add(terms_2[j]);
				tempTerm->add(terms_2[k]);
				if (____CardComplExp(terms_1[i], tempTerm)) {
					terms_1.erase(terms_1.begin() + i);
					terms_2.erase(terms_2.begin() + k);
					terms_2.erase(terms_2.begin() + j);
					j = terms_2.size() + 10;
					break;
				}
			}
		}
	}
	for (int i = terms_2.size() - 1; i >= 0; i--) {
		for (size_t j = 0; j < terms_1.size(); j++) {
			for (size_t k = j; k < terms_1.size(); k++) {
				tempTerm->remove_all();
				tempTerm->add(terms_1[j]);
				tempTerm->add(terms_1[k]);
				if (____CardComplExp(terms_2[i], tempTerm)) {
					terms_2.erase(terms_2.begin() + i);
					terms_1.erase(terms_1.begin() + k);
					terms_1.erase(terms_1.begin() + j);
					j = terms_1.size() + 10;
					break;
				}
			}
		}
	}
	return (terms_1.empty() && terms_2.empty());
}

bool ____CardDiffUnisetAndExp(const term_ptr& pterm_1, const term_ptr& pterm_2)
{
	if (!pterm_1->is(ModuleSign, term_type::operation)) return false;
	if (!pterm_1->get(0)->is(DifferenceSign, term_type::operation, 2)) return false;
	if (!pterm_1->get(0)->get(0)->is(UniversalSet, term_type::symbol)) return false;

	if (!pterm_2->is(AdditionSign, term_type::operation, 2)) return false;

	if (pterm_2->get(0)->is(ModuleSign, term_type::operation, 1)) {
		if (!pterm_2->get(0)->get(0)->is(UniversalSet, term_type::symbol)) return false;
		if (!pterm_2->get(1)->is(SubtractionSign, term_type::operation, 1)) return false;
		if (!pterm_2->get(1)->get(0)->is(ModuleSign, term_type::operation, 1)) return false;
		return Equal(pterm_1->get(0)->get(1), pterm_2->get(1)->get(0)->get(0));
	}
	else {
		if (!pterm_2->get(1)->is(ModuleSign, term_type::operation, 1)) return false;
		if (!pterm_2->get(1)->get(0)->is(UniversalSet, term_type::symbol)) return false;
		if (!pterm_2->get(0)->is(SubtractionSign, term_type::operation, 1)) return false;
		if (!pterm_2->get(0)->get(0)->is(ModuleSign, term_type::operation, 1)) return false;
		return Equal(pterm_1->get(0)->get(1), pterm_2->get(0)->get(0)->get(0));
	}
	return false;
}

bool ___CardDiffUnisetAndExp(const term_ptr& pterm_1, const term_ptr& pterm_2)
{
	// Правая: мощность разности универсума и множества#1
	// Левая: разность мощности универсума и множества#1
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pterm_1 == nullptr || pterm_2 == nullptr) return false;


	if (____CardDiffUnisetAndExp(pterm_1, pterm_2) || ____CardDiffUnisetAndExp(pterm_2, pterm_1)) return true;

	if (!pterm_1->is(AdditionSign, term_type::operation) || !pterm_2->is(AdditionSign, term_type::operation)) return false;

	vector<term_ptr> terms_1;
	vector<term_ptr> terms_2;
	vector<bool> checked(pterm_2->size(), false);
	size_t i, j;
	for (i = 0; i < pterm_1->size(); i++) {
		for (j = 0; j < pterm_2->size(); j++) {
			if (checked[j]) continue;
			if (TermTool::Equal(pterm_1->get(i), pterm_2->get(j))) {
				checked[j] = true;
				break;
			}
		}
		if (j == pterm_2->size()) {
			terms_1.push_back(pterm_1->get(i));
		}
	}
	for (i = 0; i < pterm_2->size(); i++) {
		if (!checked[i]) terms_2.push_back(pterm_2->get(i));
	}
	if (terms_1.empty() || terms_2.empty()) return false;

	term_ptr tempTerm = make_shared<Term>(AdditionSign, term_type::operation);
	for (int i = terms_1.size() - 1; i >= 0; i--)	{
		for (size_t j = 0; j < terms_2.size(); j++)	{
			for (size_t k = j; k < terms_2.size(); k++)	{
				tempTerm->remove_all();
				tempTerm->add(terms_2[j]);
				tempTerm->add(terms_2[k]);
				if (____CardDiffUnisetAndExp(terms_1[i], tempTerm)) {
					terms_1.erase(terms_1.begin() + i);
					terms_2.erase(terms_2.begin() + k);
					terms_2.erase(terms_2.begin() + j);
					j = terms_2.size() + 10;
					break;
				}
			}
		}
	}
	for (int i = terms_2.size() - 1; i >= 0; i--) {
		for (size_t j = 0; j < terms_1.size(); j++) {
			for (size_t k = j; k < terms_1.size(); k++) {
				tempTerm->remove_all();
				tempTerm->add(terms_1[j]);
				tempTerm->add(terms_1[k]);
				if (____CardDiffUnisetAndExp(terms_2[i], tempTerm)) {
					terms_2.erase(terms_2.begin() + i);
					terms_1.erase(terms_1.begin() + k);
					terms_1.erase(terms_1.begin() + j);
					j = terms_1.size() + 10;
					break;
				}
			}
		}
	}
	return (terms_1.empty() && terms_2.empty());
}

int T_MultiplyOne_M(term_ptr pTerm_1, term_ptr pTerm_2)
{
	// Правая: какое-то выражение
	// Левая: произведение какого-то выражения с единицами
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pTerm_1 == nullptr || pTerm_2 == nullptr) return ERR_T_NULL_PTR;
	// ----------------------------------------
	// Нахождение количеств единиц
	// в каждой стороне и удаление их
	// ----------------------------------------
	size_t amountFirstExp = 0, amountSecondExp = 0;
	if (pTerm_1->is(OA_3, term_type::operation)) {
		for (int i = pTerm_1->size() - 1; i >= 0; i--) {
			if (!pTerm_1->get(i)->is(1, term_type::number)) continue;
			pTerm_1->remove(i);
			amountFirstExp++;
		}
		if (pTerm_1->size() == 1) pTerm_1 = TrimTop(pTerm_1);
	}
	if (pTerm_2->is(OA_3, term_type::operation)) {
		for (int i = pTerm_2->size() - 1; i >= 0; i--) {
			if (!pTerm_2->get(i)->is(1, term_type::number)) continue;
			pTerm_2->remove(i);
			amountSecondExp++;
		}
		if (pTerm_2->size() == 1) pTerm_2 = TrimTop(pTerm_2);
	}
	// ----------------------------------------
	// Проверить равенство оставшихся частей
	// ----------------------------------------
	if (amountFirstExp == 0 && amountSecondExp == 0) return ERR_T_INCORRECT_MARKER;
	if (amountFirstExp == amountSecondExp) return ERR_T_INCORRECT_MARKER;
	if (!Equal(pTerm_1, pTerm_2)) return ERR_T_INCORRECT_MARKER;
	return ERR_T_NO_ERROR;
}

int T_MultiplyMinusOne(term_ptr pTerm_1, term_ptr pTerm_2)
{
	// Правая: минус выражение#1
	// Левая: произведение выражения#1 на минус один
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pTerm_1 == nullptr || pTerm_2 == nullptr) return ERR_T_NULL_PTR;
	// ----------------------------------------
	// Нахождение сторон
	// ----------------------------------------
	term_ptr left, right;
	if (pTerm_1->is(OA_1, term_type::operation)) {
		left = pTerm_2;
		right = pTerm_1;
	}
	else {
		if (!pTerm_2->is(OA_1, term_type::operation)) return ERR_T_NOT_FOUND_RIGHT;
		left = pTerm_1;
		right = pTerm_2;
	}
	// ----------------------------------------
	// Проверить левую сторону
	// ----------------------------------------
	if (!left->is(OA_3, term_type::operation)) return ERR_T_NOT_FOUND_LEFT;
	bool isFound = false;
	for (size_t i = 0; i < left->size(); i++)
		if (left->get(i)->is(OA_1, term_type::operation, 1)) {
			if (left->get(i)->get(0)->is(1, term_type::number)) {
				left->remove(i);
				isFound = true;
				break;
			}
		}
	if (!isFound) return ERR_T_NOT_FOUND_LEFT;
	if (left->size() == 1) left = TrimTop(left);
	if (!Equal(left, right->get(0))) return ERR_T_INCORRECT_MARKER;
	return ERR_T_NO_ERROR;
}

int T_DivideByOne(term_ptr pTerm_1, term_ptr pTerm_2)
{
	// Правая: выражение деленное на один
	// Левая: выражение
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pTerm_1 == nullptr || pTerm_2 == nullptr) return ERR_T_NULL_PTR;
	// ----------------------------------------
	// Нахождение сторон
	// ----------------------------------------
	term_ptr left, right;
	if (pTerm_1->is(OA_4, term_type::operation, 2)) {
		left = pTerm_2;
		right = pTerm_1;
	}
	else {
		if (!pTerm_2->is(OA_4, term_type::operation, 2)) return ERR_T_NOT_FOUND_RIGHT;
		left = pTerm_1;
		right = pTerm_2;
	}
	// ----------------------------------------
	// Проверка левой стороны
	// ----------------------------------------
	if (!right->get(1)->is(1, term_type::number)) return ERR_T_NOT_FOUND_RIGHT;
	if (!Equal(left, left->get(0))) return ERR_T_NOT_FOUND_LEFT;
	return ERR_T_NO_ERROR;
}

bool ___IndenticalExp(const term_ptr& pterm_1, const term_ptr& pterm_2)
{
	// Правая: сумма выражений
	// Левая: произведение коэффициента и выражения или выражение
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pterm_1 == nullptr || pterm_2 == nullptr) return false;
	// ----------------------------------------
	// Подготовка
	// ----------------------------------------
	term_ptr _pterm_1 = TermTool::Copy(pterm_1);
	term_ptr _pterm_2 = TermTool::Copy(pterm_2);

	// T1 = F11(a1)+F12(a1)+...F1n(a1)+F21(a2)+F22(a2)+...F2n(a2)+Fm1(am)+Fm2(am)+...Fmn(am)
	// Случаи:
	// 1. t11 + t12 + ... + t1k = t21 + t22 + ... + t2l
	// 2. t1 = t21 + t22 + ... + t2l
	// 3. t11 + t12 + ... + t1k = t2

	// Случай 1.
	if (_pterm_1->is(AdditionSign, term_type::operation) && _pterm_2->is(AdditionSign, term_type::operation)) {
		// Удалить все одинаковые подтермы
		TermTool::Collect(_pterm_1, _pterm_2);
		// Сбор одинаковых элементов
		struct unit {
			term_ptr	term = nullptr; // терм
			double		coef = 0;		// коэффициент
			bool		unique = true;	// уникальность элемента
		};
		bool isFound = false;
		double coef = 0;
		term_ptr tempTerm = nullptr;
		vector <unit> left_units, right_units;
		// Для левой части
		for (size_t i = 0; i < _pterm_1->size(); i++) {
			// Отделение и вычисление коэффициента
			if (!TermTool::ToCell(_pterm_1->get(i), &coef, &tempTerm)) return false;
			// Сравнение полученного терма с уже собранными
			isFound = false;
			for (size_t j = 0; j < left_units.size(); j++)
				if (TermTool::Equal(left_units[j].term, tempTerm)) {
					// Если нашли одинаковые термы
					left_units[j].coef += coef;
					left_units[j].unique = false;
					isFound = true;
					break;
				}
			// Если терм не найден, то добавляем его
			if (!isFound) left_units.push_back({ tempTerm, coef, true });
		}
		// Для правой части
		for (size_t i = 0; i < _pterm_2->size(); i++) {
			// Отделение и вычисление коэффициента
			if (!TermTool::ToCell(_pterm_2->get(i), &coef, &tempTerm)) return false;
			// Сравнение полученного терма с уже собранными
			isFound = false;
			for (size_t j = 0; j < right_units.size(); j++)
				if (TermTool::Equal(right_units[j].term, tempTerm)) {
					// Если нашли одинаковые термы
					right_units[j].coef += coef;
					right_units[j].unique = false;
					isFound = true;
					break;
				}
			// Если терм не найден, то добавляем его
			if (!isFound) right_units.push_back({ tempTerm, coef, true });
		}
		// Проверка 
		for (size_t i = 0; i < left_units.size(); i++)
		{
			if (left_units[i].term == nullptr) return false;
			isFound = false;
			for (size_t j = 0; j < right_units.size(); j++)
			{
				if (right_units[j].term == nullptr) return false;
				if (TermTool::Equal(left_units[i].term, right_units[j].term)) {
					if (left_units[i].coef != right_units[j].coef) return false;
					if (!left_units[i].unique && !right_units[j].unique) return false;
					right_units.erase(right_units.begin() + j);
					isFound = true;
					break;
				}
				if (!isFound && (left_units[i].coef != 0)) return false;
			}
		}
		for (size_t i = 0; i < right_units.size(); i++) if (right_units[i].coef != 0) return false;
		return true;
	}
	// Случай 2.
	if (_pterm_2->is(AdditionSign, term_type::operation)) {
		// Сбор одинаковых элементов
		struct unit {
			term_ptr	term = nullptr; // терм
			double		coef = 0;		// коэффициент
			bool		unique = true;	// уникальность элемента
		};
		bool isFound = false;
		double coef = 0;
		term_ptr tempTerm = nullptr;
		vector <unit> right_units;
		// Для правой части
		for (size_t i = 0; i < _pterm_2->size(); i++) {
			// Отделение и вычисление коэффициента
			if (!TermTool::ToCell(_pterm_2->get(i), &coef, &tempTerm)) return false;
			// Сравнение полученного терма с уже собранными
			isFound = false;
			for (size_t j = 0; j < right_units.size(); j++)
				if (TermTool::Equal(right_units[j].term, tempTerm)) {
					// Если нашли одинаковые термы
					right_units[j].coef += coef;
					right_units[j].unique = false;
					isFound = true;
					break;
				}
			// Если терм не найден, то добавляем его
			if (!isFound) right_units.push_back({ tempTerm, coef, true });
		}
		if (right_units.size() != 1) return false;

		if (!TermTool::ToCell(_pterm_1, &coef, &tempTerm)) return false;
		if (coef != right_units[0].coef) return false;
		if (coef == 0) {
			if (tempTerm == nullptr) return true;
			return TermTool::Equal(tempTerm, right_units[0].term);
		}
		return TermTool::Equal(tempTerm, right_units[0].term);
	}
	// Случай 3.
	if (_pterm_1->is(AdditionSign, term_type::operation)) {
		// Сбор одинаковых элементов
		struct unit {
			term_ptr	term = nullptr; // терм
			double		coef = 0;		// коэффициент
			bool		unique = true;	// уникальность элемента
		};
		bool isFound = false;
		double coef = 0;
		term_ptr tempTerm = nullptr;
		vector <unit> left_units;
		// Для правой части
		for (size_t i = 0; i < _pterm_1->size(); i++) {
			// Отделение и вычисление коэффициента
			if (!TermTool::ToCell(_pterm_1->get(i), &coef, &tempTerm)) return false;
			// Сравнение полученного терма с уже собранными
			isFound = false;
			for (size_t j = 0; j < left_units.size(); j++)
				if (TermTool::Equal(left_units[j].term, tempTerm)) {
					// Если нашли одинаковые термы
					left_units[j].coef += coef;
					left_units[j].unique = false;
					isFound = true;
					break;
				}
			// Если терм не найден, то добавляем его
			if (!isFound) left_units.push_back({ tempTerm, coef, true });
		}
		if (left_units.size() != 1) return false;

		if (!TermTool::ToCell(_pterm_2, &coef, &tempTerm)) return false;
		if (coef != left_units[0].coef) return false;
		if (coef == 0) {
			if (tempTerm == nullptr) return true;
			return TermTool::Equal(tempTerm, left_units[0].term);
		}
		return TermTool::Equal(tempTerm, left_units[0].term);
	}
	return false;
}

int T_ChangEqual(term_ptr pTerm_1_1, term_ptr pTerm_1_2, term_ptr pTerm_2_1, term_ptr pTerm_2_2)
{
	// Правая: -
	// Левая: -
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pTerm_1_1 == nullptr || pTerm_1_2 == nullptr || pTerm_2_1 == nullptr || pTerm_2_2 == nullptr) return ERR_T_NULL_PTR;
	// ----------------------------------------
	// Перенести всё в левую часть и сравнить
	// ----------------------------------------
	term_ptr firstLeft = _ChangeEqualTo(pTerm_1_1, pTerm_1_2);
	term_ptr secondLeft = _ChangeEqualTo(pTerm_2_1, pTerm_2_2);

	if (!Equal(firstLeft, secondLeft)) return ERR_T_NO_CHANGE;
	return ERR_T_NO_ERROR;
}

term_ptr _ChangeEqualTo(term_ptr pReceiver, term_ptr pSource)
{
	if (pSource->is(0, term_type::number)) return pReceiver;

	term_ptr tempTerm = nullptr;
	term_ptr resultTerm = nullptr;

	if (pSource->is(OA_2, term_type::operation))
	{
		if (pReceiver->is(OA_2, term_type::operation))
		{
			for (size_t i = 0; i < pSource->size(); i++)
			{
				tempTerm = _ChangeOfSign(pSource->get(i));
				pReceiver->add(tempTerm);
			}
		}
		else
		{
			resultTerm = shared_ptr<Term>(new Term(OA_2, term_type::operation));
			resultTerm->add(pReceiver);

			for (size_t i = 0; i < pSource->size(); i++)
			{
				tempTerm = _ChangeOfSign(pSource->get(i));
				resultTerm->add(tempTerm);
			}

			return resultTerm;
		}
	}
	else
	{
		if (pReceiver->is(OA_2, term_type::operation))
		{
			tempTerm = _ChangeOfSign(pSource);
			pReceiver->add(tempTerm);
		}
		else
		{
			resultTerm = shared_ptr<Term>(new Term(OA_2, term_type::operation));
			resultTerm->add(pReceiver);

			tempTerm = _ChangeOfSign(pSource);
			resultTerm->add(tempTerm);

			return resultTerm;
		}
	}
	return pReceiver;
}

term_ptr _ChangeOfSign(term_ptr pTerm)
{
	if (pTerm == nullptr) return nullptr;
	if (pTerm->is(OA_1, term_type::operation, 1)) return pTerm->get(0);

	term_ptr resultTerm = shared_ptr<Term>(new Term(OA_1, term_type::operation));
	resultTerm->add(pTerm);
	return resultTerm;
}

int T_Substitution(vector<term_ptr>* pMarkeredTerms_1, vector<term_ptr>* pMarkeredTerms_2, vector<term_ptr>* pTerms_1, vector<term_ptr>* pTerms_2)
{
	// Правая: -
	// Левая: -
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pMarkeredTerms_1->empty() || pMarkeredTerms_2->empty() || pTerms_1->empty() || pTerms_2->empty()) return ERR_T_NULL_PTR;
	// ----------------------------------------
	// Сопоставить части
	// ----------------------------------------
	if (pMarkeredTerms_1->size() != pTerms_1->size()) return ERR_T_INCORRECT_MARKER;
	bool isFound = false;
	for (size_t i = pMarkeredTerms_1->size() - 1; i >= 0; i--) {
		isFound = false;
		for (size_t j = 0; j < pTerms_1->size(); j++) {
			if (Equal(pMarkeredTerms_1->at(i), pTerms_1->at(j))) {
				if (Equal(pMarkeredTerms_2->at(i), pTerms_2->at(j))) {
					pMarkeredTerms_1->erase(pMarkeredTerms_1->begin() + i);
					pMarkeredTerms_2->erase(pMarkeredTerms_2->begin() + i);
					pTerms_1->erase(pTerms_1->begin() + j);
					pTerms_2->erase(pTerms_2->begin() + j);
					isFound = true;
					break;
				}
			}
			if (Equal(pMarkeredTerms_2->at(i), pTerms_1->at(j))) {
				if (Equal(pMarkeredTerms_1->at(i), pTerms_2->at(j))) {
					pMarkeredTerms_1->erase(pMarkeredTerms_1->begin() + i);
					pMarkeredTerms_2->erase(pMarkeredTerms_2->begin() + i);
					pTerms_1->erase(pTerms_1->begin() + j);
					pTerms_2->erase(pTerms_2->begin() + j);
					isFound = true;
					break;
				}
			}
		}
		if (!isFound) return ERR_T_INCORRECT_MARKER;
	}

	return ERR_T_NO_ERROR;
}

int T_Transposition(term_ptr pTerm_1, term_ptr pTerm_2)
{
	// Правая: -
	// Левая: -
	// ----------------------------------------
	// Проверка на ноль
	// ----------------------------------------
	if (pTerm_1 == nullptr || pTerm_2 == nullptr) return ERR_T_NULL_PTR;
	// ----------------------------------------
	// Сравнение нормальныхз форм
	// ----------------------------------------
	NormalForm(pTerm_1);
	NormalForm(pTerm_2);

	if (!Equal(pTerm_1, pTerm_2)) return ERR_T_INCORRECT_EXPRESSION;
	return ERR_T_NO_ERROR;
}

int T_Conclusion(const CTable* table, int lineID, const string& pTerm_1, const string& pTerm_2)
{
	int line_1 = -1;
	int line_2 = -1;
	for (size_t i = 0; i < table->Lines.size(); i++)
	{
		if (i == lineID) continue;
		if (table->Lines[i].segments.front().left == pTerm_1) {
			line_1 = i;
			break;
		}
	}

	for (size_t i = 0; i < table->Lines.size(); i++)
	{
		if (i == lineID) continue;
		if (table->Lines[i].segments.back().right == pTerm_2) {
			line_2 = i;
			break;
		}
	}

	if (line_1 == -1 || line_2 == -1 || table->Lines[line_1].connection != table->Lines[line_2].connection) return ERR_T_INCORRECT_EXPRESSION;
	return ERR_T_NO_ERROR;
}

int T_Given(UserInfo& UI, term_ptr pTerm_1, term_ptr pTerm_2)
{
	int error;
	vector<string> segments;
	term_ptr left, right;
	for (size_t i = 0; i < UI.GetCurrentTask()->GivenExpressions.size(); i++) {
		SplitIntoSegments(UI.GetCurrentTask()->GivenExpressions[i], &segments);
		left = TermCoder(segments[0], &error);
		right = TermCoder(segments[1], &error);
		if (Equal(pTerm_1, left) && Equal(pTerm_2, right)) return ERR_T_NO_ERROR;
	}
	return ERR_T_INCORRECT_EXPRESSION;
}

int T_ToFind(UserInfo& UI, term_ptr pTerm_1, term_ptr pTerm_2)
{
	int error;
	vector<string> segments;
	term_ptr left, right;
	for (size_t i = 0; i < UI.GetCurrentTask()->ToFindExpressions.size(); i++) {
		SplitIntoSegments(UI.GetCurrentTask()->ToFindExpressions[i], &segments);
		left = TermCoder(segments[0], &error);
		NormalForm(left);
		right = TermCoder(segments[1], &error);
		NormalForm(right);
		if (Equal(pTerm_1, left) && Equal(pTerm_2, right)) return ERR_T_NO_ERROR;
	}
	return ERR_T_INCORRECT_EXPRESSION;
}

int T_Answer(const CTable* table, UserInfo& UI, int lineID, term_ptr pTerm_1, term_ptr pTerm_2)
{
	// Проверяет:
	// > Есть 'ToFind'
	// > Является выводом из 'ToFind'
	// > Совпадает со значением в БД
	int error;

	int toFindLine = -1;
	for (size_t i = 0; i < table->Lines.size(); i++) {
		for (size_t j = 0; j < table->Lines[i].segments.size(); j++) {
			if (table->Lines[i].segments[j].operation == OP_TOFIND) {
				if(table->Lines[i].segments[j].error != ERR_NO_ERROR) return ERR_T_NOT_FOUND_TOFIND;
				toFindLine = i;
			}
		}
	}
	if (toFindLine == -1) return ERR_T_NOT_FOUND_TOFIND;

	int line_2 = -1;
	for (size_t i = 0; i < table->Lines.size(); i++)
	{
		if (i == lineID) continue;
		if (table->Lines[i].segments.back().right == table->Lines[lineID].segments.back().right) {
			line_2 = i;
			break;
		}
	}

	if (line_2 == -1 || table->Lines[toFindLine].connection != table->Lines[line_2].connection) return ERR_T_NOT_FOUND_TOFIND;

	vector<string> segments;
	term_ptr left, right;
	for (size_t i = 0; i < UI.GetCurrentTask()->AnswerExpressions.size(); i++) {
		SplitIntoSegments(UI.GetCurrentTask()->AnswerExpressions[i], &segments);
		left = TermCoder(segments[0], &error);
		right = TermCoder(segments[1], &error);
		if (Equal(pTerm_1, left) && Equal(pTerm_2, right)) return ERR_T_NO_ERROR;
	}
	return ERR_T_INCORRECT_EXPRESSION;
}