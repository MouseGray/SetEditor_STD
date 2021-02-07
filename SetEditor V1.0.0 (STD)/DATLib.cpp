#include "DATLib.h"

std::pair<int, int> TermTool::check(const std::string& data, const std::pair<int, int>& location)
{
	auto area = Area::Null;
	auto begin = size_t(0);
	auto end = location.second - 1;
	auto first_point = true;
	auto parentheses = std::stack<char>();

	for (auto index = location.first; index < location.second; index++)
	{
		auto value = data[index];
		auto nextIndex = index + 1;
		if (isVariable(value)) {
			if (variableType(value) == ExpressionType::Set)
			{
				if (area == Area::Num)
					return std::make_pair(index, static_cast<int>(TextError::SetVariableInNumArea));
				area = Area::Set;
				
				if (index == end) continue;

				auto nextValue = data[nextIndex];
				if (isSetOperation(nextValue)) continue;
				if (isClosingParenthese(nextValue)) continue;

				return std::make_pair(nextIndex, nextValue);
			}
			else
			{
				if (area == Area::Set) 
					return std::make_pair(index, static_cast<int>(TextError::NumVariableInSetArea));
				area = Area::Num;

				if (index == end) continue;

				auto nextValue = data[nextIndex];
				if (isUnaryNumOperation(nextValue) || isBinaryNumOperation(nextValue)) continue;
				if (nextValue == FactionEnd) continue;

				return std::make_pair(nextIndex, nextValue);
			}
		}
		if (isDigit(value))
		{
			if (area == Area::Set)
				return std::make_pair(index, static_cast<int>(TextError::DigitInSetArea));
			area = Area::Num;

			if (index == end) continue;

			auto nextValue = data[nextIndex];
			if (isNumber(nextValue)) continue;
			if (isUnaryNumOperation(nextValue) || isBinaryNumOperation(nextValue)) continue;
			if (nextValue == FactionEnd) continue;

			return std::make_pair(nextIndex, nextValue);
		}
		if (value == Dot)
		{
			if (area == Area::Set)
				return std::make_pair(index, static_cast<int>(TextError::DotInSetArea));

			if (!first_point)
				return std::make_pair(index, static_cast<int>(TextError::IncorrectDigit));

			first_point = false;
			if (index == begin)
				return std::make_pair(index, static_cast<int>(TextError::IncorrectDigit));
			if (index == end)
				return std::make_pair(index, static_cast<int>(TextError::IncorrectDigit));

			auto nextValue = data[nextIndex];
			if (isDigit(nextValue)) continue;

			return std::make_pair(nextIndex, nextValue);
		}
		if (isSetOperation(value))
		{
			if (area == Area::Num)
				return std::make_pair(index, static_cast<int>(TextError::UnexpectedSetOperation));

			if (index == begin)
				return std::make_pair(index, static_cast<int>(TextError::NotFoundLeftOperand));
			if (index == end)
				return std::make_pair(index, static_cast<int>(TextError::NotFoundRightOperand));

			auto nextValue = data[nextIndex];
			if (isVariable(nextValue))
			{
				if (variableType(nextValue) == ExpressionType::Set) continue;
			}
			if (nextValue == Faction || nextValue == Complement) continue;

			return std::make_pair(nextIndex, nextValue);
		}
		if (isUnaryNumOperation(value))
		{
			first_point = true;
			if (area == Area::Set)
				return std::make_pair(index, static_cast<int>(TextError::UnexpectedNumOperation));

			area = Area::Num;
			if (index == end)
				return std::make_pair(index, static_cast<int>(TextError::NotFoundRightOperand));

			auto nextValue = data[nextIndex];
			if (isVariable(nextValue))
			{
				if (variableType(nextValue) == ExpressionType::Num) continue;
			}
			if (isDigit(nextValue)) continue;
			if (nextValue == Faction || nextValue == Quantity) continue;

			return std::make_pair(nextIndex, nextValue);
		}
		if (isBinaryNumOperation(value))
		{
			first_point = true;
			if (area == Area::Set) 
				return std::make_pair(index, static_cast<int>(TextError::UnexpectedNumOperation));

			if (index == begin)
				return std::make_pair(index, static_cast<int>(TextError::NotFoundLeftOperand));
			if (index == end)
				return std::make_pair(index, static_cast<int>(TextError::NotFoundRightOperand));

			auto nextValue = data[nextIndex];
			if (isVariable(nextValue))
			{
				if (variableType(nextValue) == ExpressionType::Num) continue;
			}
			if (isDigit(nextValue)) continue;
			if (nextValue == Faction || nextValue == Quantity) continue;

			return std::make_pair(nextIndex, nextValue);;
		}
		if (value == Complement)
		{
			parentheses.push(value);
			if (area == Area::Num) 
				return std::make_pair(index, static_cast<int>(TextError::UnexpectedSetOperation));

			area = Area::Set;
			if (index == end)
				return std::make_pair(index, static_cast<int>(TextError::NotFoundRightOperand));

			auto nextValue = data[nextIndex];
			if (isVariable(nextValue))
			{
				if (variableType(nextValue) == ExpressionType::Set) continue;
			}
			if (nextValue == Faction || nextValue == Complement) continue;

			return std::make_pair(nextIndex, nextValue);
		}
		if (value == ComplementEnd)
		{
			if (area == Area::Num) 
				return std::make_pair(index, static_cast<int>(TextError::UnexpectedSetOperation));

			if (parentheses.empty())
				return std::make_pair(index, static_cast<int>(TextError::NotFoundOpenParenthese));
			if (parentheses.top() != invertedParenthese(value))
				return std::make_pair(index, static_cast<int>(TextError::UnexpectedClosingParenthese));
			parentheses.pop();

			if (index == end) continue;

			auto nextValue = data[nextIndex];
			if (isSetOperation(nextValue)) continue;
			if (isClosingParenthese(nextValue)) continue;

			return std::make_pair(nextIndex, nextValue);
		}
		if (value == Faction)
		{
			parentheses.push(value);
			if (index == end)
				return std::make_pair(index, static_cast<int>(TextError::NotFoundRightOperand));

			auto nextValue = data[nextIndex];
			if (isVariable(nextValue)) continue;
			if (isDigit(nextValue)) continue;
			if (isUnaryNumOperation(nextValue)) continue;
			if (isOpenParenthese(nextValue)) continue;

			return std::make_pair(nextIndex, nextValue);
		}
		if (value == FactionEnd)
		{
			if (parentheses.empty())
				return std::make_pair(index, static_cast<int>(TextError::NotFoundOpenParenthese));
			if (parentheses.top() != invertedParenthese(value))
				return std::make_pair(index, static_cast<int>(TextError::UnexpectedClosingParenthese));
			parentheses.pop();
			// if (index == begin) <- parentheses.empty()
			if (index == end) continue;

			auto nextValue = data[nextIndex];
			if (isSetOperation(nextValue)) continue;
			if (isUnaryNumOperation(nextValue) || isBinaryNumOperation(nextValue)) continue;
			if (isClosingParenthese(nextValue)) continue;

			return std::make_pair(nextIndex, static_cast<int>(nextValue));
		}
		if (value == Quantity)
		{
			parentheses.push(value);
			if (area == Area::Set)
				return std::make_pair(index, static_cast<int>(TextError::UnexpectedNumOperation));

			area = Area::Set;
			if (index == end)
				return std::make_pair(index, static_cast<int>(TextError::NotFoundRightOperand));

			auto nextValue = data[nextIndex];
			if (isVariable(nextValue))
			{
				if (variableType(nextValue) == ExpressionType::Set) continue;
			}
			if (nextValue == Faction || nextValue == Complement) continue;

			return std::make_pair(nextIndex, static_cast<int>(nextValue));
		}
		if (value == QuantityEnd)
		{
			if (area == Area::Num)
				return std::make_pair(index, static_cast<int>(TextError::UnexpectedSetOperation));

			area = Area::Num;
			if (parentheses.empty())
				return std::make_pair(index, static_cast<int>(TextError::NotFoundOpenParenthese));
			if (parentheses.top() != invertedParenthese(value))
				return std::make_pair(index, static_cast<int>(TextError::UnexpectedClosingParenthese));
			parentheses.pop();

			if (index == end) continue;

			auto nextValue = data[nextIndex];
			if (isUnaryNumOperation(nextValue) || isBinaryNumOperation(nextValue)) continue;
			if (nextValue == FactionEnd) continue;

			return std::make_pair(nextIndex, static_cast<int>(nextValue));
		}
	}
	if (!parentheses.empty())
		return std::make_pair(end, static_cast<int>(TextError::UnexpectedClosingParenthese));
	return std::make_pair(0, -1);
}

