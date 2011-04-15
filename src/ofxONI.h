#ifndef __ofxONI_h__
#define __ofxONI_h__

// Header for NITE
#include <XnVNite.h>

#include "ofMain.h"
#include <XnOpenNI.h>
#include <XnCodecIDs.h>
#include <XnCppWrapper.h>
#include <XnHash.h>
#include <XnLog.h>
#include "ofxOpenCv.h"
#include "PointDrawer.h"


static xn::Context g_Context;
static xn::DepthGenerator g_DepthGenerator;
static xn::UserGenerator g_UserGenerator;
static xn::HandsGenerator g_HandsGenerator;
static xn::ImageGenerator g_image;

// NITE objects
XnVSessionManager* g_pSessionManager;
XnVFlowRouter* g_pFlowRouter;

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
	// New user found
	if (g_bNeedPose)
	{
		g_UserGenerator.GetPoseDetectionCap().StartPoseDetection(g_strPose, nId);
	}
	else
	{
		g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
	}
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

//Hand NITE callback functions

SessionState g_SessionState = NOT_IN_SESSION;


// callback for session start
void XN_CALLBACK_TYPE SessionStarting(const XnPoint3D& ptPosition, void* UserCxt)
{
    printf("Session start: (%f,%f,%f)\n", ptPosition.X, ptPosition.Y, ptPosition.Z);
    g_SessionState = IN_SESSION;
}
// Callback for session end
void XN_CALLBACK_TYPE SessionEnding(void* UserCxt)
{
    printf("Session end\n");
    g_SessionState = NOT_IN_SESSION;
}
void XN_CALLBACK_TYPE NoHands(void* UserCxt)
{
    if (g_SessionState != NOT_IN_SESSION) {
        printf("Quick refocus\n");
        g_SessionState = QUICK_REFOCUS;
    }
}



//#define SAMPLE_XML_PATH "data/Sample-User.xml"
#define SAMPLE_XML_PATH "../bin/data/SamplesConfig.xml"

#define MAX_DEPTH 10000

static XnFloat oniColors[][3] =
{
	{0,1,1},
	{0,0,1},
	{0,1,0},
	{1,1,0},
	{1,0,0},
	{1,.5,0},
	{.5,1,0},
	{0,.5,1},
	{.5,0,1},
	{1,1,.5},
	{0,0,0}
};
static XnUInt32 nColors = 10;



class ofxONI
{
    public:
        ofxONI();
        ~ofxONI();

		void setup();
		void update();

		void drawDepth(int x, int y) {drawDepth(x, y, width, height);};
		void drawDepth(int x, int y, int w, int h);
		void drawPlayers(int x, int y) {drawPlayers(x, y, width, height);};
		void drawPlayers(int x, int y, int w, int h);
		void drawCam(int x, int y) {drawCam(x, y, width, height);};
		void drawCam(int x, int y, int w, int h);

		void drawSkeletonPt(XnUserID player, XnSkeletonJoint eJoint);
		void skeletonTracking();

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

		bool bDrawPlayers, bDrawCam;
		int width, height;

		int playerAlpha;
};

#endif

