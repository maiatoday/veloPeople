#include "HandPointDrawer.h"

// Constructor. Receives the number of previous positions to store per hand,
// and a source for depth map
HandPointDrawer::HandPointDrawer(XnUInt32 nHistory) :
	XnVPointControl("HandPointDrawer"),
	m_nHistorySize(nHistory)
{
	m_pfPositionBuffer = new XnFloat[nHistory*3];
}

// Destructor. Clear all data structures
HandPointDrawer::~HandPointDrawer()
{
	std::map<XnUInt32, std::list<XnPoint3D> >::iterator iter;
	for (iter = m_History.begin(); iter != m_History.end(); ++iter)
	{
		iter->second.clear();
	}
	m_History.clear();

	delete []m_pfPositionBuffer;
}
