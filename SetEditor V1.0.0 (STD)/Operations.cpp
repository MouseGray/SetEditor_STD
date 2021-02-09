#include "Operations.h"

#if 0
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
#endif

void infinity::doubleComplement(Term* term)
{
	while (*term == Complement)
	{
		if (term->get_ref(0) != Complement) break;
		TermTool::collapse(term, term->get(0)->get(0));
	}

	for (auto i = 0; i < term->size(); i++)
		doubleComplement(term->get(i));
}

void infinity::duplicates(Term* term, action act)
{
	if (*term == act)
	{
		for (auto base = 0; base < term->size(); base++)
			for (auto i = term->size() - 1; i > base; i--)
				if (TermTool::isEqual(term->get_ref(base), term->get_ref(i))) TermTool::remove(*term >> i);
	}

	for (auto i = 0; i < term->size(); i++)
		duplicates(term->get(i), act);
}

void normalization::quantityNullset(Term* term)
{
	if (*term == Quantity)
	{
		if (term->get_ref(0) == 'N') {
			TermTool::remove(*term >> 0);
			*term = 0.0f;
		}
	}

	for (auto i = 0; i < term->size(); i++)
		quantityNullset(term->get(i));
}


void normalization::coefficients(Term* term)
{
	for (auto i = 0; i < term->size(); i++)
		coefficients(term->get(i));

	if (*term == Addition)
	{
		auto marked = std::stack<int>();
		auto sum = 0.0f;
		for (auto i = 0; i < term->size(); i++) {
			if (*term == Term::Type::Number) {
				sum += term->get(0)->toNumber();
				marked.push(i);
			}
		}
	
		if (marked.size() >= 2) {
			if (term->size() - marked.size() > 0) {
				while (!marked.empty()) {
					TermTool::remove(*term >> marked.top());
					marked.pop();
				}
			}
			else {
				TermTool::removeChildren(term);
				*term = sum;
			}
		}
	}
	if (*term == Multiplication)
	{
		auto marked = std::stack<int>();
		auto x = 1.0f;
		for (auto i = 0; i < term->size(); i++) {
			if (*term == Term::Type::Number) {
				x *= term->get(0)->toNumber();
				marked.push(i);
			}
		}

		if (marked.size() >= 2) {
			if (term->size() - marked.size() > 0) {
				while (!marked.empty()) {
					TermTool::remove(*term >> marked.top());
					marked.pop();
				}
			}
			else {
				TermTool::removeChildren(term);
				*term = x;
			}
		}
	}
	if (*term == Division)
	{
		auto res = term->get(0)->toNumber() / term->get(1)->toNumber();
		TermTool::removeChildren(term);
		*term = res;
	}
}

Term* transformation::complementVariable(const Term* term, char oldVar, char newVar)
{
	if (*term == Complement) {
		if (*term->get(0) == oldVar)
			return new Term(newVar);
	}
	return nullptr;
}

Term* transformation::complementExpression(const Term* term)
{
	if (*term == Complement)
	{
		if (term->get_ref(0) == Intersection || term->get_ref(0) == Union)
		{
			auto res = new Term(invertedSetOperation(term->toAction()));
			for (auto i = 0; i < term->get(0)->size(); i++) {
				auto c = new Term(Complement);
				*c << TermTool::copy(term->get(0)->get(i));
				*res << c;
			}
			return res;
		}
	}
	return nullptr;
}

Term* transformation::parantheses(const Term* term, action top, action bottom)
{
	if (*term == top)
	{
		auto count = 1;

		for (auto i = 0; i < term->size(); i++)
			if (*term->get(i) == bottom) count *= term->get(i)->size();
		if (count == 1) return nullptr;

		auto res = new Term(bottom);
		for (auto i = 0; i < count; i++)
			*res << top;

		auto tmp = 1;
		for (auto i = 0; i < term->size(); i++)
		{
			if (*term->get(i) == bottom)
			{
				for (auto m = 0; m < count;)
				{
					for (auto j = 0; j < term->get(i)->size(); j++)
					{
						for (auto l = 0; l < tmp; l++)
							res->get_ref(m++) << term->get(i)->get(j);
					}
				}
				tmp *= term->get(i)->size();
				continue;
			}
			for (size_t m = 0; m < count; m++) 
				res->get_ref(m) << term->get(i);
		}
		return res;
	}
	return nullptr;
}

