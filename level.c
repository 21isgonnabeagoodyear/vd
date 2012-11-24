#include "level.h"

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
	static SDL_Surface *simpletex = NULL;//FIXME:do in load or init
	if(simpletex == NULL)
		simpletex = SDL_LoadBMP("lena_color.bmp");
	SDL_Rect tile;
	SDL_Rect position;
	//lvl_tile (*drawlvl)[TILESW][TILESH];
	lvl_tile (*drawlvl)[TILESH];
	if(layer ==0)
		//drawlvl = &(td->behind);
		drawlvl = &(td->behind[0]);
	else
		//drawlvl = &(td->infront);
		drawlvl = &(td->infront[0]);

	tile.w = TILESIZE;
	tile.h = TILESIZE;

	position.w = TILESIZE;
	position.h = TILESIZE;
	for(i=0;i<TILESW;i++)
	{
		for(j=0;j<TILESH;j++)
		{
		//	tile.x = TILESIZE*((*drawlvl)[i][j].tile % 10);
		//	tile.y = TILESIZE*(((*drawlvl)[i][j].tile % 100)/10);
			if(!drawlvl[i][j].tile)
				continue;
			tile.x = TILESIZE*(drawlvl[i][j].tile % 10);
			tile.y = TILESIZE*((drawlvl[i][j].tile % 100)/10);
			position.x = TILESIZE*i;
			position.y = TILESIZE*j;
			SDL_BlitSurface(simpletex,&tile,target,&position);
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
	for(i=0;i<tt->numbehindents;i++)
		ent_think(&tt->behindents[i]);

	for(i=0;i<tt->numinfrontents;i++)
		ent_think(&tt->infrontents[i]);
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
			td->behind[i][j].tile = i*2 + j;
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
	for(i=0;i<5;i++)
	{
		strcpy(td->behindents[i].logic, "testent");
		td->behindents[i].id = i*2;
		strcpy(td->infrontents[i].logic, "testent");
		td->infrontents[i].id = i*2+1;
	}
	td->numbehindents = 5;
	td->numinfrontents = 5;
}
