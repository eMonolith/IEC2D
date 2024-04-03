#define TOP 1
#define RIGHT 2
#define BOTTOM 3
#define LEFT 4
#define null 12345


#include <cmath>
#include "math.h"

bool walls = false;
bool GravityOn = true;
bool FrictionOn = true;

const int DELAY_AMOUNT = 0;

const double PI = 3.14159265;
const double DEGREES = PI/180;

           double GRAVITY = 0.00;  //0.01; 9.8 l+r g forces
           double AIR = 1.000; //0.999; //0.08; //1;Typical speeds of molecules in a gas are √(3RT/M).
const double FRICTION = 0.9; //0.2; F=-b*v    F=-6pie*a*n*v radius viscocity velocity
const double COEFFICIENT = 1;  //0.01; // 0.5;

const int SCREEN_WIDTH = 1280; //1024;
const int SCREEN_HEIGHT = 800; //768;
const int SCREEN_BPP =32;

Uint32 TIME;
//Particle constants
//Speed of a spawning particle
const int particleSpeed = 1;    //(v) = (2eV/m)^1/2  

const int PARTICLE_COUNT = 200000; //100000; //1000000;

      int SPAWN_RATE = 1;

      double ATTRACTION_POWER = 1;   //F=(((k*Q1*q[i])/r

      double RESIST_POWER = 1; //-0.90; 

double cosd (double x)
{
	return cos (x * DEGREES);
}

double sind (double x)
{
	return sin (x * DEGREES);
}

double arctand (double x)
{
	return atan (x)* 180/PI;
}
