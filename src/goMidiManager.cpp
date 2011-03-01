#include "goMidiManager.h"


static const int ports[] = {2,3,4,5,6,7,8,9,10};
//static const int keys[] = {48,50,52,53,55,57,59,60,62,64};
static const int keys[] = {60,61,62,63,64,65,66,68};

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
    midiFXChannel = 0;
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
        for (int i = 0; i < 8; i++)
        {
            if(ofGetElapsedTimeMillis() - ardTimer[i] > 10)  ard.sendDigital(ports[i], ARD_LOW);
        }

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

        for(int i = 0; i < 6; i++)
        {
            if (lastMidiMsg.channel >= LISTENCHBEG[i] && lastMidiMsg.channel <= LISTENCHEND[i])
            {

                if(LEARNRANGE[i])
                {
                    LISTENNTBEG[i] = MIN(lastMidiMsg.byteOne, LISTENNTBEG[i]);
                    LISTENNTEND[i] = MAX(lastMidiMsg.byteOne, LISTENNTEND[i]);
                }

                newMSGCH[i] ^= true;

                if (lastMidiMsg.byteOne >= LISTENNTBEG[i] &&  LISTENNTEND[i])
                {
                    newMSGNT[i] ^= true;
                }

                switch (REMAPMODE[i])
                {
                case SOLENOID_CONTROL:
                    if (ard.isArduinoReady())
                    {
                        for (int j = 0; j < NUM_SOLENOIDS; j++)
                        {
                            if(lastMidiMsg.status == 144)
                            {
                                int mapPort = (int)floor(ofMap(lastMidiMsg.byteOne, LISTENNTBEG[i],  LISTENNTEND[i], 0, 9, false));
                                ard.sendDigital(ports[mapPort], ARD_HIGH);
                                ardTimer[mapPort] = ofGetElapsedTimeMillis();
                            }
                            //ard.sendDigital(ports[i], ARD_LOW);
                        }
                    }
                    break;
                case PARTICLE_GENERATE:
                    cout << PARTICLEMODE[i] << endl;
                    if(PARTICLEMODE[i] == 0)
                    {
                        for (int j = 0; j < 8; j++)
                        {
                            PARTICLES->generate(j, (int)ofRandom(0,lastMidiMsg.byteTwo));
                        }
                    }
                    else
                    {
                        PARTICLES->generate(PARTICLEMODE[i]-1, lastMidiMsg.byteTwo);
                    }
                    break;
                case NOTE_TO_VIDEOS:
                    remap(i, 0.0f, GROUPS[0].numberLoaded, 0.0f, GROUPS[1].numberLoaded);
                    break;
                case RANDOM_VIDEO:
                    remap(i, GROUPS[0].numberLoaded, GROUPS[1].numberLoaded);
                    break;
                case FX_BLUR:
                    remap(i, &EFFECTS[0].blurAmount, &EFFECTS[1].blurAmount, 0.0f, 20.0f, 0.0f, 20.0f);
                    break;
                case FX_FLIP_X:
                    remap(i, &EFFECTS[0].doFlipX, &EFFECTS[1].doFlipX);
                    break;
                case FX_FLIP_Y:
                    remap(i, &EFFECTS[0].doFlipY, &EFFECTS[1].doFlipY);
                    break;
                case FX_GREYSCALE:
                    remap(i, &EFFECTS[0].doGreyscale, &EFFECTS[1].doGreyscale);
                    break;
                case FX_INVERT:
                    remap(i, &EFFECTS[0].doInvert, &EFFECTS[1].doInvert);
                    break;
                case FX_THRESHOLD:
                    remap(i, &EFFECTS[0].threshLevel, &EFFECTS[1].threshLevel, 0.0f, 10.0f, 0.0f, 10.0f);
                    break;
                case FX_SATURATION:
                    remap(i, &EFFECTS[0].saturationLevel, &EFFECTS[1].saturationLevel, 0.0f, 10.0f, 0.0f, 10.0f);
                    break;
                case FX_CONTRAST:
                    remap(i, &EFFECTS[0].contrastLevel, &EFFECTS[1].contrastLevel, 0.0f, 10.0f, 0.0f, 10.0f);
                    break;
                case FX_BRIGHTNESS:
                    remap(i, &EFFECTS[0].brightnessLevel, &EFFECTS[1].brightnessLevel, 0.0f, 10.0f, 0.0f, 10.0f);
                    break;
                case X_FADER:
                    remap(i, &XFADE, &XFADE, -1.0, 1.0, -1.0, 1.0);
                    break;
                case CH_FADER:
                    remap(i, &EFFECTS[0].fadeLevel, &EFFECTS[1].fadeLevel, 0.0f, 1.0f, 0.0f, 1.0f);
                    break;
                case REVERSE_CHANNELS:
                    remap(i, &REVERSECHANNELS, &REVERSECHANNELS);
                    break;

                }

            }
        }



        if (lastMidiMsg.channel == CONTROLCHANNEL)
        {

            if (lastMidiMsg.status == 144 || !PROTECTCONTROL)
            {
                // note values change video
                if (lastMidiMsg.byteOne >= 48 && lastMidiMsg.byteOne <=59)
                {
                    GROUPS[0].playVideoInGroup((int)lastMidiMsg.byteOne - 48 + 12 * SELECTIONGRP[0]);
                }
                if (lastMidiMsg.byteOne >= 60 && lastMidiMsg.byteOne <=72)
                {
                    GROUPS[1].playVideoInGroup((int)lastMidiMsg.byteOne - 60 + 12 * SELECTIONGRP[1]);
                }
                if(lastMidiMsg.byteOne > 35 && lastMidiMsg.byteOne < 44)
                {
                    PARTICLES->generate(lastMidiMsg.byteOne-36, lastMidiMsg.byteTwo);
                }
            }
            if (lastMidiMsg.status == 176 || !PROTECTCONTROL)
            {
                // Novation control data

                if(lastMidiMsg.byteOne == 17)
                {
                    PARTICLES->pWidth = ofMap(lastMidiMsg.byteTwo, 0.0f, 127.0f, 0, 720.0, false);
                }
                if(lastMidiMsg.byteOne == 18)
                {
                    PARTICLES->pDamp = ofMap(lastMidiMsg.byteTwo, 0.0f, 127.0f, 0, 40.0, false);
                }
                if(lastMidiMsg.byteOne == 19)
                {
                    PARTICLES->particlePattern = ofMap(lastMidiMsg.byteTwo, 0, 16, 0, 16, false);
                }
                if(lastMidiMsg.byteOne == 33)
                {
                    SELECTIONGRP[0] = 0;
                }
                if(lastMidiMsg.byteOne == 34)
                {
                    SELECTIONGRP[0] = 1;
                }
                if(lastMidiMsg.byteOne == 35)
                {
                    SELECTIONGRP[0] = 2;
                }
                if(lastMidiMsg.byteOne == 36)
                {
                    SELECTIONGRP[1] = 0;
                }
                if(lastMidiMsg.byteOne == 37)
                {
                    SELECTIONGRP[1] = 1;
                }
                if(lastMidiMsg.byteOne == 38)
                {
                    SELECTIONGRP[1] = 2;
                }
                if(lastMidiMsg.byteOne == 49)
                {
                    PARTICLES->sizeParticle = !(bool)lastMidiMsg.byteTwo;
                }
                if(lastMidiMsg.byteOne == 50)
                {
                    PARTICLES->speedParticle = !(bool)lastMidiMsg.byteTwo;
                }
                if(lastMidiMsg.byteOne == 51)
                {
                    PARTICLES->linkParticle = !(bool)lastMidiMsg.byteTwo;
                }
                if(lastMidiMsg.byteOne == 52)
                {
                    PARTICLES->particleColors = !(bool)lastMidiMsg.byteTwo;
                }
                if(lastMidiMsg.byteOne == 53)
                {
                    PARTICLES->eraseParticle = !(bool)lastMidiMsg.byteTwo;
                }
                if(lastMidiMsg.byteOne == 56)
                {
                    CONTROLLER->fullScreen();
                }
                if(lastMidiMsg.byteOne == 41 || lastMidiMsg.byteOne == 47)
                {
                    MUTE[0] = !(bool)lastMidiMsg.byteTwo;
                }
                if(lastMidiMsg.byteOne == 42 || lastMidiMsg.byteOne == 48)
                {
                    MUTE[1] = !(bool)lastMidiMsg.byteTwo;
                }
                if(lastMidiMsg.byteOne == 43)
                {
                    CHANNELADIRECT = !(bool)lastMidiMsg.byteTwo;
                }
                if(lastMidiMsg.byteOne == 44)
                {
                    EFFECTS[0].muteAll = !(bool)lastMidiMsg.byteTwo;
                }
                if(lastMidiMsg.byteOne == 45)
                {
                    CHANNELBDIRECT = !(bool)lastMidiMsg.byteTwo;
                }
                if(lastMidiMsg.byteOne == 46)
                {
                    EFFECTS[1].muteAll = !(bool)lastMidiMsg.byteTwo;
                }
                if(lastMidiMsg.byteOne == 30)
                {
                    if (lastMidiMsg.byteTwo == 127) midiFXChannel = 0;
                    else midiFXChannel = 1;
                }
                if(lastMidiMsg.byteOne == 25)
                {
                    EFFECTS[midiFXChannel].doBlur = !(bool)lastMidiMsg.byteTwo;
                }
                if(lastMidiMsg.byteOne == 26)
                {
                    EFFECTS[midiFXChannel].doThreshold = !(bool)lastMidiMsg.byteTwo;
                }
                if(lastMidiMsg.byteOne == 27)
                {
                    EFFECTS[midiFXChannel].doSaturation = !(bool)lastMidiMsg.byteTwo;
                }
                if(lastMidiMsg.byteOne == 28)
                {
                    EFFECTS[midiFXChannel].doContrast = !(bool)lastMidiMsg.byteTwo;
                }
                if(lastMidiMsg.byteOne == 29)
                {
                    EFFECTS[midiFXChannel].doBrightness = !(bool)lastMidiMsg.byteTwo;
                }
                if(lastMidiMsg.byteOne == 9)
                {
                    EFFECTS[midiFXChannel].blurAmount = ofMap(lastMidiMsg.byteTwo, 0.0f, 127.0f, 0, 20, true);
                }
                if(lastMidiMsg.byteOne == 10)
                {
                    EFFECTS[midiFXChannel].threshLevel = ofMap(lastMidiMsg.byteTwo, 0.0f, 127.0f, 0.0f, 1.0f, true);
                }
                if(lastMidiMsg.byteOne == 11)
                {
                    EFFECTS[midiFXChannel].saturationLevel = ofMap(lastMidiMsg.byteTwo, 0.0f, 127.0f, 0.0f, 10.0f, true);
                }
                if(lastMidiMsg.byteOne == 12)
                {
                    EFFECTS[midiFXChannel].contrastLevel = ofMap(lastMidiMsg.byteTwo, 0.0f, 127.0f, 0.0f, 10.0f, true);
                }
                if(lastMidiMsg.byteOne == 13)
                {
                    EFFECTS[midiFXChannel].brightnessLevel = ofMap(lastMidiMsg.byteTwo, 0.0f, 127.0f, 0.0f, 10.0f, true);
                }
                if(lastMidiMsg.byteOne == 41)
                {
                    EFFECTS[0].videoSpeed = 1;
                }
                if(lastMidiMsg.byteOne == 42)
                {
                    EFFECTS[1].videoSpeed = 1;
                }
                if(lastMidiMsg.byteOne == 7)
                {
                    EFFECTS[0].videoSpeed = ofMap(lastMidiMsg.byteTwo, 0.0f, 127.0f, -5.0f, 5.0f, true);
                }
                if(lastMidiMsg.byteOne == 8)
                {
                    EFFECTS[1].videoSpeed = ofMap(lastMidiMsg.byteTwo, 0.0f, 127.0f, -5.0f, 5.0f, true);
                }
                // x-fades
                if (lastMidiMsg.byteOne == 1)
                {
                    XFADE = ofMap(lastMidiMsg.byteTwo, 0.0f, 127.0f, -1.0f, 1.0f, true);
                }
                if (lastMidiMsg.byteOne == 2)
                {
                    EFFECTS[0].fadeLevel = ofMap(lastMidiMsg.byteTwo, 0.0f, 127.0f, 0.0f, 1.0f, true);
                }
                if (lastMidiMsg.byteOne == 3)
                {
                    EFFECTS[1].fadeLevel = ofMap(lastMidiMsg.byteTwo, 0.0f, 127.0f, 0.0f, 1.0f, true);
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
                        if(lastMidiMsg.status == 144)
                        {
                            ard.sendDigital(ports[i], ARD_HIGH);
                            ardTimer[i] = ofGetElapsedTimeMillis();
                        }
                        //if(lastMidiMsg.status == 128) ard.sendDigital(ports[i], ARD_LOW);
                    }
                }
            }
        }

    }
}