Term* transformation::opposite(const Term* term, action act, char value)
{
	if (*term == act) {
		for (auto i = 0; i < term->size(); i++)
			if (term->get_ref(i) == Complement)
				if (TermTool::isContain(*term, term->get_ref(i).get_ref(0))) {
					return new Term(value);
				}
	}
	return nullptr;
}

Term* transformation::minus(const Term* term)
{
	if (*term == Minus)
	{
		auto res = new Term(Intersection);
		auto c = new Term(Complement);
		*c << term->get(1);
		*res << term->get(0) << c;

		return res;
	}
	return nullptr;
}

std::vector<Term*> transformation::formulaExcIncX2(const Term* term)
{
	auto result = std::vector<Term*>();
	if (*term == Quantity) {
		if (term->get_ref(0) == Union) {
			int mask = 1;
			int lim = (1 << term->get_ref(0).size()) - 1;
			for (; mask < lim; mask++) {
				Term *left = nullptr, *right = nullptr;
				if ((mask & (mask - 1)) == 0) {
					for (auto i = 0; i < term->get_ref(0).size(); i++)
						if ((mask >> i) & 1 == 0)
							left = TermTool::copy(term->get_ref(0)[i]);
				}
				else {
					left = new Term(Union);
					for (auto i = 0; i < term->get_ref(0).size(); i++) 
						if ((mask >> i) & 1 == 0)
							*left << TermTool::copy(term->get_ref(0)[i]);
				}
				if (((~mask & lim) & ((~mask & lim) - 1)) == 0) {
					for (auto i = 0; i < term->get_ref(0).size(); i++)
						if ((mask >> i) & 1 == 1)
							right = TermTool::copy(term->get_ref(0)[i]);
				}
				else {
					right = new Term(Union);
					for (auto i = 0; i < term->get_ref(0).size(); i++)
						if ((mask >> i) & 1 == 1)
							*right << TermTool::copy(term->get_ref(0)[i]);
				}

				auto res = new Term(Addition);
				auto _first = new Term(Quantity);
					*_first << TermTool::copy(left);
				auto _second = new Term(Quantity);
					*_second << TermTool::copy(right);
				auto _third = new Term(Multiplication);
					auto __third = new Term(Quantity);
						auto ___third = new Term(Intersection);
							*___third << TermTool::copy(left);
							*___third << TermTool::copy(right);
						*__third << ___third;
					*_third << new Term(-1.0f);
					*_third << __third;
				*res << _first << _second << _third;
				result.push_back(res);
			}
		}
	}
	return result;
}

std::vector<Term*> transformation::formulaExcIncX3(const Term* term)
{
	auto result = std::vector<Term*>();
	if (*term == Quantity) {
		if (term->get_ref(0) == Union) {
			int mask = 1;
			int lim = (1 << (term->get_ref(0).size() << 1)) - 1;
			for (; mask < lim; mask++) {
				auto f_count = 0;
				for (auto i = 0; i < term->get_ref(0).size(); i++)
					if (mask >> (i << 1) & 3 == 0) {
						f_count++;
						break;
					}
				if (f_count == 0) continue;
				auto s_count = 0;
				for (auto i = 0; i < term->get_ref(0).size(); i++)
					if (mask >> (i << 1) & 3 == 1) {
						s_count++;
						break;
					}
				if (s_count == 0) continue;
				auto t_count = 0;
				for (auto i = 0; i < term->get_ref(0).size(); i++)
					if (mask >> (i << 1) & 3 == 2) {
						t_count++;
						break;
					}
				if (t_count == 0) continue;
				auto is = true;
				for (auto i = 0; i < term->get_ref(0).size(); i++)
					if (mask >> (i << 1) & 3 == 3) {
						is = false;
						break;
					}
				if (!is) continue;

				Term *first = nullptr, *second = nullptr, *third = nullptr;
				if (f_count == 1) {
					for (auto i = 0; i < term->get_ref(0).size(); i++)
						if (mask >> (i << 1) & 3 == 0)
							first = TermTool::copy(term->get_ref(0)[i]);
				}
				else {
					first = new Term(Union);
					for (auto i = 0; i < term->get_ref(0).size(); i++)
						if (mask >> (i << 1) & 3 == 0)
							*first << TermTool::copy(term->get_ref(0)[i]);
				}

				if (s_count == 1) {
					for (auto i = 0; i < term->get_ref(0).size(); i++)
						if (mask >> (i << 1) & 3 == 1)
							second = TermTool::copy(term->get_ref(0)[i]);
				}
				else {
					second = new Term(Union);
					for (auto i = 0; i < term->get_ref(0).size(); i++)
						if (mask >> (i << 1) & 3 == 1)
							*second << TermTool::copy(term->get_ref(0)[i]);
				}

				if (t_count == 1) {
					for (auto i = 0; i < term->get_ref(0).size(); i++)
						if (mask >> (i << 1) & 3 == 2)
							third = TermTool::copy(term->get_ref(0)[i]);
				}
				else {
					third = new Term(Union);
					for (auto i = 0; i < term->get_ref(0).size(); i++)
						if (mask >> (i << 1) & 3 == 2)
							*third << TermTool::copy(term->get_ref(0)[i]);
				}

				auto res = new Term(Addition);
				auto _first = new Term(Quantity);
				*_first << TermTool::copy(first);
				auto _second = new Term(Quantity);
				*_second << TermTool::copy(second);
				auto _third = new Term(Quantity);
				*_third << TermTool::copy(third);

				auto _fourth = new Term(Multiplication);
				auto __fourth = new Term(Quantity);
				auto ___fourth = new Term(Intersection);
				*___fourth << TermTool::copy(first) << TermTool::copy(second);
				*__fourth << ___fourth;
				*_fourth << new Term(-1.0f) << __fourth;

				auto _fifth = new Term(Multiplication);
				auto __fifth = new Term(Quantity);
				auto ___fifth = new Term(Intersection);
				*___fifth << TermTool::copy(first) << TermTool::copy(third);
				*__fifth << ___fifth;
				*_fifth << new Term(-1.0f) << __fifth;

				auto _sixth = new Term(Multiplication);
				auto __sixth = new Term(Quantity);
				auto ___sixth = new Term(Intersection);
				*___sixth << TermTool::copy(second) << TermTool::copy(third);
				*__sixth << ___sixth;
				*_sixth << new Term(-1.0f) << __sixth;

				auto _seventh = new Term(Quantity);
				auto __seventh = new Term(Intersection);
				*__seventh << TermTool::copy(first) << TermTool::copy(second) << TermTool::copy(third);
				*_seventh << __seventh;

				*res << _first << _second << _third << _fourth << _fifth << _sixth << _seventh;

				result.push_back(res);
			}
		}
	}
	return result;
}

