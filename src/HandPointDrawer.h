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
    	/**
	 * Handle a new message.
	 * Calls other callbacks for each point, then draw the depth map (if needed) and the points
	 */
	void Update(XnVMessage* pMessage);

	/**
	 * Handle creation of a new point
	 */
	void OnPointCreate(const XnVHandPointContext* cxt);
	/**
	 * Handle new position of an existing point
	 */
	void OnPointUpdate(const XnVHandPointContext* cxt);
	/**
	 * Handle destruction of an existing point
	 */
	void OnPointDestroy(XnUInt32 nID);

	/**
	 * Draw the points, each with its own color.
	 */
	void draw() const;
   protected:
    // Number of previous position to store for each hand
    XnUInt32 m_nHistorySize;
    // previous positions per hand
    std::map<XnUInt32, std::list<XnPoint3D> > m_History;

	XnFloat* m_pfPositionBuffer;
   private:
};

#endif // HANDPOINTDRAWER_H