void goMidiManager::remap(int & index, bool * var0, bool * var1)
{
    bool decision = (bool)floor(ofMap(lastMidiMsg.byteOne, LISTENNTBEG[index],  LISTENNTEND[index], 0, 2, true));
    switch (CHANNELMODE[index])
    {
    case CHANNEL_BOTH:
        *var0 = decision;
        *var1 = decision;
        break;

    case CHANNEL_A:
        *var0 = decision;
        break;

    case CHANNEL_B:
        *var1 = decision;
        break;

    case CHANNEL_SPLIT:
        if (lastMidiMsg.byteOne >= LISTENNTBEG[index] &&
                lastMidiMsg.byteOne <= floor(LISTENNTBEG[index] +  LISTENNTEND[index])/2.0f)
        {
            *var0 = (bool)floor(ofMap(lastMidiMsg.byteOne, LISTENNTBEG[index], floor(LISTENNTBEG[index] +  LISTENNTEND[index])/2.0f, 0, 2, true));
        }
        if (lastMidiMsg.byteOne >= floor(LISTENNTBEG[index] +  LISTENNTEND[index])/2.0f &&
                lastMidiMsg.byteOne <=  LISTENNTEND[index])
        {
            *var1 = (bool)floor(ofMap(lastMidiMsg.byteOne, floor(LISTENNTBEG[index] +  LISTENNTEND[index])/2.0f,  LISTENNTEND[index], 0, 2, true));
        }
        break;
    }
}

