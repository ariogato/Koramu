#pragma once

#include <string>
#include <map>
#include "Script.h"

/*	Dies ist eine Singleton Klasse, die nach dem Vorbild des TextureManagers alle Lua Skripte 
 *	mit den dazugeh�rigen Methoden, Tabellen, etc. organisiert.
 *	�ber den ScriptManager k�nnen andere Klassen auf Lua Methoden zugreifen
 */

class ScriptManager
{
private:
	ScriptManager();											//	Konstruktor
	~ScriptManager();											//	Destruktor

	bool m_isInit;												//	Zeigt an, ob init() bereits aufgerufen wurde
	lua_State* m_pLuaState;										//	In diesem Member ist praktisch alles gespeichert, das mit der Kommunikation mit Lua zu tun hat
	std::map<std::string, Script*>* m_pScriptMap;				//	Dictionary, in dem jeder ID ein Script zugeordnet wird

	//	Wichtig f�r Singleton
	static ScriptManager* s_pInstance;

public:

	bool init();												//	Initialisiert all das, was f�r Lua gebraucht wird (i.e. luaL_newstate(), etc.)
	Script& getScriptFromId(std::string id) const;				//	Gibt das der �bergebenen id zugeh�rigen Script zur�ck
	void callFunction(const char* table, const char* func);		//	Ruft eine Funktion 'func' aus der globalen Tabelle 'table' auf


	//	getter-Funktionen
	lua_State* getLuaState() { return m_pLuaState; }

	//	Wichtig f�r Singleton
	static ScriptManager* Instance();
	static void destroy();
};

typedef ScriptManager TheScriptManager;
