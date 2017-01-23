#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofBackground(50);
    
    background.load("../../../common/background.png");

    ofDirectory dir("../../../common/layers/");
    dir.allowExt("png");
    dir.listDir();
    for(int i = 0; i < dir.size(); i++){
        ofTexture tex;
        ofLoadImage(tex, dir.getPath(i));
        textures.push_back(tex);
    }
    
    float w = textures[0].getWidth() / 2;
    float h = textures[0].getHeight() / 2;
    float x = -w / 2;
    float y = -h / 2;
    
    posterBounds.set(x, y, w, h);
    
    gui.setup();
    
    groupFixCam.setName("fix cam");
    groupFixCam.add(fixCamFov.set("fov", 60, 10, 170));
    groupFixCam.add(fixCamZ.set("z", 1700, 0, 20000));
    gui.add(groupFixCam);
    groupMoveCam.setName("move cam");
    groupMoveCam.add(moveCamNear.set("near", 40, 0.1, 100));
    groupMoveCam.add(moveCamFar.set("far", 1701, 10, 100000));
    groupMoveCam.add(moveCamZ.set("z", 1700, 0, 1700));
    groupMoveCam.add(bUseMouseZ.set("use mouse z", false));
    gui.add(groupMoveCam);
    groupMask.setName("mask layers");
    groupMask.add(layerSeparation.set("separation", 100, 10, 1000));
    gui.add(groupMask);
    gui.add(posterScale.set("poster scale", 0.38, 0.01, 3));
    gui.add(bDrawDebug.set("draw debug", true));
    
//    easyCam.setFov(20);
    
    activeCam = &movingCam;
    
    camDepth.reset(350);
    camDepth.setCurve(QUADRATIC_EASE_OUT); //camDepth.setCurve(SWIFT_GOOGLE);
    camDepth.setRepeatType(LOOP);
    camDepth.setDuration(2.0f);
    camDepth.animateTo(1700);
    
    lastTime = ofGetElapsedTimef();
    
    idleCounter = 0;
    paused = false;
    
    bDrawGui = true;
}

//--------------------------------------------------------------
void ofApp::update(){
    fixedCam.setPosition(0, 0, fixCamZ);
    fixedCam.setFov(fixCamFov);
    fixedCam.setNearClip(moveCamNear);
    fixedCam.setFarClip(moveCamFar);
    
    movingCam.setFarClip(moveCamFar);
    
    camDepth.update(1.0f / 60.0f);
    
    if(!bUseMouseZ){
        cout << camDepth.getPercentDone() << endl;
        if (camDepth.getPercentDone() >= 0.994f){
            
            if(idleCounter > 2){
                camDepth.resume();
                idleCounter = 0;
                paused = false;
            }else if(!paused){
                camDepth.pause();
                paused = true;
            }
            
            idleCounter += ofGetLastFrameTime();
        }
        
        movingCam.setPosition(0, 0, moveCamZ);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofEnableDepthTest();
    
    activeCam->begin();
    {
        drawMaskPlanes();
    }
    activeCam->end();
    
    ofDisableDepthTest();
    
//    cout << " ---- " << endl;
    
    if (bDrawGui){
        ofDrawBitmapStringHighlight("layers: " + ofToString(textures.size()), 20, 20);
        ofDrawBitmapStringHighlight("cam z: " + ofToString(movingCam.getZ()), 20, 40);

        gui.draw();
    }
}

//--------------------------------------------------------------
void ofApp::drawMaskPlanes() {
    float z = 0;
    for(auto& tex : textures){
        
        // -----
        // distorted planes
        // (calculate scaling to compensate for perspective)
        float right = (ofGetWidth() / 2) + (tex.getWidth()*posterScale * 0.5);
        float left = (ofGetWidth() / 2) - (tex.getWidth()*posterScale * 0.5);
        float top = (ofGetHeight() / 2) + (tex.getHeight()*posterScale * 0.5);
        float bottom = (ofGetHeight() / 2) - (tex.getHeight()*posterScale * 0.5);
        float screenZ = fixedCam.worldToScreen(ofVec3f(0,0,z)).z;
        
        // undistorted corners
        ofVec3f tl(left, top, screenZ);
        ofVec3f tr(right, top, screenZ);
        ofVec3f br(right, bottom, screenZ);
        ofVec3f bl(left, bottom, screenZ);
        
        // distorted corners
        ofVec3f s_tl = fixedCam.screenToWorld(tl);
        ofVec3f s_tr = fixedCam.screenToWorld(tr);
        ofVec3f s_br = fixedCam.screenToWorld(br);
        ofVec3f s_bl = fixedCam.screenToWorld(bl);
        float s_width = s_tr.x - s_tl.x;
        float s_height = s_br.y - s_tr.y;
        
        ofMesh mesh;
        mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
        mesh.addVertex(s_tl);
        mesh.addVertex(s_tr);
        mesh.addVertex(s_br);
        mesh.addVertex(s_bl);
        mesh.addTriangle(0, 1, 2);
        mesh.addTriangle(0, 2, 3);
        mesh.addTexCoord(ofVec2f(0, tex.getHeight()));
        mesh.addTexCoord(ofVec2f(tex.getWidth(), tex.getHeight()));
        mesh.addTexCoord(ofVec2f(tex.getWidth(), 0));
        mesh.addTexCoord(ofVec2f(0, 0));
        
        tex.bind();
        mesh.draw();
        tex.unbind();
        
        // draw mask
        if(z == 0 && !bDrawDebug){
            ofDrawRectangle(-10000, s_tl.y-1000, s_tl.z, 20000, 1000);
            ofDrawRectangle(-10000, s_bl.y, s_bl.z, 20000, 1000);
            ofDrawRectangle(s_tl.x-10000, s_tl.y, s_tl.z, 10000, s_height);
            ofDrawRectangle(s_tr.x, s_tl.y, s_tl.z, 10000, s_height);
        }
        
        if (bDrawDebug) {
            ofPushStyle();
            ofSetColor(ofColor::red);
            ofNoFill();
            {
                ofDrawRectangle(s_tl, s_width, s_height);
                ofDrawCircle(s_tl, 5);
                ofDrawCircle(s_tr, 5);
                ofDrawCircle(s_br, 5);
                ofDrawCircle(s_bl, 5);
            }
            ofPopStyle();
            
            // ----
            // draw camera frustum
            ofPushMatrix();
            ofPushStyle();
            ofNoFill();
            {
                ofMatrix4x4 inverseCameraMatrix;
                inverseCameraMatrix.makeInvertOf(fixedCam.getModelViewProjectionMatrix());
                
                ofMultMatrix( inverseCameraMatrix );
                
                ofDrawBox(0, 0, 0, 2); // draw in camera space
            }
            ofPopStyle();
            ofPopMatrix();
            
            // draw world axys
            ofDrawAxis(100);
        }
        
        z += layerSeparation;
    }
}

//--------------------------------------------------------------
void ofApp::switchCamera() {
    if(activeCam == &fixedCam){
        activeCam = &movingCam;
    }else{
        activeCam = &fixedCam;
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case 'f':
            ofToggleFullscreen();
            break;
        case 'c':
            switchCamera();
            break;
        case 'g':
            bDrawGui = !bDrawGui;
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
