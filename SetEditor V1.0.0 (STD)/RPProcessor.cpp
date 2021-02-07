// ======================================== |Description| ======================================== //
// Главная проверки решения
// Использует 3 стадии проверки:
// 1. Проверка синтаксиса
// 2. Проверка значения
// 3. Проверка операции

// TODO: пересмотреть инклуды
// TODO: исправить [устарело]

// ======================================== |Includes| ======================================== //
#include "Header.h"
#include "Operations.h"
// ======================================== |Defines| ======================================== //
#include <iostream>
/* ========== Log ========== */
extern ofstream Log;
/* ========================= */
extern UserInfo MainUserInfo;

// ======================================== |Structs| ======================================== //


// ======================================== |Prototypes| ======================================== //

// ======================================== |Functions| ======================================== //

#define ERR_UNMARKED_PARTS_NOT_EQUAL			0x1000

// Возможные символы:
// Множества:					'A'..'Z'
#define isSet(c)				(c >= 'A' || c <= 'Z')
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

// Проверки
#define isNullChar(i)			(i == 0)
#define isEndChar(i)			(i == - 1)

// Ошибки:
#define ERR_NO_ERROR					0x0000
#define ERR_INVALID_LEFT_OPERAND		0x0100
#define ERR_INVALID_RIGHT_OPERAND		0x0200
#define ERR_NOT_FOUND_OPEN_BARKET		0x0300
#define ERR_NOT_FOUND_CLOSE_BARKET		0x0400
#define ERR_INCORRECT_BRACKET_POS		0x0500
#define ERR_SET_IN_NUMER_AREA			0x0600
#define ERR_NUMER_IN_SET_AREA			0x0700

#define ERR_NOT_EQUATION				0x0800

#define ERR_INVALID_DOT_POS				0x0900

#define ERR_INVALID_IMARKER_AREA_N1		0x1100
#define ERR_INVALID_IMARKER_AREA_N2		0x1200
#define ERR_INVALID_IMARKER_AREA_N3		0x1300
#define ERR_INVALID_IMARKER_AREA_N4		0x1400
#define ERR_INVALID_IMARKER_AREA_N5		0x1500
#define ERR_INVALID_IMARKER_AREA_N6		0x1600
#define ERR_INVALID_IMARKER_AREA_N7		0x1700
#define ERR_INVALID_IMARKER_AREA_N8		0x1800
#define ERR_INVALID_IMARKER_AREA_N9		0x1900

#define ERR_INVALID_OMARKER_AREA_N1		0x2100
#define ERR_INVALID_OMARKER_AREA_N2		0x2200
#define ERR_INVALID_OMARKER_AREA_N3		0x2300
#define ERR_INVALID_OMARKER_AREA_N4		0x2400
#define ERR_INVALID_OMARKER_AREA_N5		0x2500
#define ERR_INVALID_OMARKER_AREA_N6		0x2600
#define ERR_INVALID_OMARKER_AREA_N7		0x2700
#define ERR_INVALID_OMARKER_AREA_N8		0x2800
#define ERR_INVALID_OMARKER_AREA_N9		0x2900

#define ERR_UNDEFINED_EXPRESSION		0x0A00
#define ERR_INCORRECT_EXPRESSION		0x0B00

#define GetMarkerError(markerID, markerType) ((markerType << 7) | (markerID << 8))

#define INPUT_MARKER					0x20
#define OUTPUT_MARKER					0x40

#define LINE_TYPE_NULL 0
#define LINE_TYPE_NUM 1
#define LINE_TYPE_SET 2

int GetNextChar(const string& expression, int startPos)
{
	for (size_t i = startPos + 1; i < expression.size(); i++)
	{
		if (expression[i] == '[' || expression[i] == ']') continue;
		return i;
	}
	return -1;
}

