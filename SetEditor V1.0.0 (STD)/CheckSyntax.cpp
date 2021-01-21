#include "CheckSyntax.h"

inline void CheckSyntax::SetSegmentType(__int8 segmentType)
{
	if (this->segmentType != ST_UNDEFINED) return;
	this->segmentType = segmentType;
}

inline char CheckSyntax::GetNextChar()
{
	if (curPos > (int)text->size() - 2) return -1;
	return text->at(curPos + 1);
}

void CheckSyntax::GetLineType()
{
	lineType = LT_UNDEFINED;
	for (size_t i = (curPos == 0 ? curPos : curPos + 1); i < text->size(); i++) {
		if (text->at(i) == '\r') break;
		if (isSet(text->at(i)) || isNStdOpenBracket(text->at(i))) {
			lineType = LT_SET;
			break;
		}
		if (isDigit(text->at(i)) || isModule(text->at(i))) {
			lineType = LT_NUM;
			break;
		}
	}
}

void CheckSyntax::StartLine()
{
	GetLineType();
	segmentType = ST_UNDEFINED;
}

int CheckSyntax::isSet_()
{
	if (!isSet(text->at(curPos))) return ERR_NO_TESTED;
	SetSegmentType(ST_SET);
	if (segmentType != ST_SET) return ERR_SET_IN_NUMER_AREA;
	nextChar = GetNextChar();
	if (isLastChar(nextChar) ||
		isSetOperation(nextChar) ||
		isCloseBracket(nextChar) ||
		isModule(nextChar) ||
		isEndLine(nextChar) ||
		isSegment(nextChar))		return ERR_NO_ERROR;

	return ERR_INCORRECT_SET;
}

int CheckSyntax::isDigit_()
{
	if (!isDigit(text->at(curPos))) return ERR_NO_TESTED;
	SetSegmentType(ST_NUM);
	if (segmentType != ST_NUM) return ERR_NUMER_IN_SET_AREA;
	nextChar = GetNextChar();
	if (isLastChar(nextChar) ||
		isNumber(nextChar) ||
		isNumOperation(nextChar) ||
		isStdCloseBracket(nextChar) ||
		isEndLine(nextChar) ||
		isSegment(nextChar))		return ERR_NO_ERROR;

	return ERR_INCORRECT_NUMBER;
}

int CheckSyntax::isDot_()
{
	if (!isDot(text->at(curPos))) return ERR_NO_TESTED;
	if (segmentType != ST_NUM) return ERR_NUMER_IN_SET_AREA;
	if (isFirstChar(curPos)) return ERR_INCORRECT_NUMBER;
	nextChar = GetNextChar();
	if (isDigit(nextChar) ||
		isEndLine(nextChar)) return ERR_NO_ERROR;

	return ERR_INCORRECT_NUMBER;
}

int CheckSyntax::isBinarySetOperation_()
{
	if (!isBinarySetOperation(text->at(curPos))) return ERR_NO_TESTED;
	if (segmentType != ST_SET) return ERR_SET_IN_NUMER_AREA;
	if (isFirstChar(curPos)) return ERR_INVALID_LEFT_OPERAND;
	nextChar = GetNextChar();
	if (isSet(nextChar) ||
		isOpenBracket(nextChar))	return ERR_NO_ERROR;

	return ERR_INVALID_RIGHT_OPERAND;
}

int CheckSyntax::isUnaryNumOperation_()
{
	if (!isUnaryNumOperation(text->at(curPos))) return ERR_NO_TESTED;
	if (segmentType != ST_NUM) return ERR_NUMER_IN_SET_AREA;
	nextChar = GetNextChar();
	if (isDigit(nextChar) ||
		isStdOpenBracket(nextChar) ||
		isModule(nextChar))			return ERR_NO_ERROR;

	return ERR_INVALID_RIGHT_OPERAND;
}

int CheckSyntax::isBinaryNumOperation_()
{
	if (!isBinaryNumOperation(text->at(curPos))) return ERR_NO_TESTED;
	if (segmentType != ST_NUM) return ERR_NUMER_IN_SET_AREA;
	if (isFirstChar(curPos)) return ERR_INVALID_LEFT_OPERAND;
	nextChar = GetNextChar();
	if (isDigit(nextChar) ||
		isStdOpenBracket(nextChar) ||
		isModule(nextChar))			return ERR_NO_ERROR;

	return ERR_INVALID_RIGHT_OPERAND;
}

int CheckSyntax::isStdOpenBracket_()
{
	if (!isStdOpenBracket(text->at(curPos))) return ERR_NO_TESTED;
	brackets.push(text->at(curPos));
	nextChar = GetNextChar();
	if (isDigit(nextChar) ||
		isSet(nextChar) ||
		isModule(nextChar) ||
		isUnaryNumOperation(nextChar) ||
		isOpenBracket(nextChar))		return ERR_NO_ERROR;

	return ERR_INCORRECT_BRACKET_POS;
}

