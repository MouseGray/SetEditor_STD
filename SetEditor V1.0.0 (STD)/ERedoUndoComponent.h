#pragma once

#include <list>

#include "ERedoUndoUnit.h"

class ERedoUndoComponent : public ESelectComponent
{
public:
	enum class Type : char {
		Null,
		Undo,
		Redo
	};

	template<ERedoUndoComponent::Type T>
	void urInsert(const std::string& text);
	template<ERedoUndoComponent::Type T>
	void urErase();
	template<ERedoUndoComponent::Type T>
	void urErase(int count);

	template<ERedoUndoComponent::Type T>
	void add(ERedoUndoUnit* unit);

	void releaseUndo();
	void releaseRedo();
private:
	std::list<ERedoUndoUnit*> undo;
	std::list<ERedoUndoUnit*> redo;
};

template<ERedoUndoComponent::Type T>
inline void ERedoUndoComponent::urInsert(const std::string& text)
{
	auto cursor = m_cursor;
	auto count = insert(text);
	add<T>(new ERedoUndoInsert(cursor, count));
}

template<ERedoUndoComponent::Type T>
inline void ERedoUndoComponent::urErase()
{
	auto cursor = m_cursor;
	auto sCursor = m_sCursor;
	auto text = sErase();
	add<T>(new ERedoUndoErase(cursor, sCursor, text));
}

template<ERedoUndoComponent::Type T>
inline void ERedoUndoComponent::urErase(int count)
{
	auto cursor = m_cursor;
	auto sCursor = m_sCursor;
	auto text = sErase(count);
	add<T>(new ERedoUndoErase(cursor, sCursor, text));
}

template<ERedoUndoComponent::Type T>
inline void ERedoUndoComponent::add(ERedoUndoUnit* unit)
{
	if(T == ERedoUndoComponent::Type::Undo) undo.push_back(unit);
	else if (T == ERedoUndoComponent::Type::Redo) redo.push_back(unit);
}
