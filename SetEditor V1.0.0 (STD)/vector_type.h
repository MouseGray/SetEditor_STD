#pragma once

#include <vector>
#include <algorithm>
#include <cassert>

class Vec
{
public:
	enum class Type : char {
		null,
		set,
		number
	};

	Vec(size_t system_size);
	Vec(size_t system_size, size_t seed);
	Vec(Type type, double value, size_t system_size);

	bool operator==(const Vec& vec) const;
	bool operator!=(const Vec& vec) const;

	Vec operator~() const;
	Vec operator|(const Vec& vec) const;
	Vec operator&(const Vec& vec) const;
	Vec operator^(const Vec& vec) const;
	Vec operator/(const Vec& vec) const;
	Vec operator+(const Vec& vec) const;
	Vec operator-() const;
	Vec operator-(const Vec& vec) const;
	Vec operator*(const Vec& vec) const;

	Vec& operator|=(const Vec& vec);
	Vec& operator&=(const Vec& vec);
	Vec& operator^=(const Vec& vec);
	Vec& operator/=(const Vec& vec);
	Vec& operator+=(const Vec& vec);
	Vec& operator-=(const Vec& vec);
	Vec& operator*=(const Vec& vec);

	Vec& toQuantity();
protected:
	std::vector<float> m_data;
	Type m_type;
};