int CheckSyntax::isStdCloseBracket_()
{
	if (!isStdCloseBracket(text->at(curPos))) return ERR_NO_TESTED;
	if (!isStdOpenBracket(brackets.top())) return ERR_INCORRECT_BRACKET_POS;
	brackets.pop();
	if (isFirstChar(curPos)) return ERR_INCORRECT_BRACKET_POS;
	nextChar = GetNextChar();
	if (isLastChar(nextChar) ||
		isBinarySetOperation(nextChar) ||
		isNumOperation(nextChar) ||
		isCloseBracket(nextChar) ||
		isModule(nextChar) ||
		isEndLine(nextChar) ||
		isSegment(nextChar))			return ERR_NO_ERROR;

	return ERR_INCORRECT_BRACKET_POS;
}

int CheckSyntax::isNStdOpenBracket_()
{
	if (!isNStdOpenBracket(text->at(curPos))) return ERR_NO_TESTED;
	brackets.push(text->at(curPos));
	SetSegmentType(ST_SET);
	if (segmentType != ST_SET) return ERR_SET_IN_NUMER_AREA;
	nextChar = GetNextChar();
	if (isSet(nextChar) ||
		isOpenBracket(nextChar))	return ERR_NO_ERROR;

	return ERR_INVALID_RIGHT_OPERAND;
}

int CheckSyntax::isNStdCloseBracket_()
{
	if (!isNStdCloseBracket(text->at(curPos))) return ERR_NO_TESTED;
	if (!isNStdOpenBracket(brackets.top())) return ERR_INCORRECT_BRACKET_POS;
	brackets.pop();
	if (segmentType != ST_SET) return ERR_SET_IN_NUMER_AREA;
	if (isFirstChar(curPos)) return ERR_INCORRECT_BRACKET_POS;
	nextChar = GetNextChar();
	if (isBinarySetOperation(nextChar) ||
		isCloseBracket(nextChar) ||
		isModule(nextChar) ||
		isEndLine(nextChar) ||
		isSegment(nextChar))			return ERR_NO_ERROR;

	return ERR_INCORRECT_BRACKET_POS;
}

int CheckSyntax::isModule_()
{
	if (!isModule(text->at(curPos))) return ERR_NO_TESTED;
	if (segmentType == ST_UNDEFINED || segmentType == ST_NUM) {
		segmentType = ST_SET;
		if (lineType == LT_SET) return ERR_NUMER_IN_SET_AREA;
		brackets.push('<');
		nextChar = GetNextChar();
		if (isSet(nextChar) ||
			isEndLine(nextChar) ||
			isOpenBracket(nextChar))	return ERR_NO_ERROR;

		return ERR_INCORRECT_BRACKET_POS;
	}
	else {
		segmentType = ST_NUM;
		if (brackets.empty() || brackets.top() != '<') return ERR_INCORRECT_BRACKET_POS;
		brackets.pop();
		nextChar = GetNextChar();
		if (isLastChar(nextChar) ||
			isNumOperation(nextChar) ||
			isStdCloseBracket(nextChar) ||
			isEndLine(nextChar) ||
			isSegment(nextChar))			return ERR_NO_ERROR;
	}

	return ERR_INCORRECT_BRACKET_POS;
}

int CheckSyntax::isSegment_()
{
	if (!isSegment(text->at(curPos))) return ERR_NO_TESTED;
	if (!brackets.empty()) return ERR_INCORRECT_BRACKET_POS;
	if (isFirstChar(curPos)) return ERR_INVALID_LEFT_OPERAND;
	nextChar = GetNextChar();
	if (lineType == LT_SET) {
		if (isSet(nextChar) ||
			isUnarySetOperation(nextChar) ||
			isOpenBracket(nextChar))	return ERR_NO_ERROR;
	}
	else {
		if (isDigit(nextChar) ||
			isUnaryNumOperation(nextChar) ||
			isStdOpenBracket(nextChar) ||
			isModule(nextChar))			return ERR_NO_ERROR;
	}

	return ERR_INVALID_RIGHT_OPERAND;
}

int CheckSyntax::isEndLine_()
{
	if (!isEndLine(text->at(curPos))) return ERR_NO_TESTED;
	if (!brackets.empty()) return ERR_INCORRECT_BRACKET_POS;

	StartLine();

	return ERR_NO_ERROR;
}

CheckSyntax::CheckSyntax(const string* text)
{
	this->text = text;
}

CheckSyntax::CheckSyntax(const string* text, vector<int>* codes)
{
	this->text = text;
	this->codes = codes;
}

