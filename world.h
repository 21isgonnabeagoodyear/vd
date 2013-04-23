#ifndef WORLD_H
#define WORLD_H
#include "level.h"

#define WORLDW 30
#define WORLDH 20

typedef lvl_level world[WORLDW][WORLDH];


void world_load(world *loadto, char *toloadfrom);
void world_save(world *tosave, char *tosaveto);


#endif
