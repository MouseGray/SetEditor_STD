#pragma once
#include <string>
#include <vector>

#undef min
#undef max

struct ERedoUndoErase;
struct ERedoUndoInsert;

struct ERedoUndoCursor;

struct ERedoUndoInsertLabel;
struct ERedoUndoEraseLabel;

struct ERedoUndoInsertError;
struct ERedoUndoEraseError;

struct ERedoUndoEraseLineNumber;
struct ERedoUndoInsertLineNumber;

struct ERedoUndoEraseConnect;
struct ERedoUndoInsertConnect;

struct ERedoUndoUnit
{
	enum class Action
	{
		Null,
		Erase,
		Insert,
		Cursor,
		InsertLabel,
		EraseLabel,
		InsertError,
		EraseError,
		InsertLineNumber,
		EraseLineNumber,
		InsertConnect,
		EraseConnect
	};

	ERedoUndoUnit(Action _action);

	ERedoUndoErase* toERUErase();
	ERedoUndoInsert* toERUInsert();
	ERedoUndoCursor* toERUCursor();
	ERedoUndoInsertLabel* toERUInsertLabel();
	ERedoUndoEraseLabel* toERUEraseLabel();
	ERedoUndoInsertError* toERUInsertError();
	ERedoUndoEraseError* toERUEraseError();
	ERedoUndoInsertLineNumber* toERUInsertLineNumber();
	ERedoUndoEraseLineNumber* toERUEraseLineNumber();

	ERedoUndoInsertConnect* toERUInsertConnect();
	ERedoUndoEraseConnect* toERUEraseConnect();

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

struct ERedoUndoInsertLabel : public ERedoUndoUnit
{
	ERedoUndoInsertLabel() : ERedoUndoUnit(Action::InsertLabel) {}
	size_t begin = 0;
	size_t end = 0;
};

struct ERedoUndoEraseLabel : public ERedoUndoUnit
{
	ERedoUndoEraseLabel() : ERedoUndoUnit(Action::EraseLabel) {}
	size_t begin = 0;
	std::vector<std::pair<size_t, char>> data;
};

struct ERedoUndoInsertError : public ERedoUndoUnit
{
	ERedoUndoInsertError() : ERedoUndoUnit(Action::InsertError) {}
	size_t begin = 0;
	size_t end = 0;
};

struct ERedoUndoEraseError : public ERedoUndoUnit
{
	ERedoUndoEraseError() : ERedoUndoUnit(Action::EraseError) {}
	size_t begin = 0;
	std::vector<std::pair<size_t, char>> data;
};

struct ERedoUndoInsertLineNumber : public ERedoUndoUnit
{
	ERedoUndoInsertLineNumber() : ERedoUndoUnit(Action::InsertLineNumber) {}
	size_t begin = 0;
	size_t end = 0;
};

struct ERedoUndoEraseLineNumber : public ERedoUndoUnit
{
	ERedoUndoEraseLineNumber() : ERedoUndoUnit(Action::EraseLineNumber) {}
	size_t begin = 0;
	std::vector<int> data;
};

struct ERedoUndoInsertConnect : public ERedoUndoUnit
{
	ERedoUndoInsertConnect() : ERedoUndoUnit(Action::InsertConnect) {}
	std::vector<size_t> elements;
};

struct ERedoUndoEraseConnect : public ERedoUndoUnit
{
	ERedoUndoEraseConnect() : ERedoUndoUnit(Action::EraseConnect) {}
	std::vector<std::tuple<size_t, size_t, int>> data;
};