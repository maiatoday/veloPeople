#include "ofxONI.h"

ofxONI::ofxONI()
{
    LHandPoint.X = 0;
    LHandPoint.Y = 0;
    LHandPoint.Z = 0;
    RHandPoint.X = 0;
    RHandPoint.Y = 0;
    RHandPoint.Z = 0;
    userCount = 0;
}

void ofxONI::cleanupExit()
{
//	g_DepthGenerator.Release();
//	g_HandsGenerator.Release();
//	g_UserGenerator.Release();

    g_Context.Shutdown();
//	g_Context.Release();

}

ofxONI::~ofxONI()
{
    cleanupExit();
    delete[] tmpGrayPixels;
	delete[] tmpColorPixels;
	delete[] tmpCamColorPixels;

}

void ofxONI::setup(int midDistance)
{

    myFont.loadFont("verdana.ttf", (int)10*xscale);
    XnStatus nRetVal = XN_STATUS_OK;

    bDrawPlayers = false;
    bDrawCam = false;

    playerAlpha = 20;

    printf("InitFromXmlFile\n");
    nRetVal = g_Context.InitFromXmlFile(SAMPLE_XML_PATH);
    CHECK_RC(nRetVal, "InitFromXml");


    printf("FindExistingNode Depth\n");
    nRetVal = g_Context.FindExistingNode(XN_NODE_TYPE_DEPTH, g_DepthGenerator);
    CHECK_RC(nRetVal, "Find depth generator");
    printf("FindExistingNode User\n");
    nRetVal = g_Context.FindExistingNode(XN_NODE_TYPE_USER, g_UserGenerator);
    if (nRetVal != XN_STATUS_OK) {
        nRetVal = g_UserGenerator.Create(g_Context);
        CHECK_RC(nRetVal, "Find user generator");
    }

    printf("FindExistingNode Image\n");
    nRetVal = g_Context.FindExistingNode(XN_NODE_TYPE_IMAGE, g_image);
    CHECK_RC(nRetVal, "Find image generator");

    printf("FindExistingNode Hands\n");
    nRetVal = g_Context.FindExistingNode(XN_NODE_TYPE_HANDS, g_HandsGenerator);
    CHECK_RC(nRetVal, "Find hands generator");

    // Create NITE objects
    g_pSessionManager = new XnVSessionManager;
    nRetVal = g_pSessionManager->Initialize(&g_Context, "Click,Wave", "RaiseHand");
    CHECK_RC(nRetVal, "SessionManager::Initialize");

    g_pSessionManager->RegisterSession(NULL, SessionStarting, SessionEnding, FocusProgress);
    g_pDrawer = new HandPointDrawer(10, g_DepthGenerator);
    g_pDrawer->setMidDistance(midDistance);
    g_pFlowRouter = new XnVFlowRouter;
    g_pFlowRouter->SetActive(g_pDrawer);

    g_pSessionManager->AddListener(g_pFlowRouter);

    g_pDrawer->RegisterNoPoints(NULL, NoHands);

    nRetVal = g_Context.StartGeneratingAll();
    CHECK_RC(nRetVal, "StartGenerating");

    // old
    XnCallbackHandle hUserCBs;

    XnCallbackHandle hUserCallbacks, hCalibrationCallbacks, hPoseCallbacks;
    g_UserGenerator.RegisterUserCallbacks(User_NewUser, User_LostUser, NULL, hUserCallbacks);
    g_DepthGenerator.GetMetaData(depthMD);

    width = depthMD.XRes();
    height = depthMD.YRes();

    tmpGrayPixels = new unsigned char[width * height];
    tmpColorPixels = new unsigned char[width * height * 3];
    tmpCamColorPixels = new unsigned char[width * height * 3];

    depth.allocate(width, height);
    players.allocate(width, height);
    imgCam.allocate(width, height);
}

void ofxONI::update()
{
    g_DepthGenerator.GetMetaData(depthMD);
    g_UserGenerator.GetUserPixels(0, sceneMD);
    g_image.GetMetaData(g_imageMD);
    calculateMaps();
    g_Context.WaitAndUpdateAll();
    g_pSessionManager->Update(&g_Context);
    printSessionState(g_SessionState);
}

void ofxONI::printSessionState(SessionState eState)
{

    string str = "";
    switch (eState) {
    case IN_SESSION:
        ofSetColor(0,0,0,255);
        str.append("I see you");
        myFont.drawString(str, 10*xscale,10*yscale);
        break;
    case NOT_IN_SESSION:
        if (userCount > 0) {
            ofSetColor(0,0,0,255);
            str.append("WAVE!");
            myFont.drawString(str, 50*xscale, 50*yscale);
        }
        break;
    case QUICK_REFOCUS:
        ofSetColor(0,0,0,255);
        str.append("Wave again");
        myFont.drawString(str, 320*xscale,240*yscale);
        break;
    }


}

