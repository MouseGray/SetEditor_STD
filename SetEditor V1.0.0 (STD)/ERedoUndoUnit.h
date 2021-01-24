#pragma once
#include <string>

#include "ESelectComponent.h"

struct ERedoUndoErase;
struct ERedoUndoInsert;
struct ERedoUndosErase;

struct ERedoUndoUnit
{
	enum class Action
	{
		Null,
		Erase,
		Insert,
		sErase
	};

	ERedoUndoUnit(Action _action);

	ERedoUndoErase* toERUErase();
	ERedoUndoInsert* toERUInsert();

	Action action = Action::Null;
};

struct ERedoUndoInsert : public ERedoUndoUnit
{
	ERedoUndoInsert(int _cursor, int _count);

	int cursor = 0;
	int count = 0;
};

struct ERedoUndoErase : public ERedoUndoUnit
{
	ERedoUndoErase(int _cursor, int _sCursor, std::string _text);

	int cursor = 0;
	int sCursor = 0;
	std::string text;
};