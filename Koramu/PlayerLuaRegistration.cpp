#include "PlayerLuaRegistration.h"
#include <lua.hpp>
#include <iostream>

using namespace LuaRegistrations;

PlayerLuaRegistration::PlayerLuaRegistration()
{
	
}

PlayerLuaRegistration::~PlayerLuaRegistration()
{
	
}

void PlayerLuaRegistration::registerToLua(lua_State* pLuaState)
{
	//	Funktionen etc. werden hier definiert
	luaL_Reg regs[] =
	{
		{"sayhi", sayhi},
		{nullptr, nullptr}
	};

	//	Die Metatabelle mit den zuvor gesetzten Werten wird an Lua übergeben
	this->registerMetatable("Player", pLuaState, regs);
}

int LuaRegistrations::sayhi(lua_State* L)
{
	std::cout << "hello from c++" << std::endl;

	return 0;
}
