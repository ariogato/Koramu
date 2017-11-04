#pragma once

#include <string>
#include <vector>
#include <lua.hpp>

/*	Eine Enumeration, damit spezifizert werden kann, wie bei welchem Objekt vorgegangen
 *	werden soll. 
 */

enum OBJECT_TYPE
{
	PLAYER_TYPE = 0, NPC_TYPE,
};

/*	Diese Klasse ist eine Abstraktion eines Lua Skriptes.
 *	In ihr werden sämtliche Funktionen eines Skriptes abrufbar gespeichert.
 */

class Script
{
private:
	lua_State* m_pLuaState;											//	Ein Pointer auf den lua_State vom ScriptManager
	std::string m_scriptId;											//	Jedes Skript hat eine einzigartige ID, sodass Objekte ihr zugehöriges Skript finden können
	int m_tableReference;											//	Referenz auf Tabelle im Skript, in der sich die für C++ sichtbaren Funktionen, Variablen, etc. befinden
	
public:
	Script();
	~Script();

	void destroy();													//	Hebt die Referenz auf das Skript auf
	bool pushTable();												//	Pusht die referenzierte Tabelle auf den Stack
	void callFunction(const char* func);							//	Ruft eine Funktion auf der Tabelle mit 0 Parametern und 0 Rückgabewerten auf
	void pushArgumentMetatable(void* pObject, OBJECT_TYPE type);	//	Pusht eine Metatabelle des angegebenen Typs auf den Stack
	void pushArgumentString(const char* arg);						//	Pusht einen (c-)string auf den Stack
	void callVoidWithArgs(const char* func, int numArgs);			//	Ruft eine void Funktion mit 'numArgs' Argumenten vom Stack auf

	//	getter-Funktionen
	std::string getScriptId() const { return m_scriptId; }
	std::string getStringFromTable(const char* name);				//	Gibt einen leeren String zurück, falls ein Fehler auftritt
	int getIntFromTable(const char* name);							//	Gibt 0 zurück, falls ein Fehler auftritt
	bool getBoolFromTable(const char* name);						//	Gibt false zurück, falls ein Fehler auftritt

	//	setter-Funktionen
	void setScriptId(std::string s) { m_scriptId = s; }
	void setTableReference(int ref) { m_tableReference = ref; }
};
