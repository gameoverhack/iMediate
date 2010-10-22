#include "particle.h"


particle::particle()
{
}

particle::~particle()
{
    pos.set(0,0);
    vel.set(0,0);
    damp = .99f;
}

void particle::setup( float x_, float y_, float damp_, ofxVec2f vel_, int _pWidth, int _pHeight)
{
	//printf("particle || posX %f posY: %f damp: %f velX %f velY: %f width: %i height: %i\n", x_, y_, damp_, vel_.x, vel_.y, _pWidth, _pHeight);
	pWidth = _pWidth;
	pHeight = _pHeight;
    pos.set(x_,y_);
    vel.set(vel_.x,vel_.y);
    damp = damp_;
}

void particle::update()
{
	//printf("particleMove: %f %f %f %f", pos.x, pos.y, damp, vel.x, vel.y);
    pos += vel;
    vel *= damp;
}

void particle::addForce( ofxVec2f frc )
{
    vel += frc;
}

void particle::setWidth(int _pWidth)
{
	pWidth = _pWidth;	
}

void particle::draw()
{
		
	//ofCircle(pos.x, pos.y, 3); // use for debug
	ofFill();
    ofRect(pos.x,pos.y, pWidth, pHeight);
}
