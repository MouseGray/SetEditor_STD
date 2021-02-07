#include "vector_type.h"

Vec::Vec(size_t system_size) : m_type(Type::null)
{
	m_data.resize(system_size, 0);
}

Vec::Vec(size_t system_size, size_t seed) : m_type(Type::set)
{
	m_data.resize(system_size, 0);
	auto sw = 0;
	for (auto i = 0, c = 0; i < m_data.size(); i++, c++) {
		if (c == seed) {
			sw ^= 1;
			c = -1;
		}
		m_data[i] = sw;
	}
}

Vec::Vec(Type type, double value, size_t system_size) : m_type(Type::null)
{
	m_data.resize(system_size, value);
}

bool Vec::operator==(const Vec& vec) const
{
	return m_type == vec.m_type && m_data == vec.m_data;
}

bool Vec::operator!=(const Vec& vec) const
{
	return !(*this == vec);
}

Vec Vec::operator~() const
{
	auto res = *this;
	for (auto i = 0; i < res.m_data.size(); i++)
		res.m_data[i] = static_cast<float>(!static_cast<bool>(res.m_data[i]));
	return res;
}

Vec Vec::operator|(const Vec& vec) const
{
	return Vec(*this) |= vec;
}

Vec Vec::operator&(const Vec& vec) const
{
	return Vec(*this) &= vec;
}

Vec Vec::operator^(const Vec& vec) const
{
	return Vec(*this) ^= vec;
}

Vec Vec::operator/(const Vec& vec) const
{
	return Vec(*this) /= vec;
}

Vec Vec::operator+(const Vec& vec) const
{
	return Vec(*this) += vec;
}

Vec Vec::operator-() const
{
	auto res = *this;
	for (auto i = 0; i < res.m_data.size(); i++)
		res.m_data[i] = -res.m_data[i];
	return res;
}

Vec Vec::operator-(const Vec& vec) const
{
	return Vec(*this) -= vec;
}

Vec Vec::operator*(const Vec& vec) const
{
	return Vec(*this) *= vec;
}

Vec& Vec::operator|=(const Vec& vec)
{
	assert(m_data.size() == vec.m_data.size());
	for (auto i = 0; i < m_data.size(); i++)
		m_data[i] = static_cast<float>(static_cast<bool>(m_data[i]) | static_cast<bool>(vec.m_data[i]));
	return *this;
}

Vec& Vec::operator&=(const Vec& vec)
{
	assert(m_data.size() == vec.m_data.size());
	for (auto i = 0; i < m_data.size(); i++)
		m_data[i] = static_cast<float>(static_cast<bool>(m_data[i]) & static_cast<bool>(vec.m_data[i]));
	return *this;
}

Vec& Vec::operator^=(const Vec& vec)
{
	assert(m_data.size() == vec.m_data.size());
	for (auto i = 0; i < m_data.size(); i++)
		m_data[i] = static_cast<float>(static_cast<bool>(m_data[i]) & !static_cast<bool>(vec.m_data[i]));
	return *this;
}

Vec& Vec::operator-=(const Vec& vec)
{
	assert(m_data.size() == vec.m_data.size());
	for (auto i = 0; i < m_data.size(); i++)
		m_data[i] -= vec.m_data[i];
	return *this;
}

Vec& Vec::operator+=(const Vec& vec)
{
	assert(m_data.size() == vec.m_data.size());
	for (auto i = 0; i < m_data.size(); i++)
		m_data[i] += vec.m_data[i];
	return *this;
}

Vec& Vec::operator*=(const Vec& vec)
{
	assert(m_data.size() == vec.m_data.size());
	for (auto i = 0; i < m_data.size(); i++)
		m_data[i] *= vec.m_data[i];
	return *this;
}

Vec& Vec::toQuantity()
{
	m_type = Type::number;
	return *this;
}

Vec& Vec::operator/=(const Vec& vec)
{
	assert(m_data.size() == vec.m_data.size());
	for (auto i = 0; i < m_data.size(); i++)
		m_data[i] /= vec.m_data[i];
	return *this;
}