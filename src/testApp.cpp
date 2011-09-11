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

HatchMote* testApp:: makeHatchMote(ofPoint pos, float  m = 1.0f, float d = 1.0f)
{
    HatchMote* p = new HatchMote(pos, m, d);
    ofColor cc = pInsidePalette->getSampleColor();
    p->setInsideColor(cc);
    p->setOutsideColor(pOutsidePalette->getSampleColor());
    p->setChildColor(cc);
    p->setLabelString(pTextSampler->getSampleText());
    p->setGlyph(pGlyphSampler->getSampleGlyph());
    p->setBlankGlyph(pBlankSampler->getSampleGlyph());
    p->setFadeDist(width*0.6);
    physics.addParticle(p);
    p->release();	// cos addParticle(p) retains it
    return p;
}

void testApp::initScene()
{
    // clear all particles and springs etc
    physics.clear();
    sound.sendEvent(SOUND_EVENT_START);

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

void testApp::addRandomSpring()
{
    ofxMSAParticle *a = physics.getParticle((int)ofRandom(0, physics.numberOfParticles()));
    ofxMSAParticle *b = physics.getParticle((int)ofRandom(0, physics.numberOfParticles()));
    physics.makeSpring(a, b, ofRandom(SPRING_MIN_STRENGTH, SPRING_MAX_STRENGTH), ofRandom(10, width/2));
}


void testApp::killRandomParticle()
{
    ofxMSAParticle *p = physics.getParticle(floor(ofRandom(0, physics.numberOfParticles())));
//    if(p && p != &mouseNode) p->kill();
//    if(p && p != pAttractMote && p != pRepelMote) p->kill();
}

void testApp::killRandomSpring()
{
    ofxMSASpring *s = physics.getSpring( floor(ofRandom(0, physics.numberOfSprings())));
    if(s) s->kill();
}

void testApp::killRandomConstraint()
{
    ofxMSAConstraint *c = physics.getConstraint(floor(ofRandom(0, physics.numberOfConstraints())));
    if(c) c->kill();
}


void testApp::addRandomForce(float f)
{
    forceTimer = f;
    for(unsigned int i=0; i<physics.numberOfParticles(); i++) {
        ofxMSAParticle *p = physics.getParticle(i);
        if(p->isFree()) p->addVelocity(ofPoint(ofRandom(-f, f), ofRandom(-f, f), ofRandom(-f, f)));
    }
}

void testApp::lockRandomParticles()
{
    for(unsigned int i=0; i<physics.numberOfParticles(); i++) {
        ofxMSAParticle *p = physics.getParticle(i);
        if(ofRandom(0, 100) < FIX_PROBABILITY) p->makeFixed();
        else p->makeFree();
    }
//    mouseNode.makeFixed();
}

void testApp::unlockRandomParticles()
{
    for(unsigned int i=0; i<physics.numberOfParticles(); i++) {
        ofxMSAParticle *p = physics.getParticle(i);
        p->makeFree();
    }
//    mouseNode.makeFixed();
}

void testApp::setUserAttract(bool _attractOn)
{
    userAttract = _attractOn;
////    ofPoint attractPoint = pAttractMote->getPosition();
//    printf("attract point x %f y %f z %f\n", attractPoint.x, attractPoint.y, attractPoint.z);
    if(userAttract) {
        // loop through all particles and add attraction to mouse
        // (doesn't matter if we attach attraction from mouse-mouse cos it won't be added internally
//        if (pAttractMote->getX() != 0) {
//            for(unsigned int i=0; i<physics.numberOfAttractions(); i++) physics.getAttraction(i)->turnOn();
//        }
//        if (pRepelMote->getX() != 0) {
//            for(unsigned int i=0; i<physics.numberOfParticles(); i++) physics.makeAttraction(pRepelMote, physics.getParticle(i), ofRandom(-MIN_ATTRACTION, -MAX_ATTRACTION));
//        }
    } else {
        // loop through all existing attractsions and delete them
        for(unsigned int i=0; i<physics.numberOfAttractions(); i++) physics.getAttraction(i)->turnOff();
    }
}

void testApp::updateMoteLabel()
{
    XnLabel label;

#ifdef NO_KINECT
    if (someoneThere) {
        numberUsers = 1;
    } else {
        numberUsers = 0;
    }

    for(unsigned int i=0; i<physics.numberOfParticles(); i++) {
        HatchMote *p = static_cast<HatchMote*>(physics.getParticle(i));

        label = numberUsers;
        p->setLabel(label);
    }

#else
    bool doFork= false;
    XnUInt16 userCount = oni.getUserCount();
//    if ((numberUsers == 0) && (userCount > 0) ) {
//        //someone arrived
//        doFork = true;
//        someoneThere = true;
//        sound.sendEvent(SOUND_EVENT_SOMEONE_THERE);
////        ofBackground(255, 255,255);
////        ofSetBackgroundAuto(false);
//    } else if ((userCount == 0) && (numberUsers >0)) {
//        //last person left
//        doFork = false;
//        someoneThere = false;
//        sound.sendEvent(SOUND_EVENT_NOONE_THERE);
////        ofBackground(0,0,0);
////        ofSetBackgroundAuto(true);
//    }
    numberUsers = userCount;
    const XnLabel* pLabels = oni.sceneMD.Data();
    for(unsigned int i=0; i<physics.numberOfParticles(); i++) {
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
testApp::testApp()
{
    pInsidePalette = new ColorSampler("images/inside.jpg");
    pOutsidePalette = new ColorSampler("images/outside.jpg");
    pTextSampler = new TextSampler("data/text/sample.txt");
    pGlyphSampler = new GlyphSampler("data/images/glyphs");
    pBlankSampler = new GlyphSampler("data/images/erase");
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

    sound.sendEvent(SOUND_EVENT_STOP);
    sound.close();
    delete pInsidePalette;
    delete pOutsidePalette;
    delete pTextSampler;
    delete pGlyphSampler;
    delete pBlankSampler;

}

void testApp::setScreenRatios(void)
{
    int windowMode = ofGetWindowMode();

    kinectWidth = 640;
    kinectHeight = 480;
    if(windowMode == OF_FULLSCREEN) {
        width = ofGetScreenWidth();
        height = ofGetScreenHeight();
    } else if(windowMode == OF_WINDOW) {
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
    someoneThere = false;
    ofBackground(0,0,0);
    ofSetBackgroundAuto(false);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofSetWindowPosition(ofGetScreenWidth() - ofGetWidth() - 20, 20);

#ifndef NO_KINECT
    oni.setup();

    // players
    for (int i = 0; i < MAX_PLAYERS; i++) players[i].allocate(oni.width, oni.height);
#endif
    //========================

    ofSetFullscreen(false);
    ofHideCursor();
    setScreenRatios();

// font needs to be loaded before the particles are created because they all use it to draw
    myFont.loadFont("verdana.ttf", (int)8*fromKinectWidth);


    //	physics.verbose = true;			// dump activity to log
//    physics.setGravity(ofPoint(0, GRAVITY, 0));
    physics.setGravity(ofPoint(0, 0, 0));

    // set world dimensions, not essential, but speeds up collision
    physics.setWorldSize(ofPoint(0, 0, 0), ofPoint(width, height, width));
//    physics.clearWorldSize();
    physics.setSectorCount(SECTOR_COUNT);
    physics.setDrag(0.97f);
    physics.setDrag(1);		// FIXTHIS
    physics.enableCollision();

    initScene();
    for(int i=0; i<START_MOTE_COUNT; i++) addRandomParticle();

//    for(unsigned int i=0; i<physics.numberOfParticles(); i++) physics.makeAttraction(pAttractMote, physics.getParticle(i), ofRandom(MIN_ATTRACTION, MAX_ATTRACTION));
    for(unsigned int i=0; i<physics.numberOfAttractions(); i++) physics.getAttraction(i)->turnOff();


    addRandomForce(FORCE_AMOUNT);
    //========================

#ifdef DO_VIDEO
//    writer = cvCreateVideoWriter(
//                 "test.avi",
//                 CV_FOURCC('M','J','P','G'),
//                 15,
//                 size);
#endif
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
    if (numberUsers != nUsersPrev) {
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
    if (doVideoWrite) {

#ifdef DO_VIDEO

//        IplImage * tempImg = cvCreateImage(
//                                 cvSize(cameraWidth,cameraHeight),
//                                 IPL_DEPTH_8U,
//                                 3);
//        saveScreen.grabScreen(screenWidth-cameraWidth,screenHeight-cameraHeight,cameraWidth,cameraHeight);
//        colorImg.setFromPixels(saveScreen.getPixels(), cameraWidth,cameraHeight);
//        cvCvtColor(colorImg.getCvImage(), tempImg, CV_RGB2BGR);
//        cvWriteFrame(writer,tempImg);

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
    switch(key) {
    case 'a':
        setUserAttract(!userAttract);
        break;
    case 'p':
        for(int i=0; i<100; i++) addRandomParticle();
        break;
    case 'P':
        for(int i=0; i<100; i++) killRandomParticle();
        break;
    case 's':
        addRandomSpring();
        break;
    case 'S':
        killRandomSpring();
        break;
    case 'c':
        physics.isCollisionEnabled() ? physics.disableCollision() : physics.enableCollision();
        break;
    case 'C':
        killRandomConstraint();
        break;
    case 'r':
        doRender ^= true;
        break;
    case 'f':
        addRandomForce(FORCE_AMOUNT);
        break;
    case 'F':
        addRandomForce(FORCE_AMOUNT * 3);
        break;
    case 'l':
        lockRandomParticles();
        break;
    case 'u':
        unlockRandomParticles();
        break;
    case ' ':
//        initScene();
        someoneThere = !someoneThere;
        if (someoneThere) {
//            ofBackground(255, 255,255);
            sound.sendEvent(SOUND_EVENT_SOMEONE_THERE);
        } else {
//            ofBackground(0,0,0);
            sound.sendEvent(SOUND_EVENT_NOONE_THERE);
        }
//        ofSetBackgroundAuto(!someoneThere);
        break;
    case 'x':
        doMouseXY = true;
        someoneThere = !someoneThere;
        break;
    case 't':
        ofToggleFullscreen();
        setScreenRatios();
        break;
    case 'z':
        doMouseYZ = true;
        break;
    case ']':
        rotSpeed += 0.01f;
        break;
    case '[':
        rotSpeed -= 0.01f;
        break;
    case '+': {
//        mouseNode.setMass(mouseNode.getMass() +0.1);
        physics.setGravity(ofPoint(GRAVITY, 0, 0));
    }
    break;
    case '-':
//        mouseNode.setMass(mouseNode.getMass() -0.1);
        physics.setGravity(ofPoint(-GRAVITY, 0, 0));
        break;
    case '0':
//        mouseNode.setMass(mouseNode.getMass() -0.1);
        physics.setGravity(ofPoint(0, 0, 0));
        break;
    case 'm':
//        mouseNode.hasCollision() ? mouseNode.disableCollision() : mouseNode.enableCollision();
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
    switch(key) {
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
    switch(button) {
    case 0:
        doMouseXY = true;
        mouseMoved(x, y);
        break;
    case 2:
        doMouseYZ = true;
        mouseMoved(x, y);
        break;
    }

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
    doMouseXY = doMouseYZ = false;
    doVideoWrite = !doVideoWrite;

}

//--------------------------------------------------------------
void testApp::resized(int w, int h)
{

}

