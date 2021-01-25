#pragma once

#include "ETextComponent.h"
#include "ESelectComponent.h"
#include "ERedoUndoComponent.h"

class EFace : public ETextComponent, public EControlComponent, public ERedoUndoComponent
{
public:
	void insert(const std::string& text, ERedoUndoComponent::Type type = ERedoUndoComponent::Type::Null);
	void erase(ERedoUndoComponent::Type type = ERedoUndoComponent::Type::Null);
	void erase(int count, ERedoUndoComponent::Type type = ERedoUndoComponent::Type::Null);

	void undo();
	void redo();
};

