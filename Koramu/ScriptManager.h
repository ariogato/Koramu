#pragma once

#include <string>
#include <map>
#include "Script.h"

namespace LuaRegistrations
{
	class BaseLuaRegistration;
}

/*	Dies ist eine Singleton Klasse, die nach dem Vorbild des TextureManagers alle Lua Skripte 
 *	mit den dazugehörigen Methoden, Tabellen, etc. organisiert.
 *	über den ScriptManager können andere Klassen auf Lua Methoden zugreifen
 */

class ScriptManager
{
private:
	ScriptManager();																//	Konstruktor
	~ScriptManager();																//	Destruktor

	bool m_isInit;																	//	Zeigt an, ob init() bereits aufgerufen wurde
	lua_State* m_pLuaState;															//	In diesem Member ist praktisch alles gespeichert, das mit der Kommunikation mit Lua zu tun hat
	std::map<std::string, Script*>* m_pScriptMap;									//	Dictionary, in dem jeder ID ein Script zugeordnet wird
	std::vector<LuaRegistrations::BaseLuaRegistration*> m_registrations;			//	Alle Funktionen, Variablen, etc., die an Lua weiterzugeben sind

	//	Wichtig für Singleton
	static ScriptManager* s_pInstance;

public:

	bool init();																	//	Initialisiert all das, was für Lua gebraucht wird (i.e. luaL_newstate(), etc.)
	Script& getScriptFromId(std::string id) const;									//	Gibt das der übergebenen id zugehörigen Script zurück
	void callFunction(const char* table, const char* func);							//	Ruft eine Funktion 'func' aus der globalen Tabelle 'table' auf
	void addRegistration(LuaRegistrations::BaseLuaRegistration* reg);				//	Fügt dem Vektor aus Registrierungen ein weiteres Element hinzu

	//	getter-Funktionen
	lua_State* getLuaState() { return m_pLuaState; }

	//	Wichtig für Singleton
	static ScriptManager* Instance();
	static void destroy();
};

typedef ScriptManager TheScriptManager;
