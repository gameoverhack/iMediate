#ifndef __GOOPENCLCONTEXT_H
#define __GOOPENCLCONTEXT_H

#include "Singleton.h"
#include "MSAOpenCL.h"

#define OPENCLCONTEXT			openCLSingleton::Instance()
#define OPENCL                  OPENCLCONTEXT->openCL

class goOpenCLContext
{
    public:
        goOpenCLContext();
        virtual ~goOpenCLContext();

        MSA::OpenCL			openCL;

    protected:
    private:
};

typedef Singleton<goOpenCLContext> openCLSingleton;   // Global declaration

#endif // __GOOPENCLCONTEXT_H
