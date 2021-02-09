#pragma once

#include <vector>
#include <assert.h>

#include "Operation.h"

class Term
{
public:
	enum class Type {
		Undefined,
		Number,
		Variable,
		Operation
	};

	Term(const char value);
	Term(const float value);
	Term(const action value);

	char toVariable() const;
	float toNumber() const;
	action toAction() const;

	Term& operator=(const char value);
	Term& operator=(const float value);
	Term& operator=(const action value);

	bool operator==(const char value) const;
	bool operator==(const float value) const;
	bool operator==(const action value) const;

	bool operator==(const Term& term) const;

	bool operator==(const Type type) const;

	bool operator<=(const Term& term) const;

	bool operator>=(const Term& term) const;

	bool operator!=(const char value) const;
	bool operator!=(const float value) const;
	bool operator!=(const action value) const;

	bool operator!=(const Term& term) const;

	bool operator!=(const Type type) const;

	Term& operator<<(Term* term);

	Term& operator<<(const char value);
	Term& operator<<(const float value);
	Term& operator<<(const action value);
	
	Term* operator>>(const int pos);

	Term* operator[](const int pos) const;

	Term* get(const int pos) const;

	Term& get_ref(const int pos) const;

	bool equal(const Term& term) const;

	int size() const;

	bool empty() const;

	void trim();

	Term* copy() const;

	std::string toString(int _off = 0);
private:
	union Value {
		char variable;
		float number;
		action operation;
	};

	Type m_type;
	Value m_value;
	std::vector<Term*> m_subTerms;

	Term(Type _type, Value _value);
};


typedef Term* term_ptr;