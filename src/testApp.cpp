#include "testApp.h"
#include "DataMote.h"

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

#define FORCE_AMOUNT			10

#define EDGE_DRAG				0.98

#define	GRAVITY					1

#define MAX_ATTRACTION			10
#define MIN_ATTRACTION			3

#define SECTOR_COUNT			10

#define START_MOTE_COUNT		200

DataMote* testApp:: makeDataMote(ofPoint pos, float  m = 1.0f, float d = 1.0f) {
    DataMote* p = new DataMote(pos, m, d);
    p->setInsideColor(pInsidePalette->getSampleColor());
    p->setOutsideColor(pOutsidePalette->getSampleColor());
    p->setChildColor(pInsidePalette->getSampleColor());
    p->setBannerColor(pBannerPalette->getSampleColor());
    p->setLabelString(pTextSampler->getSampleText());
    p->setBannerString(pBannerText->getSampleText());
    p->setGlyph(pGlyphSampler->getSampleGlyph());
    p->setBlankGlyph(pBlankSampler->getSampleGlyph());
    p->setFadeDist(width*0.6);
    physics.addParticle(p);
    p->release();	// cos addParticle(p) retains it
    return p;
}

void testApp::initScene() {
    // clear all particles and springs etc
    physics.clear();
}

void testApp:: addRandomParticle() {
    float posX		= ofRandom(0, width);
    float posY		= ofRandom(0, height);
    float posZ		= ofRandom(-width/2, width/2);
    float mass		= ofRandom(MIN_MASS, MAX_MASS);
    float bounce	= ofRandom(MIN_BOUNCE, MAX_BOUNCE);
    float radius	= ofMap(mass, MIN_MASS, MAX_MASS, NODE_MIN_RADIUS*fromKinectWidth, NODE_MAX_RADIUS*fromKinectWidth);

    // physics.makeParticle returns a particle pointer so you can customize it
    DataMote* p = makeDataMote(ofPoint(posX, posY, posZ));

    // and set a bunch of properties (you don't have to set all of them, there are defaults)
    p->setMass(mass)->setBounce(bounce)->setRadius(radius)->enableCollision()->makeFree();
    p->setFont(&myFont, 0);
    p->setFont(&myFont1, 1);
    p->setFont(&myFont2, 2);
    p->setFont(&myFont3, 3);


}

void testApp::addRandomSpring() {
    ofxMSAParticle *a = physics.getParticle((int)ofRandom(0, physics.numberOfParticles()));
    ofxMSAParticle *b = physics.getParticle((int)ofRandom(0, physics.numberOfParticles()));
    physics.makeSpring(a, b, ofRandom(SPRING_MIN_STRENGTH, SPRING_MAX_STRENGTH), ofRandom(10, width/2));
}


void testApp::killRandomParticle() {
    ofxMSAParticle *p = physics.getParticle(floor(ofRandom(0, physics.numberOfParticles())));
//    if(p && p != &mouseNode) p->kill();
//    if(p && p != pAttractMote && p != pRepelMote) p->kill();
}

void testApp::killRandomSpring() {
    ofxMSASpring *s = physics.getSpring( floor(ofRandom(0, physics.numberOfSprings())));
    if(s) s->kill();
}

void testApp::killRandomConstraint() {
    ofxMSAConstraint *c = physics.getConstraint(floor(ofRandom(0, physics.numberOfConstraints())));
    if(c) c->kill();
}


void testApp::addRandomForce(float f) {
    forceTimer = f;
    for(unsigned int i=0; i<physics.numberOfParticles(); i++) {
        ofxMSAParticle *p = physics.getParticle(i);
        if(p->isFree()) p->addVelocity(ofPoint(ofRandom(-f, f), ofRandom(-f, f), ofRandom(-f, f)));
    }
}

void testApp::lockRandomParticles() {
    for(unsigned int i=0; i<physics.numberOfParticles(); i++) {
        ofxMSAParticle *p = physics.getParticle(i);
        if(ofRandom(0, 100) < FIX_PROBABILITY) p->makeFixed();
        else p->makeFree();
    }
//    mouseNode.makeFixed();
}

void testApp::unlockRandomParticles() {
    for(unsigned int i=0; i<physics.numberOfParticles(); i++) {
        ofxMSAParticle *p = physics.getParticle(i);
        p->makeFree();
    }
//    mouseNode.makeFixed();
}

