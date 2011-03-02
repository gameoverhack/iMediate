/*
 *  Constants.h
 *  allAddonsExample
 *
 *  Created by gameoverx on 22/03/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#define MAX_VIDEO_CHANNELS 2
#define NUM_SOLENOIDS 8

#define W_CONTROL_SCREEN 1920.0f
#define H_CONTROL_SCREEN 1080.0f

#define W_OUTPUT_SCREEN 1024.0f
#define H_OUTPUT_SCREEN 768.0f

#define W_FBODRAW_SCREEN 1024.0f
#define H_FBODRAW_SCREEN 768.0f

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

const int xblendModes[] =
{
GL_ZERO,
GL_ONE,
GL_SRC_COLOR,
GL_ONE_MINUS_SRC_COLOR,
GL_SRC_ALPHA,
GL_ONE_MINUS_SRC_ALPHA,
GL_DST_ALPHA,
GL_ONE_MINUS_DST_ALPHA
};

const string xblendModeNames[] =
{
"ZERO",
"ONE",
"SRC_COLOR",
"ONE_MINUS_SRC_COLOR",
"SRC_ALPHA",
"ONE_MINUS_SRC_ALPHA",
"DST_ALPHA",
"ONE_MINUS_DST_ALPHA"
};

const int xblendEquations[] =
{
GL_FUNC_ADD,
GL_MIN,
GL_MAX,
GL_FUNC_SUBTRACT,
GL_FUNC_REVERSE_SUBTRACT
};

const string xblendEquationNames[] =
{
"FUNC_ADD",
"MIN",
"MAX",
"FUNC_SUBTRACT",
"FUNC_REVERSE_SUBTRACT",
};

