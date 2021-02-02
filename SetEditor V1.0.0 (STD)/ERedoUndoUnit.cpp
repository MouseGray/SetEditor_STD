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

ERedoUndoInsertLabel* ERedoUndoUnit::toERUInsertLabel()
{
    return static_cast<ERedoUndoInsertLabel*>(this);
}

ERedoUndoEraseLabel* ERedoUndoUnit::toERUEraseLabel()
{
    return static_cast<ERedoUndoEraseLabel*>(this);
}

ERedoUndoInsertError* ERedoUndoUnit::toERUInsertError()
{
    return static_cast<ERedoUndoInsertError*>(this);
}

ERedoUndoEraseError* ERedoUndoUnit::toERUEraseError()
{
    return static_cast<ERedoUndoEraseError*>(this);
}

ERedoUndoInsertLineNumber* ERedoUndoUnit::toERUInsertLineNumber()
{
    return static_cast<ERedoUndoInsertLineNumber*>(this);
}

ERedoUndoEraseLineNumber* ERedoUndoUnit::toERUEraseLineNumber()
{
    return static_cast<ERedoUndoEraseLineNumber*>(this);
}

ERedoUndoInsertConnect* ERedoUndoUnit::toERUInsertConnect()
{
    return static_cast<ERedoUndoInsertConnect*>(this);
}

ERedoUndoEraseConnect* ERedoUndoUnit::toERUEraseConnect()
{
    return static_cast<ERedoUndoEraseConnect*>(this);
}