Term* TermTool::createTerm(const std::string& str)
{
	std::string number;
	std::stack<Term*> opts;
	Term* result = nullptr;

	for (auto ch : str)
	{
		// --------------- Преобразование строки в число --------------- //
		if (isNumber(ch))
		{
			number += ch;
			continue;
		}
		if (!number.empty())
		{
			result = new Term(std::stof(number));
			number.clear();
		}
		// --------------- Преобразование в DAT запись --------------- //
		switch (ch)
		{
			case Minus: case Division:
			{
				while (!opts.empty())
				{
					if (priority(action(ch)) > priority(opts.top()->toAction())) break;
					*opts.top() << result;
					result = opts.top();
					opts.pop();
				}
				opts.push(new Term(action(ch)));
				*opts.top() << result;
				break;
			}
			case Intersection: case Union: case Addition: case Multiplication:
			{
				while (!opts.empty())
				{
					if (priority(action(ch)) > priority(opts.top()->toAction())) break;
					*opts.top() << result;
					result = opts.top();
					opts.pop();
				}
				if (*result == action(ch))
				{
					opts.push(result);
					result = nullptr;
				}
				else
				{
					opts.push(new Term(action(ch)));
					*opts.top() << result;
				}
				break;
			}
			case Subtraction:
			{
				opts.push(new Term(action(ch)));
				break;
			}
			case Complement:
			{
				opts.push(new Term(action(ch)));
				break;
			}
			case ComplementEnd:
			{
				while (!opts.empty())
				{
					if (opts.top()->toAction() == Complement) break;
					*opts.top() << result;
					result = opts.top();
					opts.pop();
				}
				*opts.top() << result;
				result = opts.top();
				opts.pop();
				break;
			}
			case Quantity:
			{
				opts.push(new Term(action(ch)));
				break;
			}
			case QuantityEnd:
			{
				while (!opts.empty())
				{
					if (opts.top()->toAction() == Quantity) break;
					*opts.top() << result;
					result = opts.top();
					opts.pop();
				}
				*opts.top() << result;
				result = opts.top();
				opts.pop();
			}
			case Faction:
			{
				opts.push(new Term(action(ch)));
				break;
			}
			case FactionEnd:
			{
				while (!opts.empty())
				{
					if (opts.top()->toAction() == Faction) break;
					*opts.top() << result;
					result = opts.top();
					opts.pop();
				}
				opts.pop();
				break;
			}
			default:
			{
				result = new Term(ch);
			}
		}
	}
	if (!number.empty())
	{
		result = new Term(std::stof(number));
		number.clear();
	}
	while (!opts.empty())
	{
		*opts.top() << result;
		result = opts.top();
		opts.pop();
	}

	return result;
}

