#include "spritesheet.h"

#define MAX_SHEETS 10

static struct
{
	SDL_Surface *image;
	SDL_Rect size;
	int numacross;
	int numupdown;
} sheets[MAX_SHEETS] = {{0}};

ss_sheet ss_loadsheet(char *filename, int w, int h)
{
	int i;
	for(i=0;i<MAX_SHEETS && sheets[i].image != NULL;i++);
	if(i == MAX_SHEETS-1)printf("ran out of image slots, will probably crash now (FIXME:keep track of loaded filenames)");
	sheets[i].image = SDL_LoadBMP(filename);
	sheets[i].size.w = w;// = {0,0,w,h};
	sheets[i].size.h = h;// = {0,0,w,h};
	sheets[i].numacross = sheets[i].image->w/w;
	sheets[i].numupdown = sheets[i].image->h/h;
	return i;
}
void ss_draw(ss_sheet sheet, SDL_Surface *target, int sprite, int x, int y)
{
	sheets[sheet].size.x = sheets[sheet].size.w * (sprite % sheets[sheet].numacross);
	sheets[sheet].size.y = sheets[sheet].size.h * (sprite / sheets[sheet].numacross);
	SDL_Rect destpos = {x,y,0,0};

	SDL_BlitSurface(sheets[sheet].image,&sheets[sheet].size,target,&destpos);
}
