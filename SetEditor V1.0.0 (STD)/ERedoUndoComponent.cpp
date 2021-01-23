#include "ERedoUndoComponent.h"

void ERedoUndoComponent::addUndo(ERedoUndoUnit&& unit)
{
	undo.push_back(unit);
}

void ERedoUndoComponent::addRedo(ERedoUndoUnit&& unit)
{
	redo.push_back(unit);
}

void ERedoUndoComponent::releaseUndo()
{
	if (undo.empty()) return;
	undo.back().release();
	undo.pop_back();
}

void ERedoUndoComponent::releaseRedo()
{
	if (undo.empty()) return;
	redo.back().release();
	redo.pop_back();
}
