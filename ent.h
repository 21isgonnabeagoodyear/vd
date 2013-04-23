#ifndef ENTITY_H
#define ENTITY_H
#include <SDL/SDL.h>
#include "spritesheet.h"

typedef struct
{
	int x;
	int y;
	char logic[10];//name of script function
	int id;
	ss_sheet spritesheet;//int?
	int frame;
} ent_entity;


//void ent_think(ent_entity *tothink);//XXX:merge with level.h?  we're going to need access to the level in scripting
void ent_draw(ent_entity *todraw, SDL_Surface *target);
#endif
