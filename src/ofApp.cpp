#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofBackground(0);
    ofSetLineWidth(2);
    
//    background.load("../../../common/background.png");

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
    
    initGui();
    
    activeCam = &movingCam;
    
    camDepth.reset(-3000);
    camDepth.setCurve(QUADRATIC_EASE_OUT); //camDepth.setCurve(SWIFT_GOOGLE);
    camDepth.setRepeatType(LOOP);
    camDepth.setDuration(10.0f);
    camDepth.animateTo(1700);
    
    lastTime = ofGetElapsedTimef();
    
    idleCounter = 0;
    paused = false;
    
    // poster rect
    updateBounds();
    
    // projector
    fixedCam.setPosition(0, 0, fixCamZ);
    fixedCam.setFov(fixCamFov);
    fixedCam.setNearClip(moveCamNear);
    fixedCam.setFarClip(moveCamFar);
    
    // particles
    smallParticles.setName("small");
    mediumParticles.setName("medium");
    bigParticles.setName("big");
    particleSystem.add(&smallParticles);
    particleSystem.add(&mediumParticles);
    particleSystem.add(&bigParticles);
    
    overlapingParticles.setName("overlaping");
    particleSystemOverlaping.add(&overlapingParticles);
    
    refreshScene();
    
    smoothZ = 0;
}