int GetLineType(const string& line, int* lineType)
{
	for (size_t i = 0; i < line.size(); i++)
	{
		if (isSet(line[i])) {
			*lineType = LINE_TYPE_SET;
			return ERR_NO_ERROR;
		}
		if (isDigit(line[i])) {
			*lineType = LINE_TYPE_NUM;
			return ERR_NO_ERROR;
		}
		if (isDot(line[i])) {
			return ERR_INVALID_DOT_POS;
		}
		if (isBinarySetOperation(line[i])) {
			return ERR_INVALID_LEFT_OPERAND;
		}
		if (isUnaryNumOperation(line[i])) {
			*lineType = LINE_TYPE_NUM;
			return ERR_NO_ERROR;
		}
		if (isBinaryNumOperation(line[i])) {
			return ERR_INVALID_LEFT_OPERAND;
		}
		if (isStdOpenBracket(line[i])) continue;
		if (isNStdOpenBracket(line[i])) {
			*lineType = LINE_TYPE_SET;
			return ERR_NO_ERROR;
		}
		if (isCloseBracket(line[i])) {
			return ERR_INCORRECT_BRACKET_POS;
		}
		if (isSegment(line[i])) {
			return ERR_INVALID_LEFT_OPERAND;
		}
		if (isModule(line[i])) {
			*lineType = LINE_TYPE_NUM;
			return ERR_NO_ERROR;
		}
		if (isNumeration(line[i])) {
			*lineType = LINE_TYPE_NULL;
			return ERR_NO_ERROR;
		}
		if (isOther(line[i])) continue;
	}
	*lineType = LINE_TYPE_NULL;
	return ERR_NO_ERROR;
}

