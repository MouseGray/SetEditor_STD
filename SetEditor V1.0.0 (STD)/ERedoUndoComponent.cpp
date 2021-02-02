#include "ERedoUndoComponent.h"


void ERedoUndoComponent::startUndo(ERedoUndoComponent::Type ruType)
{
	switch (ruType)
	{
		case ERedoUndoComponent::Type::Null:
		case ERedoUndoComponent::Type::Undo:
			undo.push_back(std::stack<ERedoUndoUnit*>());
			break;
		case ERedoUndoComponent::Type::Redo:
			redo.push_back(std::stack<ERedoUndoUnit*>());
			break;
	}
}

void ERedoUndoComponent::finishUndo(ERedoUndoComponent::Type ruType)
{
	switch (ruType)
	{
		case ERedoUndoComponent::Type::Null:
		case ERedoUndoComponent::Type::Undo:
			assert(!undo.empty());
			if (undo.back().empty()) undo.pop_back();
			break;
		case ERedoUndoComponent::Type::Redo:
			assert(!redo.empty());
			if (redo.back().empty()) redo.pop_back();
			break;
	}
}

bool ERedoUndoComponent::releaseUndo(ERedoUndoComponent::Type ruType)
{
	switch (ruType)
	{
		case ERedoUndoComponent::Type::Null:
		case ERedoUndoComponent::Type::Undo:
			if (undo.empty()) return false;
			if (undo.back().empty()) {
				undo.pop_back();
				return false;
			}
			break;
		case ERedoUndoComponent::Type::Redo:
			if (redo.empty()) return false;
			if (redo.back().empty()) {
				redo.pop_back();
				return false;
			}
			break;
	}
	return true;
}

void ERedoUndoComponent::add(ERedoUndoUnit* unit, ERedoUndoComponent::Type ruType)
{
	switch (ruType)
	{
		case ERedoUndoComponent::Type::Null:
		case ERedoUndoComponent::Type::Undo:
			undo.back().push(unit);
			break;
		case ERedoUndoComponent::Type::Redo:
			redo.back().push(unit);
			break;
	}
}

ERedoUndoUnit* ERedoUndoComponent::getUndo(ERedoUndoComponent::Type ruType)
{
	switch (ruType)
	{
		case ERedoUndoComponent::Type::Undo:
			return undo.back().top();
		case ERedoUndoComponent::Type::Redo:
			return redo.back().top();
	}
}

void ERedoUndoComponent::removeUndo(ERedoUndoComponent::Type ruType)
{
	switch (ruType)
	{
		case ERedoUndoComponent::Type::Undo:
			undo.back().pop();
			break;
		case ERedoUndoComponent::Type::Redo:
			redo.back().pop();
			break;
	}
}

void ERedoUndoComponent::clearRedo(ERedoUndoComponent::Type ruType)
{
	if (ruType != ERedoUndoComponent::Type::Null) return;

	for (auto a : redo) {
		while (!a.empty()) {
			delete a.top();
			a.pop();
		}
	}
	redo.clear();
}

