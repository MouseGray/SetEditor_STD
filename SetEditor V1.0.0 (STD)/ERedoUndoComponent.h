#pragma once

#include <list>
#include <stack>

#include "ERedoUndoUnit.h"

class ERedoUndoComponent
{
public:
	enum class Type : char {
		Null,
		Undo,
		Redo
	};

	void beginUndo();
	void addUndo(ERedoUndoUnit* unit);

	void beginRedo();
	void addRedo(ERedoUndoUnit* unit);

	bool releaseUndo();
	ERedoUndoUnit* getUndo();
	void removeUndo();
	bool releaseRedo();
	ERedoUndoUnit* getRedo();
	void removeRedo();

	void clearRedo();
private:
	std::list<std::stack<ERedoUndoUnit*>> undo;
	std::list<std::stack<ERedoUndoUnit*>> redo;
};