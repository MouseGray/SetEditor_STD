#include "EFace.h"


void EFace::release(ERedoUndoComponent::Type ruType)
{
	auto not_ruType = ruType == ERedoUndoComponent::Type::Undo ? ERedoUndoComponent::Type::Redo : ERedoUndoComponent::Type::Undo;
	bool isFirst = true;
	while (releaseUndo(ruType))
	{
		if (isFirst) {
			startUndo(not_ruType);
			isFirst = false;
		}
		auto unknown_unit = getUndo(ruType);
		switch (unknown_unit->action)
		{
			case ERedoUndoUnit::Action::Insert: {
				auto unit = static_cast<ERedoUndoInsert*>(unknown_unit);
				ETextComponent::erase(unit->pos, unit->pos + unit->count, not_ruType);
				break;
			}
			case ERedoUndoUnit::Action::Erase: {
				auto unit = static_cast<ERedoUndoErase*>(unknown_unit);
				ETextComponent::insert(unit->pos, unit->text, not_ruType);
				break;
			}
			case ERedoUndoUnit::Action::Cursor: {
				auto unit = static_cast<ERedoUndoCursor*>(unknown_unit);
				EControlComponent::moveTo(unit->cursor, unit->sCursor, not_ruType);
				break;
			}
			case ERedoUndoUnit::Action::InsertLabel: {
				auto unit = static_cast<ERedoUndoInsertLabel*>(unknown_unit);
				ELabelComponent::erased(unit->begin, unit->end, not_ruType);
				break;
			}
			case ERedoUndoUnit::Action::EraseLabel: {
				auto unit = static_cast<ERedoUndoEraseLabel*>(unknown_unit);
				ELabelComponent::insertLabels(unit->begin, unit->data, not_ruType);
				break;
			}
			case ERedoUndoUnit::Action::InsertLineNumber: {
				auto unit = static_cast<ERedoUndoInsertLineNumber*>(unknown_unit);
				ELabelComponent::erasedLines(unit->begin, unit->end, not_ruType);
				break;
			}
			case ERedoUndoUnit::Action::EraseLineNumber: {
				auto unit = static_cast<ERedoUndoEraseLineNumber*>(unknown_unit);
				ELabelComponent::setLines(unit->begin, unit->data, not_ruType);
				break;
			}
			case ERedoUndoUnit::Action::InsertError: {
				auto unit = static_cast<ERedoUndoInsertError*>(unknown_unit);
				EErrorComponent::erased(unit->begin, unit->end, not_ruType);
				break;
			}
			case ERedoUndoUnit::Action::EraseError: {
				auto unit = static_cast<ERedoUndoEraseError*>(unknown_unit);
				EErrorComponent::setErrors(unit->begin, unit->data, not_ruType);
				break;
			}
			case ERedoUndoUnit::Action::InsertConnect: {
				auto unit = static_cast<ERedoUndoInsertConnect*>(unknown_unit);
				EConnectComponent::removeConnects(unit->elements, not_ruType);
				break;
			}
			case ERedoUndoUnit::Action::EraseConnect: {
				auto unit = static_cast<ERedoUndoEraseConnect*>(unknown_unit);
				EConnectComponent::addConnects(unit->data, not_ruType);
				break;
			}
		}
		removeUndo(ruType);
	}
}