int CheckExpression(const string& expression)
{
	bool isSet = true;
	bool inNumArea = false;
	int segment = 0;
	stack<char> brackets;
	char nextX = 0;

	int error = 0;
	int lineType = 0;
	if ((error = GetLineType(expression, &lineType)) == ERR_NO_ERROR) return error;

	for (size_t i = 0; i < expression.size(); i++)
	{
		if (isSet(expression[i])) {
			if (inNumArea) {
				return ERR_SET_IN_NUMER_AREA;
			}
			nextX = GetNextChar(expression, i);
			if (isEndChar(nextX)) continue;

			if (isSetOperation(expression[nextX]) ||
				isCloseBracket(expression[nextX]) ||
				isModule(expression[nextX])) continue;

			if (isSegment(nextX) && isSet) continue;

			return ERR_INVALID_LEFT_OPERAND;
		}
		if (isDigit(expression[i])) {
			if (!inNumArea) {
				return ERR_NUMER_IN_SET_AREA;
			}
			nextX = GetNextChar(expression, i);
			if (isEndChar(nextX)) continue;

			if (isNumber(expression[nextX]) ||
				isNumOperation(expression[nextX]) ||
				isStdCloseBracket(expression[nextX])) continue;

			if (isSegment(nextX) && !isSet) continue;

			return ERR_INVALID_LEFT_OPERAND;
		}
		if (isDot(expression[i])) {
			if (!inNumArea) {
				return ERR_NUMER_IN_SET_AREA;
			}
			if (isNullChar(i)) {
				return ERR_INVALID_LEFT_OPERAND;
			}
			nextX = GetNextChar(expression, i);
			if (isEndChar(nextX)) {
				return ERR_INVALID_RIGHT_OPERAND;
			}

			if (isDigit(expression[nextX])) continue;

			return ERR_INVALID_RIGHT_OPERAND;
		}
		if (isBinarySetOperation(expression[i])) {
			if (inNumArea) {
				return ERR_SET_IN_NUMER_AREA;
			}
			if (isNullChar(i)) {
				return ERR_INVALID_LEFT_OPERAND;
			}
			nextX = GetNextChar(expression, i);
			if (isEndChar(nextX)) {
				return ERR_INVALID_RIGHT_OPERAND;
			}

			if (isSet(expression[nextX]) ||
				isOpenBracket(expression[nextX])) continue;

			return ERR_INVALID_RIGHT_OPERAND;
		}
		if (isUnaryNumOperation(expression[i])) {
			if (!inNumArea) {
				return ERR_NUMER_IN_SET_AREA;
			}
			nextX = GetNextChar(expression, i);
			if (isEndChar(nextX)) {
				return ERR_INVALID_RIGHT_OPERAND;
			}

			if (isDigit(expression[nextX]) ||
				isStdOpenBracket(expression[nextX])) continue;

			return ERR_INVALID_RIGHT_OPERAND;
		}
		if (isBinaryNumOperation(expression[i])) {
			if (!inNumArea) {
				return ERR_NUMER_IN_SET_AREA;
			}
			if (isNullChar(i)) {
				return ERR_INVALID_LEFT_OPERAND;
			}
			nextX = GetNextChar(expression, i);
			if (isEndChar(nextX)) {
				return ERR_INVALID_RIGHT_OPERAND;
			}

			if (isDigit(expression[nextX]) ||
				isStdOpenBracket(expression[nextX])) continue;

			return ERR_INVALID_RIGHT_OPERAND;
		}
		if (isStdOpenBracket(expression[i])) {
			brackets.push(expression[i]);

			nextX = GetNextChar(expression, i);
			if (isEndChar(nextX)) {
				return ERR_NOT_FOUND_CLOSE_BARKET;
			}

			if (isDigit(expression[nextX]) ||
				isSet(expression[nextX]) ||
				isOpenBracket(expression[nextX])) continue;

			return ERR_INCORRECT_BRACKET_POS;
		}
		if (isStdCloseBracket(expression[i])) {
			if (!isStdOpenBracket(brackets.top())) {
				return ERR_INCORRECT_BRACKET_POS;
			}
			brackets.pop();
			nextX = GetNextChar(expression, i);
			if (isNullChar(i)) {
				return ERR_NOT_FOUND_OPEN_BARKET;
			}
			if (isEndChar(nextX)) continue;
			if (isBinarySetOperation(expression[nextX]) ||
				isNumOperation(expression[nextX]) ||
				isCloseBracket(expression[nextX]) ||
				isSegment(expression[nextX])) continue;

			return ERR_INCORRECT_BRACKET_POS;
		}
		if (isNStdOpenBracket(expression[i])) {
			brackets.push(expression[i]);
			if (inNumArea) {
				return ERR_SET_IN_NUMER_AREA;
			}

			nextX = GetNextChar(expression, i);
			if (isEndChar(nextX)) {
				return ERR_NOT_FOUND_CLOSE_BARKET;
			}

			if (isSet(expression[nextX]) ||
				isOpenBracket(expression[nextX])) continue;

			return ERR_INVALID_RIGHT_OPERAND;
		}
		if (isNStdCloseBracket(expression[i])) {
			if (!isNStdOpenBracket(brackets.top())) {
				return ERR_INCORRECT_BRACKET_POS;
			}
			brackets.pop();
			if (inNumArea) {
				return ERR_SET_IN_NUMER_AREA;
			}

			nextX = GetNextChar(expression, i);
			if (isEndChar(nextX)) continue;

			if (isBinarySetOperation(expression[nextX]) ||
				isCloseBracket(expression[nextX]) ||
				isSegment(expression[nextX])) continue;

			return ERR_INVALID_RIGHT_OPERAND;
		}
		if (isSegment(expression[i])) {
			if (!brackets.empty()) {
				return ERR_INCORRECT_BRACKET_POS;
			}
			segment++;
			if (inNumArea) {
				return ERR_INCORRECT_BRACKET_POS;
			}

			nextX = GetNextChar(expression, i);
			if (isEndChar(nextX)) {
				return ERR_INVALID_RIGHT_OPERAND;
			}
			if (isSet) {
				if (isSet(expression[nextX]) ||
					isOpenBracket(expression[nextX])) continue;
			}
			else {
				if (isDigit(expression[nextX]) ||
					isStdOpenBracket(expression[nextX]) ||
					isModule(expression[nextX])) continue;
			}

			return ERR_INVALID_RIGHT_OPERAND;
		}
		if (isModule(expression[i])) {
			if (!inNumArea) {
				if (segment == 0) isSet = false;
				else if (isSet) {
					return ERR_NUMER_IN_SET_AREA;
				}
				brackets.push('<');
				inNumArea = true;
				nextX = GetNextChar(expression, i);
				if (isEndChar(nextX)) {
					return ERR_INVALID_RIGHT_OPERAND;
				}
				if (isSet(expression[nextX]) ||
					isOpenBracket(expression[nextX])) continue;

				return ERR_INVALID_RIGHT_OPERAND;
			}
			else {
				if (brackets.top() != '<') {
					return ERR_INCORRECT_BRACKET_POS;
				}
				brackets.pop();
				inNumArea = false;
				nextX = GetNextChar(expression, i);
				if (isNullChar(i)) {
					return ERR_NOT_FOUND_OPEN_BARKET;
				}
				if (isEndChar(nextX)) continue;
				if (isNumOperation(expression[nextX]) ||
					isStdCloseBracket(expression[nextX]) ||
					isSegment(expression[nextX])) continue;

				return ERR_INVALID_RIGHT_OPERAND;
			}
		}
		if (isOther(expression[i])) continue;
	}
	return 0;
}

