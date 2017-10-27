#pragma once

#include "ofxiOS.h"
#include "controlThread.h"
#include "ABiOSSoundStream.h"
#include "ofxiOSExtras.h"
#include "ofxPd.h"
#include "PdExternals.h"

#define DEBUG_MODE (0)
#define nControls (21)
#define nInstruments (3)
#define nControlsPerInstrument (7)
#define nScenes (6)
#define maxTouches (11)
#define nPots (9)
#define nButtons (5)
#define touchMargin (5)
#define TOUCH_PAD (14)      // Why 14? Just to match numbers in version 1
#define POT_CONTROL (1)
#define SWITCH_CONTROL (3)

//  Determine device
#define IS_IPAD (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
#define IS_IPHONE (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone)
#define IS_IPHONE_5 (IS_IPHONE && [[UIScreen mainScreen] bounds].size.height == 568)
#define IS_IPHONE_6 (IS_IPHONE && [[UIScreen mainScreen] bounds].size.height == 667)
#define IS_IPHONE_6_PLUS (IS_IPHONE && [[UIScreen mainScreen] bounds].size.height == 736)
#define IS_IPHONE_X (IS_IPHONE && [[UIScreen mainScreen] bounds].size.height == 812)

// Defines for translating iPhone positioning to iPad
// 480 point wide -> 1024 points wide
// = 2.13 multiplier
#define IPAD_BOT_TRIM (45)


// a namespace for the Pd types
using namespace pd;

// derive from Pd receiver classes to receieve message and midi events

class ofApp : public ofxiOSApp, public PdReceiver, public PdMidiReceiver {
	
  public:
    void setup();
    void update();
    void draw();
    void exit();
	
    void touchDown(ofTouchEventArgs & touch);
    void touchMoved(ofTouchEventArgs & touch);
    void touchUp(ofTouchEventArgs & touch);
    void touchDoubleTap(ofTouchEventArgs & touch);
    void touchCancelled(ofTouchEventArgs & touch);

    void lostFocus();
    void gotFocus();
    void gotMemoryWarning();
    void deviceOrientationChanged(int newOrientation);
    
    void setupAudioStream();
    
    Boolean holdIsOff();
    
    ABiOSSoundStream* stream;
    ABiOSSoundStream* getSoundStream();
    
    float volume;
    
    controlThread myControlThread;
    
    // audio callbacks
    void audioReceived(float * input, int bufferSize, int nChannels);
    void audioRequested(float * output, int bufferSize, int nChannels);
    
    // sets the preferred sample rate, returns the *actual* samplerate
    // which may be different ie. iPhone 6S only wants 48k
    float setAVSessionSampleRate(float preferredSampleRate);
    
    void loadInstrument(int n);
    void loadMenu();
    int toggleIt(int n);
    Boolean inBounds(int controlId, int x1, int y1);
    int checkButtons(int x1, int y1);
    Boolean inBoundsExit(int x1, int y1);
    Boolean calculatePotAngle(int id);
    Boolean calculateTouchAngle(int id);
    void changeState(int s);

    ofxPd pd;
    vector<float> scopeArray;
    vector<float> scopeArray2;
    vector<Patch> instances;
    
    int midiChan;
    
    int holdState = 0;
    int menuState = 0;
    int state = 1;
    int prevState = 1;
    int scene = 0;
    int startBackgroundX = 0;
    int startTouchId = 0;
    int startTouchX = 0;
    int startTouchY = 0;
    int menuMoveStep = 10;
    
    int ledCount = 0;
    int ledTempo = 60;
    
    ofColor channelColor[13];
    
    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;
    int INSTRUMENT_WIDTH;
    int INSTRUMENT_HEIGHT;
    int X_OFFSET;
    int Y_OFFSET;
    int HITSPOT_X;
    int HITSPOT_Y;
    int HITSPOT_W;
    float SCALING;
    int IPAD_MARGIN;
    
    float backgroundX = 0;
    float prevBackgroundX = 0;
    float backgroundY = 0;
    float backgroundXCenter[nScenes] = { 0,0,-460,-460*2,-460*3,-460*4-54 };
    float instrumentWidth[nScenes];
    float instrumentHeight[nScenes];
    
    int instrumentBase = 0;
    
    int controlType[nControls] = {
        // Instrument 1:
        TOUCH_PAD, TOUCH_PAD, TOUCH_PAD, TOUCH_PAD, POT_CONTROL, POT_CONTROL, SWITCH_CONTROL,
        // Instrument 2:
        TOUCH_PAD, TOUCH_PAD, TOUCH_PAD, TOUCH_PAD, POT_CONTROL, POT_CONTROL, SWITCH_CONTROL,
        // Instrument 3:
        TOUCH_PAD, TOUCH_PAD, TOUCH_PAD, TOUCH_PAD, POT_CONTROL, POT_CONTROL, SWITCH_CONTROL
    };
    float controlValue[nControls] = {
        0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
    };
    
