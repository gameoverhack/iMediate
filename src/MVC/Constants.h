/*
 *  Constants.h
 *  allAddonsExample
 *
 *  Created by gameoverx on 22/03/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

// config structur holds all the config info for the interactive
typedef struct
{
    int W;
    int H;
} configStructure;

enum states
{
    APP_INIT,
    APP_READY,
    APP_MANAGING,
    APP_UPDATE
};
const string states[] =
{
    "APP_INIT",
    "APP_READY",
    "APP_MANAGING",
    "APP_UPDATE"
};
