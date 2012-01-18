#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxONI.h"
#include "DataMote.h"
#include "PulseMote.h"
#include "ofxOpenCv.h"

#include "ColorSampler.h"
#include "TextSampler.h"
#include "GlyphSampler.h"

#include "ofxMSAPhysics.h"
#include "ofxThreadedImageSaver.h"
#include "ofxXmlSettings.h"

#define MAX_PLAYERS 5
#define MAX_FLIPCOUNT 10
class testApp : public ofBaseApp
{

public:
    testApp();
    ~testApp();

    void setup();
    void update();
    void draw();

    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void resized(int w, int h);

    ofxONI oni;

    int numOfPlayres;
    ofxCvGrayscaleImage players[MAX_PLAYERS];

    void initScene();
    void addRandomParticle();
    void addRandomSpring();
    void killRandomParticle();
    void killRandomSpring();
    void killRandomConstraint();
    void addRandomForce(float f);
    void lockRandomParticles();
    void unlockRandomParticles();


    DataMote* makeDataMote(ofPoint pos, float  m, float d);
    void updateMoteLabel();
    void updateAttractRepelPoints();
    XnUInt16 numberUsers;
    XnUserID frontUser;
    int flipCount;
    void setUserAttract(bool _attractOn);
    void setScreenRatios(void);

    ofTrueTypeFont myFont;
    DataMote*		    pRepelMote;
    PulseMote*		    pAttractMote;

    bool doVideoWrite;
#ifdef DO_VIDEO
    ofImage saveScreen;
//    ofxCvColorImage colorImg;
//    CvVideoWriter * writer;
    ofxThreadedImageSaver TIS;
#endif
    int snapCounter;

    ColorSampler *pInsidePalette;
    ColorSampler *pOutsidePalette;
    TextSampler *pTextSampler;
    GlyphSampler *pGlyphSampler;
    GlyphSampler *pOutsideGlyphSampler;

    bool				userAttract ;
    bool				mouseRepel ;
    bool				doMouseXY;		// pressing left mmouse button moves mouse in XY plane
    bool				doMouseYZ;		// pressing right mouse button moves mouse in YZ plane
    bool				doRender;
    int					forceTimer;


    float				rotSpeed;
    float				mouseMass;

    int			width;
    int          kinectWidth;
    int			height;
    int          kinectHeight;
    float        fromKinectWidth;
    float        fromKinectHeight;
    float        toKinectWidth;
    float        toKinectHeight;


    ofxMSAPhysics		physics;    ofxXmlSettings XML;
    int minThreshold;
    int maxThreshold;
    int midDistance;
    int moteCount;
    bool fullscreen;

};

#endif
