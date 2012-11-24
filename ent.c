#include "ent.h"
#include "scripting.h"

void ent_think(ent_entity *tothink)
{
	scripting_target(tothink);
	scripting_execute(tothink->logic, tothink->id);
}
void ent_draw(ent_entity *todraw, SDL_Surface *target)
{
	//SDL_Rect r = {todraw->x, todraw->y, 5,5};
	//SDL_FillRect(target, &r, 0xffffff);
	static ss_sheet replacethis;
	static int t=0;if(t==0){t=1;replacethis = ss_loadsheet("lena.bmp", 50, 50);}
	ss_draw(replacethis, target, /*37+ todraw->id*/todraw->frame, todraw->x, todraw->y);
}
