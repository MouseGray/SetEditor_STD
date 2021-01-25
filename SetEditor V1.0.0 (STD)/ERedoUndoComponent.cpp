#include "ERedoUndoComponent.h"


ERedoUndoUnit* ERedoUndoComponent::getUndo()
{
	return undo.back().top();
}

void ERedoUndoComponent::removeUndo()
{
	undo.back().pop();
}

ERedoUndoUnit* ERedoUndoComponent::getRedo()
{
	return undo.back().top();
}

void ERedoUndoComponent::removeRedo()
{
	redo.back().pop();
}

