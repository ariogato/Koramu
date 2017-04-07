#include "StateParser.h"
#include "TextureManager.h"
#include "Game.h"
#include "ParamLoader.h"
#include "GameObject.h"
#include "GameObjectFactory.h"

bool StateParser::m_sHasLoadedTextures = false;

StateParser::StateParser()
{
}

StateParser::~StateParser()
{
}

bool StateParser::parse(std::string filename, std::vector<GameObject*>* pObjects, FiniteStateMachine::GameStateID stateID)
{
	XMLDocument* pDocument = new XMLDocument();

	//	Laden des Dokuments (anhand des übergebenen Dateinamens).
	if(pDocument->LoadFile(filename.c_str()))
	{
		/*	Beim Laden ist ein Fehler passiert.
		 *	Diesen Fehler loggen wir.
		 */
		TheGame::Instance()->logError() << "StateParser::parse(): \n\t" << filename << " konnte nicht geladen werden. " << pDocument->ErrorName() << std::endl;
		
		//	Wir können nicht mit dem Parsen fortfahren. Wir geben "false" zurück.
		return false;
	}

	//	Ermitteln des Wurzelelementes
	XMLElement* stateRoot = pDocument->RootElement();
	if (stateRoot == nullptr)
	{
		/*	Die XML-Datei besitzt kein Wurzelement und ist demnach leer. 
		 *	Diesen Fehler loggen wir.
		 */
		TheGame::Instance()->logError() << "StateParser::parse(): \n\t" << filename << " hat kein <states>-Element." << std::endl;
		
		//	Wir können nicht mit dem Parsen fortfahren. Wir geben "false" zurück.
		return false;
	}

	/*	Wir wollen die Texturen nicht jedes Mal wenn ein Zustandswechsel erfolgt neu parsen.
	 *	Aus diesem Grunde überprüfen wir anhand der statischen Membervariable 'hasLoadedTextures',
	 *	ob sie bereits geparst wurden.
	 */
	if (!m_sHasLoadedTextures)
	{
		//	Aufrufen der Methode zum Laden der Texturen und überprüfen, ob das Laden erfolgreich war.
		if (!loadTextures(stateRoot->FirstChildElement("textures")))
		{
			TheGame::Instance()->logError() << "StateParser::parse(): \n\t" << filename << ": laden der Texturen fehlgeschlagen." << std::endl;

			//	Wir können nicht mit dem Parsen fortfahren. Wir geben "false" zurück.
			return false;
		}
	}

	/*	Aufrufen der Methode zum Laden der "GameObjects" und überprüfen, ob das Laden erfolgreich war.
	 *	Der zu ladende "GameState" wird anhand der übergeben "stateID" ermittelt. Für weitere Informationen siehe "GameState.h".
	 */
	if (!loadGameObjects(stateRoot->FirstChildElement(FiniteStateMachine::stateNames[stateID]), pObjects))
	{
		TheGame::Instance()->logError() << "StateParser::parse(): \n\t" << filename << ": Laden der 'GameObjects' fehlgeschlagen" << std::endl << std::endl;

		//	Das Parsen war nicht erfolgreich. Wir geben "false" zurück.
		return false;
	}

	//	Pointer löschen, um einem Speicherleck vorzubeugen.
	delete pDocument;

	//	Das Parsen ist reibungsfrei abgelaufen. Wir geben "true" zurück.
	return true;
}

