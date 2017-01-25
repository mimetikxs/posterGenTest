//
//  ParticleCircle.h
//  posterGenerator
//
//  Created by Nestor Rubio Garcia on 24/01/2017.
//
//

#ifndef posterGenerator_ParticleCircle_h
#define posterGenerator_ParticleCircle_h

#include "Particle.h"

class ParticleCircle : public Particle {
public:
    ParticleCircle(){
    }
    
    void update(){
    }
    
    void draw(){
        if(isLine){
            ofNoFill();
        }else{
            ofFill();
        }
        
        //ofSetColor(255,100,0, opacity * 255);
        ofSetColor(color);
        
        ofPushMatrix();
        ofTranslate(position);
        ofRotateZ(rotation);
        ofScale(size, size, 0);
        ofDrawCircle(0,0,0, 1);
        ofPopMatrix();
    }
};

#endif
