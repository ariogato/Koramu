#pragma once

#include "BaseLuaRegistration.h"

struct lua_State;

/*	Erbt von BaseLuaRagistration.
 *	Sämtliche Member der Player Klasse, die den Skripten offengelegt werden sollen werden 
 *	hier festgelegt und werden global an weitergegeben.
 */

namespace LuaRegistrations
{
	class PlayerLuaRegistration : public BaseLuaRegistration
	{
	public:
		PlayerLuaRegistration();								//	Konstruktor
		~PlayerLuaRegistration();								//	Destruktor

		void registerToLua(lua_State* pluaState) override;		//	Siehe Oberklasse

	};

	//	Zu übergebene Funktionen
	int sayhi(lua_State* L);
}
