#include "level.h"
#include "spritesheet.h"
#include <stdio.h>

static SDL_Surface *cachedstaticinfront = NULL;
static SDL_Surface *cachedstaticbehind = NULL;

void lvl_init(SDL_Surface *screen)
{
	SDL_PixelFormat *pif = screen->format;
	cachedstaticinfront = SDL_CreateRGBSurface( 0, SCREENW, SCREENH, pif->BitsPerPixel, pif->Rmask, pif->Gmask, pif->Bmask, pif->Amask );
	cachedstaticbehind = SDL_CreateRGBSurface( 0, SCREENW, SCREENH, pif->BitsPerPixel, pif->Rmask, pif->Gmask, pif->Bmask, pif->Amask );
	SDL_FillRect( cachedstaticbehind, NULL, 0 );
	SDL_FillRect( cachedstaticinfront, NULL, 0xff00ff );
	SDL_SetColorKey( cachedstaticinfront, SDL_SRCCOLORKEY, 0xff00ff);//we only need this for infront since behind will always be opaque
}



void lvl_drawsimple(lvl_level *td, SDL_Surface *target,int layer)//layer is whether to draw behind or infront
{
	int i,j;
	static ss_sheet tex = -1;//FIXME:do in load or init
	if(tex == -1)
		//tex = ss_loadsheet("lena_color.bmp", TILESIZE, TILESIZE);
		tex = ss_loadsheet("tiles.bmp", TILESIZE, TILESIZE);
	//lvl_tile (*drawlvl)[TILESW][TILESH];
	lvl_tile (*drawlvl)[TILESH];
	if(layer ==0)
		//drawlvl = &(td->behind);
		drawlvl = &(td->behind[0]);
	else
		//drawlvl = &(td->infront);
		drawlvl = &(td->infront[0]);

	//tile.w = TILESIZE;
	//tile.h = TILESIZE;

	//position.w = TILESIZE;
	//position.h = TILESIZE;
	for(i=0;i<TILESW;i++)
	{
		for(j=0;j<TILESH;j++)
		{
			if(!drawlvl[i][j].tile)
				continue;
		//	tile.x = TILESIZE*(drawlvl[i][j].tile % 10);
		//	tile.y = TILESIZE*((drawlvl[i][j].tile % 100)/10);
		//	position.x = TILESIZE*i;
		//	position.y = TILESIZE*j;
		//	SDL_BlitSurface(simpletex,&tile,target,&position);
			ss_draw(tex, target, drawlvl[i][j].tile,TILESIZE*i, TILESIZE*j);
		}

	}
}


void lvl_drawfull(lvl_level *td, SDL_Surface *screen)
{
	//SDL_BlitSurface(cachedstaticbehind,NULL,screen,NULL);
	lvl_drawsimple(td, screen, 0);
	int i;
	for(i=0;i<td->numbehindents;i++)
		ent_draw(&td->behindents[i], screen);

	//SDL_BlitSurface(cachedstaticinfront,NULL,screen,NULL);
	lvl_drawsimple(td, screen, 1);
	for(i=0;i<td->numinfrontents;i++)
		ent_draw(&td->infrontents[i], screen);
}

void lvl_thinkents(lvl_level *tt)
{
	int i;
	scripting_target(&tt->behindents[0], tt);
	scripting_execute("framestart", 0);
	for(i=0;i<tt->numbehindents;i++)
	{
		//ent_think(&tt->behindents[i]);
		scripting_target(&tt->behindents[i], tt);
		scripting_execute(tt->behindents[i].logic, tt->behindents[i].id);
	}

	for(i=0;i<tt->numinfrontents;i++)
		//ent_think(&tt->infrontents[i]);
	{
		scripting_target(&tt->infrontents[i], tt);
		scripting_execute(tt->infrontents[i].logic, tt->infrontents[i].id);
	}
}

/*
void lvl_test(SDL_Surface *screen)//generate a randomish level
{
	static char layer = 1;
	layer = !layer;//oh god what have i done
	static char  generatedcache = 0;
	int i,j;
	lvl_level t;
	for(i=0;i<TILESW;i++)
	{
		for(j=0;j<TILESH;j++)
			t.behind[i][j].tile = i*2 + j;
	}
//	lvl_drawsimple(&t,screen,0);

	for(i=0;i<TILESW;i++)
	{
		for(j=0;j<TILESH;j++)
			if(!((i+ j) % 3))
				t.infront[i][j].tile = i + 10*j;
			else
				t.infront[i][j].tile = 0;
	}
//	lvl_drawsimple(&t,screen,1);
	if(!generatedcache)
	{
		lvl_drawsimple(&t,cachedstaticbehind,0);
		lvl_drawsimple(&t,cachedstaticinfront,1);
		generatedcache = 1;
	}
	
	SDL_BlitSurface(layer?cachedstaticinfront:cachedstaticbehind,NULL,screen,NULL);

}*/
void lvl_genrandom(lvl_level *td)
{
	int i, j;
	for(i=0;i<TILESW;i++)
	{
		for(j=0;j<TILESH;j++)
			td->behind[i][j].tile = 8;//i*2 + j;
	}
//	lvl_drawsimple(&t,screen,0);

	for(i=0;i<TILESW;i++)
	{
		for(j=0;j<TILESH;j++)
			if(!((i+ j) % 3))
				td->infront[i][j].tile = i + 10*j;
			else
				td->infront[i][j].tile = 0;
	}
	for(i=0;i<20;i++)
	{
		strcpy(td->behindents[i].logic, "testent");
		td->behindents[i].id = i*2;
		strcpy(td->infrontents[i].logic, "testent");
		td->infrontents[i].id = i*2+1;

		td->infrontents[i].frame = i*2+1;
		td->behindents[i].frame = i*2;
	}
	td->numbehindents = 20;
	td->numinfrontents = 20;
}


/* fuck this shit
#define MAXLEVELSPERWORLD 100

static int searcharray(lvl_level *map[], int num, lvl_level *target)
{
	int i;
	for(i=0;i<num;i++)
		if(map[i] == target)
			return i;
	return -1;
}
static int write_recursive(FILE *target, lvl_level *current, lvl_level *map[], int *numwritten, int maxrecursion)
{
	if(current == NULL)return -1;
	int location = searcharray(map, *numwritten, current);
	if(location != -1)return location;
	if(maxrecursion <= 0 || *numwritten >= MAXLEVELSPERWORLD)return -1;


}

void lvl_writeworld(lvl_level *tw, char *filename)
{
	FILE *output = fopen(filename, "w");
	int numwritten = 0;
	lvl_level *map[MAXLEVELSPERWORLD];//array of pointers
	lvl_level *current = tw;

	int max;
	for(max=0;max<1000;max++)
	{
		//just do it recursively för fan
	}
}

lvl_level *lvl_loadworld(char *filename)
{



}
*/
