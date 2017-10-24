#include "GameLuaRegistration.h"
#include <lua.hpp>
#include "Game.h"
#include "Story.h"
#include "NPC.h"
#include "NPCLuaRegistration.h"
#include "PlayerLuaRegistration.h"
#include "DialogCommand.h"

using namespace LuaRegistrations;

GameLuaRegistration::GameLuaRegistration()
{
}

GameLuaRegistration::~GameLuaRegistration()
{
}

void GameLuaRegistration::registerToLua(lua_State* pLuaState)
{
	luaL_Reg regs[] = 
	{
		{ "setGameOver", l_setGameOver },
		{ "getMainQuestId", l_getMainQuestId },
		{ "getPartQuestId", l_getPartQuestId },
		{ "nextQuest", l_nextQuest },
		{ "startDialog", l_startDialog },
		{ "startNarrator", l_startNarrator },
		{nullptr, nullptr}
	};

	this->registerMetatable("TheGame", pLuaState, regs);
}

int LuaRegistrations::l_setGameOver(lua_State* pLuaState)
{
	//	Game Over setzen
	TheGame::Instance()->setGameOver();

	//	Es gibt keinen Rückgabewert
	return 0;
}

int LuaRegistrations::l_getMainQuestId(lua_State* pLuaState)
{
	//	Die Id auf den Stack pushen
	lua_pushstring(pLuaState, TheGame::Instance()->getStory()->getMainQuest().c_str());

	//	Es gibt einen Rückgabewert (main quest id)
	return 1;
}

int LuaRegistrations::l_getPartQuestId(lua_State* pLuaState)
{
	//	Die Id auf den Stack pushen
	lua_pushstring(pLuaState, TheGame::Instance()->getStory()->getPartQuest().c_str());

	//	Es gibt einen Rückgabewert (part quest id)
	return 1;
}

int LuaRegistrations::l_nextQuest(lua_State* pLuaState)
{
	//	Die nächste Quest einleiten
	TheGame::Instance()->getStory()->nextQuest();

	//	Es gibt keinen Rückgabewert
	return 0;
}

int LuaRegistrations::l_startDialog(lua_State* pLuaState)
{
	//	Den übergebenen string speichern 
	const char* text = luaL_checkstring(pLuaState, 2);
	
	//	Checken, ob das übergebene Argument ein string ist (wenn nicht wäre nullptr übergeben worden)
	if (!text)
	{
		TheGame::Instance()->logError() << "LuaRegistrations::l_startDialog():\n\t1. Argument ist kein string." << std::endl << std::endl;
		return 0;
	}

	//	Referenz auf den NPC (nicht zu löschen)
	NPC* pNPCInstance = NPCLuaRegistration::checkAndGetNPC(pLuaState, 3);

	//	Referenz auf den Player (nicht zu löschen)
	Player* pPlayerInstance = PlayerLuaRegistration::checkAndGetPlayer(pLuaState, 4);

	//	Der Text wird in die Dialogbox geladen
	TheGame::Instance()->getCurrentState()->getDialog()->addMessage(text);

	//	Die Objekte, die den Dialog führen bekommen den Dialog Befehl
	pPlayerInstance->pushCommand(new DialogCommand());
	pNPCInstance->pushCommand(new DialogCommand());

	//	Es gibt keinen Rückgabewert
	return 0;
}

int LuaRegistrations::l_startNarrator(lua_State* pLuaState)
{
	//	Den übergebenen string speichern 
	const char* text = luaL_checkstring(pLuaState, 2);

	//	Checken, ob das übergebene Argument ein string ist (wenn nicht wäre nullptr übergeben worden)
	if (!text)
	{
		TheGame::Instance()->logError() << "LuaRegistrations::l_startDialog():\n\t1. Argument ist kein string." << std::endl << std::endl;
		return 0;
	}

	//	Referenz auf den Player (nicht zu löschen)
	Player* pPlayerInstance = PlayerLuaRegistration::checkAndGetPlayer(pLuaState, 3);

	//	Der Text wird in die Dialogbox geladen
	TheGame::Instance()->getCurrentState()->getDialog()->addMessage(text);

	//	Der Player bekommt den Dialog Befehl
	pPlayerInstance->pushCommand(new DialogCommand());

	//	Es gibt keinen Rückgabewert
	return 0;
}