bool StateParser::loadTextures(XMLElement* pTextureNode)
{
	//	Überprüfen, ob überhaupt ein XML-Element mit dem Namen "textures" existiert.
	if (pTextureNode == nullptr)
	{
		/*	Die XML-Datei besitzt kein Element mit dem Namen "textures".
		 *	Diesen Fehler loggen wir. 
		 */
		TheGame::Instance()->logError() << "StateParser::loadTextures(): \n\tDie XML-Datei besitzt kein Element mit dem Namen 'textures'." << std::endl;
		
		//	Wir können nicht mit dem Laden der Texturen forfahren. Wir geben "false" zurück.
		return false;
	}

	//	Wir schauen ob überhaupt Texturen vorhanden sind
	if (pTextureNode->NoChildren())
	{
		TheGame::Instance()->logError() << "StateParser::loadTextures(): \n\tDie XML-Datei besitzt keine Texturen." << std::endl;
		return false;
	}

	//	Hilfsvariable für die Fehlerausgabe
	int counter = 1;

	//	Über alle Texturen iterieren
	for (XMLElement* e = pTextureNode->FirstChildElement(); e != nullptr; e = e->NextSiblingElement(), counter++)
	{

		//	Die Daten der jeweiligen Texturen aus der XML-Datei herausnehmen
		const char* id = e->Attribute("id");
		const char* path = e->Attribute("path");

#pragma region Validation
		//	Daten auf Validität prüfen
		if (!id)
		{
			TheGame::Instance()->logError() << "StateParser::loadTextures(): \n\tDas " << counter << ". Texturelement besitzt keine ID" << std::endl;
			return false;
		}
		if (!path)
		{
			TheGame::Instance()->logError() << "StateParser::loadTextures(): \n\tDas " << counter << ". Texturelement besitzt keinen Dateipfad" << std::endl;
			return false;
		}
#pragma endregion
			
		//	Die validierten Daten laden
		if ( TheTextureManager::Instance()->load(id, path, TheGame::Instance()->getRenderer())){}
	}

	/*	Festhalten, dass die Texturen jetzt geladen wurden.
	 *	Es soll kein weiteres Mal geschehen.
	*/
	m_sHasLoadedTextures = true;

	//	Das Laden der Texturen ist hier reibungsfrei abgelaufen
	TheGame::Instance()->logStandard() << "StateParser::loadTextures(): \n\tTexturen erfolgreich geladen" << std::endl << std::endl;
	return true;
}

