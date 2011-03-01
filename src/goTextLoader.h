#ifndef __GOTEXTLOADER_H
#define __GOTEXTLOADER_H

#include "ofMain.h"

class goTextLoader
{
    public:
        goTextLoader();
        virtual ~goTextLoader();

        void loadText(string fileName);

        vector<string>  words;

    protected:
    private:
        void parseText(string text);
};

#endif // __GOTEXTLOADER_H
