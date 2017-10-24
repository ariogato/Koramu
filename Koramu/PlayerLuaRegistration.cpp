#include "PlayerLuaRegistration.h"
#include <lua.hpp>
#include <algorithm>
#include <iostream>
#include "Player.h"
#include "Game.h"
#include "GameObject.h"

using namespace LuaRegistrations;

PlayerLuaRegistration::PlayerLuaRegistration()
{
	
}

PlayerLuaRegistration::~PlayerLuaRegistration()
{
	
}

void PlayerLuaRegistration::registerToLua(lua_State* pLuaState)
{
	//	Funktionen etc. werden hier definiert
	luaL_Reg regs[] =
	{
		{ "getInstance", l_playerInstance },
		{ "sayhi", sayhiPlayer },
		{ "moveToPosition", l_playerMoveToPosition },
		{ "moveRelative", l_playerMoveRelative },
		{ "stun", l_PlayerStun },
		{nullptr, nullptr}
	};

	//	Die Metatabelle mit den zuvor gesetzten Werten wird an Lua übergeben
	this->registerMetatable("Player", pLuaState, regs);
}

Player* PlayerLuaRegistration::checkAndGetPlayer(lua_State* pLuaState, int argNum)
{
	/*	Gibt die Referenz auf die userdata Variable (im Grunde die Referenz auf den Player) zurück
	 *	Dabei wird gleichzeitig gecheckt, ob die Metatabelle "luaL_Player" ist
	 */
	return *static_cast<Player**>(luaL_checkudata(pLuaState, argNum, "luaL_Player"));
}

int LuaRegistrations::sayhiPlayer(lua_State* L)
{
	//	Referenz auf den Player (nicht zu löschen)
	Player* pPlayerInstance = PlayerLuaRegistration::checkAndGetPlayer(L, 1);

	std::cout << "Player " << pPlayerInstance->getUniqueId() << " says hi!" << std::endl;

	return 0;
}

int LuaRegistrations::l_playerInstance(lua_State* pLuaState)
{
	//	Eine neue Userdata Variable (Pointer in Lua) wird erstellt
	Player** pPlayerUserData = static_cast<Player**>(lua_newuserdata(pLuaState, sizeof(Player*))); 

	//	Die GameObjects vo, ObjectLayer werden geholt (Objekt nicht löschen, siehe oben)
	std::vector<GameObject*>* pGameObjects = TheGame::Instance()->getCurrentState()->getCurrentMap()->getObjectLayer()->getGameObjects();

	//	Das Player Objekt finden
	std::vector<GameObject*>::iterator it = std::find_if((*pGameObjects).begin(), (*pGameObjects).end(), [](GameObject* pGameObject)
	{
		return dynamic_cast<Player*>(pGameObject);
	});

	//	Checken, ob ein Objekt gefundne wurde (falls nicht nil übergeben)
	if (it == pGameObjects->end())
	{
		lua_pushnil(pLuaState);
		return 1;
	}

	//	Die Userdata Variable befüllen
	*pPlayerUserData = dynamic_cast<Player*>(*it);

	//	Die Metatabelle - definiert in "registerToLua" - auf den Stack pushen (siehe BaseLuaRegistration für name der Metatabelle)
	luaL_getmetatable(pLuaState, "luaL_Player");

	//	Die Metatabelle für das User data Objekt vom Player (zweites Objekt auf dem Stack) setzen
	lua_setmetatable(pLuaState, -2);

	//	Der Rückgabewert ist ein Zeiger auf den Spieler (oder nil)
	return 1;
}

int LuaRegistrations::l_playerMoveToPosition(lua_State* pLuaState)
{
	//	Die Anzahl der Argumente muss 3 sein (x, y) + userdata
	if (lua_gettop(pLuaState) < 3)
	{
		lua_settop(pLuaState, 0);
		return 0;
	}

	//	Referenz auf den Player aus den Argumenten holen (nicht zu löschen)
	Player* pPlayerInstance = PlayerLuaRegistration::checkAndGetPlayer(pLuaState, 1);

	//	Einen Vektor mit den Argumenten erstellen
	Vector2D v(lua_tointeger(pLuaState, -2), lua_tointeger(pLuaState, -1));

	//	Die Funktion des Players aufrufen
	pPlayerInstance->moveToPosition(v);

	//	Es gibt keinen Rückgabewert
	return 0;
}

int LuaRegistrations::l_playerMoveRelative(lua_State* pLuaState)
{
	//	Die Anzahl der Argumente muss 3 sein (x, y) + userdata
	if (lua_gettop(pLuaState) < 3)
	{
		lua_settop(pLuaState, 0);
		return 0;
	}

	//	Referenz auf den Player aus den Argumenten holen (nicht zu löschen)
	Player* pPlayerInstance = PlayerLuaRegistration::checkAndGetPlayer(pLuaState, 1);

	//	Einen Vektor mit den Argumenten erstellen
	Vector2D v(lua_tointeger(pLuaState, -2), lua_tointeger(pLuaState, -1));

	//	Die Funktion des Players aufrufen
	pPlayerInstance->moveRelative(v);

	//	Es gibt keinen Rückgabewert
	return 0;
}

int LuaRegistrations::l_PlayerStun(lua_State* pLuaState)
{
	//	Referenz auf den Player aus den Argumenten holen (nicht zu löschen)
	Player* pPlayerInstance = PlayerLuaRegistration::checkAndGetPlayer(pLuaState, 1);

	//	Das Argument der Zeit vom Stack holen
	int sec = lua_tointeger(pLuaState, 2);

	//	Den Player stunnen
	pPlayerInstance->stun(sec);

	//	Es gibt keinen Rückgabewert
	return 0;
}


