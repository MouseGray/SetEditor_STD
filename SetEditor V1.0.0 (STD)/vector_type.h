#pragma once

using namespace std;
typedef unsigned char byte_t;

enum class vec_type
{
	null,
	set,
	value,
	number,
	operation
};

class Vec
{
public:
	Vec(size_t system_size);
	Vec(const Vec& vec);

	Vec(vec_type type, double* vec, size_t system_size);
	Vec(vec_type type, byte_t code);
	Vec(vec_type type, double value, size_t system_size);

	double& operator[] (size_t i) noexcept(false);
	bool operator==(const Vec vec);
	bool operator!=(const Vec vec);
	Vec operator=(const Vec vec);

	Vec operator~();
	Vec operator|(Vec a);
	Vec operator&(Vec a);
	Vec operator%(Vec a);
	Vec operator/(Vec a);
	Vec operator+(Vec a);
	Vec operator-();
	Vec operator-(Vec a);
	Vec operator*(Vec a);

	vec_type type();
	void type(vec_type type);

	double value();
	void value(double value);

	byte_t code();
	void code(byte_t code);

	bool is(vec_type type, byte_t code);

	bool is_null() const;

	size_t size();

	~Vec();
protected:
	byte_t _code = 0;
	vec_type _type = vec_type::null;
	double* _vec = nullptr;
	size_t _system_size = 0;
};

