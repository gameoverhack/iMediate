#ifndef __GOOSCMANAGER_H
#define __GOOSCMANAGER_H

#include "Singleton.h"
#include "Controller.h"
#include "Model.h"
#include "Logger.h"
#include "goGuiManager.h"

#include "ofxOsc.h"
#include "ofxMidi.h"

#define OSCMANAGER			goOSCManagerSingleton::Instance()
#define OSCDATA             OSCMANAGER->data
#define OSCRANGEMIN         OSCMANAGER->oscmins
#define OSCRANGEMAX         OSCMANAGER->oscmaxs

struct oscMSG {
	int 	port;
	int		channel;
	int 	status;
	int 	byteOne;
	int 	byteTwo;
	double 	timestamp;
};

class goOSCManager
{
    public:
        goOSCManager();
        virtual ~goOSCManager();

        void        update();

        float               data[5], oscmins[5], oscmaxs[5];
        int                 ardTimer[8];
        bool				bSetupArduino;			// flag variable for setting up arduino once

        ofxMidiIn			midiIn;
        ofxMidiOut          midiOut;
        void				newMidiMessage(ofxMidiEventArgs& eventArgs);

        bool                newMSG, newMSGCH[3], newMSGNT[3];
        oscMSG             newMidiMsg;
        oscMSG				lastMidiMsg;

    protected:
    private:

        void                remap(int & index, bool * var0, bool * var1);
        void                remap(int & index, int * var0, int * var1, float rBegin0, float rEnd0, float rBegin1, float rEnd1);
        void                remap(int & index, int rBegin0, int rEnd0, int rBegin1, int rEnd1);
        void                remap(int & index, int maxR0, int maxR1);
        void                remap(int & index, float * var0, float * var1, float rBegin0, float rEnd0, float rBegin1, float rEnd1);

        int                 oscMapCount;
        ofxOscReceiver      receiver;
};

typedef Singleton<goOSCManager> goOSCManagerSingleton;   // Global declaration

#endif // __GOOSCMANAGER_H
