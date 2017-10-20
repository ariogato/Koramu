#pragma once

#include "BaseLuaRegistration.h"

struct lua_State;

/*	Erbt von BaseLuaRagistration.
*	S�mtliche Member von TheGame, die den Skripten offengelegt werden sollen werden
*	hier festgelegt und werden global an weitergegeben.
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

	//	Zu �bergebene Funktionen
	int l_setGameOver(lua_State* pLuaState);
}
