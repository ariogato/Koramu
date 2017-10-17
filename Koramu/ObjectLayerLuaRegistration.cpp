#include "ObjectLayerLuaRegistration.h"
#include <lua.hpp>

using namespace LuaRegistrations;

ObjectLayerLuaRegistration::ObjectLayerLuaRegistration()
{
}

ObjectLayerLuaRegistration::~ObjectLayerLuaRegistration()
{
}

void ObjectLayerLuaRegistration::registerToLua(lua_State* pLuaState)
{
	luaL_Reg regs[] =
	{
		{ "getPlayer",  l_getPlayer},
		{ nullptr, nullptr }
	};

	this->registerMetatable("TheGame", pLuaState, regs);
}

int LuaRegistrations::l_getPlayer(lua_State* pLuaState)
{

	//	Das Player Objekt wird zurükgegeben
	return 1;
}