void goMidiManager::remap(int & index, int rBegin0, int rEnd0, int rBegin1, int rEnd1)
{
    switch (CHANNELMODE[index])
    {
    case CHANNEL_BOTH:
        GROUPS[0].playVideoInGroup(ofMap(lastMidiMsg.byteOne, LISTENNTBEG[index],  LISTENNTEND[index], rBegin0, rEnd0, true));
        GROUPS[1].playVideoInGroup(ofMap(lastMidiMsg.byteOne, LISTENNTBEG[index],  LISTENNTEND[index], rBegin1, rEnd1, true));
        break;

    case CHANNEL_A:
        GROUPS[0].playVideoInGroup(ofMap(lastMidiMsg.byteOne, LISTENNTBEG[index],  LISTENNTEND[index], rBegin0, rEnd0, true));
        break;

    case CHANNEL_B:
        GROUPS[1].playVideoInGroup(ofMap(lastMidiMsg.byteOne, LISTENNTBEG[index],  LISTENNTEND[index], rBegin1, rEnd1, true));
        break;

    case CHANNEL_SPLIT:
        if (lastMidiMsg.byteOne >= LISTENNTBEG[index] &&
                lastMidiMsg.byteOne <= floor(LISTENNTBEG[index] +  LISTENNTEND[index])/2.0f)
        {
            GROUPS[0].playVideoInGroup(ofMap(lastMidiMsg.byteOne, LISTENNTBEG[index], floor(LISTENNTBEG[index] +  LISTENNTEND[index])/2.0f, rBegin0, rEnd0, true));
        }
        if (lastMidiMsg.byteOne >= floor(LISTENNTBEG[index] +  LISTENNTEND[index])/2.0f &&
                lastMidiMsg.byteOne <=  LISTENNTEND[index])
        {
            GROUPS[1].playVideoInGroup(ofMap(lastMidiMsg.byteOne, floor(LISTENNTBEG[index] +  LISTENNTEND[index])/2.0f,  LISTENNTEND[index], rBegin1, rEnd0, true));
        }
        break;
    }
}

