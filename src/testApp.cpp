#include "testApp.h"
#include "DataMote.h"

//========================

#include "ofxMSAPhysics.h"


#define	SPRING_MIN_STRENGTH		0.005
#define SPRING_MAX_STRENGTH		0.1

#define	SPRING_MIN_WIDTH		1
#define SPRING_MAX_WIDTH		3

#define NODE_MIN_RADIUS			2
#define NODE_MAX_RADIUS			7

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

#define START_MOTE_COUNT		400


bool				userAttract 	= false;
bool				mouseRepel  	= false;
bool				doMouseXY		= false;		// pressing left mmouse button moves mouse in XY plane
bool				doMouseYZ		= false;		// pressing right mouse button moves mouse in YZ plane
bool				doRender		= true;
int					forceTimer		= false;


float				rotSpeed		= 0;
float				mouseMass		= 1;

static int			width;
static int			height;


ofxMSAPhysics		physics;
//DataMote		    mouseNode;

ofImage				ballImage;

DataMote* testApp:: makeDataMote(ofPoint pos, float  m = 1.0f, float d = 1.0f)
{
    DataMote* p = new DataMote(pos, m, d);
    p->setInsideColor(pInsidePalette->getSampleColor());
    p->setOutsideColor(pOutsidePalette->getSampleColor());
    physics.addParticle(p);
    p->release();	// cos addParticle(p) retains it
    return p;
}

void testApp::initScene()
{
    // clear all particles and springs etc
    physics.clear();

    // you can add your own particles to the physics system
//    physics.addParticle(&mouseNode);
//    mouseNode.makeFixed();
//    mouseNode.setMass(MIN_MASS);
//    mouseNode.moveTo(ofPoint(0, 0, 0));
//    mouseNode.setRadius(NODE_MAX_RADIUS);
//    mouseNode.setFont(&myFont);



    pRepelMote = new DataMote();
    physics.addParticle(pRepelMote);
    pRepelMote->makeFixed();
    pRepelMote->setMass(MIN_MASS);
    pRepelMote->moveTo(ofPoint(0, 0, 0));
    pRepelMote->setRadius(NODE_MAX_RADIUS);
    pRepelMote->setFont(&myFont);
    pRepelMote->setInsideColor(pInsidePalette->getSampleColor());
    pRepelMote->setOutsideColor(pOutsidePalette->getSampleColor());

    pAttractMote = new DataMote();
    physics.addParticle(pAttractMote);
    pAttractMote->makeFixed();
    pAttractMote->setMass(MIN_MASS);
    pAttractMote->moveTo(ofPoint(0, 0, 0));
    pAttractMote->setRadius(NODE_MAX_RADIUS);
    pAttractMote->setFont(&myFont);
    pAttractMote->setInsideColor(pInsidePalette->getSampleColor());
    pAttractMote->setOutsideColor(pOutsidePalette->getSampleColor());

    // or tell the system to create and add particles
//    makeDataMote(ofPoint(-width/4, 0, -width/4), MIN_MASS)->makeFixed();		// create a node in top left back and fix
//    makeDataMote(ofPoint( width/4, 0, -width/4), MIN_MASS)->makeFixed();		// create a node in top right back and fix
//    makeDataMote(ofPoint(-width/4, 0,  width/4), MIN_MASS)->makeFixed();		// create a node in top left front and fix
//    makeDataMote(ofPoint( width/4, 0,  width/4), MIN_MASS)->makeFixed();		// create a node in top right front and fix
}
void testApp:: addRandomParticle()
{
    float posX		= ofRandom(0, width);
    float posY		= ofRandom(0, height);
    float posZ		= ofRandom(-width/2, width/2);
    float mass		= ofRandom(MIN_MASS, MAX_MASS);
    float bounce	= ofRandom(MIN_BOUNCE, MAX_BOUNCE);
    float radius	= ofMap(mass, MIN_MASS, MAX_MASS, NODE_MIN_RADIUS, NODE_MAX_RADIUS);

    // physics.makeParticle returns a particle pointer so you can customize it
    DataMote* p = makeDataMote(ofPoint(posX, posY, posZ));

    // and set a bunch of properties (you don't have to set all of them, there are defaults)
    p->setMass(mass)->setBounce(bounce)->setRadius(radius)->enableCollision()->makeFree();
    p->setFont(&myFont);


}

