#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "ent.h"
#include "level.h"
#include "world.h"//only for worldw, worldh now should we keep a pointer to this?


static lua_State *vm;
static ent_entity *currentent;
static lvl_level *currentlevel;
static char *keysREMOVETHIS;
static int *levelxREMOVETHIS;
static int *levelyREMOVETHIS;

//hooks
static int hook_log(lua_State *L)//function hooks.log(message, priority)
{
	const char *message = lua_tostring(L, -2);
	int level = lua_tointeger(L, -1);
	printf("priority: %d scripting message: %s\n", level, message);
	return 0;
}

static int hook_move(lua_State *L)//function hooks.move(x,y)
{
	if(currentent != NULL)
	{
		currentent->x = lua_tointeger(L,-2);
		currentent->y = lua_tointeger(L,-1);
	}
	return 0;
}
static int hook_frame(lua_State *L)//function hooks.move(x,y)
{
	if(currentent != NULL)
	{
		currentent->frame = lua_tointeger(L,-1);
	}
	return 0;
}
static int hook_terrainat(lua_State *L)
{
	if(currentlevel != NULL)
	{
		int testx = lua_tointeger(L,-2)/TILESIZE;
		int testy = lua_tointeger(L,-1)/TILESIZE;
		lua_pushinteger(L, currentlevel->infront[testx][testy].tile);

		return 1;
	}
	return 0;
}
static int hook_position(lua_State *L)
{
	if(currentent != NULL)
	{
		lua_pushinteger(L, currentent->x);
		lua_pushinteger(L, currentent->y);

		return 2;
	}
	return 0;
}
static int boxcollide(x1,y1,w1,h1,x2,y2,w2,h2)
{
	if(x1+w1 < x2)
		return 0;
	if(x1 > x2+w2)
		return 0;
	if(y1+h1 < y2)
		return 0;
	if(y1 > y2+h2)
		return 0;
	return 1;
}
static int hook_nudge(lua_State *L)//hooks.nudge(x,y,w,h,dx,dy)->rx,ry
{
	if(currentlevel != NULL && currentent != NULL)
	{
		int x  = lua_tointeger(L, -6);
		int y  = lua_tointeger(L, -5);
		int w  = lua_tointeger(L, -4);
		int h  = lua_tointeger(L, -3);
		int dx = lua_tointeger(L, -2);
		int dy = lua_tointeger(L, -1);

/*
		int tdx=dx, tdy=dy;
		for(;dx !=0;dx -= dx/abs(dx))
			if(currentlevel->infront[(x+dx)/TILESIZE][y/TILESIZE].tile != 0)
				break;
		for(;dy !=0;dy -= dy/abs(dy))
			if(currentlevel->infront[(x+dx)/TILESIZE][(y+dy)/TILESIZE].tile != 0)
				break;
		lua_pushinteger(L, x+tdx-dx);
		lua_pushinteger(L, y+tdy-dy);
*/
/*		else if(currentlevel->infront[tilex][tiley].tile &&   boxcollide(rx+ dx/abs(dx), y, w,h,   TILESIZE*tilex, TILESIZE*tiley, TILESIZE, TILESIZE))
			dy --;
		else if(currentlevel->infront[tilex+1][tiley].tile &&   boxcollide(rx+ dx/abs(dx), y, w,h,   TILESIZE*(tilex+1), TILESIZE*tiley, TILESIZE, TILESIZE))
			dy --;
		else if(currentlevel->infront[tilex+1][tiley+1].tile&&boxcollide(rx+ dx/abs(dx), y, w,h,   TILESIZE*(tilex+1), TILESIZE*(tiley+1), TILESIZE, TILESIZE))
			dy --;
		else if(currentlevel->infront[tilex][tiley+1].tile &&   boxcollide(rx+ dx/abs(dx), y, w,h,   TILESIZE*tilex, TILESIZE*(tiley+1), TILESIZE, TILESIZE))
			dy --;
*/
		int rx, ry, i;
		for(rx = x; rx != x+dx && dx != 0;rx += dx/abs(dx))
			//for(i=0;i<)
		{


			int tilex = (rx + dx/abs(dx))/TILESIZE;
			int tiley = y/TILESIZE;

			//if(currentlevel->infront[(rx + dx/abs(dx))/TILESIZE][y/TILESIZE].tile != 0   || currentlevel->infront[(rx + dx/abs(dx)+w)/TILESIZE][(y+h)/TILESIZE].tile != 0)
			if(currentlevel->infront[tilex][tiley].tile &&   boxcollide(rx+ dx/abs(dx), y, w,h,   TILESIZE*tilex, TILESIZE*tiley, TILESIZE, TILESIZE))
				break;
			if(currentlevel->infront[tilex+1][tiley].tile &&   boxcollide(rx+ dx/abs(dx), y, w,h,   TILESIZE*(tilex+1), TILESIZE*tiley, TILESIZE, TILESIZE))
				break;
			if(currentlevel->infront[tilex+1][tiley+1].tile&&boxcollide(rx+ dx/abs(dx), y, w,h,   TILESIZE*(tilex+1), TILESIZE*(tiley+1), TILESIZE, TILESIZE))
				break;
			if(currentlevel->infront[tilex][tiley+1].tile &&   boxcollide(rx+ dx/abs(dx), y, w,h,   TILESIZE*tilex, TILESIZE*(tiley+1), TILESIZE, TILESIZE))
				break;
		}
		/*		for(ry = y; ry != y+dy && dy != 0;ry += dy/abs(dy))
				if(currentlevel->infront[(rx)/TILESIZE][(ry+dy/abs(dy))/TILESIZE].tile != 0   || currentlevel->infront[(rx+w)/TILESIZE][(ry+dy/abs(dy)+h)/TILESIZE].tile != 0)
				break;
				*/
		for(ry = y; ry != y+dy && dy != 0;ry += dy/abs(dy))
			//for(i=0;i<)
		{


			int tilex = rx/TILESIZE;
			int tiley = (ry+dy/abs(dy))/TILESIZE;

			//if(currentlevel->infront[(rx + dx/abs(dx))/TILESIZE][y/TILESIZE].tile != 0   || currentlevel->infront[(rx + dx/abs(dx)+w)/TILESIZE][(y+h)/TILESIZE].tile != 0)
			if(currentlevel->infront[tilex][tiley].tile &&   boxcollide(rx, ry+dy/abs(dy), w,h,   TILESIZE*tilex, TILESIZE*tiley, TILESIZE, TILESIZE))
				break;
			if(currentlevel->infront[tilex+1][tiley].tile &&   boxcollide(rx, ry+dy/abs(dy), w,h,   TILESIZE*(tilex+1), TILESIZE*tiley, TILESIZE, TILESIZE))
				break;
			if(currentlevel->infront[tilex+1][tiley+1].tile &&   boxcollide(rx, ry+dy/abs(dy), w,h,   TILESIZE*(tilex+1), TILESIZE*(tiley+1), TILESIZE, TILESIZE))
				break;
			if(currentlevel->infront[tilex][tiley+1].tile &&   boxcollide(rx, ry+dy/abs(dy), w,h,   TILESIZE*tilex, TILESIZE*(tiley+1), TILESIZE, TILESIZE))
				break;
		}
		//if(rx==x&&ry==y)ry--;

		lua_pushinteger(L, rx);
		lua_pushinteger(L, ry);

	}
	return 2;
}
static int hook_warp(lua_State *L)
{
	//printf("need to implement this\n");
	*levelxREMOVETHIS += lua_tointeger(L, -2) % WORLDW;
	*levelyREMOVETHIS += lua_tointeger(L, -1) % WORLDH;
	return 0;
}
static int hook_spawn(lua_State *L)//hook.spawn(name,x,y)->id
{
	//printf("need to implement this\n");
	static int lastspawnedent = 0;
	if(lastspawnedent ==0)lastspawnedent=time(NULL);
	if(currentlevel != NULL)
	{
		if(currentlevel->numinfrontents >= MAXENTS-1)
		{
			printf("too many ents\n");
			return 0;
		}
		strncpy(currentlevel->infrontents[currentlevel->numinfrontents].logic, lua_tostring(L, -3), 10);
		currentlevel->infrontents[currentlevel->numinfrontents].x = lua_tointeger(L, -2);
		currentlevel->infrontents[currentlevel->numinfrontents].y = lua_tointeger(L, -1);
		currentlevel->infrontents[currentlevel->numinfrontents].id = lastspawnedent++;//FIXME
		lua_pushinteger(L, currentlevel->infrontents[currentlevel->numinfrontents].id);
		currentlevel->numinfrontents ++;
		//TODO:generate id
	}
	return 1;
}
static int hook_die(lua_State *L)
{
	*currentent = currentlevel->infrontents[--currentlevel->numinfrontents]; //FIXME:this ent will get skipped this cycle

	return 0;
}

