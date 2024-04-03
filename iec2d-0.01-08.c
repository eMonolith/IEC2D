/*
 * iec2d.cpp
 * 
 * 
 * Many features and some functions added by
 * eMonolith()
 * 
 * The focus of this program is to simulate in two dimensions, electron behaivor 
 * in a spherical electric field, focused at the center of the screen, much like 
 * an IEC Fusor.
 * Particle behavior in this program isn't as it should be in a real situation, 
 * due to ignorance of c programming algorithms.
 * 
 * ("I'd hope to make this a natural simulation scaled in time and size.")
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franiklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 */
#include <SDL.h> //"SDL.h"
#include <SDL_image.h> //"SDL/SDL_image.h"
#include <SDL_ttf.h> //"SDL/SDL_ttf.h"
#include <string.h>
#include <sstream>
#include "Particle2.h"
#include <math.h>
// particle and the particle that will respawn next
Particle part[PARTICLE_COUNT];
int respawn = 0;

double eq1 = 0.01; //0.1
double eq2 = 0.001;//0.01

// Wait for a quit
bool quit = false;

// Frame Rate Stuff
// frame number
int frame = 0;
int fps;
Uint32 start = 1;
Uint32 end = 1;

//timer stuff
Uint32 startTime = 0;
Uint32 endTime = 0;
Uint32 elapsedTime = 0;
// Surfaces
SDL_Surface *particle = NULL;
SDL_Surface *frameRate = NULL;
SDL_Surface *frameRate_message = NULL;
SDL_Surface *parcount = NULL;
SDL_Surface *parcount_message = NULL;
SDL_Surface *powerattrac = NULL;
SDL_Surface *powerattrac_message = NULL;
SDL_Surface *powerresist = NULL;
SDL_Surface *powerresist_message = NULL;
SDL_Surface *airfriction = NULL;
SDL_Surface *airfriction_message = NULL;
SDL_Surface *grav = NULL;
SDL_Surface *grav_message = NULL;
SDL_Surface *mtime = NULL;
SDL_Surface *mtime_message = NULL;
SDL_Surface *background = NULL;
SDL_Surface *screen = NULL;

// Event structure
SDL_Event event;

// Fonts
TTF_Font *font = NULL;
SDL_Color textColor = { 0, 255, 0 };
char textbuf0[20];
char textbuf1[20];
char textbuf2[20];
char textbuf3[20];
//char textbuf4[20];
//char textbuf5[20];
//char textbuf6[20];
//char textbuf7[20];

void get_timer()
{
        
        endTime = startTime;
        startTime = SDL_GetTicks();
        //Elapsed time
        elapsedTime = ((endTime - startTime));

}

void get_fps()
{
	frame++;
	if (frame % 100 == 0)
	{
		frame = 0;
		end = start;
		start = SDL_GetTicks();
		fps = 100000.0 / (start - end);
	}
}

void initiate_Particles()
{
	for (int i = 0; i < PARTICLE_COUNT; i++)
	{
		part[i].setNull();
	}
}

bool init(std::string title)
{
	// Start up SDL
	if (SDL_Init(SDL_INIT_VIDEO) == -1)
	{
		return false;
	}

	// Set up screen SWSURFACE?
//	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_HWSURFACE);
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
//	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_FULLSCREEN);

	// Check of screen if initialization worked
	if (screen == NULL)
	{
	return false;
	}

	if (TTF_Init () == -1)
	 {
	return false;
	}

	// Set title for window
	SDL_WM_SetCaption(title.c_str(), NULL);
	SDL_WM_SetIcon(SDL_LoadBMP("iec2d-icon1.bmp"), NULL);
	return true;
}

SDL_Surface *load_image(std::string filename)
{
	// The image that's loaded 
	SDL_Surface *loadedImage = NULL;

	// The optimized image that will be used 
	SDL_Surface *optimizedImage = NULL;

	// Load the image using SDL_image 
	loadedImage = IMG_Load(filename.c_str());

	// If the image loaded 
	if (loadedImage != NULL)
	{
		// Create an optimized image 
		optimizedImage = SDL_DisplayFormatAlpha(loadedImage);

		// Free the old image 
		SDL_FreeSurface(loadedImage);
	}

	// Return the optimized image 
	return optimizedImage;
}

void load_files()
{
	// Load images
	particle = load_image("./GameData/pixmaps/Pixel-e.png");
	background = load_image("./GameData/pixmaps/background1.png");

	// Load fonts
	//font = TTF_OpenFont("/usr/share/fonts/truetype/freefont/FreeMono.ttf", 12);
    //font = TTF_OpenFont("/usr/share/fonts/truetype/freefont/.ttf", 12);
    //font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/UbuntuMono-R.ttf", 10);
      font = TTF_OpenFont("./GameData/fonts/UbuntuMono-R.ttf", 12);
//    font = TTF_OpenFont("./GameData/fonts/DejaVuSerif.ttf", 10);

}

