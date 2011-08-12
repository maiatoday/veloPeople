#ifndef HANDPOINTDRAWER_H
#define HANDPOINTDRAWER_H

#include <map>
#include <list>
#include <XnCppWrapper.h>
#include <XnVPointControl.h>


class HandPointDrawer : public XnVPointControl
{
   public:
    HandPointDrawer(XnUInt32 nHistorySize);
    virtual ~HandPointDrawer();
   protected:
    // Number of previous position to store for each hand
    XnUInt32 m_nHistorySize;
    // previous positions per hand
    std::map<XnUInt32, std::list<XnPoint3D> > m_History;

	XnFloat* m_pfPositionBuffer;
   private:
};

#endif // HANDPOINTDRAWER_H
