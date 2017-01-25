//
//  ParticleGroupGaussian.h
//  posterGenerator
//
//  Created by Nestor Rubio Garcia on 24/01/2017.
//
//

#ifndef posterGenerator_ParticleGroupGaussian_h
#define posterGenerator_ParticleGroupGaussian_h

#include "ofMain.h"
#include "ParticleGroup.h"


class ParticleGroupGaussian : public ParticleGroup {
public:
    ParticleGroupGaussian(){
        
    }
    
private:
    float randomGaussian(float mean, float stddev) {
        std::default_random_engine generator;
        std::normal_distribution<float> distribution(mean, stddev);
        return distribution(generator);
    }
};

#endif
