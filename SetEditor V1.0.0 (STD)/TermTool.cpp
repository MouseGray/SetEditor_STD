#include "TermTool.h"

term_ptr TermTool::CreateTerm(const string& expression)
{
	stack<term_ptr> termStack;
	term_ptr result = nullptr;
	bool hasOpenModule = false;
	size_t nextChar = 0;
	for (size_t i = 0; i < expression.size(); i++)
	{
		// --------------- Преобразование строки в число --------------- //
		if (expression[i] >= '0' && expression[i] <= '9') {
			result = make_shared<Term>(stod(expression.substr(i), &nextChar), term_type::number);
			i += nextChar;
		}
		// --------------- Преобразование в Term запись --------------- //
		switch (expression[i])
		{
			case DifferenceSign: case DivisionSign: case EqualSign:
			{
				while (!termStack.empty() && GetPriority(expression[i]) <= GetPriority((char)termStack.top()->_value))
				{
					termStack.top()->add(result);
					result = termStack.top();
					termStack.pop();
				}
				termStack.push(make_shared<Term>(expression[i], term_type::operation));
				termStack.top()->add(result);
				break;
			}
			case IntersectionSign: case UnionSign: case AdditionSign: case MultiplicationSign:
			{
				while (!termStack.empty() && GetPriority(expression[i]) <= GetPriority((char)termStack.top()->_value))
				{
					termStack.top()->add(result);
					result = termStack.top();
					termStack.pop();
				}
				if (result->_value == expression[i])
				{
					termStack.push(result);
					result = nullptr;
				}
				else
				{
					termStack.push(make_shared<Term>(expression[i], term_type::operation));
					termStack.top()->add(result);
				}
				break;
			}
			case SubtractionSign:
			{
				if (IsBinaryMinus(expression, i)) {
					while (!termStack.empty() && GetPriority(AdditionSign) <= GetPriority((char)termStack.top()->_value))
					{
						termStack.top()->add(result);
						result = termStack.top();
						termStack.pop();
					}
					if (result->_value == AdditionSign)
					{
						termStack.push(result);
						result = nullptr;
					}
					else
					{
						termStack.push(make_shared<Term>(AdditionSign, term_type::operation));
						termStack.top()->add(result);
					}
				}
				termStack.push(make_shared<Term>(expression[i], term_type::operation));
				break;
			}
			case OpenBracket:
			{
				termStack.push(make_shared<Term>(expression[i], term_type::operation));
				break;
			}
			case CloseBracket:
			{
				while (!termStack.empty() && ((char)termStack.top()->_value != OpenBracket))
				{
					termStack.top()->add(result);
					result = termStack.top();
					termStack.pop();
				}
				termStack.pop();
				break;
			}
			case ModuleSign:
			{
				if (hasOpenModule)
				{
					while (!termStack.empty() && ((char)termStack.top()->_value != ModuleSign))
					{
						termStack.top()->add(result);
						result = termStack.top();
						termStack.pop();
					}
					termStack.top()->add(result);
					result = termStack.top();
					termStack.pop();
					hasOpenModule = false;
				}
				else
				{
					termStack.push(make_shared<Term>(ModuleSign, term_type::operation));
					hasOpenModule = true;
				}
				break;
			}
			case ComplementSign:
			{
				termStack.push(make_shared<Term>(expression[i], term_type::operation));
				break;
			}
			case CloseComplementBracket:
			{
				while (!termStack.empty() && ((char)termStack.top()->_value != ComplementSign))
				{
					termStack.top()->add(result);
					result = termStack.top();
					termStack.pop();
				}
				termStack.top()->add(result);
				result = termStack.top();
				termStack.pop();
				break;
			}
			default: result = make_shared<Term>(expression[i], term_type::symbol);
		}
	}
	while (!termStack.empty())
	{
		termStack.top()->add(result);
		result = termStack.top();
		termStack.pop();
	}

	return result;
}

