#include "GameLuaRegistration.h"
#include <lua.hpp>
#include "Game.h"
#include "Story.h"
#include "NPC.h"
#include "NPCLuaRegistration.h"
#include "PlayerLuaRegistration.h"
#include "DialogCommand.h"
#include "NotebookState.h"
#include "Notebook.h"

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
		{ "getGameWidth", l_getGameWidth },
		{ "getGameHeight", l_getGameHeight },
		{ "setGameOver", l_setGameOver },
		{ "getMainQuestId", l_getMainQuestId },
		{ "getPartQuestId", l_getPartQuestId },
		{ "nextQuest", l_nextQuest },
		{ "startDialog", l_startDialog },
		{ "startNarrator", l_startNarrator },
		{ "enterMap", l_enterMap },
		{ "exitMap", l_exitMap },
		{ "openNotebook", l_openNotebook},
		{ "closeNotebook", l_closeNotebook},
		{ "addNote", l_addNote},
		{nullptr, nullptr}
	};

	this->registerMetatable("TheGame", pLuaState, regs);
}

int LuaRegistrations::l_getGameWidth(lua_State* pLuaState)
{
	//	Den Integer auf den Lua Stack pushen
	lua_pushinteger(pLuaState, TheGame::Instance()->getGameWidth());

	//	Es gibt einen R�ckgabewert: Die Breite des Fensters als int
	return 1;
}

int LuaRegistrations::l_getGameHeight(lua_State* pLuaState)
{
	//	Den Integer auf den Lua Stack pushen
	lua_pushinteger(pLuaState, TheGame::Instance()->getGameHeight());
	
	//	Es gibt einen R�ckgabewert: Die H�he des Fensters als int
	return 1;
}

int LuaRegistrations::l_setGameOver(lua_State* pLuaState)
{
	//	Game Over setzen
	TheGame::Instance()->setGameOver();

	//	Es gibt keinen R�ckgabewert
	return 0;
}

int LuaRegistrations::l_getMainQuestId(lua_State* pLuaState)
{
	//	Die Id auf den Stack pushen
	lua_pushstring(pLuaState, TheGame::Instance()->getStory()->getMainQuest().c_str());

	//	Es gibt einen R�ckgabewert (main quest id)
	return 1;
}

int LuaRegistrations::l_getPartQuestId(lua_State* pLuaState)
{
	//	Die Id auf den Stack pushen
	lua_pushstring(pLuaState, TheGame::Instance()->getStory()->getPartQuest().c_str());

	//	Es gibt einen R�ckgabewert (part quest id)
	return 1;
}

int LuaRegistrations::l_nextQuest(lua_State* pLuaState)
{
	//	Die n�chste Quest einleiten
	TheGame::Instance()->getStory()->nextQuest();

	//	Es gibt keinen R�ckgabewert
	return 0;
}

int LuaRegistrations::l_startDialog(lua_State* pLuaState)
{
	//	Den �bergebenen string speichern 
	const char* text = luaL_checkstring(pLuaState, 2);
	
	//	Checken, ob das �bergebene Argument ein string ist (wenn nicht w�re nullptr �bergeben worden)
	if (!text)
	{
		TheGame::Instance()->logError() << "LuaRegistrations::l_startDialog():\n\t1. Argument ist kein string." << std::endl << std::endl;
		return 0;
	}

	//	Referenz auf den NPC (nicht zu l�schen)
	NPC* pNPCInstance = NPCLuaRegistration::checkAndGetNPC(pLuaState, 3);

	//	Referenz auf den Player (nicht zu l�schen)
	Player* pPlayerInstance = PlayerLuaRegistration::checkAndGetPlayer(pLuaState, 4);

	//	Der Text wird in die Dialogbox geladen
	TheGame::Instance()->getCurrentState()->getDialog()->addMessage(text);

	//	Die Objekte, die den Dialog f�hren bekommen den Dialog Befehl
	pPlayerInstance->pushCommand(new DialogCommand());
	pNPCInstance->pushCommand(new DialogCommand());

	//	Es gibt keinen R�ckgabewert
	return 0;
}

int LuaRegistrations::l_startNarrator(lua_State* pLuaState)
{
	//	Den �bergebenen string speichern 
	const char* text = luaL_checkstring(pLuaState, 2);

	//	Checken, ob das �bergebene Argument ein string ist (wenn nicht w�re nullptr �bergeben worden)
	if (!text)
	{
		TheGame::Instance()->logError() << "LuaRegistrations::l_startDialog():\n\t1. Argument ist kein string." << std::endl << std::endl;
		return 0;
	}

	//	Referenz auf den Player (nicht zu l�schen)
	Player* pPlayerInstance = PlayerLuaRegistration::checkAndGetPlayer(pLuaState, 3);

	//	Der Text wird in die Dialogbox geladen
	TheGame::Instance()->getCurrentState()->getDialog()->addMessage(text);

	//	Der Player bekommt den Dialog Befehl
	pPlayerInstance->pushCommand(new DialogCommand());

	//	Es gibt keinen R�ckgabewert
	return 0;
}

int LuaRegistrations::l_enterMap(lua_State* pLuaState)
{
	//	Den �bergebenen string speichern 
	const char* mapId = luaL_checkstring(pLuaState, 2);

	//	Checken, ob das �bergebene Argument ein string ist (wenn nicht w�re nullptr �bergeben worden)
	if (!mapId)
	{
		TheGame::Instance()->logError() << "LuaRegistrations::l_enterMap():\n\t1. Argument ist kein string." << std::endl << std::endl;
		return 0;
	}

	//	Map anhand des �bergebenen strings (Id der Map) aufstapeln
	TheGame::Instance()->getCurrentState()->pushMap(mapId);

	//	Es gibt keinen R�ckgabewert
	return 0;
}

int LuaRegistrations::l_exitMap(lua_State* pLuaState)
{
	//	Aktuelle Map abstapeln
	TheGame::Instance()->getCurrentState()->popMap();

	//	Es gibt keinen R�ckgabewert
	return 0;
}

int LuaRegistrations::l_openNotebook(lua_State * pLuaState)
{
	//	"NotebookState" aufstapeln
	TheGame::Instance()->pushState(new FiniteStateMachine::NotebookState());

	//	Es gibt keinen R�ckgabewert
	return 0;
}

int LuaRegistrations::l_closeNotebook(lua_State * pLuaState)
{
	//	"NotebookState" abstapeln
	TheGame::Instance()->popState();

	//	Es gibt keinen R�ckgabewert
	return 0;
}

int LuaRegistrations::l_addNote(lua_State * pLuaState)
{
	//	Den �bergebenen string speichern 
	const char* note = luaL_checkstring(pLuaState, 2);

	//	Checken, ob das �bergebene Argument ein string ist (wenn nicht w�re nullptr �bergeben worden)
	if (!note)
	{
		TheGame::Instance()->logError() << "LuaRegistrations::l_addNote():\n\t1. Argument ist kein string." << std::endl << std::endl;
		return 0;
	}

	//	�bergebenen Text zum Notizbuch hinzuf�gen
	TheGame::Instance()->getNotebook()->addNote(note);
	
	//	Es gibt keinen R�ckgabewert
	return 0;
}