bool TermTool::isEqual(const Term& left, const Term& right)
{
	if (left != right) return false;
	if (left.empty()) return true;

	std::vector<bool> checked(left.size(), false);
	int i, j;
	for (i = 0; i < left.size(); i++) {
		for (j = 0; j < right.size(); j++) {
			if (checked[j]) continue;
			if (isEqual(*left[i], *right[j])) {
				checked[j] = true;
				break;
			}
		}
		if (j == right.size()) return false;
	}
	for (auto b : checked) if (!b) return false;
	return true;
}

bool TermTool::isContain(const Term& term, const Term& sub_term)
{
	if (term.empty()) return false;
	if (sub_term.empty()) return false;

	for (auto i = 0; i < term.size(); i++)
		if (isEqual(term.get_ref(i), sub_term)) return true;

	if (term >= sub_term) {
		std::vector<bool> checked(term.size(), false);
		int i, j;
		for (i = 0; i < sub_term.size(); i++) {
			for (j = 0; j < term.size(); j++) {
				if (checked[j]) continue;
				if (isEqual(*sub_term[i], *term[j])) {
					checked[j] = true;
					break;
				}
			}
			if (j == term.size()) return false;
		}
		return true;
	}
	return false;
}

int TermTool::priority(const action value)
{
	switch (value) {
		case Undefined:         return 0;
		case Intersection:      return 7;
		case Union:             return 6;
		case Minus:             return 7;
		case Subtraction:       return 4;
		case Addition:          return 4;
		case Multiplication:    return 5;
		case Division:          return 5;
		case Complement:        return 3;
		case ComplementEnd:     return 3;
		case Quantity:          return 2;
		case QuantityEnd:       return 2;
		case Faction:           return 1;
		case FactionEnd:        return 1;
	}
	return 0;
}

