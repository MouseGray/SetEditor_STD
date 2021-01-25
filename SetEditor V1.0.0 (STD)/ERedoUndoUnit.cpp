#include "ERedoUndoUnit.h"

ERedoUndoUnit::ERedoUndoUnit(Action _action) :
    action(_action)
{
}

ERedoUndoErase* ERedoUndoUnit::toERUErase()
{
    return static_cast<ERedoUndoErase*>(this);
}

ERedoUndoInsert* ERedoUndoUnit::toERUInsert()
{
    return static_cast<ERedoUndoInsert*>(this);
}

ERedoUndoCursor* ERedoUndoUnit::toERUCursor()
{
    return static_cast<ERedoUndoCursor*>(this);
}