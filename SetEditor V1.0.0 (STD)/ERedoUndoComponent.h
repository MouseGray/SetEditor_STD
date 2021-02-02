#ifndef EREDOUNDOCOMPONENT
#define EREDOUNDOCOMPONENT

#include <list>
#include <stack>
#include <cassert>

#include "ERedoUndoUnit.h"

class ERedoUndoComponent
{
public:
	enum class Type : char {
		Null,
		Undo,
		Redo
	};

	void startUndo(ERedoUndoComponent::Type ruType);

	void finishUndo(ERedoUndoComponent::Type ruType);

	bool releaseUndo(ERedoUndoComponent::Type ruType);

	void add(ERedoUndoUnit* unit, ERedoUndoComponent::Type ruType);

	ERedoUndoUnit* getUndo(ERedoUndoComponent::Type ruType);
	void removeUndo(ERedoUndoComponent::Type ruType);

	void clearRedo(ERedoUndoComponent::Type ruType);
private:
	std::list<std::stack<ERedoUndoUnit*>> undo;
	std::list<std::stack<ERedoUndoUnit*>> redo;
};

#endif