//
//  ParticleGroupSimple.h
//  posterGenerator
//
//  Created by Nestor Rubio Garcia on 24/01/2017.
//
//

#ifndef posterGenerator_ParticleGroupSimple_h
#define posterGenerator_ParticleGroupSimple_h

#include "ofMain.h"
#include "ParticleGroup.h"
#include "ParticleCircle.h"
#include "ParticleSquare.h"
#include "ParticleTriangle.h"


class ParticleGroupSimple : public ParticleGroup {
public:
    // layout
    ofParameter<int> numParticles;
    ofParameter<float> minZ;
    ofParameter<float> maxZ;
    // visual properties
    ofParameter<float> maxOpacity;
    ofParameter<float> minOpacity;
    ofParameter<float> minSize;
    ofParameter<float> maxSize;
    ofParameter<float> rotationRange;
    ofParameter<int> shape; // 0=circle, 1=square, 2=triangle
    ofParameter<float> outlineChance;
    ofParameter<float> colorWhiteWeight;
    ofParameter<float> colorContrastWeight;
    ofParameter<float> colorBaseWeight;
//    ofParameter<bool> montercarlo; // TODO: set distribution function
    
    ofColor colorBase;
    ofColor colorContrast;
    
    
    ParticleGroupSimple() {
        parameters.add(numParticles.set("num particles", 0, 0, 1000));
        parameters.add(shape.set("shape", 0, 0, 2));
        parameters.add(minZ.set("minZ", -5000, -5000, 1700));
        parameters.add(maxZ.set("maxZ", 0, -5000, 1700));
        parameters.add(maxOpacity.set("max opacity", 1.0, 0.0, 1.0));
        parameters.add(minOpacity.set("min opacity", 1.0, 0.0, 1.0));
        parameters.add(maxSize.set("max size", 200.0, 0.0, 3000.0));
        parameters.add(minSize.set("min size", 1.0, 0.0, 3000.0));
        parameters.add(rotationRange.set("rotation range", 0, 0, 180));
        parameters.add(outlineChance.set("outline chance", 0, 0 ,1));
        parameters.add(colorWhiteWeight.set("color white weight", 0, 0, 1));
        parameters.add(colorContrastWeight.set("color contrast weight", 0, 0, 1));
        parameters.add(colorBaseWeight.set("color base weight", 0, 0, 1));
    }
    
    
    virtual void reset(ofRectangle bounds, ofCamera camera){
        particles.clear();

        for(int i=0; i < numParticles; i++) {
            float z = ofRandom(minZ, maxZ);
            
            ofVec3f screen;
            screen.x = ofRandom(bounds.getLeft(), bounds.getRight());
            screen.y = ofRandom(bounds.getBottom(), bounds.getTop());
            screen.z = camera.worldToScreen(ofVec3f(0,0,z)).z;  //screen.z = camera.worldToScreen(ofVec3f(0,0,-1700)).z;
            ofVec3f world = camera.screenToWorld(screen);
            
            float size = ofRandom(minSize, maxSize);
            float opacity = ofRandom(minOpacity, maxOpacity);
            
            Particle* p;
            switch(shape){
                case 0:
                    p = new ParticleCircle();
                    break;
                case 1:
                    p = new ParticleSquare();
                    break;
                case 2:
                    p = new ParticleTriangle();
                    break;
            }
            p->position.set(world);
            p->size = size;
            //p->opacity = opacity;
            p->isLine = getIsLine();
            p->rotation = ofRandom(rotationRange);
            p->color = getColor();
            p->color.a = opacity * 255;
            
            particles.push_back(p);
        }
    }
    
    
    virtual void draw() {
        ofPushStyle();
            for(auto& particle : particles){
                particle->draw();
            }
        ofPopStyle();
    }
    
    
private:
    bool getIsLine() {
        float r = ofRandomuf();
        if(r < outlineChance){
            return true;
        }
        return false;
    }
    
    ofColor getColor() {
        float r = ofRandomuf();
        
//        float whitePct = (1.f/3.f) * colorWhiteWeight;
//        float contrastPct = (1.f/3.f) * colorContrastWeight;
        
        float total = colorWhiteWeight + colorContrastWeight + colorBaseWeight;
        float whitePct = colorWhiteWeight / total;
        float contrastPct = colorContrastWeight / total;
        
        if(r <= whitePct) {
            return ofColor::white;
        }else if(r > whitePct && r < whitePct+contrastPct) {
            return colorContrast;
        }else{
            return colorBase;
        }
    }
    
    float monteCarlo() {
        while (true) {
            float r1 = ofRandom(1);
            float probability = r1 * r1;
            float r2 = ofRandom(1);
            if (r2 < probability) {
                return r1;
            }
        }
    }
    
};

#endif
