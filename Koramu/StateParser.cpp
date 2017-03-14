#include "StateParser.h"
#include "Game.h"

StateParser::StateParser()
	: m_pDocument(nullptr)
{
}

StateParser::~StateParser()
{
	delete m_pDocument;
}

bool StateParser::parse(const char* filename)
{
	if(!m_pDocument->LoadFile(filename))
	{
		TheGame::Instance()->logError() << "StateParser: 'states.xml' konnte nicht geladen werden." << std::endl;
		return false;
	}

	XMLNode* stateRoot = m_pDocument->FirstChildElement();
	if (stateRoot == nullptr)
	{
		TheGame::Instance()->logError() << "StateParser: die angegebene Datei hat kein <states>-Element." << std::endl;
		m_pDocument->Clear();
		return false;
	}
	loadTextures();
	loadGameObjects(stateRoot);
	return true;
	
}

bool StateParser::loadTextures()
{
	return false;
}

bool StateParser::loadGameObjects(XMLNode*)
{
	return false;
}