void ofxONI::calculateMaps()
{
    // Calculate the accumulative histogram

    unsigned int nValue = 0;
    unsigned int nHistValue = 0;
    unsigned int nIndex = 0;
    unsigned int nX = 0;
    unsigned int nY = 0;
    unsigned int nNumberOfPoints = 0;
    const XnDepthPixel* pDepth = depthMD.Data();
    const XnUInt8* pImage = g_imageMD.Data();

    memset(depthHist, 0, MAX_DEPTH*sizeof(float));
    int n = 0;
    for (nY=0; nY < height; nY++) {
        for (nX=0; nX < width; nX++, nIndex++) {
            nValue = pDepth[nIndex];

            if (nValue != 0) {
                depthHist[nValue]++;
                nNumberOfPoints++;
            }
        }
    }

    for (nIndex=1; nIndex < MAX_DEPTH; nIndex++) {
        depthHist[nIndex] += depthHist[nIndex-1];
    }

    if (nNumberOfPoints) {
        for (nIndex=1; nIndex < MAX_DEPTH; nIndex++) {
            depthHist[nIndex] = (unsigned int)(256 * (1.0f - (depthHist[nIndex] / nNumberOfPoints)));
        }
    }

    const XnLabel* pLabels = sceneMD.Data();
    XnLabel label;

    for (int i = 0; i < width * height; i++) {
        nValue = pDepth[i];
        label = pLabels[i];
        XnUInt32 nColorID = label % nColors;
        if (label == 0) {
            nColorID = nColors;
        }

        if (nValue != 0) {
            nHistValue = depthHist[nValue];
            tmpGrayPixels[i] = nHistValue;

            tmpColorPixels[i * 3 + 0] = playerAlpha * oniColors[nColorID][0];
            tmpColorPixels[i * 3 + 1] = playerAlpha * oniColors[nColorID][1];
            tmpColorPixels[i * 3 + 2] = playerAlpha * oniColors[nColorID][2];
        } else {
            tmpGrayPixels[i] = 0;

            tmpColorPixels[i * 3 + 0] = 0;
            tmpColorPixels[i * 3 + 1] = 0;
            tmpColorPixels[i * 3 + 2] = 0;
        }
    }

    const XnRGB24Pixel* pImageRow = g_imageMD.RGB24Data(); // - g_imageMD.YOffset();

    for (XnUInt y = 0; y < height; ++y) {
        const XnRGB24Pixel* pImage = pImageRow; // + g_imageMD.XOffset();

        for (XnUInt x = 0; x < width; ++x, ++pImage) {
            int index = (y*width + x)*3;
            gColorBuffer[index + 2] = (unsigned char) pImage->nBlue;
            gColorBuffer[index + 1] = (unsigned char) pImage->nGreen;
            gColorBuffer[index + 0] = (unsigned char) pImage->nRed;
        }
        pImageRow += width;
    }

    depth.setFromPixels(tmpGrayPixels, width, height);
    players.setFromPixels(tmpColorPixels, width, height);
    imgCam.setFromPixels(gColorBuffer, width, height);

}

void ofxONI::drawDepth(int x, int y, int w, int h)
{
    depth.draw(x, y, w, h);
}

void ofxONI::drawCam(int x, int y, int w, int h)
{
    imgCam.draw(x, y, w, h);
}

void ofxONI::drawPlayers(int x, int y, int w, int h)
{
    players.draw(x, y, w, h);

    XnUserID aUsers[15];
    XnUInt16 nUsers;
    g_UserGenerator.GetUsers(aUsers, nUsers);
    for (int i = 0; i < nUsers; ++i) {
        XnPoint3D com;
        g_UserGenerator.GetCoM(aUsers[i], com);
        g_DepthGenerator.ConvertRealWorldToProjective(1, &com, &com);

        ofSetColor(255, 255, 255);
        ofRect(com.X - 2, com.Y - 10, 10, 12);
        ofSetColor(128, 128, 55);
        ofDrawBitmapString(ofToString((int)aUsers[i]), com.X, com.Y);
    }
}
XnPoint3D ofxONI::getCoMPoint(XnUserID player)
{

    XnPoint3D com;
    g_UserGenerator.GetCoM(player, com);
    g_DepthGenerator.ConvertRealWorldToProjective(1, &com, &com);
    return com;
}

void ofxONI::getUsers(XnUserID aUsers[], XnUInt16& nUsers)
{

    g_UserGenerator.GetUsers(aUsers, nUsers);
}

XnUInt16 ofxONI::getUserCount()
{
    XnUserID aUsers[15];
    XnUInt16 nUsers;
    XnUInt16 retUsers = 0;
    g_UserGenerator.GetUsers(aUsers, nUsers);
    userCount = nUsers;

    for (int i = 0; i < nUsers; ++i) {
        XnPoint3D com;
        g_UserGenerator.GetCoM(aUsers[i], com);
        if (com.Z != 0) {
            // only count users in the scene
            retUsers++;
        }
    }
    return retUsers;
}

XnPoint3D ofxONI::getComUsersInFront(XnUserID& player, XnUInt16& nUsers)
{
    XnPoint3D com[15];
    XnPoint3D pt;
    pt.X = pt.Y = pt.Z = 0;
    player = 0;
    XnUserID aUsers[15];
    XnFloat closestZ = 10000;
    g_UserGenerator.GetUsers(aUsers, nUsers);
    userCount = nUsers;
    for (int i = 0; i < nUsers; i++) {
        g_UserGenerator.GetCoM(aUsers[i], com[i]);
        if (closestZ > com[i].Z) {
            closestZ = com[i].Z;
            pt = com[i];
            player = aUsers[i];
        }
    }

//    g_DepthGenerator.ConvertRealWorldToProjective(1, &pt, &pt);
    return pt;
}


void ofxONI::setPositionFactor(float x, float y)
{
    xscale = x;
    yscale = y;
    if (g_pDrawer) g_pDrawer->setPositionFactor(x,y);
    myFont.loadFont("verdana.ttf", (int)10*xscale);
}




