#include "ERedoUndoComponent.h"

void ERedoUndoComponent::beginUndo()
{
	undo.push_back(std::stack<ERedoUndoUnit*>());
}

void ERedoUndoComponent::addUndo(ERedoUndoUnit* unit)
{
	undo.back().push(unit);
}

void ERedoUndoComponent::beginRedo()
{
	redo.push_back(std::stack<ERedoUndoUnit*>());
}

void ERedoUndoComponent::addRedo(ERedoUndoUnit* unit)
{
	redo.back().push(unit);
}

bool ERedoUndoComponent::releaseUndo()
{
	if (undo.empty()) return false;
	if (undo.back().empty()) {
		undo.pop_back();
		return false;
	}
	return true;
}

ERedoUndoUnit* ERedoUndoComponent::getUndo()
{
	return undo.back().top();
}

void ERedoUndoComponent::removeUndo()
{
	undo.back().pop();
}

bool ERedoUndoComponent::releaseRedo()
{
	if (redo.empty()) return false;
	if (redo.back().empty()) {
		redo.pop_back();
		return false;
	}
	return true;
}

ERedoUndoUnit* ERedoUndoComponent::getRedo()
{
	return undo.back().top();
}

void ERedoUndoComponent::removeRedo()
{
	redo.back().pop();
}

void ERedoUndoComponent::clearRedo()
{
	for (auto a : redo) {
		while (!a.empty()) {
			delete a.top();
			a.pop();
		}
	}
	redo.clear();
}
