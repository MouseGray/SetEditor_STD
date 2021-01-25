#include "EFace.h"

void EFace::insert(const std::string& text, ERedoUndoComponent::Type type)
{
	if (text.size() == 0) return;
	auto count = ETextComponent::insert(m_cursor, text);
	if (count == 0) return;

	type == ERedoUndoComponent::Type::Redo ? beginRedo() : beginUndo();
	{
		auto unit = new ERedoUndoInsert();
		unit->pos = m_cursor;
		unit->count = count;
		type == ERedoUndoComponent::Type::Redo ? addRedo(unit) : addUndo(unit);
	}
	{
		auto unit = new ERedoUndoCursor();
		unit->cursor = unit->sCursor = moveTo(m_cursor + count);
		type == ERedoUndoComponent::Type::Redo ? addRedo(unit) : addUndo(unit);
	}	
	if (type == ERedoUndoComponent::Type::Null) clearRedo();
}

void EFace::erase(ERedoUndoComponent::Type type)
{
	if (!isSelected()) return;
	auto pos = std::min(m_cursor, m_sCursor);
	auto text = ETextComponent::erase(pos, std::abs(m_sCursor - m_cursor));
	if (text.size() == 0) return;

	type == ERedoUndoComponent::Type::Redo ? beginRedo() : beginUndo();
	{
		auto unit = new ERedoUndoErase();
		unit->pos = pos;
		unit->text = text;
		type == ERedoUndoComponent::Type::Redo ? addRedo(unit) : addUndo(unit);
	}
	{
		auto unit = new ERedoUndoCursor();
		unit->cursor = unit->sCursor = moveTo(pos);
		type == ERedoUndoComponent::Type::Redo ? addRedo(unit) : addUndo(unit);
	}
	if (type == ERedoUndoComponent::Type::Null) clearRedo();
}


void EFace::erase(int count, ERedoUndoComponent::Type type)
{
	auto size = static_cast<int>(m_buffer.size());
	auto pos_1 = std::max(std::min(std::min(m_cursor, m_cursor + count), size), 0);
	auto pos_2 = std::max(std::min(std::max(m_cursor, m_cursor + count), size), 0);

	auto pos = std::min(pos_1, pos_2);
	auto _count = std::min(std::abs(pos_1 - pos_2), std::abs(count));

	if (_count == 0) return;
	auto text = ETextComponent::erase(pos, _count);
	if (text.size() == 0) return;

	type == ERedoUndoComponent::Type::Redo ? beginRedo() : beginUndo();
	{
		auto unit = new ERedoUndoErase();
		unit->pos = pos;
		unit->text = text;
		type == ERedoUndoComponent::Type::Redo ? addRedo(unit) : addUndo(unit);
	}
	{
		auto unit = new ERedoUndoCursor();
		unit->cursor = unit->sCursor = moveTo(pos);
		type == ERedoUndoComponent::Type::Redo ? addRedo(unit) : addUndo(unit);
	}
	if (type == ERedoUndoComponent::Type::Null) clearRedo();
}

void EFace::undo()
{
	while (releaseUndo())
	{
		auto unknown_unit = getUndo();
		switch (unknown_unit->action)
		{
			case ERedoUndoUnit::Action::Insert: {
				auto unit = static_cast<ERedoUndoInsert*>(unknown_unit);
				m_cursor = unit->pos;
				erase(unit->count, ERedoUndoComponent::Type::Redo);
				break;
			}
			case ERedoUndoUnit::Action::Erase: {
				auto unit = static_cast<ERedoUndoErase*>(unknown_unit);
				m_cursor = unit->pos;
				insert(unit->text, ERedoUndoComponent::Type::Redo);
				break;
			}
			case ERedoUndoUnit::Action::Cursor: {
				auto unit = static_cast<ERedoUndoCursor*>(unknown_unit);
				m_cursor = unit->cursor;
				m_sCursor = unit->sCursor;
				break;
			}
		}
		removeUndo();
	}
}

void EFace::redo()
{
	while (releaseRedo())
	{
		auto unknown_redo = getRedo();
		switch (unknown_redo->action)
		{
			case ERedoUndoUnit::Action::Insert: {
				auto unit = static_cast<ERedoUndoInsert*>(unknown_redo);
				m_cursor = unit->pos;
				erase(unit->count, ERedoUndoComponent::Type::Undo);
				break;
			}
			case ERedoUndoUnit::Action::Erase: {
				auto unit = static_cast<ERedoUndoErase*>(unknown_redo);
				m_cursor = unit->pos;
				insert(unit->text, ERedoUndoComponent::Type::Undo);
				break;
			}
			case ERedoUndoUnit::Action::Cursor: {
				auto unit = static_cast<ERedoUndoCursor*>(unknown_redo);
				m_cursor = unit->cursor;
				m_sCursor = unit->sCursor;
				break;
			}
		}
		removeRedo();
	}
}