void addRandomSpring()
{
    ofxMSAParticle *a = physics.getParticle((int)ofRandom(0, physics.numberOfParticles()));
    ofxMSAParticle *b = physics.getParticle((int)ofRandom(0, physics.numberOfParticles()));
    physics.makeSpring(a, b, ofRandom(SPRING_MIN_STRENGTH, SPRING_MAX_STRENGTH), ofRandom(10, width/2));
}


void killRandomParticle()
{
    ofxMSAParticle *p = physics.getParticle(floor(ofRandom(0, physics.numberOfParticles())));
//    if(p && p != &mouseNode) p->kill();
//    if(p && p != pAttractMote && p != pRepelMote) p->kill();
}

void killRandomSpring()
{
    ofxMSASpring *s = physics.getSpring( floor(ofRandom(0, physics.numberOfSprings())));
    if(s) s->kill();
}

void killRandomConstraint()
{
    ofxMSAConstraint *c = physics.getConstraint(floor(ofRandom(0, physics.numberOfConstraints())));
    if(c) c->kill();
}


void addRandomForce(float f)
{
    forceTimer = f;
    for(unsigned int i=0; i<physics.numberOfParticles(); i++) {
        ofxMSAParticle *p = physics.getParticle(i);
        if(p->isFree()) p->addVelocity(ofPoint(ofRandom(-f, f), ofRandom(-f, f), ofRandom(-f, f)));
    }
}

void lockRandomParticles()
{
    for(unsigned int i=0; i<physics.numberOfParticles(); i++) {
        ofxMSAParticle *p = physics.getParticle(i);
        if(ofRandom(0, 100) < FIX_PROBABILITY) p->makeFixed();
        else p->makeFree();
    }
//    mouseNode.makeFixed();
}

void unlockRandomParticles()
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
    ofPoint attractPoint = pAttractMote->getPosition();
    printf("attract point x %f y %f z %f\n", attractPoint.x, attractPoint.y, attractPoint.z);
    if(userAttract) {
        // loop through all particles and add attraction to mouse
        // (doesn't matter if we attach attraction from mouse-mouse cos it won't be added internally
        if (pAttractMote->getX() != 0) {
            for(unsigned int i=0; i<physics.numberOfParticles(); i++) physics.makeAttraction(pAttractMote, physics.getParticle(i), ofRandom(MIN_ATTRACTION, MAX_ATTRACTION));
        }
//        if (pRepelMote->getX() != 0) {
//            for(unsigned int i=0; i<physics.numberOfParticles(); i++) physics.makeAttraction(pRepelMote, physics.getParticle(i), ofRandom(-MIN_ATTRACTION, -MAX_ATTRACTION));
//        }
    } else {
        // loop through all existing attractsions and delete them
        for(unsigned int i=0; i<physics.numberOfAttractions(); i++) physics.getAttraction(i)->kill();
    }
}

void testApp::updateMoteLabel()
{
    const XnLabel* pLabels = oni.sceneMD.Data();
    XnLabel label;
    for(unsigned int i=0; i<physics.numberOfParticles(); i++) {
        DataMote *p = static_cast<DataMote*>(physics.getParticle(i));
        int x = p->getX();
        int y = p->getY();
//        int z = p->getZ();
        label = pLabels[width*y+x];
        p->setLabel(label);
    }

}

void testApp::updateAttractRepelPoints()
{


    XnUserID frontUser;

    XnUInt16 userCount;
    XnPoint3D com = oni.getComUsersInFront(frontUser, userCount);
//    printf(" userCount %d", userCount);

    if ((userCount > 0) && (com.X > 0) && (com.X < width) && (com.Y > -height) && (com.Y < height) && (com.Z > 0) && (com.Z < 10000))
        pAttractMote->moveTo(ofPoint(com.X, com.Y, com.Z));
    else {
//        if (userCount > 0) printf("xxxxxxx   attract point x %f y %f z %f\n", com.X, com.Y, com.Z);
        if (userCount > 0) printf("%d .", userCount);
    }
    numberUsers = userCount;


}

//========================
//--------------------------------------------------------------
testApp::testApp()
{
    pInsidePalette = new ColorSampler("images/inside.jpg");
    pOutsidePalette = new ColorSampler("images/outside.jpg");
    numberUsers = 0;
    flipCount=0;

}

testApp::~testApp()
{
    pRepelMote->release();
    pAttractMote->release();
    delete pInsidePalette;
    delete pOutsidePalette;

}

