#pragma once

#include "BaseLuaRegistration.h"

struct lua_State;
class Player;

/*	Erbt von BaseLuaRagistration.
 *	Sämtliche Member der Player Klasse, die den Skripten offengelegt werden sollen werden 
 *	hier festgelegt und werden global an weitergegeben.
 */

namespace LuaRegistrations
{
	class PlayerLuaRegistration : public BaseLuaRegistration
	{
	public:
		PlayerLuaRegistration();												//	Konstruktor
		~PlayerLuaRegistration();												//	Destruktor

		void registerToLua(lua_State* pluaState) override;						//	Siehe Oberklasse
		static Player* checkAndGetPlayer(lua_State* pLuaState, int argNum);		//	Holt das Player Objekt aus den Argumenten (Ich weiß der Name ist speziell...)

	};

	//	Zu übergebene Funktionen
	int sayhiPlayer(lua_State* L);

	int l_playerInstance(lua_State* pLuaState);
	int l_playerMoveToPosition(lua_State* pLuaState);
	int l_playerMoveRelative(lua_State* pLuaState);
}
