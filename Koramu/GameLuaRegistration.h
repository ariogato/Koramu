#pragma once

#include "BaseLuaRegistration.h"

struct lua_State;

/*	Erbt von BaseLuaRagistration.
*	Sämtliche Member von TheGame, die den Skripten offengelegt werden sollen werden
*	hier festgelegt und werden global an Lua weitergegeben.
*/

namespace LuaRegistrations
{
	class GameLuaRegistration : public BaseLuaRegistration
	{
	public:
		GameLuaRegistration();									//	Konstruktor
		~GameLuaRegistration();									//	Destruktor

		void registerToLua(lua_State* pLuaState) override;		//	Siehe Oberklasse

	};

	//	Zu übergebene Funktionen
	int l_setGameOver(lua_State* pLuaState);
	int l_getMainQuestId(lua_State* pLuaState);
	int l_getPartQuestId(lua_State* pLuaState);
	int l_nextQuest(lua_State* pLuaState);
	int l_startDialog(lua_State* pLuaState);
	int l_startNarrator(lua_State* pLuaState);
}

