#include "StateParser.h"
#include "Game.h"

StateParser::StateParser()
	: m_pDocument(new XMLDocument())
{
}

StateParser::~StateParser()
{
	delete m_pDocument;
}

bool StateParser::parse(std::string filename, FiniteStateMachine::GameStateID stateID)
{
	if(m_pDocument->LoadFile(filename.c_str()))
	{
		TheGame::Instance()->logError() << "StateParser::parse(): " << filename << " konnte nicht geladen werden. " << m_pDocument->ErrorName() << std::endl;
		return false;
	}

	XMLElement* stateRoot = m_pDocument->RootElement();
	if (stateRoot == nullptr)
	{
		TheGame::Instance()->logError() << "StateParser::parse(): " << filename << " hat kein <states>-Element." << std::endl;
		return false;
	}

	if(!loadTextures(stateRoot->FirstChildElement("textures")))
	{
		TheGame::Instance()->logError() << "StateParser::parse(): " << filename << " hat kein <textures>-Element." << std::endl;
	}

	if(!loadGameObjects(stateRoot->FirstChildElement(FiniteStateMachine::stateNames[stateID])))
	{
		TheGame::Instance()->logError() << "StateParser::parse(): " << filename << " hat kein " << FiniteStateMachine::stateNames[stateID] << "-Element." << std::endl;
	}
	return true;
}

bool StateParser::loadTextures(XMLElement* pTextureNode)
{
	if (pTextureNode == nullptr) return false;
	std::cout << "huiiii" << std::endl;
	return false;
}

bool StateParser::loadGameObjects(XMLElement* pStateNode)
{
	if (pStateNode == nullptr) return false;
	std::cout << "huiiii42" << std::endl;
	return false;
}
