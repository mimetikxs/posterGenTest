#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxAnimatableFloat.h"
#include "ParticleSystem.h"
#include "ParticleGroupSimple.h"


class ofApp : public ofBaseApp{

public:
    vector<ofTexture> textures;
    
    ofRectangle posterBounds;
    
    ofCamera fixedCam;      // used to project from 2d to 3d
    ofEasyCam movingCam;    // moves in 3d
    ofCamera* activeCam;
    
    float smoothZ;
    
    ParticleSystem particleSystem;
    ParticleGroupSimple mediumParticles;
    ParticleGroupSimple smallParticles;
    ParticleGroupSimple bigParticles;
    
    // hack to draw in front of masks
    // TODO: make each mask be a particle
    ParticleSystem particleSystemOverlaping;
    ParticleGroupSimple overlapingParticles;
    
    ofxPanel gui;
    ofParameter<float> posterScale;
    ofParameterGroup groupFixCam;
    ofParameter<float> fixCamFov;
    ofParameter<float> fixCamZ;
    ofParameterGroup groupMoveCam;
    ofParameter<float> moveCamNear;
    ofParameter<float> moveCamFar;
    ofParameterGroup groupMask;
    ofParameter<float> layerSeparation;
    ofParameter<bool> bUseMouseZ;
    ofParameter<bool> bDrawMask;
    ofParameterGroup groupGeneral;
    ofParameter<bool> bDebug;
    
    ofxPanel gui2;
    ofParameter<bool> bAutoAnimate;
    ofParameter<float> cameraZ;
    
    ofxPanel gui3;
    ofxButton btnRefresh;
    
    ofxPanel gui4;
    ofParameter<ofColor> colorBase;
    ofParameter<ofColor> colorContrast;
    ofParameter<ofColor> colorBackground;
    
    bool bDrawGui;
    
    ofxAnimatableFloat camDepth;
    
    
    float lastTime;
    float idleCounter;
    bool paused;
    
    void switchCamera();
    void drawMaskPlanes();
    void refreshScene();
    void updateBounds();
    void initGui();
    
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
