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
		{"sayhi", sayhi},
		{nullptr, nullptr}
	};

	//	Die Metatabelle mit den zuvor gesetzten Werten wird an Lua �bergeben
	this->registerMetatable("Player", pLuaState, regs);
}

Player* PlayerLuaRegistration::checkAndGetPlayer(lua_State* pLuaState, int argNum)
{
	/*	Gibt die Referenz auf die userdata Variable (im Grunde die Referenz auf den Player) zur�ck
	 *	Dabei wird gleichzeitig gecheckt, ob die Metatabelle "luaL_Player" ist
	 */
	return *static_cast<Player**>(luaL_checkudata(pLuaState, argNum, "luaL_Player"));
}

int LuaRegistrations::sayhi(lua_State* L)
{
	//	Referenz auf den Player (nicht zu l�schen)
	Player* pPlayerInstance = PlayerLuaRegistration::checkAndGetPlayer(L, 1);

	std::cout	<< "You're colliding at... \n\tX: " << pPlayerInstance->getPosition().getX() << std::endl 
				<< "\tY: " << pPlayerInstance->getPosition().getY() << std::endl << std::endl;

	return 0;
}

int LuaRegistrations::l_playerInstance(lua_State* pLuaState)
{
	//	Eine neue Userdata Variable (Pointer in Lua) wird erstellt
	Player** pPlayerUserData = static_cast<Player**>(lua_newuserdata(pLuaState, sizeof(Player*))); 

	//	Die GameObjects vo, ObjectLayer werden geholt (Objekt nicht l�schen, siehe oben)
	std::vector<GameObject*>* pGameObjects = TheGame::Instance()->getCurrentState()->getCurrentMap()->getObjectLayer()->getGameObjects();

	//	Das Player Objekt finden
	std::vector<GameObject*>::iterator it = std::find_if((*pGameObjects).begin(), (*pGameObjects).end(), [](GameObject* pGameObject)
	{
		return dynamic_cast<Player*>(pGameObject);
	});

	//	Checken, ob ein Objekt gefundne wurde (falls nicht nil �bergeben)
	if (it == pGameObjects->end())
	{
		lua_pushnil(pLuaState);
		return 1;
	}

	//	Die Userdata Variable bef�llen
	*pPlayerUserData = dynamic_cast<Player*>(*it);

	//	Die Metatabelle - definiert in "registerToLua" - auf den Stack pushen (siehe BaseLuaRegistration f�r name der Metatabelle)
	luaL_getmetatable(pLuaState, "luaL_Player");

	//	Die Metatabelle f�r das User data Objekt vom Player (zweites Objekt auf dem Stack) setzen
	lua_setmetatable(pLuaState, -2);

	//	Der R�ckgabewert ist ein Zeiger auf den Spieler (oder nil)
	return 1;
}


