#pragma once

#include <array>

#include "ESelectComponent.h"

class EConnectComponent : public EControlComponent
{
public:
	std::array<int, 2> select(int pos);

	int toLeft(int pos);
	int toRight(int pos);

	bool isLiteral(const char ch);

	template <char CO, char CC, char S>
	int findStartOrEndExp(int pos)
	{
		auto lvl = 0;
		for (auto i = pos; i >= 0; i--)
		{
			if (m_buffer[i] == S) return i + 1;
			if (m_buffer[i] == CC) lvl++;
			if (m_buffer[i] == CO)
			{
				lvl--;
				if (lvl == 0) return i;
			}
		}
		return 0;
	}

	std::array<int, 2> priority(const char ch);
};

