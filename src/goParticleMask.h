#ifndef __GOPARTICLEMASK_H
#define __GOPARTICLEMASK_H

#include "ofMain.h"

#include "Singleton.h"
#include "Constants.h"
#include "particle.h"

#define PARTICLES			goParticleMaskSingleton::Instance()

class goParticleMask
{
    public:

        goParticleMask();
        virtual ~goParticleMask();

        void                    update();
        void                    draw (int hexColor,  int index);
        void                    generate(int _id, int _value);

        int                     particlePattern, pWidth, pDamp;

        bool                    linkParticle, speedParticle, sizeParticle, particleColors, eraseParticle;

        vector<particle*>       particles[8];

    protected:
    private:
};

typedef Singleton<goParticleMask> goParticleMaskSingleton;   // Global declaration

#endif // __GOPARTICLEMASK_H
