#include "DATLib.h"

bool IsBinaryMinus(const char* str, size_t i)
{
	if (i == 0) return false;
	i--;
	return str[i] >= 'A' && str[i] <= 'Z' || str[i] >= '0' && str[i] <= '9' || str[i] == '|' || str[i] == ')';
}

#define IsSymbol(a) (a >= 'A' && a <= 'Z' || a == '!' || a == '@')
#define IsDigit(a) (a >= '0' && a <= '9')
#define IsDigitOrDot(a) (a >= '0' && a <= '9' || a == '.')
#define IsSetOperation(a) (a == '~' || a == '#' || a == '$' || a == '^')
#define IsSetUnaryOperation(a) (a == '~')
#define IsSetBinaryOperation(a) (a == '#' || a == '$' || a == '^')
#define IsNumberOperation(a) (a == '-' || a == '+' || a == '*' || a == '/')
#define IsNumberUnaryOperation(a) (a == '-')
#define IsNumberBinaryOperation(a) (a == '+' || a == '*' || a == '/')

#define ONLY_SET												0
#define ONLY_NUMBER												1
#define ALL														2

int ComplementSelect(int start, int complement, vector<byte_t>* markers)
{
	int marker = markers->at(start);
	for (size_t j = start; j < markers->size(); j++) {
		if (markers->at(j) < marker) {
			if (markers->at(j) == 0) {
				if (markers->at(j) >> 4 + complement) return marker;
				else return 0;
			}
			else marker = markers->at(j);
		}
		else if (markers->at(j) >> 4 + complement) return marker;
	}
}

void RecomplementString(string* str, vector<byte_t>* markers, string* str_result, vector<byte_t>* marker_result)
{
	int marker = 0;
	int current = 0;
	for (size_t i = 0; i < str->size(); i++) {
		if (markers->at(i) >> 4 > current) {
			
			for (; current < markers->at(i) >> 4; current++) {
				marker = ComplementSelect(i, current, markers);
				*str_result += OS_1;
				*str_result += '(';
				marker_result->push_back(marker);
				marker_result->push_back(marker);
			}
		}
		if (markers->at(i) >> 4 < current) {
			for (; current > markers->at(i) >> 4; current--) {
				*str_result += ')';
				marker_result->push_back(marker);
			}
		}
		*str_result += str->at(i);
		marker_result->push_back(markers->at(i));
	}
	for (size_t i = 0; i < (markers->back() >> 4); i++) {
		*str_result += ')';
		marker_result->push_back(marker);
	}
}

// Возможные символы:
// Множества:					'A'..'Z'
#define isSet(c)				(c >= 'A' && c <= 'Z' || a == '!' || a == '@')
// Числа:						'0'..'9', '.'
#define isDigit(c)				(c >= '0' && c <= '9')
#define isDot(c)				(c == '.')
#define isNumber(c)				(isDigit(c) || isDot(c))
// Операции над множествами:	'~', '#', '$', '^'
#define isSetOperation(c)		(c == '~' || c == '#' || c == '$' || c == '^')
#define isUnarySetOperation(c)	(c == '~')
#define isBinarySetOperation(c)	(c == '#' || c == '$' || c == '^')
// Операции над числами:		'-', '+', '*', '/'
#define isNumOperation(c)		(c == '-' || c == '+' || c == '*' || c == '/')
#define isUnaryNumOperation(c)	(c == '-')
#define isBinaryNumOperation(c)	(c == '+' || c == '*' || c == '/')
// Скобки:						'(', ')', '{', '}'
#define isOpenBracket(c)		(c == '(' || c == '{')
#define isStdOpenBracket(c)		(c == '(')
#define isNStdOpenBracket(c)	(c == '{')
#define isCloseBracket(c)		(c == ')' || c == '}')
#define isStdCloseBracket(c)	(c == ')')
#define isNStdCloseBracket(c)	(c == '}')
// Сегмент:						'='
#define isSegment(c)			(c == '=')
// Модуль:						'|'
#define isModule(c)				(c == '|')
// Нумерация:					':', '<', '>'
#define	isNumeration(c)			(c == ':' || c == '<' || c == '>')
#define isSeparator(c)			(c == ':')
#define isNumerBracket(c)		(c == '<' || c == '>')
#define isNumerOpenBracket(c)	(c == '<')
#define isNumerCloseBracket(c)	(c == '>')
// Прочее:						'[', ']'
#define isOther(c)				(c == '[' || c == ']')

