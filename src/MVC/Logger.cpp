/*
 *  Logger.cpp
 *  openFrameworks
 *
 *  Created by gameoverx on 31/08/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Logger.h"

//--------------------------------------------------------------
bool Logger::openLogFile(string _filename)
{
    log("Attempting to open log file...");
    filename = _filename;
    toFile = true;
    logfile.open(filename.c_str(), ofstream::app);
    if (logfile.good())
    {
        log("//___________________________________________________\\");
        log("Log started...");
        return true;
    }
    else
    {
        log("Cannot open file");
        return false;
    }
}
//--------------------------------------------------------------
bool Logger::closeLogFile()
{
    log("Closing log file");
    toFile = false;
    logfile.close();
    if (!logfile.fail()) return true;
    else return false;
}
//--------------------------------------------------------------
void Logger::log(string _msg)
{
    if(!toFile)
    {
        cout << "[" << gettimestamp() << "]: " << _msg.c_str() << endl;
    }
    else
    {
        cout << "[" << gettimestamp() << "]: " << _msg.c_str() << endl;
        logfile << "[" << gettimestamp() << "]: " << _msg.c_str() << endl;
    }
}
//--------------------------------------------------------------
void Logger::log(char _msg)
{

    if(!toFile)
    {
        cout << "[" << gettimestamp() << "]: " << _msg << endl;
    }
    else
    {
        cout << "[" << gettimestamp() << "]: " << _msg << endl;
        logfile << "[" << gettimestamp() << "]: " << _msg << endl;
    }
}
//--------------------------------------------------------------
char * Logger::gettimestamp()
{
    sprintf(timestamp, "%0.2i/%0.2i/%i - %0.2i:%0.2i:%0.2i", ofGetDay(), ofGetMonth(), ofGetYear(), ofGetHours(),  ofGetMinutes(), ofGetSeconds());
    return timestamp;
}
//--------------------------------------------------------------