void TermTool::ToNormalForm(term_ptr pterm)
{
	for (size_t i = 0; i < pterm->size(); i++) ToNormalForm(pterm->get(i));

	if (pterm->is(AdditionSign, term_type::operation) || pterm->is(MultiplicationSign, term_type::operation) || 
		pterm->is(IntersectionSign, term_type::operation) || pterm->is(UnionSign, term_type::operation))
	{
		for (int i = pterm->size() - 1; i >= 0; i--) {
			if (pterm->get(i)->is(pterm->_value, term_type::operation)) {
				for (size_t j = 0; j < pterm->get(i)->size(); j++) pterm->add(pterm->get(i)->get(j));
				pterm->remove(i);
			}
		}
	}
}

bool TermTool::Equal(const term_ptr& pterm_1, const term_ptr& pterm_2, bool(*F)(const term_ptr&, const term_ptr&))
{
	if (pterm_1 == nullptr || pterm_2 == nullptr) return pterm_1 == pterm_2;

	if (F(pterm_1, pterm_2)) return true;
	if (*pterm_1 != *pterm_2) return false;
	if (!pterm_1->size()) return true;

	vector<bool> checked(pterm_2->size(), false);
	size_t i, j;
	for (i = 0; i < pterm_1->size(); i++) {
		for (j = 0; j < pterm_2->size(); j++) {
			if (checked[j]) continue;
			if (Equal(pterm_1->get(i), pterm_2->get(j), F)) {
				checked[j] = true;
				break;
			}
		}
		if (j == pterm_2->size()) return false;
	}
	for (auto b : checked) if (!b) return false;
	return true;
}

bool TermTool::Equal(const term_ptr& pterm_1, const term_ptr& pterm_2)
{
	if (pterm_1 == nullptr || pterm_2 == nullptr) return pterm_1 == pterm_2;

	if (*pterm_1 != *pterm_2) return false;
	if (!pterm_1->size()) return true;

	vector<bool> checked(pterm_2->size(), false);
	size_t i, j;
	for (i = 0; i < pterm_1->size(); i++) {
		for (j = 0; j < pterm_2->size(); j++) {
			if (checked[j]) continue;
			if (Equal(pterm_1->get(i), pterm_2->get(j))) {
				checked[j] = true;
				break;
			}
		}
		if (j == pterm_2->size()) return false;
	}
	for (auto b : checked) if (!b) return false;
	return true;
}

term_ptr TermTool::Copy(const term_ptr& pterm)
{
	if (pterm == nullptr) return nullptr;
	term_ptr result = make_shared<Term>(pterm->_value, pterm->_type);
	for (size_t i = 0; i < pterm->size(); i++) result->add(Copy(pterm->get(i)));
	return result;
}

bool TermTool::Calculate(const term_ptr& pterm, double* value)
{
	double _value = 0;
	if (pterm->_type == term_type::number) {
		*value = pterm->_value;
		return true;
	}
	if (pterm->is(SubtractionSign, term_type::operation, 1)) {
		if (!Calculate(pterm->get(0), &_value)) return false;
		*value = -_value;
		return true;
	}
	if (pterm->is(AdditionSign, term_type::operation)) {
		*value = 0;
		for (size_t i = 0; i < pterm->size(); i++) {
			if (!Calculate(pterm->get(i), &_value)) return false;
			*value += _value;
		}
		return true;
	}
	if (pterm->is(MultiplicationSign, term_type::operation)) {
		*value = 1;
		for (size_t i = 0; i < pterm->size(); i++) {
			if (!Calculate(pterm->get(i), &_value)) return false;
			*value *= _value;
		}
		return true;
	}
	if (pterm->is(DivisionSign, term_type::operation, 2)) {
		if (!Calculate(pterm->get(0), &_value)) return false;
		*value = _value;
		if (!Calculate(pterm->get(1), &_value)) return false;
		*value /= _value;
		return true;
	}
	return false;
}

