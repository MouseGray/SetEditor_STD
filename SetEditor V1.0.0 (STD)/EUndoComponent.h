#pragma once

#include <list>
#include <vector>
#include <string>

struct UndoUnit 
{
	enum class Type {
		Barrier,
		Text_Erase,
		Text_Insert,
		Cursor,
		Lable_Erase,
		Lable_Insert,
		Line_Erase,
		Line_Insert,
		Error_Erase,
		Error_Insert
	};
	union Value
	{
		std::string text;

	};
	Type type;
};

struct UndoTextUnit : public UndoUnit
{
	std::string text;
	size_t pos;
};

struct UndoTextUnit : public UndoUnit
{
	std::string text;
	size_t pos;
};

class EUndoComponent
{
	void start();
	//void add(UndoUnit* unit);
	void end();

	//bool undo();
	//bool redo();
private:
	int m_pointer = -1;
	std::vector<UndoUnit*> m_data;
};

