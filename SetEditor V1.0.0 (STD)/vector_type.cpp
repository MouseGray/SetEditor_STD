#include "vector_type.h"

Vec::Vec(size_t system_size)
{
	_system_size = system_size;
	_vec = new double[_system_size];
}

Vec::Vec(const Vec& vec)
{
	_type = vec._type;
	_code = vec._code;
	_system_size = vec._system_size;

	if (_system_size > 0)
	{
		_vec = new double[_system_size];
		for (size_t i = 0; i < _system_size; i++)
			_vec[i] = vec._vec[i];
	}
}

Vec::Vec(vec_type type, double* vec, size_t system_size)
{
	_type = type;
	_system_size = system_size;

	_vec = new double[_system_size];
	for (size_t i = 0; i < _system_size; i++)
		_vec[i] = vec[i];
}

Vec::Vec(vec_type type, byte_t code)
{
	_type = type;
	_code = code;
}

Vec::Vec(vec_type type, double value, size_t system_size)
{
	_type = type;
	_system_size = system_size;

	_vec = new double[_system_size];
	for (size_t i = 0; i < _system_size; i++)
		_vec[i] = value;
}

double& Vec::operator[](size_t i) noexcept(false)
{
	if (i >= _system_size) throw 0x0001;
	return _vec[i];
}

bool Vec::operator==(const Vec vec)
{
	if (_type == vec_type::value && vec._type == vec_type::number)
	{
		if (this->is_null() && vec._vec[0] == 0) return true;
		return false;
	}
	else if (_type == vec_type::number && vec._type == vec_type::value)
	{
		if (vec.is_null() && _vec[0] == 0) return true;
		return false;
	}
	if (_type != vec._type) return false;
	if (_type == vec_type::operation) if (_code != vec._code) return false;
	if (_system_size != vec._system_size) return false;
	for (size_t i = 0; i < _system_size; i++) 
		if (_vec[i] != vec._vec[i]) return false;
	return true;
}

bool Vec::operator!=(const Vec vec)
{
	if (_type == vec_type::value && vec._type == vec_type::number)
	{
		if (this->is_null() && vec._vec[0] == 0) return false;
		return true;
	}
	else if (_type == vec_type::number && vec._type == vec_type::value)
	{
		if (vec.is_null() && _vec[0] == 0) return false;
		return true;
	}
	if (_type != vec._type) return true;
	if (_type == vec_type::operation) if (_code != vec._code) return true;
	if (_system_size != vec._system_size) return true;
	for (size_t i = 0; i < _system_size; i++)
		if (_vec[i] != vec._vec[i]) return true;
	return false;
}

Vec Vec::operator=(const Vec vec)
{
	_type = vec._type;
	_code = vec._code;
	_system_size = vec._system_size;

	delete[] _vec;
	_vec = nullptr;

	if (_system_size > 0)
	{
		_vec = new double[_system_size];
		for (size_t i = 0; i < _system_size; i++)
			_vec[i] = vec._vec[i];
	}
	return *this;
}

Vec Vec::operator~()
{
	Vec result(vec_type::set, 0.0, _system_size);
	for (size_t i = 0; i < _system_size; i++) 
		result._vec[i] = !(bool)_vec[i];
	return result;
}

Vec Vec::operator|(Vec vec)
{
	Vec result(vec_type::set, 0.0, _system_size);

	if (_system_size != vec._system_size) return result;

	for (size_t i = 0; i < _system_size; i++) result._vec[i] = (bool)_vec[i] | (bool)vec._vec[i];
	return result;
}

Vec Vec::operator&(Vec vec)
{
	Vec result(vec_type::set, 0.0, _system_size);

	if (_system_size != vec._system_size) return result;

	for (size_t i = 0; i < _system_size; i++) result._vec[i] = (bool)_vec[i] & (bool)vec._vec[i];
	return result;
}

Vec Vec::operator%(Vec vec)
{
	Vec result(vec_type::set, 0.0, _system_size);

	if (_system_size != vec._system_size) return result;

	for (size_t i = 0; i < _system_size; i++) result._vec[i] = (bool)_vec[i] & !(bool)vec._vec[i];
	return result;
}

Vec Vec::operator+(Vec vec)
{
	Vec result((_type < vec._type ? _type : vec._type), 0.0, _system_size);

	if (_system_size != vec._system_size) return result;

	for (size_t i = 0; i < _system_size; i++) result._vec[i] = _vec[i] + vec._vec[i];
	return result;
}

Vec Vec::operator-()
{
	Vec result(_type, 0.0, _system_size);

	for (size_t i = 0; i < _system_size; i++) result._vec[i] = -_vec[i];
	return result;
}

Vec Vec::operator-(Vec vec)
{
	Vec result((_type < vec._type ? _type : vec._type), 0.0, _system_size);

	if (_system_size != vec._system_size) return result;

	for (size_t i = 0; i < _system_size; i++) result._vec[i] = _vec[i] - vec._vec[i];
	return result;
}

Vec Vec::operator*(Vec vec)
{
	Vec result((_type < vec._type ? _type : vec._type), 0.0, _system_size);

	if (_system_size != vec._system_size) return result;

	for (size_t i = 0; i < _system_size; i++) result._vec[i] = _vec[i] * vec._vec[i];
	return result;
}

Vec Vec::operator/(Vec vec)
{
	Vec result(_type, 0.0, _system_size);

	if (_system_size != vec._system_size) return result;

	for (size_t i = 0; i < _system_size; i++) result._vec[i] = _vec[i] / vec._vec[i];
	return result;
}

vec_type Vec::type()
{
	return _type;
}

void Vec::type(vec_type type)
{
	_type = type;
}

double Vec::value()
{
	if (_type != vec_type::number) return 0.0;
	return _vec[0];
}

void Vec::value(double value)
{
	for (size_t i = 0; i < _system_size; i++)
		_vec[i] = value;
}

byte_t Vec::code()
{
	return _code;
}

void Vec::code(byte_t code)
{
	_code = code;
}

bool Vec::is(vec_type type, byte_t code)
{
	return _type == type && _code == code;
}

bool Vec::is_null() const
{
	for (size_t i = 0; i < _system_size; i++)
		if (_vec[i] != 0) return false;
	return true;
}

/*bool vector_type::equal(int type, vector<double> vec)
{
	if (this->type != type) return false;
	if (this->vec != vec) return false;
	return true;
}*/

size_t Vec::size()
{
	return _system_size;
}

Vec::~Vec()
{
	delete[] _vec;
}
