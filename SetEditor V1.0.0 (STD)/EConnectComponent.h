#pragma once

#include <array>

#include "ESelectComponent.h"
#include "ETextChangeTrigger.h"

class EConnectComponent : public EControlComponent
{
public:
	void erased(size_t begin, size_t end, ERedoUndoComponent::Type ruType);
	void inserted(size_t begin, size_t end, ERedoUndoComponent::Type ruType);

	void addConnects(std::vector<std::tuple<size_t, size_t, int>>& data, ERedoUndoComponent::Type ruType);
	void removeConnects(std::vector<size_t>& data, ERedoUndoComponent::Type ruType);

	bool addConnection(size_t pos);
	bool completeConnection(size_t pos);

	void cancelConnection();
	
	bool removeConnection(size_t pos);

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
			if (data()[i] == S) return i + 1;
			if (data()[i] == CC) lvl++;
			if (data()[i] == CO)
			{
				lvl--;
				if (lvl == 0) return i;
			}
		}
		return 0;
	}

	std::array<int, 2> priority(const char ch);

	inline bool isStarted() { return m_isStarted; }
public:
	size_t m_temp;

	std::vector<std::tuple<size_t, size_t, int>> m_connections;

	bool m_isStarted = false;
};

