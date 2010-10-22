#include "goMidiManager.h"


static const int ports[] = {2,3,4,5,6,7,8,9,10};
//static const int keys[] = {48,50,52,53,55,57,59,60,62,64};
static const int keys[] = {60,61,62,63,64,65,66,68};

#define NUM_SOLENOIDS 8

goMidiManager::goMidiManager()
{
    //ctor
}

goMidiManager::~goMidiManager()
{
    //dtor
}

void goMidiManager::setup()
{
    // setup arduino
    ard.connect("COM5", 57600);

    PLAYSOLENOIDS = bSetupArduino	= false;							// flag so we setup arduino when its ready, you don't need to touch this :)

    // setup midi
    midiIn.openPort();
    midiIn.setVerbose(false);

    ofAddListener(midiIn.newMessageEvent, this, &goMidiManager::newMidiMessage);
}

// --- Listen midi events -.----
void goMidiManager::newMidiMessage(ofxMidiEventArgs &args)
{

    int 	port;
    int		channel;
    int 	status;
    int 	byteOne;
    int 	byteTwo;
    double 	timestamp;

    if (args.channel != 15) // strange signal from the SPD-S on channel 15 - need to mask
    {
        cout << "MIDI message [port: " << args.port << ", channel: " << args.channel << ", status: " << args.status << ", byteOne: " << args.byteOne << ", byteTwo: " << args.byteTwo << ", timestamp: " << args.timestamp << "]" << endl;

        newMidiMsg.port = args.port;
        newMidiMsg.channel = args.channel;
        newMidiMsg.status = args.status;
        newMidiMsg.byteOne = args.byteOne;
        newMidiMsg.byteTwo = args.byteTwo;
        newMidiMsg.timestamp = args.timestamp;

        newMSG = true;
    }
}


