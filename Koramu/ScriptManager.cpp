#include "ScriptManager.h"
#include <lua.hpp>
#include <vector>
#include <iterator>
#include "ScriptLoader.h"
#include "BaseLuaRegistration.h"
#include "Game.h"

#define FILEPATH "xmlFiles/scripts.xml"

//	Wichtig für Singleton-Klasse
ScriptManager* ScriptManager::s_pInstance = nullptr;


ScriptManager::ScriptManager()
	: m_isInit(false), 
	  m_pLuaState(nullptr), m_pScriptMap(nullptr)
{
}

ScriptManager::~ScriptManager()
{
	/*	Alle Skripts gehören dem ScriptManager, also muss er sie 
	 *	eigenständig löschen.
	 *	
	 *	Hier wird über alle Elemente iteriert. Diese werden gelöscht. 
	 *	Anschließend wird der Eintrag gelöscht.
	 */
	for (auto it = m_pScriptMap->begin(); it != m_pScriptMap->end(); ++it)
	{
		if (it->second)
		{
			//	Zuerst die Referenz auf die Tabelle aufheben, dann löschen
			it->second->destroy();
			delete it->second;
		}
		m_pScriptMap->erase(it);
	}
	delete m_pScriptMap;

	//	Alle Registrierungen müssen gelöscht werden (kann evtl. Probleme verursachen... in dem Fall einfach mit Zählervariable ersetzen)
	for (auto it = m_registrations.begin(); it != m_registrations.end(); ++it)
	{
		//	Checken, ob das Element nullptr ist
		if (*it)
			delete *it;
	}
	m_registrations.clear();

	//	Lua und Teilsysteme herunterfahren, falls initialisiert wurde
	if (m_isInit)
		lua_close(m_pLuaState);

	//	Log Message
	TheGame::Instance()->logStandard() << "ScriptManager zerstoert" << std::endl;
}

