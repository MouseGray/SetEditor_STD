#pragma once

#include "EConnectComponent.h"

#include "EErrorComponent.h"

class EFace : public EErrorComponent
{
protected:
public:
	EFace() {
		ETextComponent::addCallback(std::bind(&EControlComponent::erased, this, std::placeholders::_1, std::placeholders::_2, 
			std::placeholders::_3), ETextComponent::CallbackType::erase);
		ETextComponent::addCallback(std::bind(&EControlComponent::inserted, this, std::placeholders::_1, std::placeholders::_2, 
			std::placeholders::_3), ETextComponent::CallbackType::insert);

		ETextComponent::addCallback(std::bind(&ELabelComponent::erased, this, std::placeholders::_1, std::placeholders::_2,
			std::placeholders::_3), ETextComponent::CallbackType::erase);
		ETextComponent::addCallback(std::bind(&ELabelComponent::erasedLines, this, std::placeholders::_1, std::placeholders::_2, 
			std::placeholders::_3), ETextComponent::CallbackType::eraseLine);
		ETextComponent::addCallback(std::bind(&ELabelComponent::insertedLines, this, std::placeholders::_1, std::placeholders::_2,
			std::placeholders::_3), ETextComponent::CallbackType::insertLine);

		ETextComponent::addCallback(std::bind(&EErrorComponent::erased, this, std::placeholders::_1, std::placeholders::_2,
			std::placeholders::_3), ETextComponent::CallbackType::erase);

		ETextComponent::addCallback(std::bind(&EConnectComponent::erased, this, std::placeholders::_1, std::placeholders::_2,
			std::placeholders::_3), ETextComponent::CallbackType::erase);
		ETextComponent::addCallback(std::bind(&EConnectComponent::inserted, this, std::placeholders::_1, std::placeholders::_2,
			std::placeholders::_3), ETextComponent::CallbackType::insert);
	}

	template<ERedoUndoComponent::Type T = ERedoUndoComponent::Type::Null>
	void insert(const std::string& text);

	template<ERedoUndoComponent::Type T = ERedoUndoComponent::Type::Null>
	void erase();

	template<ERedoUndoComponent::Type T = ERedoUndoComponent::Type::Null>
	void erase(int count);

	template<ERedoUndoComponent::Type T = ERedoUndoComponent::Type::Null>
	void setOverline();

	void release(ERedoUndoComponent::Type ruType);
};

#undef min
#undef max

template<ERedoUndoComponent::Type T>
void EFace::insert(const std::string& text)
{
	if (text.size() == 0) return;
	startUndo(T);
	ETextComponent::insert(m_cursor, text, T);
	finishUndo(T);

	clearRedo(T);
}

template<ERedoUndoComponent::Type T>
void EFace::erase()
{
	if (!isSelected()) return;
	auto begin = std::min(m_cursor, m_sCursor);
	auto end   = std::max(m_cursor, m_sCursor);
	startUndo(T);
	EOverlineComponent::erase_<T>(begin, end);
	finishUndo(T);

	clearRedo(T);
}

template<ERedoUndoComponent::Type T>
void EFace::erase(int count)
{
	if (count == 0) return;
	auto begin = std::min(m_cursor, m_cursor + count);
	auto end   = std::max(m_cursor, m_cursor + count);
	startUndo(T);
	EOverlineComponent::erase_<T>(begin, end);
	finishUndo(T);

	clearRedo(T);
}

template<ERedoUndoComponent::Type T>
void EFace::setOverline()
{
	if (!isSelected()) return;
	auto start = std::min(m_cursor, m_sCursor);
	auto end = std::max(m_cursor, m_sCursor);
	startUndo(T);
	EOverlineComponent::insertOverline<T>(start, end);
	finishUndo(T);

	clearRedo(T);
}
