#include "GameLuaRegistration.h"
#include <lua.hpp>
#include "Game.h"
#include "Story.h"

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
		{ "setGameOver", l_setGameOver },
		{ "getMainQuestId", l_getMainQuestId },
		{ "getPartQuestId", l_getPartQuestId },
		{ "nextQuest", l_nextQuest },
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

int LuaRegistrations::l_getMainQuestId(lua_State* pLuaState)
{
	//	Die Id auf den Stack pushen
	lua_pushstring(pLuaState, TheGame::Instance()->getStory()->getMainQuest().c_str());

	//	Es gibt einen Rückgabewert (main quest id)
	return 1;
}

int LuaRegistrations::l_getPartQuestId(lua_State* pLuaState)
{
	//	Die Id auf den Stack pushen
	lua_pushstring(pLuaState, TheGame::Instance()->getStory()->getPartQuest().c_str());

	//	Es gibt einen Rückgabewert (part quest id)
	return 1;
}

int LuaRegistrations::l_nextQuest(lua_State* pLuaState)
{
	//	Die nächste Quest einleiten
	TheGame::Instance()->getStory()->nextQuest();

	//	Es gibt keinen Rückgabewert
	return 0;
}
