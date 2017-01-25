//
//  ParticleSystem.h
//  posterGenerator
//
//  Created by Nestor Rubio Garcia on 23/01/2017.
//
//

#ifndef posterGenerator_ParticleSystem_h
#define posterGenerator_ParticleSystem_h

#include "ofMain.h"
#include "ParticleGroup.h"


class ParticleSystem {
public:
    ParticleSystem(){
    };
    
    
    void add(ParticleGroup* group) {
        groups.push_back(group);
    }
    
    
    void update(){
        for(auto* group : groups){
            group->update();
        }
    }
    
    
    void draw() {
        for(auto* group : groups){
            group->draw();
        }
    }
    
    // reset all groups with same params
    void reset(ofRectangle bounds, ofCamera camera) {
        for(auto* group : groups){
            group->reset(bounds, camera);
        }
    }
    
    
protected:
    vector<ParticleGroup*> groups; // need pointers for polymorphic behaviour
};


#endif
