/*
 *  Model.cpp
 *  openFrameworks
 *
 *  Created by gameoverx on 1/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Model.h"

// state setters and getters
void Model::setAppState(int _state)
{
    LOG("STATE [APP] :					" + states[_state]);
    applicationState = _state;
}

int Model::getAppState()
{
    return applicationState;
}
