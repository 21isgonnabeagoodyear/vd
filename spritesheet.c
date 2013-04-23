#include "spritesheet.h"

#define MAX_SHEETS 10

static struct
{
	SDL_Surface *image;
	SDL_Rect size;
	int numacross;
	int numupdown;
	char name[10];
} sheets[MAX_SHEETS] = {{0}};

ss_sheet ss_loadsheet(char *filename, int w, int h)
{
	int i;
	for(i=0;i<MAX_SHEETS;i++)
		if(strncmp(sheets[i].name, filename, 10) ==0)//FIXME:multiple sizes on a single sheet?
			return i;
	for(i=0;i<MAX_SHEETS && sheets[i].image != NULL;i++);
	if(i == MAX_SHEETS-1)printf("ran out of image slots, will probably crash now (FIXME:keep track of loaded filenames)");
	sheets[i].image = SDL_LoadBMP(filename);

	printf("set color key %s\n", filename);
	SDL_SetColorKey( sheets[i].image, SDL_SRCCOLORKEY, 0x00ff00ff);

	if(sheets[i].image == NULL || sheets[i].image->w == 0)
	{
		printf("ERROR:failed to open image %s\n", filename);
		return;
	}
	sheets[i].size.w = w;// = {0,0,w,h};
	sheets[i].size.h = h;// = {0,0,w,h};
	sheets[i].numacross = sheets[i].image->w/w;
	sheets[i].numupdown = sheets[i].image->h/h;
	strncpy(sheets[i].name, filename, 10);
	return i;
}
void ss_draw(ss_sheet sheet, SDL_Surface *target, int sprite, int x, int y)
{
	sheets[sheet].size.x = sheets[sheet].size.w * (sprite % sheets[sheet].numacross);
	sheets[sheet].size.y = sheets[sheet].size.h * (sprite / sheets[sheet].numacross);
	SDL_Rect destpos = {x,y,0,0};

	//SDL_SetColorKey( sheets[sheet].image, SDL_SRCCOLORKEY, 0xff00ff);
/*	int cc;
	SDL_GetColorKey(sheets[sheet].image, &cc);
	printf("%hd\n", cc);*///what the fuck
	SDL_BlitSurface(sheets[sheet].image,&sheets[sheet].size,target,&destpos);
}
