#pragma once

#include "BaseLuaRegistration.h"

struct lua_State;

/*	Erbt von BaseLuaRagistration.
*	S�mtliche Member der ObbjectLayer Klasse, die den Skripten offengelegt werden sollen werden
*	hier festgelegt und werden global an weitergegeben.
*/

namespace LuaRegistrations
{
	class ObjectLayerLuaRegistration : public BaseLuaRegistration
	{
	public:
		ObjectLayerLuaRegistration();								//	Konstruktor
		~ObjectLayerLuaRegistration();								//	Destruktor

		void registerToLua(lua_State* pLuaState) override;		//	Siehe Oberklasse

	};

	//	Zu �bergebene Funktionen
	int l_getPlayer(lua_State* pLuaState);
}
