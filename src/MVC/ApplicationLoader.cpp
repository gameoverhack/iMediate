/*
 *  ApplicationLoader.cpp
 *  allAddonsExample
 *
 *  Created by gameoverx on 22/03/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ApplicationLoader.h"

// CONFIG XML FUNCTIONS
bool ApplicationLoader::setup(string _filename)
{

    LOG("Loading config file: " + ofToDataPath(_filename));

    if(loadXML(ofToDataPath(_filename), &CG))
    {

        LOG("Config loaded succesfully");

        if(configParse(&CG))
        {

            LOG("Config parsed...");
            return true;

        }
        else
        {

            LOG("Config could not be parsed...");
            return false;

        } // end configParse

    }
    else     // if not loaded
    {

        LOG("Config could not be loaded...");
        return false;

    } // end load config

}


//--------------------------------------------------------------
bool ApplicationLoader::loadXML(string _filename, ofxXmlSettings * XML)
{

    if (XML->loadFile(_filename))
    {
        return true;
    }
    else
    {
        return false;
    }

}
//--------------------------------------------------------------
bool ApplicationLoader::configParse(ofxXmlSettings * XML)
{

    if(XML->pushTag("config", 0))
    {

        // assign xml config to Model config
        CONFIG.W = atoi(XML->getValue("WIDTH", "1024", 0).c_str());
        CONFIG.H = atoi(XML->getValue("HEIGHT", "768", 0).c_str());

        return true;

    }
    else     // no config tag
    {

        return false;
    } // end parse config node

}
