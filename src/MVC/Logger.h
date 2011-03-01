/*
 *  Logger.h
 *  openFrameworks
 *
 *  Created by gameoverx on 31/08/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef __LOGGER_H
#define __LOGGER_H

#include <iostream>
#include <fstream>
#include "Singleton.h"
#include "ofUtils.h"

#define LOGGER			LoggerSingleton::Instance()
#define LOG				LoggerSingleton::Instance()->log
#define LOG_OPEN_FILE	LoggerSingleton::Instance()->openLogFile
//#define LOG_CLOSE_FILE	LoggerSingleton::Instance()->closeLogFile()

using namespace std;

class Logger
{

public:

    Logger()
    {

        log("Instantiating logger");

        toFile = false;
    };

    ~Logger()
    {

        log("Logging off...");
        log("\\___________________________________________________//");

        if (toFile) closeLogFile();
    };

    bool	openLogFile(string _filename);
    bool	closeLogFile();

    void	log(string	_msg);
    void	log(char	_msg);
    char *	gettimestamp();

private:

    ofstream logfile;
    string filename;
    char timestamp[255];
    bool toFile;

};

typedef Singleton<Logger> LoggerSingleton;   // Global declaration

#endif
