
#include "world.h"
#include <stdio.h>


void world_load(world *loadto, char *toloadfrom)
{
	FILE *input = fopen(toloadfrom, "r");
	if(input == NULL)
		return;
	fread(loadto, sizeof(world), 1, input);
/*
	int i, j;
	for(i=0;i<WORDLW; i++)
		for(j=0;j<WORDLH; j++)
	
				*loadto[i][j].neighbors[k]
*/
}
void world_save(world *tosave, char *tosaveto)
{
	FILE *input = fopen(tosaveto, "w");
	if(input == NULL)
		return;
	fwrite(tosave, sizeof(world), 1, input);
}
