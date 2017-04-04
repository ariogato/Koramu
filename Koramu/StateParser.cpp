#include "StateParser.h"
#include "Game.h"

StateParser::StateParser()
	: m_pDocument(new XMLDocument())
{
}

StateParser::~StateParser()
{
	//	Pointer löschen, um einem Speicherleck vorzubeugen.
	delete m_pDocument;
}

bool StateParser::parse(std::string filename, FiniteStateMachine::GameStateID stateID)
{
	//	Laden des Dokuments (anhand des übergebenen Dateinamens).
	if(m_pDocument->LoadFile(filename.c_str()))
	{
		/*	Beim Laden ist ein Fehler passiert.
		 *	Diesen Fehler loggen wir.
		 */
		TheGame::Instance()->logError() << "StateParser::parse(): " << filename << " konnte nicht geladen werden. " << m_pDocument->ErrorName() << std::endl;
		//	Wir können nicht mit dem Parsen fortfahren. Wir geben "false" zurück.
		return false;
	}

	//	Ermitteln des Wurzelelementes
	XMLElement* stateRoot = m_pDocument->RootElement();
	if (stateRoot == nullptr)
	{
		/*	Die XML-Datei besitzt kein Wurzelement und ist demnach leer. 
		 *	Diesen Fehler loggen wir.
		 */
		TheGame::Instance()->logError() << "StateParser::parse(): " << filename << " hat kein <states>-Element." << std::endl;
		//	Wir können nicht mit dem Parsen fortfahren. Wir geben "false" zurück.
		return false;
	}

	//	Aufrufen der Methode zum Laden der Texturen und überprüfen, ob das Laden erfolgreich war.
	if(!loadTextures(stateRoot->FirstChildElement("textures")))
	{
		TheGame::Instance()->logError() << "StateParser::parse(): " << filename << ": laden der Texturen fehlgeschlagen." << std::endl;
		//	Wir können nicht mit dem Parsen fortfahren. Wir geben "false" zurück.
		return false;
	}

	/*	Aufrufen der Methode zum Laden der "GameObjects" und überprüfen, ob das Laden erfolgreich war.
	 *	Der zu ladende "GameState" wird anhand der übergeben "stateID" ermittelt. Für weitere Informitionen siehe "GameState.h".
	 */
	if(!loadGameObjects(stateRoot->FirstChildElement(FiniteStateMachine::stateNames[stateID])))
	{
		TheGame::Instance()->logError() << "StateParser::parse(): " << filename << ": laden der 'GameObjects' fehlgeschlagen-" << std::endl;
		//	Das Parsen war nicht erfolgreich. Wir geben "false" zurück.
		return false;
	}

	//	Das Parsen ist reibungsfrei abgelaufen. Wir geben "true" zurück.
	return true;
}

bool StateParser::loadTextures(XMLElement* pTextureNode)
{
	//	Überprüfen, ob Überhaupt ein XML-Element mit dem Namen "textures" existiert.
	if (pTextureNode == nullptr)
	{
		/*	Die XML-Datei besitzt kein Element mit dem Namen "textures".
		 *	Diesen Fehler loggen wir. 
		 */
		TheGame::Instance()->logError() << "StateParser::loadTextures(): Die XML-Datei besitzt kein Element mit dem Namen 'textures'." << std::endl;
		//	Wir können nicht mit dem Laden der Texturen forfahren. Wir geben "false" zurück.
		return false;
	}
	std::cout << "huiiii" << std::endl;
	return true;
}

bool StateParser::loadGameObjects(XMLElement* pStateNode)
{
	//	Überprüfen, ob Überhaupt ein XML-Element mit dem Namen des gewünschten States existiert.
	if (pStateNode == nullptr)
	{
		/*	Die XML-Datei besitzt kein Element mit dem Namen des gewünschten States.
		*	Diesen Fehler loggen wir.
		*/
		TheGame::Instance()->logError() << "StateParser::loadTextures(): Die XML-Datei besitzt kein Element mit dem Namen des gewünschten States" << std::endl;
		//	Wir können nicht mit dem Laden der "GameObjects" forfahren. Wir geben "false" zurück.
		return false;
	}
	std::cout << "huiiii42" << std::endl;
	return true;
}
