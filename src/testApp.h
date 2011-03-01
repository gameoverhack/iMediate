#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "Controller.h"

class testApp : public ofBaseApp
{

public:

    testApp() {};
    ~testApp()
    {
        LOG("Shutting down and destroying objects...");
        delete CONTROLLER;
        delete MODEL;
        delete VIEW;
        delete LOGGER;
    };

    void setup();
    //void update();
    //void draw();

    //void keyPressed  (int key);
    //void keyReleased(int key);
    //void mouseMoved(int x, int y );
    //void mouseDragged(int x, int y, int button);
    //void mousePressed(int x, int y, int button);
    //void mouseReleased(int x, int y, int button);
    //void resized(int w, int h);

};

#endif
