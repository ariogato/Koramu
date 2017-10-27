#pragma once

#include "BaseLuaRegistration.h"

class NPC;

/*	Beschreibung: siehe andere LuaRegistration
 */

namespace LuaRegistrations {

	class NPCLuaRegistration : public BaseLuaRegistration
	{
	public:
		NPCLuaRegistration();												//	Konstruktor
		~NPCLuaRegistration();												//	Destruktor

		void registerToLua(lua_State* pLuaState) override;					//	Siehe Oberklasse
		static NPC* checkAndGetNPC(lua_State* pLuaState, int argNum);		//	Holt das NPC Objekt aus den Argumenten (Ich wei�, der Name ist speziell...)
	};

	//	Zu �bergebene Funktionen
	int sayhiNPC(lua_State* L);
	int l_getNPCInstance(lua_State* pLuaState);
	int l_setCurrentCol(lua_State* pLuaState);
	int l_NPCMoveToPosition(lua_State* pLuaState);
	int l_NPCMoveRelative(lua_State* pLuaState);
	int l_NPCStun(lua_State* pLuaState);
}