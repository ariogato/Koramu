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
	//	Referenz auf den Player (nicht zu löschen)
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

	//	Die GameObjects vo, ObjectLayer werden geholt (Objekt nicht löschen)
	std::vector<GameObject*>* pGameObjects = TheGame::Instance()->getCurrentState()->getCurrentMap()->getObjectLayer()->getGameObjects();

	//	Das entsprechende Objekt mit der id heraussuchen
	std::vector<GameObject*>::iterator it = std::find_if(pGameObjects->begin(), pGameObjects->end(), [uniqueId](GameObject* gO)
	{
		return gO->getUniqueId() == uniqueId;
	});

	//	Checken, ob das Objekt gefunden wurde
	if (it == pGameObjects->end())
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
	//	Referenz auf den Player (nicht zu löschen)
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