void apply_surface(int x, int y, SDL_Surface * source, SDL_Surface * destination)
{
	// Make a temporary rectangle to hold the offsets 
	SDL_Rect offset;

	// Give the offsets to the rectangle 
	offset.x = x;
	offset.y = y;

	// Blit the surface 
	SDL_BlitSurface(source, NULL, destination, &offset);
}

void draw_graphics()
{
	// Draw background
	apply_surface(0, 0, background, screen);

        //Draw the text
        frameRate_message = TTF_RenderText_Solid( font, "FPS", textColor );
        parcount_message = TTF_RenderText_Solid( font, "parcount", textColor );
        powerattrac_message = TTF_RenderText_Solid( font, "power", textColor );
        powerresist_message = TTF_RenderText_Solid( font, "repel", textColor );
        airfriction_message = TTF_RenderText_Solid( font, "air friction", textColor );
        grav_message = TTF_RenderText_Solid( font, "gravity", textColor );       //particlespeed next too?
        mtime_message = TTF_RenderText_Solid( font, "nano seconds", textColor );
        //If there was an error in rendering the text
//        if( parcount_message == NULL )
//        {
//        return 1;
//        }

	// Cast the frame rate to a string so it can be drawn
	std::stringstream fpsString;
	fpsString << fps;
	// Draw frame rate
	frameRate = TTF_RenderText_Solid (font, fpsString.str().c_str(), textColor);
//sprintf(textbuf0,"fps");
	apply_surface (6, 5,frameRate,screen);

        // Draw frameRate_message
        apply_surface (6,15+1,frameRate_message,screen);
 
	// Draw parcount
        std::stringstream parcountString;
	parcountString << respawn;
        parcount = TTF_RenderText_Solid (font, parcountString.str().c_str(), textColor);
        apply_surface (6,25+1,parcount,screen);

        // Draw parcount_message
//        parcount_message = TTF_RenderText_Solid( font, "parcount", textColor );
        apply_surface (6,33+1,parcount_message,screen);

        // Draw RESIST_POWER
        std::stringstream resistString;
	resistString << RESIST_POWER;
        powerresist = TTF_RenderText_Solid (font, resistString.str().c_str(), textColor);
        apply_surface (6,45+1,powerresist,screen);

        // Draw powerresist_message
        apply_surface (6,53+1,powerresist_message,screen);  //53

        // Draw ATTRACTION_POWER
        std::stringstream powerString;
	powerString << ATTRACTION_POWER;
        powerattrac = TTF_RenderText_Solid (font, powerString.str().c_str(), textColor);
        apply_surface (6,65+1,powerattrac,screen);

        // Draw powerattrac_message
        apply_surface (6,73+1,powerattrac_message,screen);  //53

        // Draw air friction
        std::stringstream airString;
	airString << AIR;
        airfriction = TTF_RenderText_Solid (font, airString.str().c_str(), textColor);
        apply_surface (6,85+1,airfriction,screen);

        //Draw airfriction_message
        apply_surface (6,93+1,airfriction_message,screen);  //75

	// Draw GRAVITY
        std::stringstream gravityString;
	gravityString << GRAVITY;
        grav = TTF_RenderText_Solid (font, gravityString.str().c_str(), textColor);
        apply_surface (6,105+1,grav,screen);

        // Draw grav_message
        apply_surface (6,113+1,grav_message,screen);

	// Draw TIME
        std::stringstream timeString;
	timeString << SDL_GetTicks() - startTime;
        mtime = TTF_RenderText_Solid (font, timeString.str().c_str(), textColor);
        apply_surface (6,125+1,mtime,screen);

        // Draw time_message
        apply_surface (6,138+1,mtime_message,screen); 


	// Draw the particles
	for (int i = 0; i < PARTICLE_COUNT; i++)
	{
		if (!part[i].isNull())
		{
			apply_surface(part[i].getx(), part[i].gety(), particle, screen);
	} //repell(part[i].getx(), part[i].gety());	}//add repulsion power to particles?
	}

	// Update the screen 
	SDL_Flip(screen);
}

void clean_up()
{
	// Clean up
	SDL_FreeSurface(background);
	SDL_FreeSurface(particle);
	SDL_FreeSurface(parcount);
	SDL_FreeSurface(frameRate);
	SDL_FreeSurface(airfriction);
	SDL_FreeSurface(powerattrac);
	SDL_FreeSurface(powerresist);
	SDL_FreeSurface(grav);
	SDL_FreeSurface(mtime);

	TTF_CloseFont (font);
	 TTF_Quit ();

	SDL_Quit();
}


