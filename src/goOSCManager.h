#ifndef __GOOSCMANAGER_H
#define __GOOSCMANAGER_H

#include "Singleton.h"
#include "ofxOsc.h"

#define OSCMANAGER			goOSCManagerSingleton::Instance()

class goOSCManager
{
    public:
        goOSCManager();
        virtual ~goOSCManager();

        void        update();

        bool        newMSG;

        float       co1, co2, co3, amp, note;

    protected:
    private:

        ofxOscReceiver      receiver;
};

typedef Singleton<goOSCManager> goOSCManagerSingleton;   // Global declaration

#endif // __GOOSCMANAGER_H
