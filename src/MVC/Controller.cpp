/*
 *  Controller.cpp
 *  openFrameworks
 *
 *  Created by gameoverx on 1/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Controller.h"

void Controller::setup()
{

    SETAPPSTATE(APP_INIT);

    LOG("Application Initializing");

    // Load in the config XML and setup the Model
    bool abort = true;

    if (APPLOADER->setup("config.xml"))
    {
        abort = false;
    }
    else
    {
        LOG("...config error. Exiting!");
        abort = true;
    }

    if(VIEW->setup())
    {
        abort = false;
    }
    else
    {
        LOG("...view failed to setup. Exiting!");
        abort = true;
    }

    if(abort)
    {
        // exit!
        exit(0);
    }
    else
    {

        ofSetLogLevel(OF_LOG_VERBOSE);

        // list root video folders - TODO: delegate to Application Loader

        string rootVideoFolder;

#ifdef TARGET_OSX
        rootVideoFolder = "/Volumes/GhostDriver/MATT/vjMedia/";
#else
        rootVideoFolder = "E:/vjMedia"; //C:/Users/gameoverwell/Desktop/vjMedia";
#endif

        //FILES.setVerbose(true);
        FILES.allowExt("mov");
        FILES.listDir(rootVideoFolder, true);

        for (int i = 0; i < FILES.getNumberOfDirectories(); i++)
        {
            FOLDERARRAY[i] = FILES.directoriesWithFiles.at(i).substr(rootVideoFolder.size());
        }

        // dummy allocate to force openCL to initialize
        EFFECTS[0].setup();

        GUIMANAGER->setup();

        GROUPS[0].setup(900.0f, 405 + 10.0f);
        GROUPS[1].setup(900.0f + 720/10.0f * 4.0 + 10.0f, 405 + 10.0f);

        MIDIMANAGER->setup();

        /*DICTIONARY.loadText(ofToDataPath("Spanish.txt"));
        DICTIONARY.loadText(ofToDataPath("French.txt"));
        DICTIONARY.loadText(ofToDataPath("Latin.txt"));
        DICTIONARY.loadText(ofToDataPath("German.txt"));
        DICTIONARY.loadText(ofToDataPath("Italian.txt"));
        DICTIONARY.loadText(ofToDataPath("Portuguese.txt"));

        for (int i = 0; i < DICTIONARY.words.size(); i++)
        {
            //cout << i << " = " << DICTIONARY.words[i] << endl;
        }
        */

        // start application
        bCustomFullscreen = false;
        SETAPPSTATE(APP_READY);
    }
}

void Controller::update()
{
    OSCMANAGER->update();
    MIDIMANAGER->update();
    GUIMANAGER->update();

    for (int i = 0; i < MAX_VIDEO_CHANNELS; i++)
    {
        EFFECTS[i].update();
        GROUPS[i].update();
    }

    // scratch video : TODO: this is a hack but let's us do this for now!!!
    if (GUI.control("Channel A Position").lock) GROUPS[0].scratching = true;
    else GROUPS[0].scratching = false;

    if (GUI.control("Channel B Position").lock) GROUPS[1].scratching = true;
    else GROUPS[1].scratching = false;

}

void Controller::fullScreen()
{
#ifdef TARGET_WIN32
    if (!bCustomFullscreen)
    {
        LOG("Trying to force fullscreen on Windows 7" + ofToString(ofGetWidth()));
        windowTitle = "imMediate";
        ofSetWindowTitle(windowTitle);
        int x = 0;
        int y = 0;
        int width = W_CONTROL_SCREEN + W_OUTPUT_SCREEN;
        int height = H_CONTROL_SCREEN;
        int storedWindowX, storedWindowY, storedWindowH, storedWindowW;
        HWND vWnd  = FindWindow(NULL,  "imMediate");
        long windowStyle = GetWindowLong(vWnd, GWL_STYLE);
        windowStyle &= ~WS_OVERLAPPEDWINDOW;
        windowStyle |= WS_POPUP;
        SetWindowLong(vWnd, GWL_STYLE, windowStyle);
        SetWindowPos(vWnd, HWND_TOP, x, y, width, height, SWP_FRAMECHANGED);
        bCustomFullscreen = true;
    }
    else
    {
        int x = 0;
        int y = 0;
        int width = W_CONTROL_SCREEN;
        int height = H_CONTROL_SCREEN;
        HWND vWnd  = FindWindow(NULL,  "imMediate");
        long windowStyle = GetWindowLong(vWnd, GWL_STYLE);
        windowStyle |= WS_TILEDWINDOW;
        SetWindowLong(vWnd, GWL_STYLE, windowStyle);
        SetWindowPos(vWnd, HWND_TOP, x, y, width, height, SWP_FRAMECHANGED);
        bCustomFullscreen = false;
    }

#else
    ofToggleFullscreen();
#endif
}


