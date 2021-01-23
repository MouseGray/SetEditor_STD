#pragma once

#include <list>

#include "ERedoUndoUnit.h"

class ERedoUndoComponent
{
public:
	enum class Type : char {
		Null,
		Undo,
		Redo
	};

	void addUndo(ERedoUndoUnit&& unit);
	void addRedo(ERedoUndoUnit&& unit);

	void releaseUndo();
	void releaseRedo();

	std::list<ERedoUndoUnit> undo;
	std::list<ERedoUndoUnit> redo;
};