//--------------------------------------------------------------
void ofApp::update(){
    fixedCam.setPosition(0, 0, fixCamZ);
    fixedCam.setFov(fixCamFov);
    fixedCam.setNearClip(moveCamNear);
    fixedCam.setFarClip(moveCamFar);
    
    movingCam.setNearClip(moveCamNear);
    movingCam.setFarClip(moveCamFar);
//    movingCam.setFov(20);
    
    camDepth.update(1.0f / 60.0f);
    
    if(bDebug){
        movingCam.enableMouseInput();
    }else{
        movingCam.disableMouseInput();
        if(bAutoAnimate){
            /*if(camDepth.getPercentDone() >= 0.994f){
                if(idleCounter > 2){
                    camDepth.resume();
                    idleCounter = 0;
                    paused = false;
                }else if(!paused){
                    camDepth.pause();
                    paused = true;
                }
                idleCounter += ofGetLastFrameTime();
            }*/
            movingCam.setPosition(0, 0, camDepth);
        }else{
            smoothZ += (cameraZ - smoothZ) * 0.1;
            
            movingCam.setOrientation(ofVec3f(0,0,0));
            movingCam.setPosition(0, 0, smoothZ);
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofEnableAlphaBlending();
    activeCam->begin();
    {
        particleSystem.draw();
        
        if (bDrawMask) {
            ofEnableDepthTest();
            drawMaskPlanes();
            ofDisableDepthTest();
        }
        
        particleSystemOverlaping.draw();
    }
    activeCam->end();
    ofDisableAlphaBlending();
    
//    ofPushStyle();
//    ofNoFill();
//    ofDrawRectangle(posterBounds);
//    ofPopStyle();
    
    if (bDrawGui){
        //gui.draw();
        gui2.draw();
        gui3.draw();
        gui4.draw();
    }
}

//--------------------------------------------------------------
void ofApp::refreshScene() {    
    // normal rect
    smallParticles.reset(posterBounds, fixedCam);
    smallParticles.colorBase.set(colorBase);
    smallParticles.colorContrast.set(colorContrast);
    mediumParticles.reset(posterBounds, fixedCam);
    mediumParticles.colorBase.set(colorBase);
    mediumParticles.colorContrast.set(colorContrast);
    
    ofRectangle rect;
    
    // big rect (bounds are screen edges)
    rect.set(0, 0, ofGetScreenWidth(), ofGetScreenHeight());
    bigParticles.reset(rect, fixedCam);
    bigParticles.colorBase.set(colorBase);
    bigParticles.colorContrast.set(colorContrast);
    
    // rect (bounds are close to silhouette)
    rect.set(posterBounds.x, posterBounds.y + 200, posterBounds.width, posterBounds.height-400);
    overlapingParticles.reset(rect, fixedCam);
    overlapingParticles.colorBase.set(colorBase);
    overlapingParticles.colorContrast.set(colorContrast);
}

//--------------------------------------------------------------
void ofApp::updateBounds() {
    ofTexture& tex = textures[0];
    posterBounds.x = (ofGetWidth() / 2) - (tex.getWidth()*posterScale * 0.5);
    posterBounds.y =  (ofGetHeight() / 2) - (tex.getHeight()*posterScale * 0.5);
    posterBounds.width = tex.getWidth()*posterScale;
    posterBounds.height = tex.getHeight()*posterScale;
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
        if(z == 0 && !bDebug){
            ofPushStyle();
//            ofSetColor(220, 220, 220);
            ofDrawRectangle(-10000, s_tl.y-1000, s_tl.z, 20000, 1000);
            ofDrawRectangle(-10000, s_bl.y, s_bl.z, 20000, 1000);
            ofDrawRectangle(s_tl.x-10000, s_tl.y, s_tl.z, 10000, s_height);
            ofDrawRectangle(s_tr.x, s_tl.y, s_tl.z, 10000, s_height);
            ofPopStyle();
        }
        
        if (bDebug) {
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
void ofApp::initGui(){
    // invisible params
    fixCamFov.set("fov", 60, 10, 170);
    fixCamZ.set("z", 1700, 0, 20000);
    posterScale.set("poster scale", 0.38, 0.01, 3);
    layerSeparation.set("separation", 100, 10, 1000);
    
    // --
    gui.setup();
    //    groupFixCam.setName("fix cam");
    //    groupFixCam.add(fixCamFov.set("fov", 60, 10, 170));
    //    groupFixCam.add(fixCamZ.set("z", 1700, 0, 20000));
    //    gui.add(groupFixCam);
    groupMoveCam.setName("move cam");
    groupMoveCam.add(moveCamNear.set("near", 40, 0.1, 100));
    groupMoveCam.add(moveCamFar.set("far", 10000, 10, 100000));
    gui.add(groupMoveCam);
    //    groupMask.setName("mask layers");
    //    groupMask.add(layerSeparation.set("separation", 100, 10, 1000));
    //    gui.add(groupMask);
    //    gui.add(posterScale.set("poster scale", 0.38, 0.01, 3));
    
    gui2.setDefaultWidth(300);
    
    // --
    gui2.setup();
    gui2.add(cameraZ.set("camera Z", 1700, -5000, 1700));
    gui2.add(bAutoAnimate.set("auto animate", false));
    gui2.add(bDebug.set("debug mode", false));
    gui2.add(bDrawMask.set("draw mask", false));
    gui2.setWidthElements(300);
    
    // --
    gui3.setup("scene");
    gui3.add(btnRefresh.setup("refresh scene"));
    gui3.add(smallParticles.parameters);
    gui3.add(mediumParticles.parameters);
    gui3.add(bigParticles.parameters);
    gui3.add(overlapingParticles.parameters);
    gui3.setPosition(10, 130);
    gui3.minimizeAll();
    
    btnRefresh.addListener(this, &ofApp::refreshScene);
    
    // --
    gui4.setup("color");
    gui4.add(colorBase.set("base", ofColor(127,175,35), ofColor(0,0,0), ofColor(255,255,255)));
    gui4.add(colorContrast.set("contrast", ofColor(34,102,174), ofColor(0,0,0), ofColor(255,255,255)));
    gui4.minimizeAll();
    gui4.setPosition(350, 10);
    
    bDrawGui = true;
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
//        case 'c':
//            switchCamera();
//            break;
        case 'g':
            bDrawGui = !bDrawGui;
            break;
        case 'r':
            refreshScene();
            break;
        case ' ':
            bAutoAnimate = !bAutoAnimate;
            break;
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
    updateBounds();
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
