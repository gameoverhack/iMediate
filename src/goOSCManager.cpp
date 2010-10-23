#include "goOSCManager.h"
static const int ports[] = {2,3,4,5,6,7,8,9,10};

goOSCManager::goOSCManager()
{
    //ctor
    receiver.setup( 60000 );

    oscmins[0] = oscmins[1] = oscmins[2] = oscmins[3] = oscmins[4] = 0;
    oscmaxs[0] = oscmaxs[1] = oscmaxs[2] = oscmaxs[3] = 1.0;
    oscmaxs[4] = 127;

    oscMapCount = 0;
}

goOSCManager::~goOSCManager()
{
    //dtor
}

void goOSCManager::update()
{
    while( receiver.hasWaitingMessages() )
    {
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage( &m );

        if (m.getAddress() == "/co1")
        {
            //cout << m.getArgAsFloat(0) << endl;
            data[0] = ABS(m.getArgAsFloat(0));
            newMSG = true;

        }
        if (m.getAddress() == "/co2")
        {
            //cout << m.getArgAsFloat(0) << endl;
            data[1] = ABS(m.getArgAsFloat(0));
            newMSG = true;
        }
        if (m.getAddress() == "/co3")
        {
            //cout << m.getArgAsFloat(0) << endl;
            data[2] = ABS(m.getArgAsFloat(0));
            newMSG = true;
        }
        if (m.getAddress() == "/amp")
        {
            //cout << m.getArgAsFloat(0) << endl;
            data[3] = ABS(m.getArgAsFloat(0));
            newMSG = true;
        }
        if (m.getAddress() == "/nota")
        {
            //cout << m.getArgAsFloat(0) << endl;
            data[4] = (float)m.getArgAsInt32(0);
            newMSG = true;
        }

    }

    if ( ARD.isArduinoReady())
    {

        for (int i = 0; i < 8; i++)
        {
            if(ofGetElapsedTimeMillis() - ardTimer[i] > 10)  ARD.sendDigital(ports[i], ARD_LOW);
        }

    }

    if(newMSG)
    {
        newMSG = false;

        if (REMAPOSC[oscMapCount])
        {

            lastMidiMsg.port = 0;
            lastMidiMsg.channel = OSCCHANNEL[oscMapCount];
            lastMidiMsg.status = 144;
            lastMidiMsg.byteOne = ofMap(OSCDATA[oscMapCount], OSCRANGEMIN[oscMapCount], OSCRANGEMAX[oscMapCount], 0, 127);
            lastMidiMsg.byteTwo = ofRandom(0, 127);
            lastMidiMsg.timestamp = ofGetElapsedTimeMillis();

            //midiOut.sendNoteOn(OSCCHANNEL[i], , );
        }

        if (oscMapCount < 5)
        {
            oscMapCount++;
        }
        else
        {
            oscMapCount = 0;
        }



        for(int i = 3; i < 6; i++)
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
                    if (ARD.isArduinoReady())
                    {
                        for (int j = 0; j < NUM_SOLENOIDS; j++)
                        {
                            if(lastMidiMsg.status == 144)
                            {
                                int mapPort = (int)floor(ofMap(lastMidiMsg.byteOne, LISTENNTBEG[i],  LISTENNTEND[i], 0, 9, false));
                                ARD.sendDigital(ports[mapPort], ARD_HIGH);
                                ardTimer[mapPort] = ofGetElapsedTimeMillis();
                            }
                            //ard.sendDigital(ports[i], ARD_LOW);
                        }
                    }
                    break;
                case PARTICLE_GENERATE:

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
    }
}

void goOSCManager::remap(int & index, bool * var0, bool * var1)
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

void goOSCManager::remap(int & index, int rBegin0, int rEnd0, int rBegin1, int rEnd1)
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

void goOSCManager::remap(int & index, int maxR0, int maxR1)
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

void goOSCManager::remap(int & index, int * var0, int * var1, float rBegin0, float rEnd0, float rBegin1, float rEnd1)
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

void goOSCManager::remap(int & index, float * var0, float * var1, float rBegin0, float rEnd0, float rBegin1, float rEnd1)
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