void TermTool::Collect(term_ptr& pterm_1, term_ptr& pterm_2)
{
	if (pterm_1 == nullptr || pterm_2 == nullptr) return;
	if (!pterm_1->is(pterm_2)) return;

	for (int i = pterm_1->size() - 1; i >= 0; i--)
	{
		for (size_t j = 0; j < pterm_2->size(); j++)
		{
			if (Equal(pterm_1->get(i), pterm_2->get(j))) {
				pterm_1->remove(i);
				pterm_2->remove(j);
				break;
			}
		}
	}
}

bool TermTool::ToCell(const term_ptr& pterm, double* coef, term_ptr* resTerm)
{
	double _value = 0;
	term_ptr _resTerm = nullptr;
	if (pterm->_type == term_type::number) {
		*coef = pterm->_value;
		return true;
	}
	if (pterm->is(ModuleSign, term_type::operation, 1)) {
		*coef = 1;
		*resTerm = pterm;
		return true;
	}
	if (pterm->is(SubtractionSign, term_type::operation, 1)) {
		if (!ToCell(pterm->get(0), &_value, &_resTerm)) return false;
		*coef = -_value;
		*resTerm = _resTerm;
		return true;
	}
	if (pterm->is(AdditionSign, term_type::operation)) {
		*coef = 0;
		for (size_t i = 0; i < pterm->size(); i++) {
			if (!ToCell(pterm->get(i), &_value, &_resTerm)) return false;
			*coef += _value;
			if(_resTerm != nullptr) *resTerm = _resTerm;
		}
		return true;
	}
	if (pterm->is(MultiplicationSign, term_type::operation)) {
		*coef = 1;
		for (size_t i = 0; i < pterm->size(); i++) {
			if (!ToCell(pterm->get(i), &_value, &_resTerm)) return false;
			*coef *= _value;
			if (_resTerm != nullptr) *resTerm = _resTerm;
		}
		return true;
	}
	if (pterm->is(DivisionSign, term_type::operation, 2)) {
		if (!ToCell(pterm->get(0), &_value, &_resTerm)) return false;
		if (_resTerm != nullptr) *resTerm = _resTerm;
		*coef = _value;
		if (!ToCell(pterm->get(1), &_value, &_resTerm)) return false;
		if (_resTerm != nullptr) *resTerm = _resTerm;
		*coef /= _value;
		return true;
	}
	return false;
}

void TermTool::PrintTerm(const term_ptr& pterm)
{
	ofstream out("C:\\Users\\Admin\\Desktop\\Term.txt", ios_base::app);
	if (out.is_open()) {
		_PrintTerm(out, pterm);
		out << "asd" << endl;
		out.close();
	}
}

void TermTool::_PrintTerm(ofstream& stream, const term_ptr& pterm)
{
	if (pterm->_type == term_type::number) stream << " " << pterm->_value << "[";
	else  stream << " " << (char)pterm->_value << "[";

	for (size_t i = 0; i < pterm->size(); i++) {
		_PrintTerm(stream, pterm->get(i));
		stream << ",";
	}
	stream << "]";
}

bool TermTool::IsBinaryMinus(const string& str, size_t pos)
{
	if (!pos) return false;
	pos--;
	return str[pos] >= '0' && str[pos] <= '9' || str[pos] == '|' || str[pos] == ')';
}

int TermTool::GetPriority(char operation)
{
	switch (operation) {
		case ComplementSign: return 3;
		case CloseComplementBracket: return 2;
		case IntersectionSign: return 9;
		case UnionSign: return 8;
		case DifferenceSign: return 7;

		case ModuleSign: return 6;
		case SubtractionSign: return 5;
		case MultiplicationSign: return 4;
		case AdditionSign: return 3;

		case OpenBracket: return 2;
		case CloseBracket: return 1;
		case EqualSign: return 0;
	}
	return 0;
}