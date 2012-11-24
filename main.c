//gcc -g  main.c -o main `sdl-config --cflags --libs` `pkg-config --cflags --libs lua5.2`
#include <SDL/SDL.h>
#include <stdio.h>
#include <stdlib.h>

/*
#define SCREENW 1024
#define SCREENH 600
#define SCREENW 640
#define SCREENH 480
*/


//#include "level.c"
#include "level.h"
#include "scripting.h"




struct 
{
	char j;
	char u;
	char d;
	char l;
	char r;

	char e;
}keystates = {0};
char quitnow = 0;

void parsekeys()
{
	//set keys to "are down" from "pushed"
	int i;
	for(i=0;i<6;i++)
		if(((char*)&keystates)[i] ==1)
			((char*)&keystates)[i] =2;


	SDL_Event evt;
	while(SDL_PollEvent(&evt))
	{
		if(evt.type == SDL_KEYDOWN)
		{
			if(((SDL_KeyboardEvent*)&evt)->keysym.sym == SDLK_LEFT)
				keystates.l = 1;//printf("l\n");
			if(((SDL_KeyboardEvent*)&evt)->keysym.sym == SDLK_RIGHT)
				keystates.r = 1;//printf("r\n");
			if(((SDL_KeyboardEvent*)&evt)->keysym.sym == SDLK_UP)
				keystates.u = 1;//printf("l\n");
			if(((SDL_KeyboardEvent*)&evt)->keysym.sym == SDLK_DOWN)
				keystates.d = 1;//printf("r\n");
			if(((SDL_KeyboardEvent*)&evt)->keysym.sym == SDLK_z)
				keystates.j = 1;//printf("r\n");

			if(((SDL_KeyboardEvent*)&evt)->keysym.sym == SDLK_e)
				keystates.e = 1;//printf("r\n");

			if(((SDL_KeyboardEvent*)&evt)->keysym.sym == SDLK_ESCAPE)
			{
				quitnow = 1;
			//	SDL_Quit();
			//	exit(0);
			}
			/*
			if(((SDL_KeyboardEvent*)&evt)->keysym.sym == 'a')
			if(((SDL_KeyboardEvent*)&evt)->keysym.sym == 'a')
			if(((SDL_KeyboardEvent*)&evt)->keysym.sym == 'a')
			if(((SDL_KeyboardEvent*)&evt)->keysym.sym == 'a')
			if(((SDL_KeyboardEvent*)&evt)->keysym.sym == 'a')
			*/
		}
		else if(evt.type == SDL_KEYUP)
		{
			if(((SDL_KeyboardEvent*)&evt)->keysym.sym == SDLK_LEFT)
				keystates.l = 0;
			if(((SDL_KeyboardEvent*)&evt)->keysym.sym == SDLK_RIGHT)
				keystates.r = 0;
			if(((SDL_KeyboardEvent*)&evt)->keysym.sym == SDLK_UP)
				keystates.u = 0;//printf("l\n");
			if(((SDL_KeyboardEvent*)&evt)->keysym.sym == SDLK_DOWN)
				keystates.d = 0;//printf("r\n");
			if(((SDL_KeyboardEvent*)&evt)->keysym.sym == SDLK_z)
				keystates.j = 0;//printf("r\n");

			if(((SDL_KeyboardEvent*)&evt)->keysym.sym == SDLK_e)
				keystates.e = 0;
		}
		else if(evt.type == SDL_QUIT)
		{
			quitnow = 1;
		//	SDL_Quit();
		//	exit(0);
		}

	}

	//printf("key states r%dl%du%dd%dj%d\n", keystates.r,keystates.l,keystates.u,keystates.d,keystates.j);



}