void testApp::setUserAttract(bool _attractOn) {
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

void testApp::updateMoteLabel() {
    XnLabel label;

    label = myMode;
    for(unsigned int i=0; i<physics.numberOfParticles(); i++) {
        DataMote *p = static_cast<DataMote*>(physics.getParticle(i));
        p->setLabel(label);
    }

}


//========================
//--------------------------------------------------------------
testApp::testApp() :
    moteCount(START_MOTE_COUNT),
    fullscreen(false),
    drawCount(120),
    textCount(120) {
    pInsidePalette = new ColorSampler("images/inside.jpg");
    pOutsidePalette = new ColorSampler("images/outside.jpg");
    pBannerPalette = new ColorSampler("images/bsod_white.png");
    pTextSampler = new TextSampler("data/text/sample.txt");
    pBannerText = new TextSampler("data/text/banner.txt");
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

testApp::~testApp() {

    delete pInsidePalette;
    delete pOutsidePalette;
    delete pBannerPalette;
    delete pTextSampler;
    delete pBannerText;
    delete pGlyphSampler;
    delete pBlankSampler;

}

void testApp::setScreenRatios(void) {
    int windowMode = ofGetWindowMode();

    kinectWidth = ofGetWidth();
    kinectHeight = ofGetHeight();
    if(windowMode == OF_FULLSCREEN) {
        width = ofGetScreenWidth();
        height = ofGetScreenHeight();
        fromKinectWidth = (float)width/(float)kinectWidth;
        fromKinectHeight = (float)height/(float)kinectHeight;
        toKinectWidth = (float)kinectWidth/(float)width;
        toKinectHeight = (float)kinectHeight/(float)height;
    } else if(windowMode == OF_WINDOW) {
        width = ofGetWidth();
        height = ofGetHeight();

        fromKinectWidth = 1;
        fromKinectHeight = 1;
        toKinectWidth = 1;
        toKinectHeight = 1;
    }
    physics.setWorldSize(ofPoint(0, -height, 0), ofPoint(width, height, width));
}
//--------------------------------------------------------------
void testApp::setup() {
    if (XML.loadFile("mySettings.xml")) {
        moteCount    = XML.getValue("ROOM:MOTE_COUNT", START_MOTE_COUNT);
        fullscreen   = (XML.getValue("ROOM:FULLSCREEN", 1) == 1)?true:false;
        drawCount    = XML.getValue("ROOM:DRAW_TIMEOUT", 150);
        textCount    = XML.getValue("ROOM:TEXT_TIMEOUT", 150);
    }
    myMode = MODE_CODE;
    ofBackground(0,0,0);
    ofSetBackgroundAuto(true);
    ofEnableAlphaBlending();
    ofSetWindowPosition(ofGetScreenWidth() - ofGetWidth() - 20, 20);

    //========================

    ofSetFullscreen(fullscreen);
    ofHideCursor();
    setScreenRatios();

// font needs to be loaded before the particles are created because they all use it to draw
    myFont.loadFont("verdana.ttf", (int)8*fromKinectWidth);

    myFont1.loadFont("verdana.ttf", (int)12*fromKinectWidth);
    myFont2.loadFont("verdana.ttf", (int)16*fromKinectWidth);
    myFont3.loadFont("verdana.ttf", (int)24*fromKinectWidth);


    //	physics.verbose = true;			// dump activity to log
//    physics.setGravity(ofPoint(0, GRAVITY, 0));
    physics.setGravity(ofPoint(0, GRAVITY/2, 0));

    // set world dimensions, not essential, but speeds up collision
    physics.setWorldSize(ofPoint(0, 0, 0), ofPoint(width, height, width));
    physics.setSectorCount(SECTOR_COUNT);
    physics.setDrag(0.97f);
    physics.setDrag(1);		// FIXTHIS
    physics.enableCollision();

    initScene();
    for(int i=0; i<moteCount; i++) addRandomParticle();

//    for(unsigned int i=0; i<physics.numberOfParticles(); i++) physics.makeAttraction(pAttractMote, physics.getParticle(i), ofRandom(MIN_ATTRACTION, MAX_ATTRACTION));
    for(unsigned int i=0; i<physics.numberOfAttractions(); i++) physics.getAttraction(i)->turnOff();
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
    changeCountdown = textCount;
}

//--------------------------------------------------------------
void testApp::update() {

    switchMode();
    XnUInt16 nUsersPrev = numberUsers;
    ofSetWindowTitle(ofToString(ofGetFrameRate()));

//========================

    physics.update();
    updateMoteLabel();
    if (numberUsers != nUsersPrev) {
        printf(" numberUsers %d", numberUsers);
    }
}

//--------------------------------------------------------------
void testApp::draw() {

    glPushMatrix();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
void testApp::keyPressed  (int key) {
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
        changeCountdown = 0;
        switchMode();

        break;
    case 'x':
        doMouseXY = true;
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
        physics.setGravity(ofPoint(0, GRAVITY, 0));
    }
    break;
    case '-':
//        mouseNode.setMass(mouseNode.getMass() -0.1);
        physics.setGravity(ofPoint(0, -GRAVITY, 0));
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
void testApp::keyReleased(int key) {
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
void testApp::mouseMoved(int x, int y ) {

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
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
void testApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {
    doMouseXY = doMouseYZ = false;
    doVideoWrite = !doVideoWrite;

}

//--------------------------------------------------------------
void testApp::resized(int w, int h) {

}

void testApp::switchMode() {
    if (changeCountdown == 0) {
        switch (myMode) {
        case MODE_PAINT_BOTH:
            ofBackground(255, 255,255);
            changeCountdown = drawCount;
            myMode = MODE_PAINT;
            ofSetBackgroundAuto(false);
            break;
        case MODE_PAINT:
            ofBackground(255, 255,255);
            changeCountdown = drawCount;
            myMode = MODE_PAINT_WARM;
            ofSetBackgroundAuto(false);
            break;
        case MODE_PAINT_WARM:
            myMode = MODE_ME;
            ofBackground(0,0,0);
            changeCountdown = textCount;
            ofSetBackgroundAuto(true);
            break;
        case MODE_ME:
            myMode = MODE_CODE;
            ofBackground(0,0,0);
            changeCountdown = textCount;
            ofSetBackgroundAuto(true);
            break;
        default:
        case MODE_CODE:
            ofBackground(255, 255,255);
            changeCountdown = drawCount;
            myMode = MODE_PAINT_BOTH;
            ofSetBackgroundAuto(false);
            break;
        }
    }
    changeCountdown--;

}

