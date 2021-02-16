#pragma once

#include <vector>
#include <algorithm>
#include <numeric>

template<int C, typename T = char>
class CombinationGenerator
{
	std::vector<T> data_;
	std::vector<std::vector<T>> baskets_;

public:
	void start(size_t n) {
		data_.resize(n, 0);
		baskets_.resize(C, std::vector<T>());
	}

	bool nextInBaskets() {
		if (!next()) return false;
		for (auto b : baskets_) b.clear();
		for (auto i = 0; i < data_.size(); i++)
			baskets_[data_[i]].push_back(i);
	}

	const std::vector<T>& basket(int i) const {
		return baskets_[i];
	}

	bool next() {
		if (!next_()) return false;
		for (auto i = 0; i < C; i++)
			while (!isCorrectly_())
				if (!next_()) return false;
		return true;
	}

	const std::vector<T>& data() const {
		return data_;
	}
private:
	bool isCorrectly_() {
		for (auto i = 0; i < C; i++) 
			if (std::find(data_.begin(), data_.end(), i) == data_.end()) 
				return false; 
		return true;
	}

	bool next_()
	{
		auto it = std::find_if(data_.rbegin(), data_.rend(), [lim = C - 1](auto& e) { return e < lim; });
		if (it == data_.rend()) return false;
		(*it)++;
		std::fill(data_.rbegin(), it, 0);
		return true;
	}
};

