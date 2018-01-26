#include "NPCLuaRegistration.h"
#include <lua.hpp>
#include <algorithm>
#include "GameObject.h"
#include "Game.h"
#include "NPC.h"

LuaRegistrations::NPCLuaRegistration::NPCLuaRegistration()
{
	
}

LuaRegistrations::NPCLuaRegistration::~NPCLuaRegistration()
{
	
}

void LuaRegistrations::NPCLuaRegistration::registerToLua(lua_State* pLuaState)
{
	//	Funktionen etc. werden hier definiert
	luaL_Reg regs[] =
	{
		{ "getInstance", l_getNPCInstance },
		{ "sayhi", sayhiNPC },
		{ "setCurrentCol", l_setCurrentCol },
		{ "moveToPosition", l_NPCMoveToPosition },
		{ "moveRelative", l_NPCMoveRelative },
		{ "stun", l_NPCStun },
		{ nullptr, nullptr }
	};

	//	Die Metatabelle mit den zuvor gesetzten Werten wird an Lua übergeben
	this->registerMetatable("NPC", pLuaState, regs);
}

NPC* LuaRegistrations::NPCLuaRegistration::checkAndGetNPC(lua_State* pLuaState, int argNum)
{
	/*	Gibt die Referenz auf die userdata Variable (im Grunde die Referenz auf den NPC) zurück
	*	Dabei wird gleichzeitig gecheckt, ob die Metatabelle "luaL_NPC" ist
	*/
	return *static_cast<NPC**>(luaL_checkudata(pLuaState, argNum, "luaL_NPC"));
}

int LuaRegistrations::sayhiNPC(lua_State* L)
{
	//	Referenz auf den NPC (nicht zu löschen)
	NPC* pNPCInstance = NPCLuaRegistration::checkAndGetNPC(L, 1);

	std::cout << "NPC " << pNPCInstance->getUniqueId() << " says hi!" << std::endl;

	return 0;
}

int LuaRegistrations::l_getNPCInstance(lua_State* pLuaState)
{
	//	Das übergebene Argument (id des NPC) poppen
	const char* uniqueId = luaL_checkstring(pLuaState, 1);
	//	Checken, ob das übergebene Argument ein string ist (wenn nicht wäre nullptr übergeben worden)
	if (!uniqueId)
	{
		TheGame::Instance()->logError() << "LuaRegistrations::l_getNPCInstance():\n\tArgument ist kein string." << std::endl << std::endl;
		lua_pushnil(pLuaState);
		return 1;
	}

	printStack(pLuaState);
	//	Die GameObjects des ObjectLayers werden geholt (Objekt nicht löschen)
	std::vector<GameObject*> gameObjects = TheGame::Instance()->getCurrentState()->getAllObjects();

	//	Das entsprechende Objekt mit der id heraussuchen
	std::vector<GameObject*>::iterator it = std::find_if(gameObjects.begin(), gameObjects.end(), [uniqueId](GameObject* gO)
	{
		return gO->getUniqueId() == uniqueId;
	});

	//	Checken, ob das Objekt gefunden wurde
	if (it == gameObjects.end())
	{
		TheGame::Instance()->logError() << "LuaRegistrations::l_getNPCInstance():\n\tKein Objekt mit der ID: " << uniqueId << std::endl << std::endl;
		lua_pushnil(pLuaState);
		return 1;
	}

	//	Eine neue Userdata Variable (Pointer in Lua) wird erstellt
	NPC** pNPCUserData = static_cast<NPC**>(lua_newuserdata(pLuaState, sizeof(NPC*)));

	//	Die userdata Variable befüllen
	*pNPCUserData = dynamic_cast<NPC*>(*it);

	//	Checken, ob das Objekt ein NPC ist
	if (!(*pNPCUserData))
	{
		//	Die userdata Variable vom Stack poppen
		lua_pop(pLuaState, 1);

		TheGame::Instance()->logError() << "LuaRegistrations::l_getNPCInstance():\n\tObjekt mit der ID: " << uniqueId << " ist kein NPC." << std::endl << std::endl;
		lua_pushnil(pLuaState);
		return 1;
	}

	//	Die Metatabelle - definiert in "registerToLua" - auf den Stack pushen (siehe BaseLuaRegistration für name der Metatabelle)
	luaL_getmetatable(pLuaState, "luaL_NPC");

	//	Die Metatabelle für das User data Objekt vom NPC (zweites Objekt auf dem Stack) setzen
	lua_setmetatable(pLuaState, -2);

	//	Der Pointer auf den NPC wird übergeben
	return 1;
}

int LuaRegistrations::l_setCurrentCol(lua_State* pLuaState)
{
	//	Referenz auf den NPC (nicht zu löschen)
	NPC* pNPCInstance = NPCLuaRegistration::checkAndGetNPC(pLuaState, 1);

	//	Checken, ob ein Integer übergeben wurde
	if (!lua_isinteger(pLuaState, -1))
	{
		return 0;
	}

	//	Das Argument vom Stack holen
	int col = lua_tointeger(pLuaState, -1);

	//	Die currentRow setzen
	pNPCInstance->setCurrentCol(col);

	//	Es gibt keinen Rückgabewert
	return 0;
}

int LuaRegistrations::l_NPCMoveToPosition(lua_State* pLuaState)
{
	//	Die Anzahl der Argumente muss 3 sein (x, y) + userdata
	if (lua_gettop(pLuaState) < 3)
	{
		lua_settop(pLuaState, 0);
		return 0;
	}

	//	Referenz auf den NPC aus den Argumenten holen (nicht zu löschen)
	NPC* pNPCInstance = NPCLuaRegistration::checkAndGetNPC(pLuaState, 1);

	//	Einen Vektor mit den Argumenten erstellen
	Vector2D v(lua_tointeger(pLuaState, -2), lua_tointeger(pLuaState, -1));

	//	Die Funktion des NPC aufrufen
	pNPCInstance->moveToPosition(v);

	//	Es gibt keinen Rückgabewert
	return 0;
}

int LuaRegistrations::l_NPCMoveRelative(lua_State* pLuaState)
{
	//	Die Anzahl der Argumente muss 3 sein (x, y) + userdata
	if (lua_gettop(pLuaState) < 3)
	{
		lua_settop(pLuaState, 0);
		return 0;
	}

	//	Referenz auf den NPC aus den Argumenten holen (nicht zu löschen)
	NPC* pNPCInstance = NPCLuaRegistration::checkAndGetNPC(pLuaState, 1);

	//	Einen Vektor mit den Argumenten erstellen
	Vector2D v(lua_tointeger(pLuaState, -2), lua_tointeger(pLuaState, -1));

	//	Die Funktion des NPC aufrufen
	pNPCInstance->moveRelative(v);

	//	Es gibt keinen Rückgabewert
	return 0;
}

int LuaRegistrations::l_NPCStun(lua_State* pLuaState)
{
	//	Referenz auf den NPC aus den Argumenten holen (nicht zu löschen)
	NPC* pNPCInstance = NPCLuaRegistration::checkAndGetNPC(pLuaState, 1);

	//	Das Argument der Zeit vom Stack holen
	float sec = luaL_checknumber(pLuaState, 2);

	//	Den NPC stunnen
	pNPCInstance->stun(sec);

	//	Es gibt keinen Rückgabewert
	return 0;
}
