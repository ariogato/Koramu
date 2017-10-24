#pragma once

struct lua_State;
struct luaL_Reg;

/*	Diese Klasse ist die Basisklasse f�r s�mtliche Klassen, die f�r die Lua Skripte nutzbar sind.
 *	Memberfunktionen, die f�r Lua sichtbar sein sollen m�ssen mit einer Wrapper Funktion eingeschlossen werden.
 *	Diese Wrapper Funktion wird dann in registerToLua() an Lua weitergegeben.
 *	Auch einzelne Funktionen k�nnen in registerToLua() weitergegeben werden.
 *	
 *	Es wird nach diesem Muster registriert: https://gist.github.com/kizzx2/1594905 (zuletzt besucht am 17.10.17)
 *	
 *	Klassen werden in Form von globalen Metatabellen an Lua weitergegeben.
 */

namespace LuaRegistrations
{
	class BaseLuaRegistration
	{
	protected:
		virtual void registerMetatable(const char* name,							//	Hilfsfunktion, die einen Teil des Codes �bernimmt
			lua_State* pLuaState, const luaL_Reg* pRegs);			
	public:
		BaseLuaRegistration() {}													//	Konstruktor
		virtual ~BaseLuaRegistration() {}											//	Destruktor

		virtual void registerToLua(lua_State* pLuaState) = 0;						//	Registriert alles was zu registrieren ist (Implementierung h�ngt von Unterklasse ab)
	};

	//	Dies ist lediglich eine kleine Hilfsfunktion, die mir einiges beim debugging erleichtert
	void printStack(lua_State* L);
}
