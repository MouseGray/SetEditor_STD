#ifndef EREDOUNDOCOMPONENT
#define EREDOUNDOCOMPONENT

#include <list>
#include <stack>

#include "ERedoUndoUnit.h"

class ERedoUndoComponent
{
public:
	enum class Type : char {
		Null,
		Undo,
		Redo
	};

	template<ERedoUndoComponent::Type>
	void begin() { undo.push_back(std::stack<ERedoUndoUnit*>()); }

	template<>
	void begin<ERedoUndoComponent::Type::Redo>() { redo.push_back(std::stack<ERedoUndoUnit*>()); }

	template<ERedoUndoComponent::Type>
	void add(ERedoUndoUnit* unit) { undo.back().push(unit); }

	template<>
	void add<ERedoUndoComponent::Type::Redo>(ERedoUndoUnit* unit) { redo.back().push(unit); }

	template<ERedoUndoComponent::Type>
	bool release() {
		if (undo.empty()) return false;
		if (undo.back().empty()) {
			undo.pop_back();
			return false;
		}
		return true;
	}

	template<>
	bool release<ERedoUndoComponent::Type::Redo>() {
		if (redo.empty()) return false;
		if (redo.back().empty()) {
			redo.pop_back();
			return false;
		}
		return true;
	}

	ERedoUndoUnit* getUndo();
	void removeUndo();

	ERedoUndoUnit* getRedo();
	void removeRedo();

	template<ERedoUndoComponent::Type>
	void clearRedo() {}

	template<>
	void clearRedo<ERedoUndoComponent::Type::Null>() {
		for (auto a : redo) {
			while (!a.empty()) {
				delete a.top();
				a.pop();
			}
		}
		redo.clear();
	}
private:
	std::list<std::stack<ERedoUndoUnit*>> undo;
	std::list<std::stack<ERedoUndoUnit*>> redo;
};

#endif