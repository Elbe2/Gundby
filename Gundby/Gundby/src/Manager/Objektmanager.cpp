// ------------------------------------------------------------------
// Objektmanager.cpp
// Lukas Beinlich, 04.11.2015
// Eine Liste aller Objekte
// ------------------------------------------------------------------

#include "src\Manager\Objektmanager.h"

bool CObjektManger::Render(void)
{
	for (int i = 0;i < m_Elements.size();i++)
		if (m_Elements[i])
			if (!m_Elements[i]->Render())
				return false;
	return true;
}

bool CObjektManger::RenderFont(void)
{
	for (int i = 0;i < m_Elements.size();i++)
		if (m_Elements[i])
			if (!m_Elements[i]->RenderFont())
				return false;
	return true;
}
