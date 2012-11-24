#ifndef LEVEL_H
#include <SDL/SDL.h>
#include "ent.h"

//level structures and functions and shit

#define SCREENW 800
//#define SCREENH 300
#define SCREENH 320


#define TILESIZE 32
#define TILESW SCREENW/TILESIZE
#define TILESH SCREENH/TILESIZE 
#define MAXENTS 50



typedef struct
{
	short tileset;//negative can be special?
	unsigned short tile;
	int physics;

} lvl_tile;
/*typedef struct
{
	short type;
	int x;
	int y;
	int data;//store type dependant data here

} lvl_ent;*/
typedef struct lvl_level
{
	lvl_tile behind[TILESW][TILESH];//use an array?
	lvl_tile infront[TILESW][TILESH];
	ent_entity behindents[MAXENTS];
	ent_entity infrontents[MAXENTS];
	int numbehindents;
	int numinfrontents;
	struct lvl_level *up;//TODO:figure out recursive definition
	struct lvl_level *down;
	struct lvl_level *left;
	struct lvl_level *right;
} lvl_level;


//functions
void lvl_init(SDL_Surface *screen);
void lvl_drawsimple(lvl_level *td, SDL_Surface *target,int layer);//layer is whether to draw behind or infront

void lvl_thinkents(lvl_level *tt);
void lvl_drawfull(lvl_level *td, SDL_Surface *screen);


void lvl_genrandom(lvl_level *td);
#endif
