#include "ent.h"
#include "scripting.h"

void ent_think(ent_entity *tothink)
{
	scripting_target(tothink);
	scripting_execute(tothink->logic, tothink->id);
}
void ent_draw(ent_entity *todraw, SDL_Surface *target)
{
	SDL_Rect r = {todraw->x, todraw->y, 5,5};
	SDL_FillRect(target, &r, 0xffffff);

}
