#pragma once

#include <map>

/*	Diese Klasse existiert um sämtliche Lua Skripte erstmal in unser Programm zu laden 
 *	und um diese dann eventuell an ihren richtigen Platz zu verweisen.
 */

class ScriptLoader
{
private:

	ScriptLoader();																		//	Konstruktor
	~ScriptLoader();																	//	Destruktor

public:
	static std::map<std::string, std::string>* loadScripts(const char* filename);		//	Parst scripts.xml (id, module) 

};
