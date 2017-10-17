#include "GameLuaRegistration.h"
#include <lua.hpp>
#include "Game.h"

using namespace LuaRegistrations;

GameLuaRegistration::GameLuaRegistration()
{
}

GameLuaRegistration::~GameLuaRegistration()
{
}

void GameLuaRegistration::registerToLua(lua_State* pLuaState)
{
	luaL_Reg regs[] = 
	{
		{"setGameOver", l_setGameOver},
		{nullptr, nullptr}
	};

	this->registerMetatable("TheGame", pLuaState, regs);
}

int LuaRegistrations::l_setGameOver(lua_State* pLuaState)
{
	//	Game Over setzen
	TheGame::Instance()->setGameOver();

	//	Es gibt keinen Rückgabewert
	return 0;
}
