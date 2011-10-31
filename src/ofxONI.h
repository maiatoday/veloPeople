#ifndef __ofxONI_h__
#define __ofxONI_h__

#include "ofMain.h"
#include <XnOpenNI.h>
#include <XnCodecIDs.h>
#include <XnCppWrapper.h>
#include "ofxOpenCv.h"
#include "HandPointDrawer.h"

#include "FlipBook.h"

// Header for NITE
#include "XnVNite.h"

static xn::Context g_Context;
static xn::DepthGenerator g_DepthGenerator;
static xn::UserGenerator g_UserGenerator;
static xn::HandsGenerator g_HandsGenerator;
static xn::ImageGenerator g_image;


// NITE objects
typedef enum
{
	IN_SESSION,
	NOT_IN_SESSION,
	QUICK_REFOCUS
} SessionState;

static XnVSessionManager* g_pSessionManager;
static SessionState g_SessionState = NOT_IN_SESSION;
static XnVFlowRouter* g_pFlowRouter;


// the drawer
static HandPointDrawer* g_pDrawer;

static XnBool g_bNeedPose = FALSE;
static XnChar g_strPose[20] = "";

#define CHECK_RC(nRetVal, what)										\
	if (nRetVal != XN_STATUS_OK)									\
	{																\
		printf("%s failed: %s\n", what, xnGetStatusString(nRetVal));\
	}

static void XN_CALLBACK_TYPE User_NewUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie)
{
	printf("New User %d\n", nId);
};
static void XN_CALLBACK_TYPE User_LostUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie)
{
	printf("Lost User id: %i\n", (unsigned int)nId);
};

// Callback: Detected a pose
static void XN_CALLBACK_TYPE UserPose_PoseDetected(xn::PoseDetectionCapability& capability, const XnChar* strPose, XnUserID nId, void* pCookie)
{
	printf("Pose %s detected for user %d\n", strPose, nId);
	g_UserGenerator.GetPoseDetectionCap().StopPoseDetection(nId);
	g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
};
// Callback: Started calibration
static void XN_CALLBACK_TYPE UserCalibration_CalibrationStart(xn::SkeletonCapability& capability, XnUserID nId, void* pCookie)
{
	printf("Calibration started for user %d\n", nId);
};
// Callback: Finished calibration
static void XN_CALLBACK_TYPE UserCalibration_CalibrationEnd(xn::SkeletonCapability& capability, XnUserID nId, XnBool bSuccess, void* pCookie)
{
	if (bSuccess)
	{
		// Calibration succeeded
		printf("Calibration complete, start tracking user %d\n", nId);
		g_UserGenerator.GetSkeletonCap().StartTracking(nId);
	}
	else
	{
		// Calibration failed
		printf("Calibration failed for user %d\n", nId);
		if (g_bNeedPose)
		{
			g_UserGenerator.GetPoseDetectionCap().StartPoseDetection(g_strPose, nId);
		}
		else
		{
			g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
		}
	}
};

// Callback for when the focus is in progress
static void XN_CALLBACK_TYPE FocusProgress(const XnChar* strFocus, const XnPoint3D& ptPosition, XnFloat fProgress, void* UserCxt)
{
//	printf("Focus progress: %s @(%f,%f,%f): %f\n", strFocus, ptPosition.X, ptPosition.Y, ptPosition.Z, fProgress);
}
// callback for session start
static void XN_CALLBACK_TYPE SessionStarting(const XnPoint3D& ptPosition, void* UserCxt)
{
    printf("Session start: (%f,%f,%f)\n", ptPosition.X, ptPosition.Y, ptPosition.Z);
    g_SessionState = IN_SESSION;
}
// Callback for session end
static void XN_CALLBACK_TYPE SessionEnding(void* UserCxt)
{
    printf("Session end\n");
    g_SessionState = NOT_IN_SESSION;
}
static void XN_CALLBACK_TYPE NoHands(void* UserCxt)
{
    if (g_SessionState != NOT_IN_SESSION) {
        printf("Quick refocus\n");
        g_SessionState = QUICK_REFOCUS;
    }
}

#define SAMPLE_XML_PATH "../bin/data/SamplesConfig.xml"

#define MAX_DEPTH 10000

static XnFloat oniColors[][3] =
{
	{1,1,1},
	{1,1,1},
	{1,1,1},
	{1,1,1},
	{1,1,1},
	{1,1,1},
	{1,1,1},
	{1,1,1},
	{1,1,1},
	{1,1,1},
	{0,0,1}
};
static XnUInt32 nColors = 10;



class ofxONI
{
    public:
        ofxONI();
        ~ofxONI();

		void setup(int midDistance, ofPoint flipPosition, ofPoint flipSize, int frameRepeat);
		void update();
		void cleanupExit();

		void drawDepth(int x, int y) {drawDepth(x, y, width, height);};
		void drawDepth(int x, int y, int w, int h);
		void drawPlayers(int x, int y) {drawPlayers(x, y, width, height);};
		void drawPlayers(int x, int y, int w, int h);
		void drawCam(int x, int y) {drawCam(x, y, width, height);};
		void drawCam(int x, int y, int w, int h);
		void drawOther();

		void printSessionState(SessionState eState);

		void calculateMaps();

		xn::SceneMetaData sceneMD;
		xn::DepthMetaData depthMD;
		xn::ImageMetaData g_imageMD;

		ofxCvGrayscaleImage depth;
		ofxCvColorImage players;
		ofxCvColorImage imgCam;

		float depthHist[MAX_DEPTH];
		unsigned char gColorBuffer[640*480*3];         // BGRA

		unsigned char * tmpGrayPixels;
		unsigned char * tmpColorPixels;
		unsigned char * tmpCamColorPixels;

		XnPoint3D LHandPoint;
		XnPoint3D RHandPoint;

		XnPoint3D getCoMPoint(XnUserID player);
		void getUsers(XnUserID aUsers[], XnUInt16& nUsers);
		XnUInt16 getUserCount();
		XnPoint3D getComUsersInFront(XnUserID& player, XnUInt16& nUsers);

		bool bDrawPlayers, bDrawCam;
		int width, height;

		int playerAlpha;
		ofTrueTypeFont myFont;

		void setPositionFactor(float x, float y);

		private:
		float xscale;
		float yscale;
		int userCount;

        FlipBook *pFlipBook;
        SessionState previousState;
};

#endif