term_ptr TermCoder(const string& str, int* error)
{
	string string_of_number;
	stack<term_ptr> stck;
	term_ptr result = nullptr;

	bool opbr = false;
	for (size_t i = 0; i < str.size(); i++)
	{
		// --------------- Преобразование строки в число --------------- //
		if(isNumber(str[i]))
		{
			string_of_number += str[i];
			continue;
		}
		if (!string_of_number.empty())
		{
			result = make_shared<Term>(stod(string_of_number), term_type::number);
			string_of_number.clear();
		}
		// --------------- Преобразование в DAT запись --------------- //
		switch (str[i])
		{
			case OS_4: case OA_4:
			{
				while (!stck.empty() && GetPriority(str[i]) >= GetPriority((char)stck.top()->_value))
				{
					stck.top()->add(result);
					result = stck.top();
					stck.pop();
				}
				stck.push(make_shared<Term>(str[i], term_type::operation));
				stck.top()->add(result);
				break;
			}
			case OS_2: case OS_3: case OA_2: case OA_3:
			{
				while (!stck.empty() && GetPriority(str[i]) >= GetPriority((char)stck.top()->_value))
				{
					stck.top()->add(result);
					result = stck.top();
					stck.pop();
				}
				if (result->_value == str[i])
				{
					stck.push(result);
					result = nullptr;
				}
				else
				{
					stck.push(make_shared<Term>(str[i], term_type::operation));
					stck.top()->add(result);
				}
				break;
			}
			case OA_1:
			{
				if (IsBinaryMinus(str.data(), i)) {
					while (!stck.empty() && GetPriority(OA_2) >= GetPriority((char)stck.top()->_value))
					{
						stck.top()->add(result);
						result = stck.top();
						stck.pop();
					}
					if (result->_value == OA_2)
					{
						stck.push(result);
						result = nullptr;
					}
					else
					{
						stck.push(make_shared<Term>(OA_2, term_type::operation));
						stck.top()->add(result);
					}
				}
				stck.push(make_shared<Term>(str[i], term_type::operation));
				break;
			}
			case OB_1:
			{
				stck.push(make_shared<Term>(str[i], term_type::operation));
				break;
			}
			case OB_2:
			{
				while (!stck.empty() && GetPriority(str[i]) >= GetPriority((char)stck.top()->_value))
				{
					stck.top()->add(result);
					result = stck.top();
					stck.pop();
				}
				stck.pop();
				break;
			}
			case OB__:
			{
				if (opbr)
				{
					while (!stck.empty() && GetPriority(OB_4) >= GetPriority((char)stck.top()->_value))
					{
						stck.top()->add(result);
						result = stck.top();
						stck.pop();
					}
					stck.push(make_shared<Term>(str[i], term_type::operation));
					//stck.top()->add(result);
					opbr = false;
				}
				else
				{
					stck.push(make_shared<Term>(OB_3, term_type::operation));
					opbr = true;
				}
				break;
			}
			case '{':
			{
				stck.push(make_shared<Term>(str[i], term_type::operation));
				break;
			}
			case '}':
			{
				while (!stck.empty() && ((char)stck.top()->_value != '{'))
				{
					stck.top()->add(result);
					result = stck.top();
					stck.pop();
				}
				stck.pop();
				stck.push(make_shared<Term>(OS_1, term_type::operation));
				break;
			}
			default:
			{
				result = make_shared<Term>(str[i], term_type::symbol);
			}
		}
	}
	if (!string_of_number.empty())
	{
		result = make_shared<Term>(stod(string_of_number), term_type::number);
		string_of_number.clear();
	}
	while (!stck.empty())
	{
		stck.top()->add(result);
		result = stck.top();
		stck.pop();
	}

	return result;
}

