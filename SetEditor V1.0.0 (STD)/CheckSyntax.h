#pragma once
#include <string>
#include <vector>
#include <stack>

using namespace std;

// Возможные символы:
// Множества:					'A'..'Z'
#define isSet(c)				(c >= 'A' && c <= 'Z' || c == '!' || c == '@')
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
#define isEndLine(c)			(c == '\r')
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

#define ERR_NO_ERROR					0x0A0000
#define ERR_NO_TESTED					0x000000

#define ERR_SET_IN_NUMER_AREA			0x060000
#define ERR_NUMER_IN_SET_AREA			0x070000

#define ERR_NOT_FOUND_OPEN_BARKET		0x030000
#define ERR_NOT_FOUND_CLOSE_BARKET		0x040000
#define ERR_INCORRECT_BRACKET_POS		0x050000

#define ERR_INCORRECT_NUMBER			0x080000
#define ERR_INCORRECT_SET				0x090000

#define ERR_INVALID_LEFT_OPERAND		0x010000
#define ERR_INVALID_RIGHT_OPERAND		0x020000

#define isFirstChar(i)			(i == 0)
#define isLastChar(i)			(i == -1)

#define SetError(code, err)		((code &= 0x0000FFFF) |= err)

#define LT_UNDEFINED	0
#define LT_SET			1
#define LT_NUM			2

#define ST_UNDEFINED	LT_UNDEFINED
#define ST_SET			LT_SET
#define ST_NUM			LT_NUM

class CheckSyntax
{
public:
	CheckSyntax(const string* text);
	CheckSyntax(const string* text, vector<int>* codes);

	bool MarkErrors();
	bool Check();

private:
	inline void SetSegmentType(__int8 segmenType);
	inline char GetNextChar();
	void GetLineType();
	void StartLine();
	int isSet_();
	int isDigit_();
	int isDot_();
	int isBinarySetOperation_();
	int isUnaryNumOperation_();
	int isBinaryNumOperation_();
	int isStdOpenBracket_();
	int isStdCloseBracket_();
	int isNStdOpenBracket_();
	int isNStdCloseBracket_();
	int isModule_();
	int isSegment_();
	int isEndLine_();

	__int8			segmentType = ST_UNDEFINED;
	__int8			lineType = LT_UNDEFINED;

	int				startPos = 0;
	int				curPos = 0;
	int				endPos = 0;

	char			nextChar = '\0';

	const string* text = nullptr;
	vector<int>* codes = nullptr;

	stack<char>		brackets;
};