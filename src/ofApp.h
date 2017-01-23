#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxAnimatableFloat.h"

class ofApp : public ofBaseApp{

public:
    vector<ofTexture> textures;
    
    ofRectangle posterBounds;
    
    ofCamera fixedCam;
    ofEasyCam movingCam;
    ofCamera* activeCam;
    
    ofImage background;
    
    ofParameter<float> posterScale;
    
    ofxPanel gui;
    
    ofParameterGroup groupFixCam;
    ofParameter<float> fixCamFov;
    ofParameter<float> fixCamZ;
    ofParameterGroup groupMoveCam;
    ofParameter<float> moveCamZ;
    ofParameter<float> moveCamNear;
    ofParameter<float> moveCamFar;
    ofParameterGroup groupMask;
    ofParameter<float> layerSeparation;
    
    ofParameter<bool> bDrawDebug;
    ofParameter<bool> bUseMouseZ;
    
    bool bDrawGui;
    
    ofxAnimatableFloat camDepth;
    
    
    float lastTime;
    float idleCounter;
    bool paused;
    
    void switchCamera();

    void drawMaskPlanes();
    
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
		
};
