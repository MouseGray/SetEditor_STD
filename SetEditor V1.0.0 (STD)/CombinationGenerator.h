#pragma once

#include <array>
#include <numeric>

template<int C>
class CombinationGenerator
{
	std::array<int, C> data_;
	size_t n_ = 0;

	void start(size_t n) {
		n_ = n;
		std::iota(data_.begin(), data_.end(), 0);
	}

	bool next()
	{
		auto amount = data_.size();
		for (auto i = amount - 1; i >= 0; --i)
			if (data_[i] < n_ - amount + i + 1) {
				data_[i]++;
				for (auto j = i + 1; j < amount; j++)
					data_[j] = data_[j - 1] + 1;
				return true;
			}
		return false;
	}

};

