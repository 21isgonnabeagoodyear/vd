//gcc -g -rdynamic *.c -o main `sdl-config --cflags --libs` `pkg-config --cflags --libs lua5.2`
#include <SDL/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define BT
#ifdef BT
#include <signal.h>
#include <execinfo.h>

//borrowed from stackoverflow
void handler(int sig) {
  void *array[10];
  size_t size;

  // get void*'s for all entries on the stack
  size = backtrace(array, 10);

  // print out all the frames to stderr
  fprintf(stderr, "Error: signal %d:\n", sig);
  backtrace_symbols_fd(array, size, 2);
  exit(1);
}


#endif


/*
#define SCREENW 1024
#define SCREENH 600
#define SCREENW 640
#define SCREENH 480
*/


//#include "level.c"
#include "level.h"
#include "world.h"




struct 
{
	char j;
	char u;
	char d;
	char l;
	char r;
	char a;

	char e;
}keystates = {0};

#include "scripting.h"//we need access to keystates


char quitnow = 0;

void parsekeys()
{
	//set keys to "are down" from "pushed"
	int i;
	for(i=0;i<7;i++)
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
			if(((SDL_KeyboardEvent*)&evt)->keysym.sym == SDLK_x)
				keystates.a = 1;//printf("r\n");

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
			if(((SDL_KeyboardEvent*)&evt)->keysym.sym == SDLK_x)
				keystates.a = 0;//printf("r\n");

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


void editmain(SDL_Surface *screen, world *curworld)
{

	lvl_level *workingon = &*curworld[0][0];
	int workingx = 0, workingy =0;
	SDL_Rect srect = {0,0,SCREENW,SCREENH};
//	printf("editor not implemented yet\n");
	int tiletoset = 0;
	char mode = 0;//0 = tiles, 1 = ents, 2 = delete ents
	static int lastid = 0;


	const int numlogics = 9;
	char logics[][10] = {"default", "shrubbery", "testent", "player", "critter", "critter2", "snowspawn", "gnome", "deer"};
	while(!quitnow)
	{
		parsekeys();
		char * alltheotherkeys = SDL_GetKeyState(NULL);
		if(keystates.e == 1)
			break;
		if(keystates.u == 1)
			workingy --;//workingon = &*curworld[workingx][++workingy];//tiletoset ++;
		if(keystates.d == 1)
			workingy ++;//workingon = &*curworld[workingx][--workingy];//tiletoset ++;
		if(keystates.r == 1)
			workingx ++;//workingon = &*curworld[++workingx][workingy];
		if(keystates.l == 1)
			workingx --;//workingon = &*curworld[--workingx][workingy];
		workingx = workingx <0?0:workingx>WORLDW?WORLDW:workingx;
		workingy = workingy <0?0:workingy>WORLDH?WORLDH:workingy;
		workingon = &((*curworld)[workingx][workingy]);
			//tiletoset --;
		if(alltheotherkeys[SDLK_0])
			tiletoset = 0;
		if(alltheotherkeys[SDLK_q])
			mode = 1;
		if(alltheotherkeys[SDLK_w])
			mode = 0;
		if(alltheotherkeys[SDLK_DELETE])
			mode = 2;
		if(alltheotherkeys[SDLK_EQUALS])
		{
			tiletoset ++;
			SDL_Delay(200);//keys are not framerate insensitive in edit mode
		}
		if(alltheotherkeys[SDLK_MINUS])
		{
			tiletoset = tiletoset>0?tiletoset-1:0;
			SDL_Delay(200);
		}
		int mousex, mousey;
		int buttons = SDL_GetMouseState(&mousex, &mousey);
		SDL_Rect r = {mousex-2, mousey-2, TILESIZE+4, TILESIZE+4};
		if(buttons & 0x1)
		{
			SDL_FillRect(screen, &r, 0xff0000);
			if(!mode)
				workingon->infront[mousex/TILESIZE][mousey/TILESIZE].tile = tiletoset;
			else
			{
				if(workingon->numinfrontents >= MAXENTS && mode == 1)
					printf("too many ents\n");
				else if(mode ==2)
				{
					int closest = 100000, closesti =0;
					int i;
					for(i=0;i<workingon->numinfrontents;i++)
						if(abs(workingon->infrontents[i].x-mousex) + abs(workingon->infrontents[i].y-mousey) < closest)
						{
							closest=abs(workingon->infrontents[i].x-mousex) + abs(workingon->infrontents[i].y-mousey);
							closesti=i;
						}
					if(workingon->numinfrontents >0)
					{
						workingon->numinfrontents --;
						workingon->infrontents[closesti] = workingon->infrontents[workingon->numinfrontents];
					}

				}
				else
				{
					//TODO:get logic from some clever place
					strcpy(workingon->infrontents[workingon->numinfrontents ].logic, /*"default"*/logics[tiletoset%numlogics]);

					workingon->infrontents[workingon->numinfrontents  ].frame = 5;
					workingon->infrontents[workingon->numinfrontents  ].x = alltheotherkeys[SDLK_LSHIFT]?TILESIZE*(mousex/TILESIZE):mousex;
					workingon->infrontents[workingon->numinfrontents++].y =  alltheotherkeys[SDLK_LSHIFT]?TILESIZE*(mousey/TILESIZE):mousey;
					workingon->infrontents[workingon->numinfrontents  ].id = clock()*10000+lastid++;
					printf("finns %d ents\n", workingon->numinfrontents);
				}
				//mode = 0;
				SDL_Delay(500);//keys are not framerate insensitive in edit mode
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
				SDL_Delay(500);//keys are not framerate insensitive in edit mode
			}
			//mode = 0;
		}
		else if(buttons & 0x02)
			if(workingon->infront[mousex/TILESIZE][mousey/TILESIZE].tile == 0)
				tiletoset = workingon->behind[mousex/TILESIZE][mousey/TILESIZE].tile;
			else
				tiletoset = workingon->infront[mousex/TILESIZE][mousey/TILESIZE].tile;

		else
		{
			;//SDL_FillRect(screen, &r, 0x000000);
		}

		if(!mode)
			ss_draw(ss_loadsheet(/*"lena_color.bmp"*/"tiles.bmp",100,100)/*0*//*FIXME:pulled this 0 out my ass, use the actual one from the level*/, screen, tiletoset, TILESIZE*(mousex/TILESIZE), TILESIZE*(mousey/TILESIZE));
		else if(mode == 2)
		{
			int closest = 100000, closesti =0;
			int i;
			for(i=0;i<workingon->numinfrontents;i++)
				if(abs(workingon->infrontents[i].x-mousex) + abs(workingon->infrontents[i].y-mousey) < closest)
				{
					closest=abs(workingon->infrontents[i].x-mousex) + abs(workingon->infrontents[i].y-mousey);
					closesti=i;//printf("herp %d\n",i);
				}
			SDL_Rect todelete = {workingon->infrontents[closesti].x, workingon->infrontents[closesti].y, 5, 5};
			SDL_FillRect(screen, &todelete, 0xff00ff);
		}
		else
			ss_draw(ss_loadsheet("previews.bmp",32,32)/*FIXME:pulled this 1 out my ass, use the actual one from the level*/, screen, tiletoset%numlogics, mousex, mousey);

		SDL_Flip(screen);
		SDL_FillRect(screen, &srect, 0x000000);


		if(alltheotherkeys[SDLK_i])
			lvl_drawsimple(workingon, screen, 0);
		else if(alltheotherkeys[SDLK_o])
			lvl_drawsimple(workingon, screen, 1);
		else if(!alltheotherkeys[SDLK_p])// || mode)
		{
			lvl_drawfull(workingon, screen);
		}
		else
		{
			lvl_drawsimple(workingon, screen, 0);
			lvl_drawsimple(workingon, screen, 1);
		}
		if(alltheotherkeys[SDLK_TAB])
		{
			SDL_Rect clear = {0,0,SCREENW, SCREENH};
			//	SDL_FillRect(screen, &clear, 0x000000);
			//TODO:draw ui here
			clear.w = WORLDW*3;
			clear.h = WORLDH*3;
			SDL_FillRect(screen, &clear, 0xFFFFFF);
			SDL_Rect levelpos = {workingx,workingy,3, 3};
			SDL_FillRect(screen, &levelpos, 0x000000);
		}


	}
	keystates.e = 0;
	world_save(curworld, "test.lvl");
}


int main(int argc, char *argv[])
{
#ifdef BT
	signal(SIGSEGV, handler);
	signal(SIGFPE, handler);
#endif

	int worldx =0, worldy =0;
	scripting_init((char*)&keystates, &worldx, &worldy);

	//ss_loadsheet("tiles.bmp", 32, 32);


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
	//lvl_level alevel;
	world everything;
	world_load(&everything, "test.lvl");
	//lvl_genrandom(&everything[0][0]);
	char recording = 0;
	long timenext =SDL_GetTicks();
	for(i=0;!quitnow;i++)
	{
	//	if(clock()*1000/CLOCKS_PER_SEC < timenext){
	//		continue;//busy loop
	//		printf("waiting for next frame %ld %ld\n", clock()*1000/CLOCKS_PER_SEC , timenext);}
		long ttimenext = SDL_GetTicks();
		if(timenext-ttimenext > 1)
			SDL_Delay(timenext-ttimenext);
		else if (!(i %500))
			printf("low framerate on 500th frame, took %d ms\n", 16-(int)(timenext-ttimenext));
		else if( 16-(int)(timenext-ttimenext) > 25)
			printf("really low framerate took %d ms\n",  16-(int)(timenext-ttimenext));
		timenext = SDL_GetTicks()+16;
		renderedframes ++;
		parsekeys();
		if(keystates.e)
			editmain(screen, &everything);
		SDL_FillRect(screen, &srect, 0x000000);

		lvl_thinkents(&everything[worldx][worldy]);
		lvl_drawfull(&everything[worldx][worldy], screen);
		//lvl_test(screen);
		SDL_Flip(screen);
		//	SDL_Delay(16);

		//screen recording
		char fna[] = "record/0000.bmp";
		char *fn = fna+7;
		if(keystates.d == 1)
		{
			printf("recording %d\n", recording);
			recording = !recording;

		}
		if(i%4 == 0 && argc > 1 || recording)
		{
			fn[0] = (i/(4*1000))%10 + '0';
			fn[1] = (i/(4*100))%10 + '0';
			fn[2] = (i/(4*10))%10 + '0';
			fn[3] = (i/(4*1))%10 + '0';
			SDL_SaveBMP(screen, fna);
			printf("saving %s\n", fna);
		}


	}
	stime = SDL_GetTicks() - stime;
	printf("%d frames in %d ms (%f fps)\n",(int)renderedframes, (int)stime, renderedframes/(float)stime *1000 );
	SDL_Quit();

}
