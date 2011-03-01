#include "goTextLoader.h"

goTextLoader::goTextLoader()
{
    //ctor
}

goTextLoader::~goTextLoader()
{
    //dtor
}

void goTextLoader::loadText(string fileName)
{
    FILE * pFile;
    long lSize;
    char * buffer;
    size_t result;

    pFile = fopen (ofToDataPath(fileName).c_str(),"r+");
    //if (pFile==NULL)

    // obtain file size:
    fseek (pFile , 0 , SEEK_END);
    lSize = ftell (pFile);
    rewind (pFile);

    // allocate memory to contain the whole file:
    buffer = (char*) malloc (sizeof(char)*lSize);
    //if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

    // copy the file into the buffer:
    result = fread (buffer,1,lSize,pFile);
    //if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

    string bufferStr = buffer;

    parseText(bufferStr);
    /* the whole file is now loaded in the memory buffer. */

    // terminate
    fclose (pFile);
    free (buffer);
}

void goTextLoader::parseText(string text)
{
    vector<string> lines = ofSplitString(text, "\n");
    for (int i = 0; i < lines.size(); i++)
    {
        vector<string> linewords = ofSplitString(lines[i], "\t");
        for (int j = 0; j < linewords.size(); j++)
        {
            words.push_back(linewords[j]);
        }
        linewords.clear();
    }
    lines.clear();
}
