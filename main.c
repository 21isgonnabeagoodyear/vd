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
}keystates;
char quitnow = 0;

void parsekeys()
{
	//set keys to "are down" from "pushed"
	int i;
	for(i=0;i<5;i++)
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




int main()
{
	scripting_init();


	SDL_Init(SDL_INIT_VIDEO);
	SDL_Surface *screen = SDL_SetVideoMode(SCREENW,SCREENH,32,0*SDL_FULLSCREEN);
	lvl_init(screen);
	SDL_Rect drect = {50,50,100,100};
	SDL_Rect srect = {0,0,SCREENW,SCREENH};

	SDL_Surface *testbmp = SDL_LoadBMP("lena.bmp");
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
		drect.x ++;
		if(keystates.u)
			i -=10;
		if(keystates.d)
			i += 10;
		if(keystates.r)
			i *= 1.1;
		if(keystates.l)
			i /= 1.1;
		SDL_FillRect(screen, &srect, 0x000000);
		//lvl_test(screen);
		SDL_FillRect(screen, &drect, 0xff00ff);
		srand(0);
		int j;
		SDL_Rect trect;
		/*for(j=0;j<500;j++)
		{
			smallr.x = (rand()*512.f)/RAND_MAX + (rand()/(float)RAND_MAX -0.5) * i;
			smallr.y = (rand()*512.f)/RAND_MAX + (rand()/(float)RAND_MAX -0.5) * i;



			smallr.w = 50+(rand()*50.f)/RAND_MAX + 0.2*(rand()/(float)RAND_MAX -0.5) * i;
			smallr.h = 50+(rand()*50.f)/RAND_MAX + 0.2*(rand()/(float)RAND_MAX -0.5) * i;

			trect.x = (rand()*640.f)/RAND_MAX + (rand()*3.2/RAND_MAX -1.5) * i;
			trect.y = (rand()*480.f)/RAND_MAX + (rand()*3.2/RAND_MAX -1.5) * i;
			trect.w=20;
			trect.h=20;
			int col=rand();
			//SDL_FillRect(screen, &trect, col);
			SDL_BlitSurface(testbmp, &smallr,screen,&trect);
			//printf("col %x %d %d\n",col, trect.x, trect.y);

		}*/
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
