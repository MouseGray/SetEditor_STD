#include "DATerm.h"

Term::Term(double value, term_type type)
{
	_value = value;
	_type = type;
}

Term::Term(const Term& pTerm)
{
	_value = pTerm._value;
	_type = pTerm._type;

	_subterm_size = pTerm._subterm_size;
	_subterm_p = pTerm._subterm_p;

	if (_subterm_size != 0)
	{
		try
		{
			_subterms = new shared_ptr<Term>[_subterm_size];
			for (size_t i = 0; i < pTerm._subterm_p; i++)
				_subterms[i] = pTerm._subterms[i];
		}
		catch (std::bad_alloc)
		{
			throw 0x0001;
		}
	}
}

Term::Term(shared_ptr<Term> pTerm)
{
	_value = pTerm->_value;
	_type = pTerm->_type;

	_subterm_size = pTerm->_subterm_size;
	_subterm_p = pTerm->_subterm_p;

	if (_subterm_size != 0)
	{
		try
		{
			_subterms = new shared_ptr<Term>[_subterm_size];
			for (size_t i = 0; i < pTerm->_subterm_p; i++)
				_subterms[i] = pTerm->_subterms[i];
		}
		catch (std::bad_alloc)
		{
			throw 0x0001;
		}
	}
}

void Term::set(double value, term_type type)
{
	_value = value;
	_type = type;
}

size_t Term::size() { return _subterm_p; }

bool Term::empty() { return _subterm_p == 0; }

bool Term::operator==(Term pTerm)
{
	if (_value != pTerm._value) return false;
	if (_type != pTerm._type) return false;
	if (_subterm_p != pTerm._subterm_p) return false;
	return true;
}

bool Term::operator!=(Term pTerm)
{
	if (_value != pTerm._value) return true;
	if (_type != pTerm._type) return true;
	if (_subterm_p != pTerm._subterm_p) return true;
	return false;
}

void Term::add(shared_ptr<Term> pTerm)
{
	if (_subterm_p == _subterm_size)
	{
		_subterm_size++;
		shared_ptr<Term>* nsubterms = new shared_ptr<Term>[_subterm_size];
		for (size_t i = 0; i < _subterm_p; i++) nsubterms[i] = _subterms[i];
		delete[] _subterms;
		_subterms = nsubterms;
	}

	_subterms[_subterm_p] = pTerm;
	_subterm_p++;
}

shared_ptr<Term> Term::get(size_t n)
{
	if (n >= _subterm_p) throw 0x0002;
	return _subterms[n];
}

void Term::copy(Term pTerm)
{
	_value = pTerm._value;
	_type = pTerm._type;

	_subterm_size = pTerm._subterm_size;
	_subterm_p = 0;

	delete[] _subterms;
	_subterms = nullptr;

	if (_subterm_size != 0)
	{
		try
		{
			_subterms = new shared_ptr<Term>[_subterm_size];
		}
		catch (std::bad_alloc)
		{
			throw 0x0001;
		}
	}
}

void Term::fcopy(const Term term)
{
	_value = term._value;
	_type = term._type;

	_subterm_size = term._subterm_size;
	_subterm_p = term._subterm_p;

	delete[] _subterms;
	_subterms = nullptr;

	if (_subterm_size != 0)
	{
		try
		{
			_subterms = new shared_ptr<Term>[_subterm_size];
			for (size_t i = 0; i < term._subterm_p; i++)
				_subterms[i] = term._subterms[i];
		}
		catch (std::bad_alloc)
		{
			throw 0x0001;
		}
	}
}

bool Term::is(double value, term_type type) { return (_value == value) && (_type == type); }

bool Term::is(double value, term_type type, size_t size) { return (_value == value) && (_type == type) && (_subterm_p == size); }

bool Term::is(shared_ptr<Term> pTerm) { return (_value == pTerm->_value) && (_type == pTerm->_type); }

bool Term::ismin(double value, term_type type, size_t size) { return (_value == value) && (_type == type) && (_subterm_size >= size); }

bool Term::ismax(double value, term_type type, size_t size) { return (_value == value) && (_type == type) && (_subterm_size <= size); }

void Term::remove(size_t n)
{
	if (n >= _subterm_p) throw 0x0002;
	_subterm_p--;
	_subterms[n] = _subterms[_subterm_p];
}

void Term::remove_all() { _subterm_p = 0; }

Term::~Term()
{
	delete[] _subterms;
	_subterms = nullptr;
}