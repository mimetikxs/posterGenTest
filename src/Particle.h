//
//  Particle.h
//  posterGenerator
//
//  Created by Nestor Rubio Garcia on 23/01/2017.
//
//

#ifndef posterGenerator_Particle_h
#define posterGenerator_Particle_h

#include "ofMain.h"

const float TO_RADS = PI / 180;

class Particle {
    
public:
    
    ofTexture* tex;
    ofVec3f position;
    float size;
    float rotation;
    float opacity;
    bool isLine;
    ofColor color;

    
    Particle(){
        size = 1;
        opacity = 1;
        rotation = 0;
    };
    
    
    virtual void update() = 0;
    
    virtual void draw() = 0;
};

#endif
