#include "HandPointDrawer.h"

#include "ofMain.h"

// Constructor. Receives the number of previous positions to store per hand,
// and a source for depth map
HandPointDrawer::HandPointDrawer(XnUInt32 nHistory, xn::DepthGenerator depthGenerator) :
    XnVPointControl("HandPointDrawer"),
    m_nHistorySize(nHistory), m_DepthGenerator(depthGenerator), xscale(10), yscale(1)
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
    m_DepthGenerator.ConvertRealWorldToProjective(1, &ptProjective, &ptProjective);
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
        XnUInt32 i = 0;
        XnUInt32 Id = PointIterator->first;

        // Go over all previous positions of current hand
        std::list<XnPoint3D>::const_iterator PositionIterator;
        for (PositionIterator = PointIterator->second.begin();
                PositionIterator != PointIterator->second.end();
                ++PositionIterator, ++i) {
            // Add position to buffer
            XnPoint3D pt(*PositionIterator);
            m_pfPositionBuffer[3*i] = (pt.X)*xscale;
            m_pfPositionBuffer[3*i + 1] = (pt.Y)*yscale;
            m_pfPositionBuffer[3*i + 2] = 0;//pt.Z;

            // the Z works in real world dimensions i.e. mm from the sensor
            // so at 3m=3000mm the radius will be at it's smallest
            float zSolidRadius = ofLerp(6,64,1-(pt.Z/3000))*xscale*2;
            float zEmptyRadius = ofLerp(8,76,1-(pt.Z/3000))*xscale*2;

            // Set color
            // Draw buffer:
            glColor4f(1.0,0,0, 0.8f);
            glPointSize(8);
            glVertexPointer(3, GL_FLOAT, 0, m_pfPositionBuffer);
            glDrawArrays(GL_LINE_STRIP, 0, i);

            glColor4f(0,0,0, 0.8f);
            ofFill();
            ofRect((pt.X)*xscale, (pt.Y)*yscale, zSolidRadius, zSolidRadius);
            ofNoFill();
            glColor4f(1,1,1, 0.5f);
            ofRect((pt.X)*xscale, (pt.Y)*yscale, zEmptyRadius, zEmptyRadius);
//            drawCircle((pt.X)*xscale, (pt.Y)*yscale, zSolidRadius, 32, true);
//            glColor4f(1,1,1, 0.5f);
//            drawCircle((pt.X)*xscale, (pt.Y)*yscale, zEmptyRadius, 32, false);
            glFlush();

        }
    }
}

// Handle a new Message
void HandPointDrawer::Update(XnVMessage* pMessage)
{
    // PointControl's Update calls all callbacks for each hand
    XnVPointControl::Update(pMessage);

    draw();
}

void HandPointDrawer::drawCircle(float cx, float cy, float r, int num_segments, bool filled=false) const
{
    float theta = 2 * 3.1415926 / float(num_segments);
    float c = cosf(theta);//precalculate the sine and cosine
    float s = sinf(theta);
    float t;

    float x = r;//we start at angle = 0
    float y = 0;
    if (filled) {
        glBegin(GL_POLYGON);
    } else {
        glBegin(GL_LINE_LOOP);
    }
    for(int ii = 0; ii < num_segments; ii++) {
        glVertex2f(x + cx, y + cy);//output vertex

        //apply the rotation matrix
        t = x;
        x = c * x - s * y;
        y = s * t + c * y;
    }
    glEnd();
}
