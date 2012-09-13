
//level structures and functions and shit
#define TILESIZE 32
#define TILESW SCREENW/TILESIZE
#define TILESH SCREENH/TILESIZE 
#define MAXENTS 10



typedef struct
{
	short tileset;//negative can be special?
	unsigned short tile;
	int physics;

} lvl_tile;
typedef struct
{
	short type;
	int x;
	int y;
	int data;//store type dependant data here

} lvl_ent;
typedef struct lvl_level
{
	lvl_tile behind[TILESW][TILESH];//use an array?
	lvl_tile infront[TILESW][TILESH];
	lvl_ent behindents[MAXENTS];
	lvl_ent infrontents[MAXENTS];
	struct lvl_level *up;//TODO:figure out recursive definition
	struct lvl_level *down;
	struct lvl_level *left;
	struct lvl_level *right;
} lvl_level;



void lvl_drawsimple(lvl_level *td, SDL_Surface *target,int level)//level is whether to draw behind or infront
{
	int i,j;
	static SDL_Surface *simpletex = NULL;//FIXME:do in load or init
	if(simpletex == NULL)
		simpletex = SDL_LoadBMP("lena.bmp");
	SDL_Rect tile;
	SDL_Rect position;
	//lvl_tile (*drawlvl)[TILESW][TILESH];
	lvl_tile (*drawlvl)[TILESH];
	if(level ==0)
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




void lvl_test(SDL_Surface *screen)
{
	static char layer = 1;
	layer = !layer;//oh god what have i done
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
			if((i+ j) % 3)
				t.infront[i][j].tile = i + 10*j;
			else
				t.infront[i][j].tile = 0;
	}
//	lvl_drawsimple(&t,screen,1);
	lvl_drawsimple(&t,screen,layer);
}








