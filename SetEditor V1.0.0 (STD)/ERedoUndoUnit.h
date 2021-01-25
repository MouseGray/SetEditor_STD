#pragma once
#include <string>

#undef min
#undef max

struct ERedoUndoErase;
struct ERedoUndoInsert;
struct ERedoUndoCursor;

struct ERedoUndoUnit
{
	enum class Action
	{
		Null,
		Erase,
		Insert,
		Cursor,
		InsertOverline,
		EraseOverline
	};

	ERedoUndoUnit(Action _action);

	ERedoUndoErase* toERUErase();
	ERedoUndoInsert* toERUInsert();
	ERedoUndoCursor* toERUCursor();

	Action action = Action::Null;
};

struct ERedoUndoInsert : public ERedoUndoUnit
{
	ERedoUndoInsert() : ERedoUndoUnit(Action::Insert) {}
	int pos   = 0;
	int count = 0;
};

struct ERedoUndoErase : public ERedoUndoUnit
{
	ERedoUndoErase() : ERedoUndoUnit(Action::Erase) {}
	int pos = 0;
	std::string text;
};

struct ERedoUndoCursor : public ERedoUndoUnit
{
	ERedoUndoCursor() : ERedoUndoUnit(Action::Cursor) {}
	int cursor = 0;
	int sCursor = 0;
};

struct ERedoUndoInsertOverline : public ERedoUndoUnit
{
	ERedoUndoInsertOverline() : ERedoUndoUnit(Action::InsertOverline) {}
	int startPos = 0;
	int endPos = 0;
};

struct ERedoUndoEraseOverline : public ERedoUndoUnit
{
	ERedoUndoEraseOverline() : ERedoUndoUnit(Action::EraseOverline) {}
	int startPos = 0;
	int endPos = 0;
};