bool ScriptManager::init()
{
	//	Log Message
	TheGame::Instance()->logStandard() << "ScriptManager wird initialisiert..." << std::endl;

	//	Wir wollen nicht, dass zwei mal initialisiert wird
	if (m_isInit)
		return true;

	//	Ein neuer Lua State wird generiert
	m_pLuaState = luaL_newstate();

	//	Die von den Skripten benötigten Libraries werden in den Lua Stack geladen
	static const luaL_Reg s_luaLibs[] =
	{
		{ "base", luaopen_base },
		{ nullptr, nullptr }
	};

	//	Nun wird über jede angegebene Library iteriert, wobei ihre Lade Funktion aufgerufen wird
	for (const luaL_Reg* lib = s_luaLibs; lib->func != nullptr; lib++)
	{
		lib->func(m_pLuaState);
		lua_settop(m_pLuaState, 0);
	}

	//	Alles was zu regisitrieren ist soll an Lua weitergegeben werden
	for (auto it = m_registrations.begin(); it != m_registrations.end(); ++it)
	{
		//	Checken ob das Element nullptr ist
		if (*it)
		{
			//	Die Registrierungsfunktion aufrufen
			(*it)->registerToLua(m_pLuaState);
		}
	}

	//	Die Namen der verwendeten Dateien laden (soll nachher gelöscht werden, da die ID in m_pScriptMap übertragen wird)
	std::map<std::string, std::string>* pScriptNameMap = ScriptLoader::loadScripts(FILEPATH);

	//	Ein zu befüllendes Dictionary wird erstellt
	m_pScriptMap = new std::map<std::string, Script*>();

	//	Alle Chunks (Dateien) laden
	for (std::map<std::string, std::string>::iterator it = pScriptNameMap->begin(); it != pScriptNameMap->end(); ++it)
	{
		/*	Hier wird die Lua Datei ausgeführt. Nach dem ausführen befinden sich sämtliche Tabellen, 
		 *	die im Script returnt wurden auf dem Stack. In ihnen befinden sich sämtliche Methoden, Attribute, etc.
		 *	
		 *	Mit 'if' wird gecheckt, ob beim ausführen alles glatt lief.
		 */
		if (luaL_dofile(m_pLuaState, (it->second).c_str()))
		{
			return false;
		}

		//	Checken, ob tatsächlich eine Tabelle vom Script zurückgegeben wurde
		if (!lua_istable(m_pLuaState, -1))
			continue;

		/*	Hier wird die Tabelle, die vom ausgeführten Skript auf den Stack gepusht wurde, abgerufen.
		 *	Dieser Pointer darf nicht gelöscht werden, da er an das Script Objekt weitergegeben wird.
		 *	Deshalb wird die Tabelle referenziert, damit der Garbagecollector sie nicht löscht.
		 */
		int tableReference = luaL_ref(m_pLuaState, LUA_REGISTRYINDEX);

		/*	Nun erstellen wir ein Script Objekt, das diese Tabelle speichert.
		 *	Es muss ein Pointer sein, da sonst die Referenz auf die Tabelle gelöscht wird.
		 */
		Script* tempScript = new Script();

		//	Die Attribute des Skriptes werden gesetzt
		tempScript->setScriptId(it->first);
		tempScript->setTableReference(tableReference);

		//	Das Script Objekt wird in dem Dictionary gespeichert
		m_pScriptMap->insert(std::pair<std::string, Script*>(it->first, tempScript));

		//	Zuletzt wird der Stack geleert
		lua_settop(m_pLuaState, 0);

		//	Hier wird ausgegeben, dass ein Skript erfolgreich geladen wurde
		TheGame::Instance()->logStandard() << "\t" << it->second << " wurde erfolgreich geladen!" << std::endl;
	}

	//	Das Dictionary aus Dateinamen löschen
	delete pScriptNameMap;

	//	Log Message
	TheGame::Instance()->logStandard() << "ScriptManager wurde initialisiert!" << std::endl << std::endl;

	//	Sobald wir hier angekommen sind, ist sicher, dass alles glatt lief
	m_isInit = true;
	return true;
}

Script& ScriptManager::getScriptFromId(std::string id) const
{
	//	Checken, ob das Script mit der gegebenen ID existiert
	if (m_pScriptMap->find(id) == m_pScriptMap->end())
	{
		//	Es braucht keinen Error log, da einige Objekte kein Script haben werden

		//	Eine Referenz auf das leere Default Skript zurückgeben
		return (*(*m_pScriptMap)[std::string("default")]);
	}

	//	Eine Referenz auf das Script zurückgeben
	return (*(*m_pScriptMap)[id]);
}

void ScriptManager::callFunction(const char* table, const char* func)
{
	//	Die globale Tabelle auf den Stack pushen
	lua_getglobal(m_pLuaState, table);

	//	Die Funktion auf den Stack pushen
	lua_getfield(m_pLuaState, -1, func);

	//	Die Funktion aufrufen
	lua_call(m_pLuaState, 0, 0);

	//	Die Tabelle aus dem Stack entfernen
	lua_pop(m_pLuaState, 1);
}

void ScriptManager::addRegistration(LuaRegistrations::BaseLuaRegistration* reg)
{
	//	Checken, ob nullptr übergeben wurde
	if (!reg)
		return;

	m_registrations.push_back(reg);
}

ScriptManager* ScriptManager::Instance()
{
	/*	Checken ob die Instanz schon existiert.
	*	wenn nein:
	*		eine neue erstellen
	*/
	if (!s_pInstance)
		s_pInstance = new ScriptManager();
	
	//	Instanz zurückgeben
	return s_pInstance;
}

void ScriptManager::destroy()
{
	//	Den Destruktor aufrufen
	delete s_pInstance;

	//	Nochmal - weil es gute Programmierpraxis ist - die Instanz = 'nullptr' setzen
	s_pInstance = nullptr;
}