static int hook_keys(lua_State *L)
{
	lua_pushinteger(L, keysREMOVETHIS[0]);//keystates.j);
	lua_pushinteger(L, keysREMOVETHIS[1]);//keystates.j);
	lua_pushinteger(L, keysREMOVETHIS[2]);//keystates.j);
	lua_pushinteger(L, keysREMOVETHIS[3]);//keystates.j);
	lua_pushinteger(L, keysREMOVETHIS[4]);//keystates.j);
	lua_pushinteger(L, keysREMOVETHIS[5]);//keystates.j);
	//	lua_pushinteger(L, keystates.u);
	//	lua_pushinteger(L, keystates.d);
	//	lua_pushinteger(L, keystates.l);
	//	lua_pushinteger(L, keystates.r);
	return 6;
}
static int hook_rand(lua_State *L)
{
	lua_pushinteger(L, rand()%100);
	return 1;
}
static luaL_Reg hooks[] = 
{
	{"log", hook_log},
	{"move", hook_move},
	{"frame", hook_frame},
	{"terrainat", hook_terrainat},
	{"position", hook_position},
	{"keys", hook_keys},
	{"nudge", hook_nudge},
	{"warp", hook_warp},
	{"spawn", hook_spawn},
	{"die", hook_die},
	{"rand", hook_rand},
	{NULL, NULL}
};





