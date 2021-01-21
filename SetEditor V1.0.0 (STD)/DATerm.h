#pragma once
#include <memory>

using namespace std;

enum class term_type
{
	null,
	number,
	symbol,
	operation
};

enum class term_subtype
{
	set,
	number
};

class Term
{
public:
	Term(double value, term_type type);
	Term(const Term& pTerm);
	Term(shared_ptr<Term> pTerm);

	void set(double value, term_type type);
	
	size_t size();

	bool empty();

	bool operator==(Term term);
	bool operator!=(Term term);

	void add(shared_ptr<Term> pTerm);
	shared_ptr<Term> get(size_t n);
	
	void copy(Term term);
	void fcopy(const Term term);

	bool is(double value, term_type type);
	bool is(double value, term_type type, size_t size);
	bool is(shared_ptr<Term> pTerm);

	bool ismin(double value, term_type type, size_t size);
	bool ismax(double value, term_type type, size_t size);


	void remove(size_t n);
	void remove_all();
	
	~Term();

	double _value = 0;

	term_type _type = term_type::null;
private:
	shared_ptr<Term>* _subterms = nullptr;
	size_t _subterm_p = 0;
	size_t _subterm_size = 0;
};


typedef shared_ptr<Term> term_ptr;