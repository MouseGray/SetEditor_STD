#include "EFace.h"


void EFace::undo()
{
	bool isFirst = true;
	while (release<ERedoUndoComponent::Type::Undo>())
	{
		if (isFirst) {
			begin<ERedoUndoComponent::Type::Redo>();
			isFirst = false;
		}
		auto unknown_unit = getUndo();
		switch (unknown_unit->action)
		{
			case ERedoUndoUnit::Action::Insert: {
				auto unit = static_cast<ERedoUndoInsert*>(unknown_unit);
				ETextComponent::erase<ERedoUndoComponent::Type::Redo>(unit->pos, unit->count);
				break;
			}
			case ERedoUndoUnit::Action::Erase: {
				auto unit = static_cast<ERedoUndoErase*>(unknown_unit);
				ETextComponent::insert<ERedoUndoComponent::Type::Redo>(unit->pos, unit->text);
				break;
			}
			case ERedoUndoUnit::Action::Cursor: {
				auto unit = static_cast<ERedoUndoCursor*>(unknown_unit);
				EControlComponent::moveTo<ERedoUndoComponent::Type::Redo>(unit->cursor, unit->sCursor);
				break;
			}
			case ERedoUndoUnit::Action::InsertOverline: {
				auto unit = static_cast<ERedoUndoInsertOverline*>(unknown_unit);
				EControlComponent::eraseOverline<ERedoUndoComponent::Type::Redo>(unit->startPos, unit->endPos);
				break;
			}
			case ERedoUndoUnit::Action::EraseOverline: {
				auto unit = static_cast<ERedoUndoEraseOverline*>(unknown_unit);
				EControlComponent::insertOverline<ERedoUndoComponent::Type::Redo>(unit->startPos, unit->endPos);
				break;
			}
		}
		removeUndo();
	}
}

void EFace::redo()
{
	bool isFirst = true;
	while (release<ERedoUndoComponent::Type::Redo>())
	{
		if (isFirst) {
			begin<ERedoUndoComponent::Type::Undo>();
			isFirst = false;
		}
		auto unknown_unit = getRedo();
		switch (unknown_unit->action)
		{
			case ERedoUndoUnit::Action::Insert: {
				auto unit = static_cast<ERedoUndoInsert*>(unknown_unit);
				ETextComponent::erase<ERedoUndoComponent::Type::Undo>(unit->pos, unit->count);
				break;
			}
			case ERedoUndoUnit::Action::Erase: {
				auto unit = static_cast<ERedoUndoErase*>(unknown_unit);
				ETextComponent::insert<ERedoUndoComponent::Type::Undo>(unit->pos, unit->text);
				break;
			}
			case ERedoUndoUnit::Action::Cursor: {
				auto unit = static_cast<ERedoUndoCursor*>(unknown_unit);
				EControlComponent::moveTo<ERedoUndoComponent::Type::Undo>(unit->cursor, unit->sCursor);
				break;
			}
			case ERedoUndoUnit::Action::InsertOverline: {
				auto unit = static_cast<ERedoUndoInsertOverline*>(unknown_unit);
				EControlComponent::eraseOverline<ERedoUndoComponent::Type::Undo>(unit->startPos, unit->endPos);
				break;
			}
			case ERedoUndoUnit::Action::EraseOverline: {
				auto unit = static_cast<ERedoUndoEraseOverline*>(unknown_unit);
				EControlComponent::insertOverline<ERedoUndoComponent::Type::Undo>(unit->startPos, unit->endPos);
				break;
			}
		}
		removeRedo();
	}
}
