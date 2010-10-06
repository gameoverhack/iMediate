#include "goOpenCLContext.h"

goOpenCLContext::goOpenCLContext()
{
    //ctor
    cout << "HERE!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;

    // init openCL from OpenGL context to enable GL-CL data sharing
        openCL.setupFromOpenGL();
        // load and compile openCL program
        openCL.loadProgramFromFile("MSAopenCL/ImageProcessing.cl");

        // load kernels
        openCL.loadKernel("msa_boxblur");
        openCL.loadKernel("msa_flipx");
        openCL.loadKernel("msa_flipy");
        openCL.loadKernel("msa_greyscale");
        openCL.loadKernel("msa_invert");
        openCL.loadKernel("msa_threshold");
        openCL.loadKernel("go_hue");
        openCL.loadKernel("go_saturation");
        openCL.loadKernel("go_contrast");
        openCL.loadKernel("go_brightness");
}

goOpenCLContext::~goOpenCLContext()
{
    //dtor
}
