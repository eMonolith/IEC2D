//Random starting velocity
//xvel = (rand () % particleSpeed) + 1;
//yvel = (rand () % particleSpeed) + 1;
#include "WorldConstants.h"

class Particle
{
	private:
	//Location and trajectory
	int x, y;
	int x1, y1, x2, y2;
	double xvel, yvel, mass, charge;
	public:
	
	//apply the velocity to the position
	void updatePosition ();
	
	//apply particle to screen
	void show ();
	
	//In screen area?
	int inBounds  (int X , int Y);
	
	//spawn a new one
	void respawnParticle (int X, int Y, double Xvel, double Yvel);
	
	//attract particles to center
	void attract (int pax, int pay); //center x and y
	
        //repell particles from particles
        void repell (int x, int y); //particle x and y

	//set to null
	void setNull ();
	
	//is null?
	bool isNull ();
	
	//Get angle
	//Get angle between particle and center
	
	//get location
	int getx ();
	int gety ();

	//get angle
	//double Particle::getAngle (int x1, int y1, int x2, int y2);
	
};

int Particle::getx ()
{
	return x;
}

int Particle::gety ()
{
	return y;
}

bool Particle::isNull ()
{
	if (x == null)
		return true;
	else
		return false;
}

void Particle::setNull ()
{
	x = null;
	y = null;
	
	xvel = null;
	yvel = null;
}

void Particle::respawnParticle (int X, int Y, double Xvel, double Yvel)
{
	x = X;
	y = Y;
	
	xvel = Xvel;
	yvel = Yvel;
}

int Particle::inBounds (int  X, int Y)
{
	if (X == null)
		return null;
	//Past the right wall
	if (X > SCREEN_WIDTH)
	{
		return 2;
	}
	
	//below the ground
	if (Y > SCREEN_HEIGHT)
	{
		return 3;
	}
	
	//past left
	if (X < 0)
	{
		return 4;
	}
	
	//above
	if (Y < 0)
	{
		return 1;
	} 
	
	//Completely in bounds
	return 0;
}

void Particle::updatePosition ()
{
	if (x != null && walls) //If walls are on and particle is spawned
	{
		if (GravityOn)
			if (y < SCREEN_HEIGHT - 10)
			{
				yvel += GRAVITY;
			}
		if (y > SCREEN_HEIGHT - 10)
		{
			y = SCREEN_HEIGHT - 10;
			yvel = -yvel * COEFFICIENT;
		}
		if (x > SCREEN_WIDTH)
		{
			x = 1;
		}
		if (x <= 0)
		{
			x = SCREEN_WIDTH - 1;
		}
		
		if (FrictionOn)
		{
			xvel *= particleSpeed/AIR;  //xvel *= -1/AIR;    //PV=nRT  P=nRT/V  V=nRT/P  T=PV/AIR AIR=nR?
			yvel *= particleSpeed/AIR;  //yvel *= -1/AIR    // xvel *= AIR ; yvel *= AIR;
		}
		
		x += round (xvel);
		y += round (yvel);
	}
	
	if (x != null && !walls) //If walls are off
	{
		if (GravityOn)
			yvel += GRAVITY;
		
		if (FrictionOn)
		{
			xvel *= particleSpeed/AIR;  //PV=nRT  P=nRT/V  V=nRT/P  T=PV/AIR AIR=nR?
			yvel *= particleSpeed/AIR;  // xvel *= AIR ; yvel *= AIR;
		}
		x += round (xvel);
		y += round (yvel);
	}
}


//Finds angle between two points
double getAngle (int x1, int y1, int x2, int y2) 
{
	double ax = x2 - x1;
	double ay = y2 - y1;

/*	if (ax == 0 && ay == 0)
		return 0.0;
	else if (ax == 0 && ay > 0)
		return 90.0;
	else if (ax == 0 && ay < 0)
		return 270.0;
	else if (ay == 0 && ax > 0)
		return 0.0;
	else if (ay == 0 && ax < 0)
		return 180.0;
	else if (ax > 0 && ay > 0)
		return arctand (ay/ax);
	else if (ax < 0 && ay > 0)
		return 180.0 + arctand (ay/ax);
	else if (ax > 0 && ay < 0)
		return 360.0 + arctand (ay/ax);
	else if (ax < 0 && ay < 0)
		return 180.0 + arctand (ay/ax);
	else

		return 0;
*/
	return atan2(ay,ax)*180/M_PI;//return atan2(ay,ax)*180/M_PI;

	
}

void Particle::attract (int cx, int cy)
{
	if (!isNull ())
	{
		double cAngle = getAngle (x,y, cx, cy);
		// e = a*sin(2*pie*f*d) 
		xvel += 2*cosd(cAngle*ATTRACTION_POWER);
		yvel += 2*sind(cAngle*ATTRACTION_POWER);
	}
}

void Particle::repell (int X, int Y)
{
	if (!isNull ())
	{
		double cAngle = getAngle (x, y, X, Y);
		// e = a*sin(2*pie*f*d) 
		xvel += 2*cosd(cAngle) * (REPELL_POWER);    //  2*cosd(cAngle) * (-0.9 / ATTRACTION_POWER);
		yvel += 2*sind(cAngle) * (REPELL_POWER);
	}
}