bool CheckSyntax::MarkErrors()
{
	int error = 0;
	bool isError = false;

	StartLine();
	for (curPos = 0; curPos < text->size(); curPos++)
	{
		error = isSet_();
		if (error != ERR_NO_TESTED) {
			SetError(codes->at(curPos), error);
			if (error != ERR_NO_ERROR) isError = true;
			continue;
		}
		error = isDigit_();
		if (error != ERR_NO_TESTED) {
			SetError(codes->at(curPos), error);
			if (error != ERR_NO_ERROR) isError = true;
			continue;
		}
		error = isDot_();
		if (error != ERR_NO_TESTED) {
			SetError(codes->at(curPos), error);
			if (error != ERR_NO_ERROR) isError = true;
			continue;
		}
		error = isBinarySetOperation_();
		if (error != ERR_NO_TESTED) {
			SetError(codes->at(curPos), error);
			if (error != ERR_NO_ERROR) isError = true;
			continue;
		}
		error = isUnaryNumOperation_();
		if (error != ERR_NO_TESTED) {
			SetError(codes->at(curPos), error);
			if (error != ERR_NO_ERROR) isError = true;
			continue;
		}
		error = isBinaryNumOperation_();
		if (error != ERR_NO_TESTED) {
			SetError(codes->at(curPos), error);
			if (error != ERR_NO_ERROR) isError = true;
			continue;
		}
		error = isStdOpenBracket_();
		if (error != ERR_NO_TESTED) {
			SetError(codes->at(curPos), error);
			if (error != ERR_NO_ERROR) isError = true;
			continue;
		}
		error = isStdCloseBracket_();
		if (error != ERR_NO_TESTED) {
			SetError(codes->at(curPos), error);
			if (error != ERR_NO_ERROR) isError = true;
			continue;
		}
		error = isNStdOpenBracket_();
		if (error != ERR_NO_TESTED) {
			SetError(codes->at(curPos), error);
			if (error != ERR_NO_ERROR) isError = true;
			continue;
		}
		error = isNStdCloseBracket_();
		if (error != ERR_NO_TESTED) {
			SetError(codes->at(curPos), error);
			if (error != ERR_NO_ERROR) isError = true;
			continue;
		}
		error = isModule_();
		if (error != ERR_NO_TESTED) {
			SetError(codes->at(curPos), error);
			if (error != ERR_NO_ERROR) isError = true;
			continue;
		}
		error = isSegment_();
		if (error != ERR_NO_TESTED) {
			SetError(codes->at(curPos), error);
			if (error != ERR_NO_ERROR) isError = true;
			continue;
		}
		error = isEndLine_();
		if (error != ERR_NO_TESTED) {
			SetError(codes->at(curPos), error);
			if (error != ERR_NO_ERROR) isError = true;
			continue;
		}
	}
	return isError;
}

bool CheckSyntax::Check()
{
	int error = 0;

	StartLine();
	for (curPos = 0; curPos < text->size(); curPos++)
	{
		error = isSet_();
		if (error != ERR_NO_TESTED) {
			if (error != ERR_NO_ERROR) return false;
			continue;
		}
		error = isDigit_();
		if (error != ERR_NO_TESTED) {
			if (error != ERR_NO_ERROR) return false;
			continue;
		}
		error = isDot_();
		if (error != ERR_NO_TESTED) {
			if (error != ERR_NO_ERROR) return false;
			continue;
		}
		error = isBinarySetOperation_();
		if (error != ERR_NO_TESTED) {
			if (error != ERR_NO_ERROR) return false;
			continue;
		}
		error = isUnaryNumOperation_();
		if (error != ERR_NO_TESTED) {
			if (error != ERR_NO_ERROR) return false;
			continue;
		}
		error = isBinaryNumOperation_();
		if (error != ERR_NO_TESTED) {
			if (error != ERR_NO_ERROR) return false;
			continue;
		}
		error = isStdOpenBracket_();
		if (error != ERR_NO_TESTED) {
			if (error != ERR_NO_ERROR) return false;
			continue;
		}
		error = isStdCloseBracket_();
		if (error != ERR_NO_TESTED) {
			if (error != ERR_NO_ERROR) return false;
			continue;
		}
		error = isNStdOpenBracket_();
		if (error != ERR_NO_TESTED) {
			if (error != ERR_NO_ERROR) return false;
			continue;
		}
		error = isNStdCloseBracket_();
		if (error != ERR_NO_TESTED) {
			if (error != ERR_NO_ERROR) return false;
			continue;
		}
		error = isModule_();
		if (error != ERR_NO_TESTED) {
			if (error != ERR_NO_ERROR) return false;
			continue;
		}
		error = isSegment_();
		if (error != ERR_NO_TESTED) {
			if (error != ERR_NO_ERROR) return false;
			continue;
		}
		error = isEndLine_();
		if (error != ERR_NO_TESTED) {
			if (error != ERR_NO_ERROR) return false;
			continue;
		}
	}
	return true;
}