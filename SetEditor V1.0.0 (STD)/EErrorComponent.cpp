#include "EErrorComponent.h"

void EErrorComponent::erased(size_t begin, size_t end)
{
	::erased(m_textErrors, begin, end);
}

void EErrorComponent::inserted(size_t pos, int count)
{
	::inserted(m_textErrors, pos, count);
}
