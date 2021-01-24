#include "ERedoUndoComponent.h"

void ERedoUndoComponent::releaseUndo()
{
	if (undo.empty()) return;
	switch (undo.back()->action)
	{
		case ERedoUndoUnit::Action::Insert: {
			auto unit = undo.back()->toERUInsert();
			setCursorPos(unit->cursor);
			urErase<ERedoUndoComponent::Type::Redo>(unit->count);
			break;
		}
		case ERedoUndoUnit::Action::Erase: {
			auto unit = undo.back()->toERUErase();
			setCursorPos(unit->cursor);
			urInsert<ERedoUndoComponent::Type::Redo>(unit->text);
			setCursorPos(unit->cursor);
			setSelectCursorPos(unit->sCursor);
			break;
		}
	}
	delete undo.back();
	undo.pop_back();
}

void ERedoUndoComponent::releaseRedo()
{
	if (redo.empty()) return;
	switch (redo.back()->action)
	{
		case ERedoUndoUnit::Action::Insert: {
			auto unit = redo.back()->toERUInsert();
			setCursorPos(unit->cursor);
			urErase<ERedoUndoComponent::Type::Undo>(unit->count);
			break;
		}
		case ERedoUndoUnit::Action::sErase: {
			auto unit = redo.back()->toERUErase();
			setCursorPos(unit->cursor);
			urInsert<ERedoUndoComponent::Type::Undo>(unit->text);
			setCursorPos(unit->cursor);
			setSelectCursorPos(unit->sCursor);
			break;
		}
	}
	delete undo.back();
	redo.pop_back();
}
