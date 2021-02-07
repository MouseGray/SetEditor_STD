#pragma once

#include <string>

// TODO: change
enum class ExpressionType : bool
{
    Set,
    Num
};

ExpressionType variableType(const char ch);

enum action : char
{
    Undefined = '\0',
    Complement = '{',
    Intersection = '#',
    Union = '$',
    Minus = '^',
    Subtraction = '-',
    Addition = '+',
    Multiplication = '*',
    Division = '/',

    ComplementEnd = '}',

    Quantity = '[',
    QuantityEnd = ']',
    Faction = '(',
    FactionEnd = ')',

    Dot = '.',

    Equal = '='
};

constexpr char Uniset = 'U';
constexpr char Nullset = '@';

enum class Area : unsigned char
{
    Null,
    Set,
    Num
};

enum class TextError
{
    NotFoundOpenParenthese,
    NotFoundClosingParenthese,

    UnexpectedNumOperation,
    UnexpectedSetOperation,

    UnexpectedOpenParenthese,
    UnexpectedClosingParenthese,

    UnexpectedVariable,
    UnexpectedNumber,
    UnexpectedDigit,

    IncorrectDigit,

    NotFoundLeftOperand,
    NotFoundRightOperand,

    SetVariableInNumArea,
    NumVariableInSetArea,

    DigitInSetArea,
    DotInSetArea
};

bool isVariable(const char ch);
bool isSetOperation(const char ch);
bool isUnaryNumOperation(const char ch);
bool isBinaryNumOperation(const char ch);
bool isOpenParenthese(const char ch);
bool isClosingParenthese(const char ch);
bool isDigit(const char ch);
bool isNumber(const char ch);

char invertedParenthese(const char ch);

action invertedSetOperation(const action act);
