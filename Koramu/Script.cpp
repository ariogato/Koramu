#include "Script.h"
#include <iostream>
#include "ScriptManager.h"
#include "Game.h"


Script::Script()
	: m_pLuaState(TheScriptManager::Instance()->getLuaState()), 
	  m_scriptId(), m_tableReference(0)
{
	
}

Script::~Script()
{
	/*	Nicht zu löschende Pointer:
	 *		m_pLuaState:
	 *			Gehört dem ScriptManager, wird lediglich gespeichert, damit
	 *			nicht immer TheScriptManager::Instance()->getLuaState() 
	 *			aufgerufen werden muss.
	 *
	 */
}

bool Script::pushTable()
{
	//	Die Tabelle wird über die Referenz auf dem REGISTRYINDEX auf den Stack gepusht
	lua_rawgeti(m_pLuaState, LUA_REGISTRYINDEX, m_tableReference);

	//	Checken, ob tatsächlich eine Tabelle gepusht wurde (ein wenig redundant, da bereits in ScriptManager gecheckt wird)
	if (!lua_istable(m_pLuaState, -1))
	{
		//	Falls keine Tabelle vorliegen sollte, soll der Wert wieder gepoppt werden
		lua_pop(m_pLuaState, 1);
		return false;
	}

	//	Hier ist klar, dass alles glatt lief
	return true;
}


void Script::destroy()
{
	//	Die Referenz auf die Tabelle wird aufgehoben
	luaL_unref(m_pLuaState, LUA_REGISTRYINDEX, m_tableReference);
}

void Script::callFunction(const char* func)
{
	//	Die Tabelle auf den Stack pushen
	if (!pushTable())
		return;

	//	Die Funktion der Tabelle wird auf den Stack gepusht
	lua_getfield(m_pLuaState, -1, func);

	/*	Checken, ob tatsächlich eine Funktion gepusht wurde.
	 *	Falls nicht, soll das gepushte einfach wieder gepoppt werden.
	 */
	if (lua_isfunction(m_pLuaState, -1))
	{
		//	Funktion aufrufen
		lua_pcall(m_pLuaState, 0, 0, -1);
	}
	else
	{
		lua_pop(m_pLuaState, 1);
	}

	//	Die Tabelle wieder vom Stack poppen
	lua_pop(m_pLuaState, 1);
}

std::string Script::getStringFromTable(const char* name)
{
	//	Der zurückzugebende String wird initialisiert
	std::string returnValue = "";

	//	Die Tabelle auf den Stack pushen
	if (!pushTable())
		return returnValue;

	//	Die Variable der Tabelle wird auf den Stack gepusht
	lua_getfield(m_pLuaState, -1, name);
	
	/*	Checken, ob tatsächlich eine Funktion gepusht wurde.
	*	Falls nicht, soll das gepushte einfach wieder gepoppt werden.
	*/
	if (lua_isstring(m_pLuaState, -1))
	{
		//	Den String befüllen und vom Stack poppen
		returnValue = lua_tostring(m_pLuaState, -1);
		lua_pop(m_pLuaState, 1);
	}
	else
	{
		lua_pop(m_pLuaState, 1);
	}

	//	Die Tabelle wieder vom Stack poppen
	lua_pop(m_pLuaState, 1);

	//	Zurückgegeben wird entweder der leere String oder der befüllte
	return returnValue;
}

int Script::getIntFromTable(const char* name)
{
	//	Der zurückzugebende int wird mit 0 initialisiert
	int returnValue = 0;

	//	Die Tabelle auf den Stack pushen
	if (!pushTable())
		return returnValue;

	//	Die Variable der Tabelle wird auf den Stack gepusht
	lua_getfield(m_pLuaState, -1, name);
	
	/*	Checken, ob tatsächlich eine Funktion gepusht wurde.
	*	Falls nicht, soll das gepushte einfach wieder gepoppt werden.
	*/
	if (lua_isinteger(m_pLuaState, -1))
	{
		//	Den String befüllen und vom Stack poppen
		returnValue = lua_tointeger(m_pLuaState, -1);
		lua_pop(m_pLuaState, 1);
	}
	else
	{
		lua_pop(m_pLuaState, 1);
	}

	//	Die Tabelle wieder vom Stack poppen
	lua_pop(m_pLuaState, 1);

	//	Zurückgegeben wird entweder der 0 oder der Wert der Variablen
	return returnValue;
}

bool Script::getBoolFromTable(const char* name)
{
	//	Der zurückzugebende int wird mit false initialisiert
	bool returnValue = false;

	//	Die Tabelle auf den Stack pushen
	if (!pushTable())
		return returnValue;

	//	Die Variable der Tabelle wird auf den Stack gepusht
	lua_getfield(m_pLuaState, -1, name);

	/*	Checken, ob tatsächlich eine Funktion gepusht wurde.
	*	Falls nicht, soll das gepushte einfach wieder gepoppt werden.
	*/
	if (lua_isboolean(m_pLuaState, -1))
	{
		//	Den String befüllen und vom Stack poppen
		returnValue = lua_toboolean(m_pLuaState, -1);
		lua_pop(m_pLuaState, 1);
	}
	else
	{
		lua_pop(m_pLuaState, 1);
	}

	//	Die Tabelle wieder vom Stack poppen
	lua_pop(m_pLuaState, 1);

	//	Zurückgegeben wird entweder der 0 oder der Wert der Variablen
	return returnValue;
}



