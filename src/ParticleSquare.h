//
//  ParticleSquare.h
//  posterGenerator
//
//  Created by Nestor Rubio Garcia on 24/01/2017.
//
//

#ifndef posterGenerator_ParticleSquare_h
#define posterGenerator_ParticleSquare_h

#include "Particle.h"

class ParticleSquare : public Particle {
public:
    ParticleSquare(){
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
        
        ofPushMatrix();
        ofTranslate(position);
        ofRotateZ(rotation);
        ofScale(size, size, 0);
        ofDrawRectangle(-0.5,-0.5,0, 1,1);
        ofPopMatrix();
    }
};

#endif
