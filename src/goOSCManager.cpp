#include "goOSCManager.h"

goOSCManager::goOSCManager()
{
    //ctor
    receiver.setup( 60000 );
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

        newMSG ^= true;

        if (m.getAddress() == "/co1")
        {
            //cout << m.getArgAsFloat(0) << endl;
            co1 = ABS(m.getArgAsFloat(0));

        }
        if (m.getAddress() == "/co2")
        {
            //cout << m.getArgAsFloat(0) << endl;
            co2 = ABS(m.getArgAsFloat(0));
        }
        if (m.getAddress() == "/co3")
        {
            //cout << m.getArgAsFloat(0) << endl;
            co3 = ABS(m.getArgAsFloat(0));
        }
        if (m.getAddress() == "/amp")
        {
            //cout << m.getArgAsFloat(0) << endl;
            amp = ABS(m.getArgAsFloat(0));
        }
        if (m.getAddress() == "/nota")
        {
            //cout << m.getArgAsFloat(0) << endl;
            note = m.getArgAsInt32(0);
        }

    }

}
