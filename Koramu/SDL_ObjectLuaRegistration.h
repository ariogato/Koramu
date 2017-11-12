#pragma once

#include "BaseLuaRegistration.h"

class SDL_GameObject;

/*	Beschreibung: siehe andere LuaRegistration
 *	
 *	Diese Registration ist für alle Objekte, die nicht vom Typ Player oder NPC sind,
 *	also Bäume, etc. Lua erlaubt uns bei den Registrations keine Vererbung, deshalb müssen
 *	die Unterklassen wie Button usw auch registriert werden.
*/

namespace LuaRegistrations {

	class SDL_ObjectLuaRegistration : public BaseLuaRegistration
	{
	public:
		SDL_ObjectLuaRegistration();														//	Konstruktor
		~SDL_ObjectLuaRegistration();														//	Destruktor

		void registerToLua(lua_State* pLuaState) override;									//	Siehe Oberklasse
		static SDL_GameObject* checkAndGetObject(lua_State* pLuaState, int argNum);			//	Holt das SDL_GameObject aus den Argumenten (Ich weiß, der Name ist speziell...)
	};

	//	Zu übergebene Funktionen
	int l_SDL_GameObjectGetInstance(lua_State* pLuaState);
	int l_SDL_GameObjectSetPosition(lua_State* pLuaState);
	int l_SDL_GameObjectGetSize(lua_State* pLuaState);
	int l_SDL_GameObjectGetPosition(lua_State* pLuaState);
}