void goMidiManager::remap(int & index, int maxR0, int maxR1)
{
    switch (CHANNELMODE[index])
    {
    case CHANNEL_BOTH:
        GROUPS[0].playVideoInGroup(ofRandom(0, maxR0));
        GROUPS[1].playVideoInGroup(ofRandom(0, maxR1));
        break;

    case CHANNEL_A:
        GROUPS[0].playVideoInGroup(ofRandom(0, maxR0));
        break;

    case CHANNEL_B:
        GROUPS[1].playVideoInGroup(ofRandom(0, maxR1));
        break;

    case CHANNEL_SPLIT:
        if (lastMidiMsg.byteOne >= LISTENNTBEG[index] &&
                lastMidiMsg.byteOne <= floor(LISTENNTBEG[index] +  LISTENNTEND[index])/2.0f)
        {
            GROUPS[0].playVideoInGroup(ofRandom(0, maxR0));
        }
        if (lastMidiMsg.byteOne >= floor(LISTENNTBEG[index] +  LISTENNTEND[index])/2.0f &&
                lastMidiMsg.byteOne <=  LISTENNTEND[index])
        {
            GROUPS[1].playVideoInGroup(ofRandom(0, maxR1));
        }
        break;
    }
}

void goMidiManager::remap(int & index, int * var0, int * var1, float rBegin0, float rEnd0, float rBegin1, float rEnd1)
{
    switch (CHANNELMODE[index])
    {
    case CHANNEL_BOTH:
        *var0 = ofMap(lastMidiMsg.byteOne, LISTENNTBEG[index],  LISTENNTEND[index], rBegin0, rEnd0, true);
        *var1 = ofMap(lastMidiMsg.byteOne, LISTENNTBEG[index],  LISTENNTEND[index], rBegin1, rEnd1, true);
        break;

    case CHANNEL_A:
        *var0 = ofMap(lastMidiMsg.byteOne, LISTENNTBEG[index],  LISTENNTEND[index], rBegin0, rEnd0, true);
        break;

    case CHANNEL_B:
        *var1 = ofMap(lastMidiMsg.byteOne, LISTENNTBEG[index],  LISTENNTEND[index], rBegin1, rEnd1, true);
        break;

    case CHANNEL_SPLIT:
        if (lastMidiMsg.byteOne >= LISTENNTBEG[index] &&
                lastMidiMsg.byteOne <= floor(LISTENNTBEG[index] +  LISTENNTEND[index])/2.0f)
        {
            *var0 = ofMap(lastMidiMsg.byteOne, LISTENNTBEG[index], floor(LISTENNTBEG[index] +  LISTENNTEND[index])/2.0f, rBegin0, rEnd0, true);
        }
        if (lastMidiMsg.byteOne >= floor(LISTENNTBEG[index] +  LISTENNTEND[index])/2.0f &&
                lastMidiMsg.byteOne <=  LISTENNTEND[index])
        {
            *var1 = ofMap(lastMidiMsg.byteOne, floor(LISTENNTBEG[index] +  LISTENNTEND[index])/2.0f,  LISTENNTEND[index], rBegin1, rEnd0, true);
        }
        break;
    }
}

