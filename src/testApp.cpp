#include "testApp.h"

//========================



#define	SPRING_MIN_STRENGTH		0.005
#define SPRING_MAX_STRENGTH		0.1

#define	SPRING_MIN_WIDTH		1
#define SPRING_MAX_WIDTH		3


#define MIN_MASS				1
#define MAX_MASS				3

#define MIN_BOUNCE				0.2
#define MAX_BOUNCE				0.9

#define	FIX_PROBABILITY			10		// % probability of a particle being fixed on creation

#define FORCE_AMOUNT			2

#define EDGE_DRAG				0.98

#define	GRAVITY					1

#define MAX_ATTRACTION			10
#define MIN_ATTRACTION			3

#define SECTOR_COUNT			10

#define START_MOTE_COUNT		256
#define MID_DISTANCE            3000

HatchMote* testApp:: makeHatchMote(ofPoint pos, float  m = 1.0f, float d = 1.0f)
{
    HatchMote* p = new HatchMote(pos, m, d);
    ofColor cc = pInsidePalette->getSampleColor();
    p->setInsideColor(cc);
    p->setOutsideColor(pOutsidePalette->getSampleColor());
    p->setChildColor(pHaloPalette->getSampleColor());
    p->setLabelString(pTextSampler->getSampleText());
//    p->setGlyph(pGlyphSampler->getSampleGlyph());
//    p->setBlankGlyph(pBlankSampler->getSampleGlyph());
    p->setFadeDist(width*0.6);
    physics.addParticle(p);
    p->release();	// cos addParticle(p) retains it
    return p;
}

void testApp::initScene()
{
    // clear all particles and springs etc
    physics.clear();

}

void testApp:: addRandomParticle()
{
    float posX		= ofRandom(0, width);
    float posY		= ofRandom(0, height);
    float posZ		= ofRandom(-width/2, width/2);
    float mass		= ofRandom(MIN_MASS, MAX_MASS);
    float bounce	= ofRandom(MIN_BOUNCE, MAX_BOUNCE);
    float radius	= ofMap(mass, MIN_MASS, MAX_MASS, NODE_MIN_RADIUS*fromKinectWidth, NODE_MAX_RADIUS*fromKinectWidth);

    // physics.makeParticle returns a particle pointer so you can customize it
    HatchMote* p = makeHatchMote(ofPoint(posX, posY, posZ));

    // and set a bunch of properties (you don't have to set all of them, there are defaults)
    p->setMass(mass)->setBounce(bounce)->setRadius(radius)->makeFree()->disableCollision();
    p->setFont(&myFont);

}

void testApp::addRandomForce(float f)
{
    forceTimer = f;
    for(unsigned int i=0; i<physics.numberOfParticles(); i++)
    {
        ofxMSAParticle *p = physics.getParticle(i);
        if(p->isFree()) p->addVelocity(ofPoint(ofRandom(-f, f), ofRandom(-f, f), ofRandom(-f, f)));
    }
}

void testApp::setUserAttract(bool _attractOn)
{
    userAttract = _attractOn;
    if(userAttract)
    {

    }
    else
    {
        // loop through all existing attractsions and delete them
        for(unsigned int i=0; i<physics.numberOfAttractions(); i++) physics.getAttraction(i)->turnOff();
    }
}

void testApp::updateMoteLabel()
{
    XnLabel label;

#ifdef NO_KINECT
    numberUsers = 1;

    for(unsigned int i=0; i<physics.numberOfParticles(); i++)
    {
        HatchMote *p = static_cast<HatchMote*>(physics.getParticle(i));

        label = numberUsers;
        p->setLabel(label);
    }

#else
    XnUInt16 userCount = oni.getUserCount();
    numberUsers = userCount;
    const XnLabel* pLabels = oni.sceneMD.Data();
    for(unsigned int i=0; i<physics.numberOfParticles(); i++)
    {
        HatchMote *p = static_cast<HatchMote*>(physics.getParticle(i));
        int x = p->getX()*toKinectWidth;
        int y = p->getY()*toKinectHeight;
        label = pLabels[kinectWidth*y+x];
        p->setLabel(label);

    }

#endif

}


//========================
//--------------------------------------------------------------
testApp::testApp():
    midDistance(MID_DISTANCE),
    moteCount(START_MOTE_COUNT),
    fullscreen(false)
{
    pInsidePalette = new ColorSampler("images/outside.jpg");
    pHaloPalette = new ColorSampler("images/inside.jpg");
    pOutsidePalette = new ColorSampler("images/bsod_white.png");
    pTextSampler = new TextSampler("data/text/sample.txt");
    numberUsers = 0;
    flipCount=0;
    userAttract 	= false;
    mouseRepel  	= false;
    doMouseXY		= false;		// pressing left mmouse button moves mouse in XY plane
    doMouseYZ		= false;		// pressing right mouse button moves mouse in YZ plane
    doRender		= true;
    forceTimer		= false;
    rotSpeed		= 0;
    mouseMass		= 1;
}