//--------------------------------------------------------------
void testApp::setup()
{
    ofEnableAlphaBlending();
    ofSetWindowPosition(ofGetScreenWidth() - ofGetWidth() - 20, 20);

    oni.setup();

    // players
    for (int i = 0; i < MAX_PLAYERS; i++) players[i].allocate(oni.width, oni.height);

    //========================


// font needs to be loaded before the particles are created because they all use it to draw
    myFont.loadFont("verdana.ttf", 8);

    ballImage.loadImage("ball.png");

    width = ofGetWidth();
    height = ofGetHeight();

    //	physics.verbose = true;			// dump activity to log
//    physics.setGravity(ofPoint(0, GRAVITY, 0));
    physics.setGravity(ofPoint(0, GRAVITY/2, 0));

    // set world dimensions, not essential, but speeds up collision
    physics.setWorldSize(ofPoint(0, -height, 0), ofPoint(width, height, width));
    physics.setSectorCount(SECTOR_COUNT);
    physics.setDrag(0.97f);
    physics.setDrag(1);		// FIXTHIS
    //physics.enableCollision();

    initScene();
    for(int i=0; i<START_MOTE_COUNT; i++) addRandomParticle();
    //========================

//    writer = cvCreateVideoWriter(
//                 "test.avi",
//                 CV_FOURCC('M','J','P','G'),
//                 15,
//                 size);
    snapCounter = 0;
}

//--------------------------------------------------------------
void testApp::update()
{

    XnUInt16 nUsersPrev = numberUsers;
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    oni.update();

    //========================
    width = ofGetWidth();
    height = ofGetHeight();

    physics.update();
    updateMoteLabel();
    updateAttractRepelPoints();
    if (numberUsers != nUsersPrev) {
        flipCount = 0;
        printf("flip!\n");

    } else {
        if (flipCount < MAX_FLIPCOUNT) {
            if (numberUsers == 0)
                flipCount++; // take longer to switch attract off
            else
                flipCount += 2; //switch attract on quickly
        } else if (flipCount == MAX_FLIPCOUNT) {
            flipCount++;
            // only change attraction if number of users change and it stays like this for MAX_FLIPCOUNT updates
            if (numberUsers > 0) {
                printf ("user count %d  attract ON\n", numberUsers);
                setUserAttract(true);
            } else {
                printf ("user count %d attract OFF\n", numberUsers);
                setUserAttract(false);
            }
        }
    }


    //========================
}

//--------------------------------------------------------------
void testApp::draw()
{
    ofBackground(0, 0, 0);
    glPushMatrix();

    glScalef(ofGetWidth() / (float)oni.width, ofGetHeight() / (float)oni.height, 1);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    ofSetColor(255, 255, 255);
//    if (oni.bDrawCam)
//        oni.drawCam(0, 0);
//    else
//        oni.drawDepth(0, 0);

    ofSetColor(255, 255, 255, 100);
    if (oni.bDrawPlayers)
        oni.drawPlayers(0, 0);
    glPopMatrix();

    oni.skeletonTracking();
    physics.draw();
    if (doVideoWrite) {

//        IplImage * tempImg = cvCreateImage(
//                                 cvSize(cameraWidth,cameraHeight),
//                                 IPL_DEPTH_8U,
//                                 3);
//        saveScreen.grabScreen(screenWidth-cameraWidth,screenHeight-cameraHeight,cameraWidth,cameraHeight);
//        colorImg.setFromPixels(saveScreen.getPixels(), cameraWidth,cameraHeight);
//        cvCvtColor(colorImg.getCvImage(), tempImg, CV_RGB2BGR);
//        cvWriteFrame(writer,tempImg);

    }
}



//--------------------------------------------------------------
void testApp::keyPressed  (int key)
{
    if(key == '1') oni.bDrawPlayers = !oni.bDrawPlayers;
    if(key == '2') oni.bDrawCam = !oni.bDrawCam;
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
        initScene();
        break;
    case 'x':
        doMouseXY = true;
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
        screenImg.allocate(640, 480, OF_IMAGE_COLOR);
        screenImg.grabScreen(0,0,640,480);
        screenImg.saveImage("screenshot.png-"+ofToString(snapCounter)+".png");

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
//    static int oldMouseX = -10000;
//    static int oldMouseY = -10000;
//    int velX = x - oldMouseX;
//    int velY = y - oldMouseY;
//    if(doMouseXY) mouseNode.moveBy(ofPoint(velX, velY, 0));
//    if(doMouseYZ) mouseNode.moveBy(ofPoint(velX, 0, velY));
//    oldMouseX = x;
//    oldMouseY = y;

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
