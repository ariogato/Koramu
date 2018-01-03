#include "Script.h"
#include <iostream>
#include "ScriptManager.h"
#include "Game.h"

#include "NPC.h"
#include "Player.h"


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

void Script::pushArgumentMetatable(void* pObject, OBJECT_TYPE type)
{
	//	Variable die abhängig vom Argument 'type' gesetzt wird
	std::string tableName;

	/*	Wie verfahren wird, ist abhängig vom Typen des Objekts.
	 *	'pObject' wird gecastet und zu seiner zugehörigen Metatabelle hinzugefügt.
	 */
	switch (type)
	{
	case PLAYER_TYPE: {
		//	Namen der Metatabelle setzen
		tableName = "luaL_Player";

		//	Eine userdata Variable für das Objekt wird erstellt und auf den Stack gepusht
		Player** pPlayerUserData = static_cast<Player**>(lua_newuserdata(m_pLuaState, sizeof(Player*)));

		//	Die userdata Variable auf dem Stack befüllen
		*pPlayerUserData = static_cast<Player*>(pObject);

		break; }

	case NPC_TYPE: {
		//	Namen der Metatabelle setzen
		tableName = "luaL_NPC";

		//	Eine userdata Variable für das Objekt wird erstellt und auf den Stack gepusht
		NPC** pNPCUserData = static_cast<NPC**>(lua_newuserdata(m_pLuaState, sizeof(NPC*)));

		//	Die userdata Variable auf dem Stack befüllen
		*pNPCUserData = static_cast<NPC*>(pObject);

		break; }

	default:
		//	Das darf nicht passieren
		TheGame::Instance()->logError() << "Script::pushArgumentMetatable():\n\tTyp " << type << " existiert nicht" << std::endl << std::endl;
		return;
	}

	//	Die zum Objekt gehörige Metatabelle auf den Stack pushen
	luaL_getmetatable(m_pLuaState, tableName.c_str());

	//	Das Objekt mit der Metatabelle verknüpfen
	lua_setmetatable(m_pLuaState, -2);
}

void Script::pushArgumentString(const char* arg)
{
	//	Checken, ob nullptr übergeben wurde
	if (!arg)
		return;

	//	Lediglich ein Wrapper um die Lua API Funktion
	lua_pushstring(m_pLuaState, arg);
}

void Script::callVoidWithArgs(const char* func, int numArgs)
{
	//	Checken, ob sich genug Argumente auf dem Stack befinden
	if (numArgs > lua_gettop(m_pLuaState))
	{
		TheGame::Instance()->logError() << "Script::callVoidWithArgs():\n\tEs befinden sich weniger als " << numArgs << " Werte auf dem Stack" << std::endl << std::endl;
		lua_settop(m_pLuaState, 0);
		return;
	}

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
		//	Die Tabelle wieder vom Stack entfernen
		lua_remove(m_pLuaState, -2);

		//	Die Lua Funktion unter die Argumente einsetzen (so will es 'lua_pcall()')
		lua_insert(m_pLuaState, 1);

		//	Funktion aufrufen und checken, ob Fehler aufgetreten sind
		if (lua_pcall(m_pLuaState, numArgs, 0, 0))
		{
			TheGame::Instance()->logError() << "Script::callVoidWithArgs():\n\tFunktion: " << func << " in der Tabelle " << m_scriptId << "\n\t" << lua_tostring(m_pLuaState, -1) << std::endl << std::endl;
			TheGame::Instance()->emergencyExit("Fehler beim Aufruf einer Lua Funktion!\nSiehe error log");
		}
	}
	else
	{
		//	Den Stack leeren
		lua_settop(m_pLuaState, 0);
	}
}

std::string Script::callString(const char* func)
{
	//	Die Tabelle auf den Stack pushen
	if (!pushTable())
		return "";

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

	//	Es wird gecheckt, ob ein String zurückgegeben wurde; Dieser wird danach in retString geladen
	std::string retString = luaL_checkstring(m_pLuaState, -1);

	//	Die Tabelle wieder vom Stack poppen
	lua_pop(m_pLuaState, 1);

	return retString;
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



