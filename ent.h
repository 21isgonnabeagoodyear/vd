#ifndef ENTITY_H
#include <SDL/SDL.h>

typedef struct
{
	int x;
	int y;
	char logic[10];//name of script function
	int id;
	int spritesheet;//int?
	int frame;
} ent_entity;


void ent_think(ent_entity *tothink);
void ent_draw(ent_entity *todraw, SDL_Surface *target);
#endif
