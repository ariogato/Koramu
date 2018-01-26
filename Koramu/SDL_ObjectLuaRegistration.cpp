#include "SDL_ObjectLuaRegistration.h"
#include <lua.hpp>
#include <algorithm>
#include "SDL_GameObject.h"
#include "Game.h"
#include "ButtonLuaRegistration.h"

using namespace LuaRegistrations;

SDL_ObjectLuaRegistration::SDL_ObjectLuaRegistration()
{}

SDL_ObjectLuaRegistration::~SDL_ObjectLuaRegistration()
{
}

void SDL_ObjectLuaRegistration::registerToLua(lua_State* pLuaState)
{
	//	Funktionen etc. werden hier definiert
	luaL_Reg regs[] =
	{
		{ "getInstance", l_SDL_GameObjectGetInstance },
		{ "setPosition", l_SDL_GameObjectSetPosition },
		{ "getSize", l_SDL_GameObjectGetSize },
		{ "getPosition", l_SDL_GameObjectGetPosition },
		{ "setSize", l_SDL_GameObjectSetSize },
		{ nullptr, nullptr }
	};

	//	Die Metatabelle mit den zuvor gesetzten Werten wird an Lua �bergeben
	this->registerMetatable("SDL_GameObject", pLuaState, regs);
}

SDL_GameObject* SDL_ObjectLuaRegistration::checkAndGetObject(lua_State* pLuaState, int argNum)
{
	/*	Gibt die Referenz auf die userdata Variable (im Grunde die Referenz auf das Objekt) zur�ck
	*	Dabei wird gleichzeitig gecheckt, ob die Metatabelle "luaL_SDL_GameObject" ist
	*/
	return *static_cast<SDL_GameObject**>(luaL_checkudata(pLuaState, argNum, "luaL_SDL_GameObject"));
}

int LuaRegistrations::l_SDL_GameObjectGetInstance(lua_State* pLuaState)
{
	//	Das �bergebene Argument (id des Objekts) poppen
	const char* uniqueId = luaL_checkstring(pLuaState, 2);
	//	Checken, ob das �bergebene Argument ein string ist (wenn nicht w�re nullptr �bergeben worden)
	if (!uniqueId)
	{
		TheGame::Instance()->logError() << "LuaRegistrations::l_getSDL_GameObjectInstance():\n\tArgument ist kein string." << std::endl << std::endl;
		lua_pushnil(pLuaState);
		return 1;
	}

	//	Die GameObjects des ObjectLayers werden geholt (Objekt nicht l�schen)
	std::vector<GameObject*> gameObjects = TheGame::Instance()->getCurrentState()->getAllObjects();

	//	Das entsprechende Objekt mit der id heraussuchen
	std::vector<GameObject*>::iterator it = std::find_if(gameObjects.begin(), gameObjects.end(), [uniqueId](GameObject* gO)
	{
		return gO->getUniqueId() == uniqueId;
	});

	//	Checken, ob das Objekt gefunden wurde
	if (it == gameObjects.end())
	{
		TheGame::Instance()->logError() << "LuaRegistrations::l_getSDL_GameObjectInstance():\n\tKein Objekt mit der ID: " << uniqueId << std::endl << std::endl;
		lua_pushnil(pLuaState);
		return 1;
	}

	//	Eine neue Userdata Variable (Pointer in Lua) wird erstellt
	SDL_GameObject** pObjectUserData = static_cast<SDL_GameObject**>(lua_newuserdata(pLuaState, sizeof(SDL_GameObject*)));

	//	Die userdata Variable bef�llen
	*pObjectUserData = static_cast<SDL_GameObject*>(*it);

	//	Die Metatabelle - definiert in "registerToLua" - auf den Stack pushen (siehe BaseLuaRegistration f�r den Namen der Metatabelle)
	luaL_getmetatable(pLuaState, "luaL_SDL_GameObject");

	//	Die Metatabelle (erstes Objekt auf dem Stack) f�r das User data Objekt vom SDL_GameObject (zweites Objekt auf dem Stack) setzen
	lua_setmetatable(pLuaState, -2);

	//	Der Pointer auf das SDL_GameObject wird �bergeben
	return 1;
}

int LuaRegistrations::l_SDL_GameObjectSetPosition(lua_State* pLuaState)
{
	//	Die Referenz auf den SDL_GameObject aus den Argumenten holen
	SDL_GameObject* pSDL_GameObject = SDL_ObjectLuaRegistration::checkAndGetObject(pLuaState, 1);

	//	Einen Vektor mit den Argumenten erstellen
	Vector2D v(lua_tonumber(pLuaState, -2), lua_tonumber(pLuaState, -1));

	//	Die Position des SDL_GameObjects setzen
	pSDL_GameObject->setPosition(v.getX(), v.getY());

	//	Es gibt keinen R�ckgabewert
	return 0;
}

int LuaRegistrations::l_SDL_GameObjectGetSize(lua_State* pLuaState)
{
	//	Die Referenz auf den SDL_GameObject aus den Argumenten holen
	SDL_GameObject* pSDL_GameObject = SDL_ObjectLuaRegistration::checkAndGetObject(pLuaState, 1);

	//	Die Werte auf den Stack pushen
	lua_pushinteger(pLuaState, pSDL_GameObject->getWidth());
	lua_pushinteger(pLuaState, pSDL_GameObject->getHeight());

	//	Es gibt zwei R�ckgabewerte (width, height)
	return 2;
}

int LuaRegistrations::l_SDL_GameObjectGetPosition(lua_State* pLuaState)
{
	//	Die Referenz auf den SDL_GameObject aus den Argumenten holen
	SDL_GameObject* pSDL_GameObject = SDL_ObjectLuaRegistration::checkAndGetObject(pLuaState, 1);

	//	Die Werte auf den Stack pushen
	lua_pushnumber(pLuaState, pSDL_GameObject->getPosition().getX());
	lua_pushnumber(pLuaState, pSDL_GameObject->getPosition().getY());

	//	Es gibt zwei R�ckgabewerte (x, y)
	return 2;
}

int LuaRegistrations::l_SDL_GameObjectSetSize(lua_State* pLuaState)
{
	//	Die Referenz auf den Button aus den Argumenten holen
	SDL_GameObject* pSDL_GameObject = SDL_ObjectLuaRegistration::checkAndGetObject(pLuaState, 1);

	//	H�he und Breite vom Stack holen
	int width = lua_tonumber(pLuaState, -2);
	int height = lua_tonumber(pLuaState, -1);

	//	Hreite und B�he setzen
	pSDL_GameObject->setHeight(height);
	pSDL_GameObject->setWidth(width);

	//	Es gibt keinen R�ckgabewert
	return 0;
}
