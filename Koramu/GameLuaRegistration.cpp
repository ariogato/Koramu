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
#include "ItemList.h"
#include "Clock.h"

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
		{ "nextQuestDelay", l_nextQuestDelay },
		{ "startDialog", l_startDialog },
		{ "startNarrator", l_startNarrator },
		{ "enterMap", l_enterMap },
		{ "exitMap", l_exitMap },
		{ "openNotebook", l_openNotebook},
		{ "closeNotebook", l_closeNotebook},
		{ "addNote", l_addNote },
		{ "addItem", l_addItem },
		{ "removeItem", l_removeItem },
		{ "getItemCount", l_getItemCount },
		{ "getClockTimeSeconds", l_getClockTimeSeconds },
		{ "setClockTimeSeconds", l_setClockTimeSeconds },
		{ "addClockTimeSeconds", l_addClockTimeSeconds },
		{ "showClock", l_showClock },
		{ "hideClock", l_hideClock },
		{nullptr, nullptr}
	};

	this->registerMetatable("TheGame", pLuaState, regs);
}

int LuaRegistrations::l_getGameWidth(lua_State* pLuaState)
{
	//	Den Integer auf den Lua Stack pushen
	lua_pushinteger(pLuaState, TheGame::Instance()->getGameWidth());

	//	Es gibt einen Rückgabewert: Die Breite des Fensters als int
	return 1;
}

