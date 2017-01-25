//
//  ParticleGroup.h
//  posterGenerator
//
//  Created by Nestor Rubio Garcia on 24/01/2017.
//
//

#ifndef posterGenerator_ParticleGroup_h
#define posterGenerator_ParticleGroup_h

#include "ofMain.h"
#include "Particle.h"


class ParticleGroup {
public:
    ofParameterGroup parameters;
    
    
    ParticleGroup() {
        parameters.setName("particle group");
    }
    
    
    void setName(string name){
        parameters.setName(name);
    }
    
    
    virtual void reset(ofRectangle bounds, ofCamera camera)=0;
    
    
    virtual void update(){
        for(auto* particle : particles){
            particle->update();
        }
    }
    
    
    virtual void draw() {
        ofPushStyle();
        for(auto* particle : particles){
            particle->draw();
        }
        ofPopStyle();
    }
    

protected:
    vector<Particle*> particles;
};


#endif
