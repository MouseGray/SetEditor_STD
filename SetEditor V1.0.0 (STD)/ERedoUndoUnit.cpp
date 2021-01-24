#include "ERedoUndoUnit.h"

ERedoUndoUnit::ERedoUndoUnit(Action _action) :
    action(_action)
{
}

ERedoUndoErase* ERedoUndoUnit::toERUErase()
{
    if (action == ERedoUndoUnit::Action::Erase) return static_cast<ERedoUndoErase*>(this);
}

ERedoUndoInsert* ERedoUndoUnit::toERUInsert()
{
    if (action == ERedoUndoUnit::Action::Insert) return static_cast<ERedoUndoInsert*>(this);
}


ERedoUndoInsert::ERedoUndoInsert(int _cursor, int _count) :
    ERedoUndoUnit(ERedoUndoUnit::Action::Insert),
    cursor(_cursor), 
    count(_count)
{
}

ERedoUndoErase::ERedoUndoErase(int _cursor, int _sCursor, std::string _text) :
    ERedoUndoUnit(ERedoUndoUnit::Action::Erase),
    cursor(_cursor), 
    sCursor(_sCursor), 
    text(_text)
{
}
