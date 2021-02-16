#include "EUndoComponent.h"

void EUndoComponent::start()
{
	m_pointer++;
}

void EUndoComponent::end()
{
	if (m_data[m_pointer - 1]->type == UndoUnit::Type::Barrier) {
		m_pointer--;
		return;
	}
	//add(new UndoUnit(UndoUnit::Type::Barrier));
}