    float touchValueX[nControls] = {
        0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
    };
    
    float touchValueY[nControls] = {
        0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
    };
    
    float controlAngle[nControls] = {
        5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5
    };
    
    float prevControlAngle[nControls] = {
        5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5
    };
    
    float controlX[nControls] = {
        79, 79, 402, 402, 189, 291, 242,  // Instrument 1
        79, 79, 402, 402, 191, 289, 242,  // Instrument 2
        79, 79, 402, 402, 189, 291, 242   // Instrument 3
    };
    float controlY[nControls] = {
        79, 242, 79, 242, 110, 110, 266,
        79, 242, 79, 242, 113, 113, 266,  // Instrument 2
        79, 242, 79, 242, 110, 110, 266   // Instrument 3
    };
    
    float controlW[nControls] = {
        135, 135, 135, 135, 40, 40, 48,  // Instrument 1
        135, 135, 135, 135, 40, 40, 48,  // Instrument 2
        135, 135, 135, 135, 40, 40, 48   // Instrument 3
    };
    
    float controlH[nControls] = {
         135, 135, 135, 135, 40, 40, 34,
         135, 135, 135, 135, 40, 40, 34,
         135, 135, 135, 135, 40, 40, 34
    };
    
    float buttonX[nButtons] = { 103, 272, 441, 179, 363 };
    float buttonY[nButtons] = { 72, 72, 72, 220, 220 };
    float buttonW[nButtons] = { 116, 116, 116, 116, 116 };
    float buttonH[nButtons] = { 116, 116, 116, 116, 116 };
    
    string instrumentImages[nInstruments+2] = {
        "NoiseKit", "NoiseKitOrange", "NoiseKitPink", "AboutLayer", "AboutLayer"
    };
        
    string patchInput[nControls] = {
        "devnull", "devnull", "devnull", "devnull", "feedbake_osc1", "feedbake_osc2", "feedbake_hold",
         "devnull", "devnull", "devnull", "devnull", "feedbake_osc1", "feedbake_osc2", "feedbake_hold",
         "devnull", "devnull", "devnull", "devnull", "feedbake_osc1", "feedbake_osc2", "feedbake_hold"
    };
    string touchInputX[nControls] = {
        "feedbake_1", "feedbake_3", "feedbake_5", "feedbake_7", "devnull", "devnull", "devnull",
        "feedbake_1", "feedbake_3", "feedbake_5", "feedbake_7", "devnull", "devnull", "devnull",
        "feedbake_1", "feedbake_3", "feedbake_5", "feedbake_7", "devnull", "devnull", "devnull"
    };
    string touchInputY[nControls] = {
        "feedbake_2", "feedbake_4", "feedbake_6", "feedbake_8", "devnull", "devnull", "devnull",
       "feedbake_2", "feedbake_4", "feedbake_6", "feedbake_8", "devnull", "devnull", "devnull",
        "feedbake_2", "feedbake_4", "feedbake_6", "feedbake_8", "devnull", "devnull", "devnull"
    };
    
    string onOffInput[nControls] = {
        "pad1OFF", "pad2OFF", "pad3OFF", "pad4OFF", "devnull", "devnull", "devnull",
       "pad1OFF", "pad2OFF", "pad3OFF", "pad4OFF", "devnull", "devnull", "devnull",
        "pad1OFF", "pad2OFF", "pad3OFF", "pad4OFF", "devnull", "devnull", "devnull"
    };
    
    Boolean controlChanged[nControls];
    Boolean pinnedHigh[nControls];
    Boolean pinnedLow[nControls];
    Boolean crossedHighCW[nControls];
    Boolean crossedHighCC[nControls];
    Boolean crossedLowCW[nControls];
    Boolean crossedLowCC[nControls];
    Boolean showHighlight[nControls];
    
    Boolean exitHit = NO;
    int buttonHit = -1;
    
    int buttonState[3] = {0, 0, 0};
    int masterTempo = 1;
    int masterTempoX = 160;
    
    int width;
    int height;
    
    ofImage background;
    ofImage background2;
    ofImage pot;
    ofImage potHighlight;
    ofImage holdUp;
    ofImage holdDown;
    ofImage exitButton;
    ofImage ledImage;
    ofImage holdOff;
    ofImage holdOn;
    
    ofTrueTypeFont lcdFont;
    
    Patch patch;
    ofDirectory dir;
    int numFiles;
    
    //The iPhone supports 5 simultaneous touches, and cancels them all on the 6th touch.
    //Current iPad models (through Air 2) support 11 simultaneous touches, and do nothing on a 12th
    //iPad Pro has 17?
    
    int touchX[maxTouches] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int touchY[maxTouches] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int touchControl[maxTouches] = {-1, -1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
    
    int lineSelected = 0;
    int menuLow = 0;
    int menuHigh = 3;
    int menuLines = 7;
    int menuOffset = 0;
    int patchLoaded = -1;

};