void editmain(SDL_Surface *screen, lvl_level *workingon)
{
	SDL_Rect srect = {0,0,SCREENW,SCREENH};
//	printf("editor not implemented yet\n");
	int tiletoset = 0;
	char mode = 0;//0 = tiles, 1 = ents
	while(!quitnow)
	{
		parsekeys();
		char * alltheotherkeys = SDL_GetKeyState(NULL);
		if(keystates.e == 1)
			break;
		if(keystates.u == 1)
			tiletoset ++;
		if(keystates.d == 1)
			tiletoset --;
		if(alltheotherkeys[SDLK_0])
			tiletoset = 0;
		if(alltheotherkeys[SDLK_q])
			mode = 1;
		if(alltheotherkeys[SDLK_w])
			mode = 0;
		int mousex, mousey;
		int buttons = SDL_GetMouseState(&mousex, &mousey);
		SDL_Rect r = {mousex-1, mousey-1, TILESIZE+2, TILESIZE+2};
		if(buttons & 0x1)
		{
			SDL_FillRect(screen, &r, 0xff0000);
			if(!mode)
				workingon->infront[mousex/TILESIZE][mousey/TILESIZE].tile = tiletoset;
			else
			{
				if(workingon->numinfrontents >= MAXENTS)
					printf("too many ents\n");
				else
				{
					//TODO:get logic from some clever place
					strcpy(workingon->infrontents[workingon->numinfrontents ].logic, "default");

					workingon->infrontents[workingon->numinfrontents  ].frame = 5;
					workingon->infrontents[workingon->numinfrontents  ].x = mousex;
					workingon->infrontents[workingon->numinfrontents++].y = mousey;
					printf("finns %d ents\n", workingon->numinfrontents);
				}
				mode = 0;
			}
		}
		else if(buttons & 0x4)
		{
			SDL_FillRect(screen, &r, 0x00ff00);
			if(!mode)
				workingon->behind[mousex/TILESIZE][mousey/TILESIZE].tile = tiletoset;
				else
				{
					//TODO:get logic from some clever place
					strcpy(workingon->behindents[workingon->numbehindents ].logic, "default");

					workingon->behindents[workingon->numbehindents].frame = 5;
					workingon->behindents[workingon->numbehindents].x = mousex;
					workingon->behindents[workingon->numbehindents++].y = mousey;
					printf("finns %d ents\n", workingon->numbehindents);
				}
				mode = 0;
		}
		else
		{
			SDL_FillRect(screen, &r, 0x000000);
		}

		if(!mode)
			ss_draw(0/*FIXME:pulled this 0 out my ass, use the actual one from the level*/, screen, tiletoset, mousex, mousey);
		else
			ss_draw(1/*FIXME:pulled this 0 out my ass, use the actual one from the level*/, screen, tiletoset, mousex, mousey);
		
		SDL_Flip(screen);
		SDL_FillRect(screen, &srect, 0x000000);
		if(alltheotherkeys[SDLK_p] || mode)
		{
			lvl_drawfull(workingon, screen);
		}
		else if(alltheotherkeys[SDLK_i])
			lvl_drawsimple(workingon, screen, 0);
		else if(alltheotherkeys[SDLK_o])
			lvl_drawsimple(workingon, screen, 1);
		else
		{
			lvl_drawsimple(workingon, screen, 0);
			lvl_drawsimple(workingon, screen, 1);
		}
	}
	keystates.e = 0;
}


int main()
{
	scripting_init();


	SDL_Init(SDL_INIT_VIDEO);
	SDL_Surface *screen = SDL_SetVideoMode(SCREENW,SCREENH,32,0*SDL_FULLSCREEN);
	lvl_init(screen);
	SDL_Rect drect = {50,50,100,100};
	SDL_Rect srect = {0,0,SCREENW,SCREENH};

	//SDL_Surface *testbmp = SDL_LoadBMP("lena.bmp");
	SDL_Rect smallr = {200,200,50,50};

	unsigned long stime = SDL_GetTicks();
	int i;
	long renderedframes = 0;
	lvl_level alevel;
	lvl_genrandom(&alevel);
	for(i=0;!quitnow;i++)
	{
		renderedframes ++;
		parsekeys();
/*		drect.x ++;
		if(keystates.u)
			i -=10;
		if(keystates.d)
			i += 10;
		if(keystates.r)
			i *= 1.1;
		if(keystates.l)
			i /= 1.1;*/
		if(keystates.e)
			editmain(screen, &alevel);
		SDL_FillRect(screen, &srect, 0x000000);

		lvl_thinkents(&alevel);
		lvl_drawfull(&alevel, screen);
		//lvl_test(screen);
		SDL_Flip(screen);
	//	SDL_Delay(16);
	}
	stime = SDL_GetTicks() - stime;
	printf("%d frames in %d ms (%f fps)\n",(int)renderedframes, (int)stime, renderedframes/(float)stime *1000 );
	SDL_Quit();

}
