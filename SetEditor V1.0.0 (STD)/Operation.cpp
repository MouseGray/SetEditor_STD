#include "Operation.h"

action invertedSetOperation(const action act)
{
	switch (act)
	{
		case action::Intersection:
			return action::Union;
		case action::Union:
			return action::Intersection;
		default:
			return act;
	}
	return act;
}

ExpressionType variableType(const char ch)
{
	return ExpressionType::Set;
}

bool isVariable(const char ch)
{
	return ('A' <= ch && ch <= 'Z') || ch == Nullset;
}

bool isSetOperation(const char ch)
{
	return ch == static_cast<char>(Intersection) || ch == static_cast<char>(Union) || ch == static_cast<char>(Minus);
}

bool isUnaryNumOperation(const char ch)
{
	return ch == static_cast<char>(Subtraction);
}

bool isBinaryNumOperation(const char ch)
{
	return ch == static_cast<char>(Addition) || ch == static_cast<char>(Multiplication) || ch == static_cast<char>(Division);
}

bool isOpenParenthese(const char ch)
{
	return ch == static_cast<char>(Faction) || ch == static_cast<char>(Complement) || ch == static_cast<char>(Quantity);
}

bool isClosingParenthese(const char ch)
{
	return ch == static_cast<char>(FactionEnd) || ch == static_cast<char>(ComplementEnd) || ch == static_cast<char>(QuantityEnd);
}

bool isDigit(const char ch)
{
	return '0' <= ch && ch <= '9';
}

bool isNumber(const char ch)
{
	return isDigit(ch) || ch == static_cast<char>(Dot);
}

char invertedParenthese(const char ch)
{
	switch (ch)
	{
		case '{': return '}';
		case '}': return '{';
		case '(': return ')';
		case ')': return '(';
		case '[': return ']';
		case ']': return '[';
	}
	return '\0';
}