bool StateParser::loadGameObjects(XMLElement* pCurrentStateNode, std::vector<GameObject*>* pObjects)
{
	//	Überprüfen, ob überhaupt ein XML-Element mit dem Namen des gewünschten States existiert.
	if (pCurrentStateNode == nullptr)
	{
		/*	Die XML-Datei besitzt kein Element mit dem Namen des gewünschten States.
		*	Diesen Fehler loggen wir.
		*/
		TheGame::Instance()->logError() << "StateParser::loadTextures(): \n\tDie XML-Datei besitzt kein Element mit dem Namen des gewünschten States" << std::endl;
		
		//	Wir können nicht mit dem Laden der "GameObjects" forfahren. Wir geben "false" zurück.
		return false;
	}

	//	Wir schauen ob die Liste von GameObjects vorhanden sind
	if (pCurrentStateNode->NoChildren())
	{
		TheGame::Instance()->logError() << "StateParser::loadObjects(): \n\tState ohne GameObject-Liste" << std::endl;
		return false;
	}

	/*	Aufgrund der Struktor der XML-Datei 
	 *	(state enthält textures; textures enthält einzelne texturen)
	 *	muss hier der tag mit den GameObjects geholt werden
	 */
	XMLElement* pObjectNode = pCurrentStateNode->FirstChildElement("gameObjects");
	
	//	Wir schauen ob überhaupt Objekte vorhanden sind
	if (pObjectNode->NoChildren() || !pObjectNode)
	{
		TheGame::Instance()->logError() << "StateParser::loadObjects(): \n\tState ohne GameObjects" << std::endl;
		return false;
	}


	//	Hier werden alle Daten des GameObjects gespeichert
	ParamLoader parameters;										//	parameter Objekt für load()
	int x, y, width, height, numRows, numCols, animSpeed;		//	integer Daten 
	int counter = 1;											//	Hilfsvariable für die Fehlerausgabe

	//	Über alle Objekte iterieren
	for (XMLElement* e = pObjectNode->FirstChildElement(); e != nullptr; e = e->NextSiblingElement(), counter++)
	{
		/*	Die Daten der jeweiligen Texturen aus der XML-Datei herausnehmen.
		 *	Gleichzeitig werden die Daten validiert (bei den int-werten), indem der Rückgabewert 
		 *	der Funktion "Query[..]Attribute" (XMLError) gelesen wird.
		 *	
		 *	Einige Attribute müssen bei jedem Objekt vorhanden sein:
		 *		type, textureId,
		 *		xPos, yPos,
		 *		width, height,
		 *		numRows, numCols
		 *		
		 *	Andere Attribute sind optional, weshalb sie auch nicht validiert werden.
		 */

		//	xPos
		if (e->QueryAttribute("xPos", &x))
		{
			TheGame::Instance()->logError() << "StateParser::loadObjects(): \n\tDas " << counter << ". Objekt besitzt keine xPos" << std::endl << std::endl;
			return false;
		}
		//	yPos
		if (e->QueryAttribute("yPos", &y))
		{
			TheGame::Instance()->logError() << "StateParser::loadObjects(): \n\tDas " << counter << ". Objekt besitzt keine yPos" << std::endl << std::endl;
			return false;
		}
		//	width
		if (e->QueryAttribute("width", &width))
		{
			TheGame::Instance()->logError() << "StateParser::loadObjects(): \n\tDas " << counter << ". Objekt besitzt keine width" << std::endl << std::endl;
			return false;
		}
		//	height
		if (e->QueryAttribute("height", &height))
		{
			TheGame::Instance()->logError() << "StateParser::loadObjects(): \n\tDas " << counter << ". Objekt besitzt keine height" << std::endl << std::endl;
			return false;
		}
		//	numRows
		if (e->QueryAttribute("numRows", &numRows))
		{
			TheGame::Instance()->logError() << "StateParser::loadObjects(): \n\tDas " << counter << ". Objekt besitzt keine numRows" << std::endl << std::endl;
			return false;
		}
		//	numCols
		if (e->QueryAttribute("numCols", &numCols))
		{
			TheGame::Instance()->logError() << "StateParser::loadObjects(): \n\tDas " << counter << ". Objekt besitzt keine numCols" << std::endl << std::endl;
			return false;
		}

		//	Strings müssen extra geladen und validiert werden
		const char* type = e->Attribute("type");
		const char* textureId = e->Attribute("textureId");

#pragma region StringValidation
		//	Strings auf Validität prüfen
		if (!type)
		{
			TheGame::Instance()->logError() << "StateParser::loadObjects(): \n\tDas " << counter << ". Objekt besitzt keinen Typ" << std::endl << std::endl;
			return false;
		}
		if (!textureId)
		{
			TheGame::Instance()->logError() << "StateParser::loadObjects(): \n\tDas " << counter << ". Objekt besitzt keine textureId" << std::endl << std::endl;
			return false;
		}
#pragma endregion

		//	Optionale Attribute
		e->QueryIntAttribute("animSpeed", &animSpeed);

		//	Ab hier sind alle Daten validiert und werden für das Laden der Objekte abgespeichert
		parameters.setTextureId(textureId);
		parameters.setX(x);
		parameters.setY(y);
		parameters.setWidth(width);
		parameters.setHeight(height);
		parameters.setNumCols(numCols);
		parameters.setNumRows(numRows);

		/*	Das gewünschte Objekt von der 'GameObjectFactory'
		 *	erstellen lassen.
		 *	Anschließend wird es mit der Funktion 'GameObject::load()' 
		 *	mit den aus der XML-Datei herausgenommenen Daten beladen.
		 *	
		 *	Zuletzt wird das Objekt in die Liste (den 'std::vector') geschoben.
		 *	Dadurch, dass 'pObjects' ein Pointer auf einen 'std::vector' ist, muss nichts returnt werden.
		 *	Es wird direkt in die Speicheradresse geschrieben.
		 */
		GameObject* objectToLoad = TheGameObjectFactory::Instance()->create(type);

		//	Ermitteln ob das Objekt erfolgreich geladen wurde
		if (!objectToLoad)
		{
			TheGame::Instance()->logError() << "StateParser::loadGameObjects(): \n\tDas Objekt vom Typ " << type << " konnte nicht erstellt werden" << std::endl << std::endl;

			return false;
		}

		objectToLoad->load(parameters);
		pObjects->push_back(objectToLoad);
	}

	//	Das Laden der GameObjects ist hier reibungsfrei abgelaufen
	TheGame::Instance()->logStandard() << "StateParser::loadObjects(): \n\tObjekte erfolgreich geladen" << std::endl << std::endl;
	return true;
}
