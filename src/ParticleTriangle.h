//
//  ParticleTriangle.h
//  posterGenerator
//
//  Created by Nestor Rubio Garcia on 24/01/2017.
//
//

#ifndef posterGenerator_ParticleTriangle_h
#define posterGenerator_ParticleTriangle_h

#include "Particle.h"

class ParticleTriangle : public Particle {
public:
    ParticleTriangle(){
    }
    
    
    void update(){
    }
    
    
    void draw(){
        if(isLine){
            ofNoFill();
        }else{
            ofFill();
        }
        
        ofSetColor(color);
        
        float angle1 = 120*TO_RADS;
        float angle2 = 240*TO_RADS;
        ofVec3f v1(sin(0)     , cos(0)     , 0);
        ofVec3f v2(sin(angle1), cos(angle1), 0);
        ofVec3f v3(sin(angle2), cos(angle2), 0);
        
        ofPushMatrix();
        ofTranslate(position);
        ofRotateZ(rotation);
        ofScale(size, size, 0);
        ofDrawTriangle(v1, v2, v3);
        ofPopMatrix();
    }
    
private:
    
};

#endif
