//
//  ParticleGroupMeteors.h
//  posterGenerator
//
//  Created by Nestor Rubio Garcia on 25/01/2017.
//
//

#ifndef posterGenerator_ParticleGroupMeteors_h
#define posterGenerator_ParticleGroupMeteors_h

#include "ofMain.h"
#include "ParticleGroup.h"


class ParticleGroupMeteors : public ParticleGroup {
public:
    ofParameter<int> numParticles;
    ofParameter<float> minZ;
    ofParameter<float> maxZ;
    ofParameter<float> angle;
    // visual properties
    ofParameter<float> maxOpacity;
    ofParameter<float> minOpacity;
    ofParameter<float> minSize;
    ofParameter<float> maxSize;
    
    
    ParticleGroupMeteors(){
    }
    
    
    virtual void reset(ofRectangle bounds, ofCamera camera){
        
    }
};
#endif
