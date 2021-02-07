#include "DATerm.h"

Term::Term(Type _type, Value _value) :
	m_type(_type),
	m_value(_value)
{
}

Term::Term(const char value) :
	m_type(Type::Variable)
{
	m_value.variable = value;
}

Term::Term(const float value) :
	m_type(Type::Number)
{
	m_value.number = value;
}

Term::Term(const action value) :
	m_type(Type::Operation)
{
	m_value.operation = value;
}

char Term::toVariable() const
{
	assert(m_type == Type::Variable);
	return m_value.variable;
}

float Term::toNumber() const
{
	assert(m_type == Type::Number);
	return m_value.number;
}

action Term::toAction() const
{
	assert(m_type == Type::Operation);
	return m_value.operation;
}

Term& Term::operator=(const char value)
{
	m_type = Type::Variable;
	m_value.variable = value;
	return *this;
}

Term& Term::operator=(const float value)
{
	m_type = Type::Number;
	m_value.number = value;
	return *this;
}

Term& Term::operator=(const action value)
{
	m_type = Type::Operation;
	m_value.operation = value;
	return *this;
}

bool Term::operator==(const Term& term) const
{
	return equal(term) && m_subTerms.size() == term.m_subTerms.size();
}

bool Term::operator==(const Type type) const
{
	return m_type == type;
}

bool Term::operator==(const char value) const
{
	return m_type == Type::Variable && m_value.variable == value;
}

bool Term::operator==(const float value) const
{
	return m_type == Type::Number && m_value.number == value;
}

bool Term::operator==(const action value) const
{
	return m_type == Type::Operation && m_value.operation == value;
}

bool Term::operator!=(const Term& term) const
{
	return !(*this == term);
}

bool Term::operator!=(const Type type) const
{
	return !(*this == type);
}

bool Term::operator!=(const char value) const
{
	return !(*this == value);
}

bool Term::operator!=(const float value) const
{
	return !(*this == value);
}

bool Term::operator!=(const action value) const
{
	return !(*this == value);
}

bool Term::operator<=(const Term& term) const
{
	return equal(term) && m_subTerms.size() <= term.m_subTerms.size();
}

bool Term::operator>=(const Term& term) const
{
	return equal(term) && m_subTerms.size() >= term.m_subTerms.size();
}

Term& Term::operator<<(Term* term)
{
	m_subTerms.push_back(term);
}

Term& Term::operator<<(const char value)
{
	m_subTerms.push_back(new Term(value));
}

Term& Term::operator<<(const float value)
{
	m_subTerms.push_back(new Term(value));
}

Term& Term::operator<<(const action value)
{
	m_subTerms.push_back(new Term(value));
}

Term* Term::operator>>(const int pos)
{
	auto tmp = m_subTerms[pos];
	m_subTerms.erase(m_subTerms.begin() + pos);
	return tmp;
}

Term* Term::operator[](const int pos) const
{
	return m_subTerms[pos];
}

Term* Term::get(const int pos) const
{
	return m_subTerms[pos];
}

Term& Term::get_ref(const int pos) const
{
	return *m_subTerms[pos];
}

int Term::size() const { return static_cast<int>(m_subTerms.size()); }

bool Term::empty() const { return m_subTerms.empty(); }

void Term::trim()
{
	m_subTerms.clear();
}

void Term::swap(const int pos_1, const int pos_2)
{
	auto tmp = m_subTerms[pos_1];
	m_subTerms[pos_1] = m_subTerms[pos_2];
	m_subTerms[pos_2] = tmp;
}

Term* Term::copy() const
{
	return new Term(m_type, m_value);
}

bool Term::equal(const Term& term) const
{
	if (m_type != term.m_type) return false;
	switch (m_type)
	{
		case Type::Undefined: return false;
		case Type::Variable: return m_value.variable == term.m_value.variable;
		case Type::Number: return m_value.number == term.m_value.number;
		case Type::Operation: return m_value.operation == term.m_value.operation;
	}
	return true;
}
