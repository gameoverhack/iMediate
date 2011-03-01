/*
 *  ApplicationLoader.h
 *  allAddonsExample
 *
 *  Created by gameoverx on 22/03/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _APPLICATION_LOADER
#define _APPLICATION_LOADER

#define APPLOADER		ApplicationLoaderSingleton::Instance()

#include "Model.h"

class ApplicationLoader
{

public:

    bool		setup(string _filename);

private:

    bool		loadXML(string _filename, ofxXmlSettings * XML);
    bool		configParse(ofxXmlSettings * XML);

};

typedef Singleton<ApplicationLoader> ApplicationLoaderSingleton;   // Global declaration

#endif