int LuaRegistrations::l_getGameHeight(lua_State* pLuaState)
{
	//	Den Integer auf den Lua Stack pushen
	lua_pushinteger(pLuaState, TheGame::Instance()->getGameHeight());
	
	//	Es gibt einen Rückgabewert: Die Höhe des Fensters als int
	return 1;
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

int LuaRegistrations::l_nextQuestDelay(lua_State* pLuaState)
{
	//	Den übergebenen int speichern
	int delay = luaL_checkinteger(pLuaState, 2);

	//	Den Funktionsaufruf tätigen
	TheGame::Instance()->getStory()->nextQuestDelay(delay);

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

	//	Der NPC wendet sich zum Player
	pNPCInstance->turnToObject(pPlayerInstance);

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

int LuaRegistrations::l_enterMap(lua_State* pLuaState)
{
	//	Den übergebenen string speichern 
	const char* mapId = luaL_checkstring(pLuaState, 2);

	//	Checken, ob das übergebene Argument ein string ist (wenn nicht wäre nullptr übergeben worden)
	if (!mapId)
	{
		TheGame::Instance()->logError() << "LuaRegistrations::l_enterMap():\n\t1. Argument ist kein string." << std::endl << std::endl;
		return 0;
	}

	//	Map anhand des übergebenen strings (Id der Map) aufstapeln
	TheGame::Instance()->getCurrentState()->pushMap(mapId);

	//	Es gibt keinen Rückgabewert
	return 0;
}

int LuaRegistrations::l_exitMap(lua_State* pLuaState)
{
	//	Aktuelle Map abstapeln
	TheGame::Instance()->getCurrentState()->popMap();

	//	Es gibt keinen Rückgabewert
	return 0;
}

int LuaRegistrations::l_openNotebook(lua_State * pLuaState)
{
	//	"NotebookState" aufstapeln
	TheGame::Instance()->pushState(new FiniteStateMachine::NotebookState());

	//	Es gibt keinen Rückgabewert
	return 0;
}

int LuaRegistrations::l_closeNotebook(lua_State * pLuaState)
{
	//	"NotebookState" abstapeln
	TheGame::Instance()->popState();

	//	Es gibt keinen Rückgabewert
	return 0;
}

int LuaRegistrations::l_addNote(lua_State * pLuaState)
{
	//	Den übergebenen string speichern 
	const char* note = luaL_checkstring(pLuaState, 2);

	//	Checken, ob das übergebene Argument ein string ist (wenn nicht wäre nullptr übergeben worden)
	if (!note)
	{
		TheGame::Instance()->logError() << "LuaRegistrations::l_addNote():\n\t1. Argument ist kein string." << std::endl << std::endl;
		return 0;
	}

	//	Übergebenen Text zum Notizbuch hinzufügen
	TheGame::Instance()->getNotebook()->addNote(note);
	
	//	Es gibt keinen Rückgabewert
	return 0;
}

int LuaRegistrations::l_addItem(lua_State* pLuaState)
{
	//	Es gibt zwei Parameter: id & count
	const char* id = luaL_checkstring(pLuaState, 2);
	int count = luaL_checkinteger(pLuaState, 3);

	if (!id)
	{
		TheGame::Instance()->logError() << "LuaRegistrations::l_addItem():\n\t1. Argument ist kein string." << std::endl << std::endl;
		return 0;
	}

	//	Das Item hinzufügen
	TheGame::Instance()->getItemList()->addItem(id, count);

	//	Es gibt keinen Rückgabewert
	return 0;
}

int LuaRegistrations::l_removeItem(lua_State* pLuaState)
{
	//	Es gibt einen Parameter: id
	const char* id = luaL_checkstring(pLuaState, 2);
	int count = luaL_checkinteger(pLuaState, 3);

	if (!id)
	{
		TheGame::Instance()->logError() << "LuaRegistrations::l_removeItem():\n\t1. Argument ist kein string." << std::endl << std::endl;
		return 0;
	}

	//	Das Item entfernen
	TheGame::Instance()->getItemList()->removeItem(id, count);

	//	Es gibt keinen Rückgabewert
	return 0;
}

int LuaRegistrations::l_getItemCount(lua_State* pLuaState)
{
	/*	Diese Funktion nimmt einen string id entgegen und gibt 
	 *	die Anzahl der Items mit der id im INventar des Spielers zurück.
	 */
	 //	Es gibt einen Parameter: id
	const char* id = luaL_checkstring(pLuaState, 2);

	if (!id)
	{
		TheGame::Instance()->logError() << "LuaRegistrations::l_getItemCount():\n\t1. Argument ist kein string." << std::endl << std::endl;
		return 0;
	}

	//	Die Anzahl der Items speichern
	int count = TheGame::Instance()->getItemList()->getItemCount(id);

	//	Die Zahl auf den Stack pushen, damit lua ihn als Rückgabewert annimmt
	lua_pushinteger(pLuaState, count);

	//	Die Anzahl wird zurück gegeben
	return 1;
}

int LuaRegistrations::l_getClockTimeSeconds(lua_State* pLuaState)
{
	//	Die Variable auf den Stack pushen
	lua_pushinteger(pLuaState, TheGame::Instance()->getClock()->getTimeInSeconds());

	//	Die Zeit in Sekunden wird zurückgegeben
	return 1;
}

int LuaRegistrations::l_setClockTimeSeconds(lua_State* pLuaState)
{
	//	Die Zahl vom Stack poppen
	int seconds = luaL_checkinteger(pLuaState, 2);

	//	Die Zeit setzen
	TheGame::Instance()->getClock()->setTime(seconds);

	//	Es gibt keinen Rückgabewert
	return 0;
}

int LuaRegistrations::l_addClockTimeSeconds(lua_State* pLuaState)
{
	//	Die Zeit vom Stack poppen
	int seconds = luaL_checkinteger(pLuaState, 2);

	//	Die Geschwindigkeit vom Stack poppen
	int velocity = luaL_checkinteger(pLuaState, 3);

	//	Die Zeit addieren
	TheGame::Instance()->getClock()->addTime(seconds, velocity);

	//	Es gibt keinen Rückgabewert
	return 0;
}

int LuaRegistrations::l_showClock(lua_State* pLuaState)
{
	//	Die flag setzen
	TheGame::Instance()->getClock()->setVisible(true);

	//	Es gibt keinen Rückgabewert
	return 0;
}

int LuaRegistrations::l_hideClock(lua_State* pLuaState)
{
	//	Die flag setzen
	TheGame::Instance()->getClock()->setVisible(false);

	//	Es gibt keinen Rückgabewert
	return 0;
}
