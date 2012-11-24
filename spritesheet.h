#ifndef SPRITESHEET_H
#include <SDL/SDL.h>

typedef int ss_sheet;


ss_sheet ss_loadsheet(char *filename, int w, int h);
void ss_draw(ss_sheet sheet, SDL_Surface *target, int sprite, int x, int y);

#endif