//--------------------------------------------------------------
void goMidiManager::update()
{

    if ( ard.isArduinoReady())
    {
        // 1st: setup the arduino if haven't already:
        if (bSetupArduino == false)
        {
            setupArduino();
            bSetupArduino = true;	// only do this once
        }
        // auto test by using the update arduino
        //updateArduino();
    }

    if(newMSG)
    {
        newMSG = false;
        lastMidiMsg.port = newMidiMsg.port;
        lastMidiMsg.channel = newMidiMsg.channel;
        lastMidiMsg.status = newMidiMsg.status;
        lastMidiMsg.byteOne = newMidiMsg.byteOne;
        lastMidiMsg.byteTwo = newMidiMsg.byteTwo;
        lastMidiMsg.timestamp = newMidiMsg.timestamp;

        if (lastMidiMsg.channel >= LISTENCHBEG && lastMidiMsg.channel <= LISTENCHEND)
        {
            newMSGCH ^= true;
            if (lastMidiMsg.byteOne >= LISTENNTBEG && LISTENNTEND)
            {
                newMSGNT ^= true;
            }

            switch (remapMode)
            {
            case CHANGE_VIDEOS:

                if (lastMidiMsg.byteOne >= LISTENNTBEG &&
                        lastMidiMsg.byteOne <= floor(LISTENNTBEG + LISTENNTEND)/2.0f)
                {
                    GROUPS[0].playVideoInGroup(ofMap(lastMidiMsg.byteOne, LISTENNTBEG, floor(LISTENNTBEG + LISTENNTEND)/2.0f, 0, GROUPS[0].numberLoaded, true));
                }
                if (lastMidiMsg.byteOne >= floor(LISTENNTBEG + LISTENNTEND)/2.0f &&
                        lastMidiMsg.byteOne <= LISTENNTEND)
                {
                    GROUPS[1].playVideoInGroup(ofMap(lastMidiMsg.byteOne, floor(LISTENNTBEG + LISTENNTEND)/2.0f, LISTENNTEND, 0, GROUPS[1].numberLoaded, true));
                }

                break;

            case CHANGE_BLUR:

                if (lastMidiMsg.byteOne >= LISTENNTBEG &&
                        lastMidiMsg.byteOne <= floor(LISTENNTBEG + LISTENNTEND)/2.0f)
                {
                    EFFECTS[0].blurAmount = ofMap(lastMidiMsg.byteOne, LISTENNTBEG, floor(LISTENNTBEG + LISTENNTEND)/2.0f, 0, 20.0, true);
                }
                if (lastMidiMsg.byteOne >= floor(LISTENNTBEG + LISTENNTEND)/2.0f &&
                        lastMidiMsg.byteOne <= LISTENNTEND)
                {
                    EFFECTS[1].blurAmount = ofMap(lastMidiMsg.byteOne, floor(LISTENNTBEG + LISTENNTEND)/2.0f, LISTENNTEND, 0, 20.0, true);
                }

                break;
            }

        }

        if (lastMidiMsg.channel == CONTROLCHANNEL)
        {
            // note values change video
            if (lastMidiMsg.byteOne >= 48 && lastMidiMsg.byteOne <=59)
            {
                GROUPS[0].playVideoInGroup((int)lastMidiMsg.byteOne - 48);
            }
            if (lastMidiMsg.byteOne >= 60 && lastMidiMsg.byteOne <=72)
            {
                GROUPS[1].playVideoInGroup((int)lastMidiMsg.byteOne - 60);
            }

            // control data

            if (lastMidiMsg.status == 176)
            {
                // x-fades
                if (lastMidiMsg.byteOne == 1)
                {
                    XFADE = ofMap(lastMidiMsg.byteTwo, 0.0f, 127.0f, -1.0f, 1.0f, true);
                }
                if (lastMidiMsg.byteOne == 2)
                {
                    EFFECTS[0].fadeLevel = ofMap(lastMidiMsg.byteTwo, 0.0f, 127.0f, -1.0f, 1.0f, true);
                }
                if (lastMidiMsg.byteOne == 3)
                {
                    EFFECTS[1].fadeLevel = ofMap(lastMidiMsg.byteTwo, 0.0f, 127.0f, -1.0f, 1.0f, true);
                }

                // channel speeds
                if (lastMidiMsg.byteOne == 17)
                {
                    EFFECTS[0].videoSpeed = ofMap(lastMidiMsg.byteTwo, 0.0f, 127.0f, -5.0f, 5.0f, true);
                }
                if (lastMidiMsg.byteOne == 18)
                {
                    EFFECTS[1].videoSpeed = ofMap(lastMidiMsg.byteTwo, 0.0f, 127.0f, -5.0f, 5.0f, true);
                }


            }

        }



        if (lastMidiMsg.channel == SOLENOIDCHANNEL)
        {
            if (lastMidiMsg.byteOne == 71)
            {
                if(lastMidiMsg.status == 144) PLAYSOLENOIDS ^= true;
                //if(lastMidiMsg.status == 128) playSolenoids = false;
            }

            if (ard.isArduinoReady() && PLAYSOLENOIDS)
            {
                for (int i = 0; i < NUM_SOLENOIDS; i++)
                {
                    if (keys[i] == lastMidiMsg.byteOne)
                    {
                        if(lastMidiMsg.status == 144) ard.sendDigital(ports[i], ARD_HIGH);
                        if(lastMidiMsg.status == 128) ard.sendDigital(ports[i], ARD_LOW);
                    }
                }
            }
        }

    }
}
//--------------------------------------------------------------
void goMidiManager::setupArduino()
{

    // this is where you setup all the pins and pin modes, etc
    for (int i = 0; i < NUM_SOLENOIDS; i++)
    {
        ard.sendDigitalPinMode(ports[i], ARD_OUTPUT);
    }

}

//--------------------------------------------------------------
void goMidiManager::updateArduino()
{

    // update the arduino, get any data or messages:
    ard.update();

    if(ofGetFrameNum()%2 == 1)
    {
        for (int i = 0; i < NUM_SOLENOIDS; i++) ard.sendDigital(ports[i], ARD_LOW);

    }
    else
    {
        for (int i = 0; i < NUM_SOLENOIDS; i++) ard.sendDigital(ports[i], ARD_HIGH);
    }
}

goMidiManager::goMidiManager(const goMidiManager& other)
{
    //copy ctor
}

goMidiManager& goMidiManager::operator=(const goMidiManager& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}
