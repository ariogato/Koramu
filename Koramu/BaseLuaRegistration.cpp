#include "BaseLuaRegistration.h"
#include <lua.hpp>
#include <string>

void LuaRegistrations::BaseLuaRegistration::registerMetatable(const char* name, lua_State* pLuaState, const luaL_Reg* pRegs)
{
	/*	Lua kann keine Klasse in dem Sinne annehmen, mit der gearbeitet werden kann.
	 *	Um eine Klasse fürs Scripting zugänglich zu machen bedient man sich des folgenden Tricks:
	 *	
	 *	Es wird eine Metatabelle erstellt, die die Methoden der Klasse enthält. 
	 *	Des Weiteren werden Konstruktor und Destruktor extra definiert. So hat Lua im Endeffekt doch 
	 *	die Möglichkeit eines Zugriffs auf eine Klasse und seine Member.
	 */

	//	Eine neue Metatabelle wird auf den Stack gepusht (Form: luaL_ + name)
	luaL_newmetatable(pLuaState, (std::string("luaL_") + std::string(name)).c_str());

	//	Die zuvor definierten Funktionen (befindlich in pRegs) werden in die Tabelle an oberster Stelle im Stack registriert
	luaL_setfuncs(pLuaState, pRegs, 0);

	//	Die Metatabelle wird kopiert und auf den Stack gepusht
	lua_pushvalue(pLuaState, -1);

	//	Der __index der Metatabelle zeigt auf sich selber
	lua_setfield(pLuaState, -1, "__index");

	//	Die Metatabelle wird unter der Bezeichnung name global sichtbar
	lua_setglobal(pLuaState, name);
}
