#pragma once

#include "BaseLuaRegistration.h"

class Button;

/*	Beschreibung: siehe andere LuaRegistration
*/

namespace LuaRegistrations {

	class ButtonLuaRegistration : public BaseLuaRegistration
	{
	public:
		ButtonLuaRegistration();												//	Konstruktor
		~ButtonLuaRegistration();												//	Destruktor

		void registerToLua(lua_State* pLuaState) override;						//	Siehe Oberklasse
		static Button* checkAndGetButton(lua_State* pLuaState, int argNum);		//	Holt das Button Objekt aus den Argumenten (Ich weiß, der Name ist speziell...)
	};

	//	Zu übergebene Funktionen
	int l_ButtonGetInstance(lua_State* pLuaState);
	int l_ButtonSetPosition(lua_State* pLuaState);
	int l_ButtonGetSize(lua_State* pLuaState);
	int l_ButtonGetPosition(lua_State* pLuaState);
}