testApp::~testApp()
{
    delete pInsidePalette;
    delete pHaloPalette;
    delete pOutsidePalette;
    delete pTextSampler;
}

void testApp::setScreenRatios(void)
{
    int windowMode = ofGetWindowMode();

    kinectWidth = 640;
    kinectHeight = 480;
    if(windowMode == OF_FULLSCREEN)
    {
        width = ofGetScreenWidth();
        height = ofGetScreenHeight();
    }
    else if(windowMode == OF_WINDOW)
    {
        width = ofGetWidth();
        height = ofGetHeight();

    }
    fromKinectWidth = (float)width/(float)kinectWidth;
    fromKinectHeight = (float)height/(float)kinectHeight;
    toKinectWidth = (float)kinectWidth/(float)width;
    toKinectHeight = (float)kinectHeight/(float)height;
    physics.setWorldSize(ofPoint(0, 0, 0), ofPoint(width, height, width));
    oni.setPositionFactor(fromKinectWidth, fromKinectHeight);
}
//--------------------------------------------------------------
void testApp::setup()
{
    if (XML.loadFile("mySettings.xml")) {
        midDistance  = XML.getValue("ROOM:MIDDLE", MID_DISTANCE);
        moteCount    = XML.getValue("ROOM:MOTE_COUNT", START_MOTE_COUNT);
        fullscreen   = (XML.getValue("ROOM:FULLSCREEN", 1) == 1)?true:false;
//        pFlipBook->setframeRepeatMax(XML.getValue("ROOM:FRAME_REPEAT", 5));
    }
    ofBackground(0,0,0);
    ofSetBackgroundAuto(false);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofSetWindowPosition(ofGetScreenWidth() - ofGetWidth() - 20, 20);

#ifndef NO_KINECT
    oni.setup(midDistance);

    // players
    for (int i = 0; i < MAX_PLAYERS; i++) players[i].allocate(oni.width, oni.height);
#endif
    //========================

    ofSetFullscreen(fullscreen);
    ofHideCursor();
    setScreenRatios();

// font needs to be loaded before the particles are created because they all use it to draw
    myFont.loadFont("verdana.ttf", (int)8*fromKinectWidth);

    physics.setGravity(ofPoint(0, 0, 0));

    // set world dimensions, not essential, but speeds up collision
    physics.setWorldSize(ofPoint(0, 0, 0), ofPoint(width, height, width));
    physics.setSectorCount(SECTOR_COUNT);
    physics.setDrag(0.97f);
    physics.setDrag(1);		// FIXTHIS
    physics.enableCollision();

    initScene();
    for(int i=0; i<moteCount; i++) addRandomParticle();

    for(unsigned int i=0; i<physics.numberOfAttractions(); i++) physics.getAttraction(i)->turnOff();

    addRandomForce(FORCE_AMOUNT);
    //========================
    snapCounter = 0;
    width = ofGetWidth();
    height = ofGetHeight();
}

//--------------------------------------------------------------
void testApp::update()
{
    XnUInt16 nUsersPrev = numberUsers;
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
#ifndef NO_KINECT
    oni.update();
#endif
    //========================
    physics.update();
    updateMoteLabel();
    if (numberUsers != nUsersPrev)
    {
        printf(" numberUsers %d", numberUsers);
    }
}

//--------------------------------------------------------------
void testApp::draw()
{

    glPushMatrix();

#ifdef NO_KINECT
#else
    glScalef(ofGetWidth() / (float)oni.width, ofGetHeight() / (float)oni.height, 1);
#endif
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    if (oni.bDrawPlayers)
        oni.drawPlayers(0, 0);

    ofSetColor(255, 255, 255, 100);
    glPopMatrix();
    physics.draw();
    if (doVideoWrite)
    {

#ifdef DO_VIDEO
        saveScreen.grabScreen(0,0,width,height);
        TIS.saveThreaded(saveScreen);
#endif

    }

}



//--------------------------------------------------------------
void testApp::keyPressed  (int key)
{
#ifndef NO_KINECT
    if(key == '1') oni.bDrawPlayers = !oni.bDrawPlayers;
    if(key == '2') oni.bDrawCam = !oni.bDrawCam;
#endif
    switch(key)
    {
    case 't':
        ofToggleFullscreen();
        setScreenRatios();
        break;
    case '`':
        ofImage screenImg;
        screenImg.allocate(width, height, OF_IMAGE_COLOR);
        screenImg.grabScreen(0,0,width,height);
        screenImg.saveImage("screenshot-"+ofToString(snapCounter)+".png");
        snapCounter++;
        break;
    }

}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{
    switch(key)
    {
    case 'x':
        doMouseXY = false;
        break;
    case 'z':
        doMouseYZ = false;
        break;
    }

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y )
{

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
    doVideoWrite = !doVideoWrite;

}

//--------------------------------------------------------------
void testApp::resized(int w, int h)
{

}