Term* TermTool::copy(const Term* term)
{
	auto result = term->copy();
	for (size_t i = 0; i < term->size(); i++) 
		*result << copy((*term)[i]);
	return result;
}

void TermTool::remove(Term* term)
{
	for (size_t i = 0; i < term->size(); i++)
		remove(term->get(i));
	delete term;
}

void TermTool::removeChildren(Term* term)
{
	for (auto i = 0; i < term->size(); i++)
		TermTool::remove(term->get(0));
	term->trim();
}

void TermTool::collapse(Term* dest, Term* src)
{
	auto tmp_top = dest->get(0);
	auto tmp_bottom = src;
	dest = src;
	src->trim();
	TermTool::remove(tmp_top);
}

TermCalculator::TermCalculator(const std::string sets)
{
	m_system_size = (1 << sets.size());
	for(auto i = 0; i < sets.size(); i++)
		m_vecs.insert({ sets[i], Vec(m_system_size, 1 << i) });
}

Vec TermCalculator::calculate(const Term& term)
{
	if (term == Term::Type::Number)
		return Vec(Vec::Type::number, term.toNumber(), m_system_size);

	if (term == Term::Type::Variable) {
		auto vec = m_vecs.find(term.toVariable());
		return vec->second;
	}

	if (term == Term::Type::Operation) {
		switch (term.toAction())
		{
			case Complement: {
				return ~calculate(*term[0]);
			}
			case Intersection: {
				Vec result(Vec::Type::set, 1, m_system_size);
				for (size_t i = 0; i < term.size(); i++)
					result &= calculate(*term[i]);
				return result;
			}
			case Union: {
				Vec result(Vec::Type::set, double(0), m_system_size);
				for (size_t i = 0; i < term.size(); i++)
					result |= calculate(*term[i]);
				return result;
			}
			case Minus: {
				return calculate(*term[0]) ^ calculate(*term[1]);
			}
			case Addition: {
				Vec result(Vec::Type::number, double(0), m_system_size);
				for (size_t i = 0; i < term.size(); i++)
					result += calculate(*term[i]);
				return result;
			}
			case Multiplication: {
				Vec result(Vec::Type::number, double(1), m_system_size);
				for (size_t i = 0; i < term.size(); i++)
					result *= calculate(*term[i]);
				return result;
			}
			case Division: {
				return calculate(*term[0]) / calculate(*term[1]);;
			}
			case Quantity: {
				return calculate(*term[0]).toQuantity();
			}
		}
	}
	return Vec(m_system_size);
}
