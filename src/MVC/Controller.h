/*
 *  Controller.h
 *  openFrameworks
 *
 *  Created by gameoverx on 1/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __CONTROLLER_H
#define __CONTROLLER_H

#define CONTROLLER		ControllerSingleton::Instance()

#include "Singleton.h"
#include "Model.h"
#include "View.h"
#include "Logger.h"
#include "ApplicationLoader.h"

class Controller
{

public:

    Controller()
    {
        LOG("Controller instantiated");
    };
    ~Controller()
    {
        LOG("Controller destroyed");
    };

    void						setup();

private:

};

typedef Singleton<Controller> ControllerSingleton;   // Global declaration

#endif
