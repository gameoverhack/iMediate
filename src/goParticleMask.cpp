#include "goParticleMask.h"

goParticleMask::goParticleMask()
{
    //ctor
}

goParticleMask::~goParticleMask()
{
    //dtor
}

void goParticleMask::update()
{
    for( int i = 0; i < 8; i++)
    {
        for( int j = 0; j < particles[i].size(); j++)
        {
            particles[i][j]->update();
            if(particles[i][j]->getX() > W_OUTPUT_SCREEN || particles[i][j]->getY() > H_OUTPUT_SCREEN || eraseParticle)
            {
                if(eraseParticle) eraseParticle = false;
                particles[i].erase(particles[i].begin()+j);
            }
        }
    }
}

void goParticleMask::draw(int hexColor,  int index)
{
    glPushMatrix();
    if(particleColors) ofSetColor(hexColor);
    for( int i = 0; i < particles[index].size(); i++)
    {
        (particles[index][i])->draw();
    }
    glPopMatrix();
}

void goParticleMask::generate(int _id, int _valueC)
{

    int tWidth;
    if(linkParticle)
    {
        tWidth = float(_valueC)/32.0f*float(pWidth);
    }
    else
    {
        tWidth = pWidth;
    }
    float tDamp;

    if(!speedParticle)
    {
        tDamp = 1.0f;
    }
    else
    {
        if (linkParticle) {
            tDamp = float(_valueC)/64.0f*float(pDamp)/64.0f;
        } else {
            tDamp = float(pDamp)/64.0f;
        }

    }

    particle *tParticle;
    tParticle = new particle;

    particle *tParticle2;
    tParticle2 = new particle;
    ofxVec2f tVec1 = ofxVec2f(0.0, float(_valueC)*tDamp);
    int tHeight1 = W_OUTPUT_SCREEN;
    int tRandomHeight2 = ofRandom(0,tHeight1);

    ofxVec2f tVec = ofxVec2f(float(_valueC)*tDamp, 0.0);
    int tHeight = H_OUTPUT_SCREEN;
    int tRandomHeight = ofRandom(0,tHeight);
    int tSqr = ofRandom(0,10)*tWidth;
    bool single = true;

    switch (particlePattern)
    {
    case 0:
        tParticle->setup(-tWidth, 0.0f, 1.0f, tVec, tWidth, tHeight);
        break;
    case 1:
        tParticle->setup(float(_id * tHeight/7), 0.0f, 1.0f, tVec, tWidth, _id * tHeight/7);
        break;
    case 2:
        tParticle->setup(-tWidth, float(_id * tHeight/7), 1.0f, tVec, tWidth, tHeight/7);
        break;
    case 3:
        tParticle->setup(-tWidth, float(ofRandom(-tHeight/2,tHeight)), 1.0f, tVec, tWidth, tRandomHeight);
        break;
    case 4:
        tParticle->setup(-tWidth, float(ofRandom(-tHeight/2,tHeight)), 1.0f, tVec, tSqr, tSqr);
        break;
    case 5:
        tParticle->setup(0.0f, 0.0f, 1.0f, tVec1, tHeight1, tWidth);
        break;
    case 6:
        tParticle->setup(float(_id * tHeight1/7), -tWidth, 1.0f, tVec1, _id * tHeight1/7, tWidth);
        break;
    case 7:
        tParticle->setup(float(_id * tHeight1/7), -tWidth, 1.0f, tVec1, tHeight1/7, tWidth);
        break;
    case 8:
        tParticle->setup(float(ofRandom(-tHeight1/2+tWidth,tHeight1/2-tWidth)), -tWidth, 1.0f, tVec1, tRandomHeight2, tWidth);
        break;
    case 9:
        tParticle->setup(float(ofRandom(-tHeight1/2,tHeight1)), -tWidth, 1.0f, tVec1, tSqr, tSqr);
        break;
    case 10:
        single = false;
        tParticle->setup(0.0f, 0.0f, 1.0f, tVec1, tHeight1, tWidth);
        tParticle2->setup(-tWidth, float(ofRandom(-tHeight/2,tHeight)), 1.0f, tVec, tWidth, tRandomHeight);
        break;
    case 11:
        single = false;
        tParticle2->setup(-tWidth, 0.0f, 1.0f, tVec, tWidth, tHeight);
        tParticle->setup(float(ofRandom(-tHeight1/2+tWidth,tHeight1/2-tWidth)), -tWidth, 1.0f, tVec1, tRandomHeight2, tWidth);
        break;
    case 12:
        single = false;
        tParticle2->setup(-tWidth, float(ofRandom(-tHeight/2,tHeight)), 1.0f, tVec, tSqr, tSqr);
        tParticle->setup(float(ofRandom(-tHeight1/2,tHeight1)), -tWidth, 1.0f, tVec1, tSqr, tSqr);
        break;
    case 13:
        single = false;
        tParticle->setup(0.0f, 0.0f, 1.0f, tVec1, tHeight1, tWidth);
        tParticle2->setup(-tWidth, 0.0f, 1.0f, tVec, tWidth, tHeight);
        break;
    case 14:
        single = false;
        tParticle2->setup(float(_id * tHeight/7), 0.0f, 1.0f, tVec, tWidth, _id * tHeight/7);
        tParticle->setup(float(_id * tHeight1/7), -tWidth, 1.0f, tVec1, _id * tHeight1/7, tWidth);
        break;
    case 15:
        single = false;
        tParticle2->setup(-tWidth, float(_id * tHeight/7), 1.0f, tVec, tWidth, tHeight/7);
        tParticle->setup(float(_id * tHeight1/7), -tWidth, 1.0f, tVec1, tHeight1/7, tWidth);
        break;
    case 16:
        single = false;
        tParticle2->setup(-tWidth, float(ofRandom(-tHeight/2,tHeight)), 1.0f, tVec, tWidth, tRandomHeight);
        tParticle->setup(float(ofRandom(-tHeight1/2+tWidth,tHeight1/2-tWidth)), -tWidth, 1.0f, tVec1, tRandomHeight2, tWidth);
        break;
    }

    if(single)
    {
        particles[_id].push_back(tParticle);
    }
    else
    {
        particles[_id].push_back(tParticle);
        particles[_id].push_back(tParticle2);
    }
}