void goMidiManager::remap(int & index, float * var0, float * var1, float rBegin0, float rEnd0, float rBegin1, float rEnd1)
{
    switch (CHANNELMODE[index])
    {
    case CHANNEL_BOTH:
        *var0 = ofMap(lastMidiMsg.byteOne, LISTENNTBEG[index],  LISTENNTEND[index], rBegin0, rEnd0, true);
        *var1 = ofMap(lastMidiMsg.byteOne, LISTENNTBEG[index],  LISTENNTEND[index], rBegin1, rEnd1, true);
        break;

    case CHANNEL_A:
        *var0 = ofMap(lastMidiMsg.byteOne, LISTENNTBEG[index],  LISTENNTEND[index], rBegin0, rEnd0, true);
        break;

    case CHANNEL_B:
        *var1 = ofMap(lastMidiMsg.byteOne, LISTENNTBEG[index],  LISTENNTEND[index], rBegin1, rEnd1, true);
        break;

    case CHANNEL_SPLIT:
        if (lastMidiMsg.byteOne >= LISTENNTBEG[index] &&
                lastMidiMsg.byteOne <= floor(LISTENNTBEG[index] +  LISTENNTEND[index])/2.0f)
        {
            *var0 = ofMap(lastMidiMsg.byteOne, LISTENNTBEG[index], floor(LISTENNTBEG[index] +  LISTENNTEND[index])/2.0f, rBegin0, rEnd0, true);
        }
        if (lastMidiMsg.byteOne >= floor(LISTENNTBEG[index] +  LISTENNTEND[index])/2.0f &&
                lastMidiMsg.byteOne <=  LISTENNTEND[index])
        {
            *var1 = ofMap(lastMidiMsg.byteOne, floor(LISTENNTBEG[index] +  LISTENNTEND[index])/2.0f,  LISTENNTEND[index], rBegin1, rEnd0, true);
        }
        break;
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