void scripting_init(char *keysFIXTHIS, int *levelxFIXTHIS, int *levelyFIXTHIS)
{
	keysREMOVETHIS = keysFIXTHIS;
	levelxREMOVETHIS = levelxFIXTHIS;
	levelyREMOVETHIS = levelyFIXTHIS;
	FILE *luascript = fopen("main.lua", "r");
	char someluashit[100000];
	someluashit[fread(someluashit, 1, 100000, luascript)] = '\0';
	vm = luaL_newstate();
	luaL_openlibs(vm);//XXX:playing with fire
	luaL_newlib(vm, hooks);
	lua_setglobal(vm, "hooks");


	luaL_loadbuffer(vm, someluashit, strlen(someluashit), "luamain");//load our file
	if(lua_pcall(vm, 0, 0, 0) != LUA_OK)//run our file (initialization)
		printf("error loading lua: %s\n", lua_tostring(vm, -1));
}
void scripting_execute(char *function, int data)
{
	lua_settop(vm, 0);
	lua_getglobal(vm, function);
	lua_pushinteger(vm, data);
	if(lua_pcall(vm, 1, 0, 0) != LUA_OK)
		printf("scripting error, calling %s failed: %s\n", function, lua_tostring(vm, -1));
}
void scripting_target(ent_entity *tg, lvl_level *tgl)
{
	currentent = tg;
	currentlevel = tgl;
}
