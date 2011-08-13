#include "HandPointDrawer.h"

#include "ofMain.h"

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
    for (iter = m_History.begin(); iter != m_History.end(); ++iter) {
        iter->second.clear();
    }
    m_History.clear();

    delete []m_pfPositionBuffer;
}

static XnBool bShouldPrint = false;
void HandPointDrawer::OnPointCreate(const XnVHandPointContext* cxt)
{
    printf("** %d\n", cxt->nID);
    // Create entry for the hand
    m_History[cxt->nID].clear();
    bShouldPrint = true;
    OnPointUpdate(cxt);
    bShouldPrint = true;
}
// Handle new position of an existing hand
void HandPointDrawer::OnPointUpdate(const XnVHandPointContext* cxt)
{
    // positions are kept in projective coordinates, since they are only used for drawing
    XnPoint3D ptProjective(cxt->ptPosition);

    if (bShouldPrint)printf("Point (%f,%f,%f)", ptProjective.X, ptProjective.Y, ptProjective.Z);
//	m_DepthGenerator.ConvertRealWorldToProjective(1, &ptProjective, &ptProjective);
    if (bShouldPrint)printf(" -> (%f,%f,%f)\n", ptProjective.X, ptProjective.Y, ptProjective.Z);

    // Add new position to the history buffer
    m_History[cxt->nID].push_front(ptProjective);
    // Keep size of history buffer
    if (m_History[cxt->nID].size() > m_nHistorySize)
        m_History[cxt->nID].pop_back();
    bShouldPrint = false;
}

// Handle destruction of an existing hand
void HandPointDrawer::OnPointDestroy(XnUInt32 nID)
{
    // No need for the history buffer
    m_History.erase(nID);
}
void HandPointDrawer::draw() const
{


    std::map<XnUInt32, std::list<XnPoint3D> >::const_iterator PointIterator;

    // Go over each existing hand
    for (PointIterator = m_History.begin();
            PointIterator != m_History.end();
            ++PointIterator) {
        // Clear buffer
        XnUInt32 nPoints = 0;
        XnUInt32 i = 0;
        XnUInt32 Id = PointIterator->first;

        // Go over all previous positions of current hand
        std::list<XnPoint3D>::const_iterator PositionIterator;
        for (PositionIterator = PointIterator->second.begin();
                PositionIterator != PointIterator->second.end();
                ++PositionIterator, ++i) {
            // Add position to buffer
            XnPoint3D pt(*PositionIterator);
//            m_pfPositionBuffer[3*i] = pt.X;
//            m_pfPositionBuffer[3*i + 1] = pt.Y;
//            m_pfPositionBuffer[3*i + 2] = 0;//pt.Z();
            ofSetColor(0,255,0, 255);
            ofFill();
            ofRect(pt.X,pt.Y,20,20);
        }

//
//		// Set color
//		XnUInt32 nColor = Id % nColors;
//		XnUInt32 nSingle = GetPrimaryID();
//		if (Id == GetPrimaryID())
//			nColor = 6;
//		// Draw buffer:
//		glColor4f(Colors[nColor][0],
//				Colors[nColor][1],
//				Colors[nColor][2],
//				1.0f);
//		glPointSize(2);
//		glVertexPointer(3, GL_FLOAT, 0, m_pfPositionBuffer);
//		glDrawArrays(GL_LINE_STRIP, 0, i);
//
//		glPointSize(8);
//		glDrawArrays(GL_POINTS, 0, 1);
//		glFlush();
    }
}

// Handle a new Message
void HandPointDrawer::Update(XnVMessage* pMessage)
{
    // PointControl's Update calls all callbacks for each hand
    XnVPointControl::Update(pMessage);

    draw();
}