Term* transformation::indentical(const Term* term)
{
	if (*term == Addition) 
	{
		auto colletion = std::vector<std::pair<Term*, float>>();
		for (auto i = 0; i < term->size(); i++)
		{
			if (term->get_ref(i) == Quantity) {
				auto find = false;
				for (auto e : colletion) {
					if (TermTool::isEqual(*e.first, term->get_ref(0))) {
						e.second += 1;
						find = true;
						break;
					}
				}
				if (!find)
					colletion.push_back(std::make_pair(term->get(0), 1));
			}
			if (term->get_ref(i) == Multiplication) {
				if (term->get(0)->get_ref(0) == Quantity) {
					auto find = false;
					for (auto e : colletion) {
						if (TermTool::isEqual(*e.first, term->get(0)->get_ref(0))) {
							e.second += term->get(0)->get(1)->toNumber();
							find = true;
							break;
						}
					}
					if (!find)
						colletion.push_back(std::make_pair(term->get(0), term->get(0)->get(1)->toNumber()));
				}
				else {
					auto find = false;
					for (auto e : colletion) {
						if (TermTool::isEqual(*e.first, term->get(0)->get_ref(1))) {
							e.second += term->get(0)->get(0)->toNumber();
							find = true;
							break;
						}
					}
					if (!find)
						colletion.push_back(std::make_pair(term->get(0), term->get(0)->get(0)->toNumber()));
				}
			}
		}
		auto res = new Term(Addition);
		for (auto a: colletion) {
			if (a.second == 0) continue;
			if (a.second == 1) *res << TermTool::copy(a.first);
			else {
				auto tmp = new Term(Multiplication);
				*tmp << new Term(a.second) << TermTool::copy(a.first);
				*res << tmp;
			}
		}
		if (res->empty()) {
			TermTool::remove(res);
			return new Term(0.0f);
		}
		return res;
	}
	return nullptr;
}

Term* transformation::unisetComplement(const Term* term)
{
	if (*term == Quantity)
	{
		if (term->get_ref(0) == Complement) {
			auto res = new Term(Addition);
			auto cu = new Term(Quantity);
			*cu << new Term('U');

			auto mul = new Term(Multiplication);
			auto ce = new Term(Quantity);
			*ce << TermTool::copy(term->get(0)->get(0));
			*mul << new Term(0.0f) << ce;

			*res << cu << ce;

			return res;
		}
	}

	return nullptr;
}