int GetMarkers(const vector<int>& input, const vector<int>& output)
{
	int inputMarkers = 0;
	int outputMarkers = 0;
	for (size_t i = 0; i < input.size(); i++)
		inputMarkers |= input[i];
	for (size_t i = 0; i < output.size(); i++)
		outputMarkers |= output[i];
	return inputMarkers & outputMarkers;
}

void Split(const string& text, const vector<int>& codes, const int markers, string* nomarkered, vector<string>* markered)
{
	bool isFirst = true;
	for (size_t i = 1; i < DARK_MARKERS + LIGHT_MARKERS; i <<= 1)
	{
		if ((markers & i) == 0) continue;
		markered->push_back(string());
		for (size_t j = 0; j < text.size(); j++)
		{
			if (codes[j] & i) {
				if (isFirst) {
					nomarkered->push_back(markered->size() - 1);
					isFirst = false;
				}
				markered->back().push_back(text[j]);
				continue;
			}
			nomarkered->push_back(text[j]);
		}
	}
}

int GetLine(const CTable* table, int num)
{
	for (size_t i = 0; i < table->Lines.size(); i++)
	{
		if (table->Lines[i].number == num) return i;
	}
	return -1;
}

#if 0 

void NormalForm(term_ptr pTerm)
{
	// Спуск вниз //
	for (size_t i = 0; i < pTerm->size(); i++) NormalForm(pTerm->get(i));
	// Удаление ассоциативности //
	if (pTerm->is(OA_2, term_type::operation) || pTerm->is(OA_3, term_type::operation) || pTerm->is(OS_2, term_type::operation) || pTerm->is(OS_3, term_type::operation))
	{
		for (int i = pTerm->size() - 1; i >= 0; i--)
		{
			if (pTerm->get(i)->is(pTerm->_value, term_type::operation)) {
				for (size_t j = 0; j < pTerm->get(i)->size(); j++) pTerm->add(pTerm->get(i)->get(j));
				pTerm->remove(i);
			}
		}
	}
}

bool IsConclusion(term_ptr pTerm_1, term_ptr pTerm_2, vector<vector<term_ptr>>* connection)
{
	int inputLine = -1;
	int outputLine = -2;
	for (size_t i = 0; i < connection->size(); i++)
	{
		for (size_t j = 0; j < connection->at(i).size(); j++)
		{
			if (Equal(pTerm_1, connection->at(i).at(j))) inputLine = i;
			if (Equal(pTerm_2, connection->at(i).at(j))) outputLine = i;
		}
	}
	return inputLine == outputLine;
}
#endif