Vec TermCalculate(term_ptr pTerm, vector<Vec>* nVecs, int system_size, int* error)
{
	if (pTerm->_type == term_type::number) {
		return Vec(vec_type::number, pTerm->_value, system_size);
	}
	if (pTerm->_type == term_type::symbol) {
		size_t k;
		for (k = 0; k < nVecs->size(); k++)
		{
			if (nVecs->at(k).code() == pTerm->_value) {
				Vec result(nVecs->at(k));
				return result;
			}
		}
		if (k == nVecs->size())
		{
			*error = ERR_FOUNT_INVALID_SET;
			return Vec(system_size);
		}
	}
	if (pTerm->_type == term_type::operation) {
		switch ((char)pTerm->_value)
		{
			case OS_1: {
				Vec result(~TermCalculate(pTerm->get(0), nVecs, system_size, error));
				return result;
			}
			case OS_2: {
				Vec result(TermCalculate(pTerm->get(0), nVecs, system_size, error));
				for (size_t i = 1; i < pTerm->size(); i++) {
					result = result & TermCalculate(pTerm->get(i), nVecs, system_size, error);
				}
				return result;
			}
			case OS_3: {
				Vec result(TermCalculate(pTerm->get(0), nVecs, system_size, error));
				for (size_t i = 1; i < pTerm->size(); i++) {
					result = result | TermCalculate(pTerm->get(i), nVecs, system_size, error);
				}
				return result;
			}
			case OS_4: {
				Vec result(TermCalculate(pTerm->get(0), nVecs, system_size, error));
				for (size_t i = 1; i < pTerm->size(); i++) {
					result = result % TermCalculate(pTerm->get(i), nVecs, system_size, error);
				}
				return result;
			}
			case OA_2: {
				Vec result(TermCalculate(pTerm->get(0), nVecs, system_size, error));
				for (size_t i = 1; i < pTerm->size(); i++) {
					result = result + TermCalculate(pTerm->get(i), nVecs, system_size, error);
				}
				return result;
			}
			case OA_3: {
				Vec result(TermCalculate(pTerm->get(0), nVecs, system_size, error));
				for (size_t i = 1; i < pTerm->size(); i++) {
					result = result * TermCalculate(pTerm->get(i), nVecs, system_size, error);
				}
				return result;
			}
			case OA_4: {
				Vec result(TermCalculate(pTerm->get(0), nVecs, system_size, error));
				for (size_t i = 1; i < pTerm->size(); i++) {
					result = result / TermCalculate(pTerm->get(i), nVecs, system_size, error);
				}
				return result;
			}
			case OB__: {
				Vec result(TermCalculate(pTerm->get(0), nVecs, system_size, error));
				result.type(vec_type::number);
				return result;
			}
		}
	}
	return Vec(system_size);
}

term_ptr _Copy_(term_ptr pTerm)
{
	if (pTerm == nullptr) return nullptr;

	term_ptr receiver = make_shared<Term>(*pTerm);
	receiver->remove_all();

	for (size_t i = 0; i < pTerm->size(); i++) receiver->add(_Copy_(pTerm->get(i)));

	return receiver;
}

size_t GetDeep(term_ptr pTerm)
{
	if (pTerm == nullptr) return 0;

	size_t deep = 0;
	for (size_t i = 0; i < pTerm->size(); i++)
		deep = max(deep, GetDeep(pTerm->get(i)));
	return deep + 1;
}

int GetMaxDeep(term_ptr pTerm)
{
	if (pTerm == nullptr) return -1;

	size_t deep = 0;
	size_t maxDeep = 0;

	int indexMaxDeep = -1;

	for (size_t i = 0; i < pTerm->size(); i++)
	{
		deep = GetDeep(pTerm->get(i));
		if (deep > maxDeep)
		{
			maxDeep = deep;
			indexMaxDeep = i;
		}
	}
	return indexMaxDeep;
}

term_ptr TrimTop(term_ptr pTerm)
{
	if (pTerm == nullptr) return nullptr;
	if (pTerm->size() != 1) return nullptr;
	return pTerm->get(0);
}

bool Equal(term_ptr pTerm_1, term_ptr pTerm_2)
{
	size_t j, i;
	__int64 e, line = 0xFFFFFFFFFFFFFFFF;
	
	if (pTerm_1 == nullptr || pTerm_2 == nullptr)
		return pTerm_1 == pTerm_2;

	if (*pTerm_1 != *pTerm_2) return false;
	if (pTerm_1->size() == 0) return true;

	if (pTerm_1->size() > 64) throw 0x0311;

	line <<= pTerm_1->size();
	for (i = 0; i < pTerm_1->size(); i++)
	{
		for (j = 0, e = 1; j < pTerm_2->size(); j++, e <<= 1)
		{
			if (line & e) continue;
			if (Equal(pTerm_1->get(i), pTerm_2->get(j)))
			{
				line |= e;
				break;
			}
		}
		if (j == pTerm_2->size()) return false;
	}
	if (~line) return false;

	return true;
}