void get_input()
{
	// Get events
	while (SDL_PollEvent(&event))
	{
		// Has the user exited?
		if (event.type == SDL_QUIT)
		{
			quit = true;
		}
	}


	/*****************************Key Input************************/

 Uint8 *keystate = SDL_GetKeyState(NULL); 


	int pax = (SCREEN_WIDTH / 2);	//center of screen/ATTRACTION_POWER
	int pay = (SCREEN_HEIGHT / 2);

	if (keystate[SDLK_RETURN])  //if (keystate[SDLK_RETURN])  if (keystate[SDL_BUTTON(1)])
	{
		// spawn particles
		for (int i = 0; i < SPAWN_RATE; i++)
		for (int p = 0; p < 359; p++)		//360 degrees
		{
			// loop back to first particle if too many are spawned
			if (respawn == PARTICLE_COUNT)
			{
				respawn = 0; //respawn = 0;
			}

			// spawn the particle at its position
			double direction = (p);
			//double direction = (sin((p*3.14) / 360));
		    //double direction = (p*3.14);
            //double direction = (part[i].getAngle() + 10 %360);

			// change direction to x and y velocity
			double yvel = (particleSpeed*ATTRACTION_POWER)* cosd(direction);  //  double xvel = particleSpeed * cosd(direction);  //  x1 = r * cos(angle * PI / 180);
			double xvel = (particleSpeed*ATTRACTION_POWER)* sind(direction);  //  double yvel = particleSpeed * sind(direction);  //  y1 = r * sin(angle * PI / 180);
			// actually create particle
 part[respawn].respawnParticle(pax, pay, xvel, yvel); 

			// go to next particle
			respawn=respawn+1; //respawn++;
		}
	}

	//Uint8 *keystate = SDL_GetKeyState(NULL);
// keyboard buttons that are pressed

	if (!keystate[SDLK_SPACE])
	{
		for (int i = 0; i < PARTICLE_COUNT; i++)
		{
			part[i].resist(pax,pay);
			part[i].attract(pax, pay);
		}
	}

    if (keystate[SDLK_q]) {
        RESIST_POWER = RESIST_POWER + 1; }
    if (keystate[SDLK_w]) {
        RESIST_POWER = RESIST_POWER + 0.0001; }
    if (keystate[SDLK_e]) {
        RESIST_POWER = 1; } //
    if (keystate[SDLK_r]) {
        RESIST_POWER = RESIST_POWER - 0.0001; } //MORE 
    if (keystate[SDLK_t]) {
        RESIST_POWER = RESIST_POWER - 1; }

    if (keystate[SDLK_y]) {
        ATTRACTION_POWER = ATTRACTION_POWER + 0.1; }
    if (keystate[SDLK_u]) {
        ATTRACTION_POWER = ATTRACTION_POWER + 0.0001; }
    if (keystate[SDLK_i]) {
        ATTRACTION_POWER = 1; } //
    if (keystate[SDLK_o]) {
        ATTRACTION_POWER = ATTRACTION_POWER - 0.0001; } //MORE 
    if (keystate[SDLK_p]) {
        ATTRACTION_POWER = ATTRACTION_POWER - 0.1; }

    if (keystate[SDLK_g]) {
        AIR = AIR + eq1;}  //AIR + 0.1; } //MORE VAC!
if (keystate[SDLK_h]) {
        AIR = AIR + eq2; } //AIR + 0.001; } //MORE VAC!
    if (keystate[SDLK_j]) {
        AIR = AIR - AIR + 1;}  //MORE
if (keystate[SDLK_k]) {
        AIR = AIR - eq2; } //AIR - 0.001; } //LESS VAC!
    if (keystate[SDLK_l]) {
        AIR = AIR - eq1;}  //AIR - 0.1; } //LESS VAC!

    if (keystate[SDLK_b]) {
        GRAVITY = GRAVITY + 0.001; } //MORE GRAVITY

    if (keystate[SDLK_n]) {
        GRAVITY = GRAVITY - GRAVITY; } //NULL GRAVITY

    if (keystate[SDLK_m]) {
        GRAVITY = GRAVITY - 0.001; } //LESS GRAVITY


    if (keystate[SDLK_s]) {
        screen  = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_FULLSCREEN); } //screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_FULLSCREEN);
    if (keystate[SDLK_d]) {
        screen  = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE); } //screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_HWSURFACE);

    if (keystate[SDLK_z]) {
        parcount = respawn-10; } //NULL PARTICLE_COUNT //
        
    if (keystate[SDLK_x]) {
    	quit = true; }
    
}

void physics()
{
	// Update particle positions
	for (int i = 0; i < PARTICLE_COUNT; i++)
//{     for(int j = 0; j <= PARTICLE_COUNT; j++){
//if (i != j)
	{
		part[i].updatePosition();
	}//}}
}

int main(int argc, char *args[])
{

	printf("\nIEC2D by eMonolith\n");
	printf("\nKEYBOARD CONTROL KEYS:\ns d > toggle fullscreen\nspace bar > deactivate center mass attraction power\nenter > spawn particles\nq w e r t > +/1/- repel\ny u i o p > +/1/- power\ng h j k l > +/1/- air friction\nb n m > +/0/- gravity\n");

	// Start up SDL
	init("IEC2D v0.01-08");

	// Load stuff
	load_files();

	// Start particles at null
	initiate_Particles();


	// Main loop begin
	while (!quit)
	{
         //Elapsed time
        get_timer();

		// Calculate fps
		get_fps();

		// Get input
		get_input();

		// Calculate particle Physics
		physics();

		// Render
		draw_graphics();
		SDL_Delay(DELAY_AMOUNT);
	}

	// SDL_Delay (2000);

	clean_up();

	return 0;
}
