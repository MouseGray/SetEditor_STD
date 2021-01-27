#pragma once

#include "EConnectComponent.h"

class EFace : public EConnectComponent
{
public:
	template<ERedoUndoComponent::Type T = ERedoUndoComponent::Type::Null>
	void insert(const std::string& text);

	template<ERedoUndoComponent::Type T = ERedoUndoComponent::Type::Null>
	void erase();

	template<ERedoUndoComponent::Type T = ERedoUndoComponent::Type::Null>
	void erase(int count);

	template<ERedoUndoComponent::Type T = ERedoUndoComponent::Type::Null>
	void setOverline();

	void undo();
	void redo();
};

#undef min
#undef max

template<ERedoUndoComponent::Type T>
void EFace::insert(const std::string& text)
{
	if (text.size() == 0) return;
	begin<T>();
	auto count = ETextComponent::insert<T>(m_cursor, text);
	if (count == 0) release<T>();
	EControlComponent::moveTo<T>(m_cursor + count, m_cursor + count);
	ERedoUndoComponent::clearRedo<T>();
}

template<ERedoUndoComponent::Type T>
void EFace::erase()
{
	if (!isSelected()) return;
	auto pos = std::min(m_cursor, m_sCursor);
	begin<T>();
	if (!EOverlineComponent::erase_<T>(pos, std::abs(m_sCursor - m_cursor))) release<T>();
	EControlComponent::moveTo<T>(pos, pos);
	ERedoUndoComponent::clearRedo<T>();
}

template<ERedoUndoComponent::Type T>
void EFace::erase(int count)
{
	if (count == 0) return;
	auto pos = std::min(m_cursor, m_cursor + count);
	begin<T>();
	if (!EOverlineComponent::erase_<T>(pos, std::abs(count))) release<T>();
	EControlComponent::moveTo<T>(pos, pos);
	ERedoUndoComponent::clearRedo<T>();
}

template<ERedoUndoComponent::Type T>
void EFace::setOverline()
{
	if (!isSelected()) return;
	auto start = std::min(m_cursor, m_sCursor);
	auto end = std::max(m_cursor, m_sCursor);
	begin<T>();
	EOverlineComponent::insertOverline<T>(start, end);
	EControlComponent::moveTo<T>(start, start);
	ERedoUndoComponent::clearRedo<T>();
}
