#include "ButtonLuaRegistration.h"
#include <lua.hpp>
#include <algorithm>
#include "Game.h"
#include "Button.h"

LuaRegistrations::ButtonLuaRegistration::ButtonLuaRegistration()
{
}

LuaRegistrations::ButtonLuaRegistration::~ButtonLuaRegistration()
{
}

void LuaRegistrations::ButtonLuaRegistration::registerToLua(lua_State* pLuaState)
{
	//	Funktionen etc. werden hier definiert
	luaL_Reg regs[] =
	{
		{ "getInstance", l_ButtonGetInstance },
		{ "setPosition", l_ButtonSetPosition },
		{ "getSize", l_ButtonGetSize },
		{ "getPosition", l_ButtonGetPosition },
		{ "setSize", l_ButtonSetSize },
		{ nullptr, nullptr }
	};

	//	Die Metatabelle mit den zuvor gesetzten Werten wird an Lua übergeben
	this->registerMetatable("Button", pLuaState, regs);
}

Button* LuaRegistrations::ButtonLuaRegistration::checkAndGetButton(lua_State* pLuaState, int argNum)
{
	/*	Gibt die Referenz auf die userdata Variable (im Grunde die Referenz auf den Button) zurück
	*	Dabei wird gleichzeitig gecheckt, ob die Metatabelle "luaL_Button" ist
	*/
	return *static_cast<Button**>(luaL_checkudata(pLuaState, argNum, "luaL_Button"));
}

int LuaRegistrations::l_ButtonGetInstance(lua_State* pLuaState)
{
	//	Das übergebene Argument (id des Button) poppen
	const char* uniqueId = luaL_checkstring(pLuaState, 1);
	//	Checken, ob das übergebene Argument ein string ist (wenn nicht wäre nullptr übergeben worden)
	if (!uniqueId)
	{
		TheGame::Instance()->logError() << "LuaRegistrations::l_getButtonInstance():\n\tArgument ist kein string." << std::endl << std::endl;
		lua_pushnil(pLuaState);
		return 1;
	}

	//	Die GameObjects des ObjectLayers werden geholt (Objekt nicht löschen)
	std::vector<GameObject*>* pGameObjects = TheGame::Instance()->getCurrentState()->getCurrentMap()->getObjectLayer()->getGameObjects();

	//	Das entsprechende Objekt mit der id heraussuchen
	std::vector<GameObject*>::iterator it = std::find_if(pGameObjects->begin(), pGameObjects->end(), [uniqueId](GameObject* gO)
	{
		return gO->getUniqueId() == uniqueId;
	});

	//	Checken, ob das Objekt gefunden wurde
	if (it == pGameObjects->end())
	{
		TheGame::Instance()->logError() << "LuaRegistrations::l_getButtonInstance():\n\tKein Objekt mit der ID: " << uniqueId << std::endl << std::endl;
		lua_pushnil(pLuaState);
		return 1;
	}

	//	Eine neue Userdata Variable (Pointer in Lua) wird erstellt
	Button** pButtonUserData = static_cast<Button**>(lua_newuserdata(pLuaState, sizeof(Button*)));

	//	Die userdata Variable befüllen
	*pButtonUserData = dynamic_cast<Button*>(*it);

	//	Checken, ob das Objekt ein Button ist
	if (!(*pButtonUserData))
	{
		//	Die userdata Variable vom Stack poppen
		lua_pop(pLuaState, 1);

		TheGame::Instance()->logError() << "LuaRegistrations::l_getButtonInstance():\n\tObjekt mit der ID: " << uniqueId << " ist kein Button." << std::endl << std::endl;
		lua_pushnil(pLuaState);
		return 1;
	}

	//	Die Metatabelle - definiert in "registerToLua" - auf den Stack pushen (siehe BaseLuaRegistration für name der Metatabelle)
	luaL_getmetatable(pLuaState, "luaL_Button");

	//	Die Metatabelle für das User data Objekt vom Button (zweites Objekt auf dem Stack) setzen
	lua_setmetatable(pLuaState, -2);

	//	Der Pointer auf den Button wird übergeben
	return 1;
}

int LuaRegistrations::l_ButtonSetPosition(lua_State* pLuaState)
{
	//	Die Referenz auf den Button aus den Argumenten holen
	Button* pButton = ButtonLuaRegistration::checkAndGetButton(pLuaState, 1);

	//	Einen Vektor mit den Argumenten erstellen
	Vector2D v(lua_tonumber(pLuaState, -2), lua_tonumber(pLuaState, -1));

	//	Die Position des Buttons setzen
	pButton->setPosition(v.getX(), v.getY());

	//	Es gibt keinen Rückgabewert
	return 0;
}

int LuaRegistrations::l_ButtonGetSize(lua_State* pLuaState)
{
	//	Die Referenz auf den Button aus den Argumenten holen
	Button* pButton = ButtonLuaRegistration::checkAndGetButton(pLuaState, 1);

	//	Die Werte auf den Stack pushen
	lua_pushinteger(pLuaState, pButton->getWidth());
	lua_pushinteger(pLuaState, pButton->getHeight());

	//	Es gibt zwei Rückgabewerte (width, height)
	return 2;
}

int LuaRegistrations::l_ButtonGetPosition(lua_State* pLuaState)
{
	//	Die Referenz auf den Button aus den Argumenten holen
	Button* pButton = ButtonLuaRegistration::checkAndGetButton(pLuaState, 1);

	//	Die Werte auf den Stack pushen
	lua_pushnumber(pLuaState, pButton->getPosition().getX());
	lua_pushnumber(pLuaState, pButton->getPosition().getY());

	//	Es gibt zwei Rückgabewerte (x, y)
	return 2;
}

int LuaRegistrations::l_ButtonSetSize(lua_State * pLuaState)
{
	//	Die Referenz auf den Button aus den Argumenten holen
	Button* pButton = ButtonLuaRegistration::checkAndGetButton(pLuaState, 1);

	int width = lua_tointeger(pLuaState, -2);
	int height = lua_tointeger(pLuaState, -1);
	pButton->setHeight(height);
	pButton->setWidth(width);

	return 0;
}
