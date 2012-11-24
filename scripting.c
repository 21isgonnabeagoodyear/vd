#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "ent.h"


static lua_State *vm;
static ent_entity *currentent;

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

static luaL_Reg hooks[] = 
{
	{"log", hook_log},
	{"move", hook_move},
	{NULL, NULL}
};





void scripting_init()
{
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
void scripting_target(ent_entity *tg)
{
	currentent = tg;
}
