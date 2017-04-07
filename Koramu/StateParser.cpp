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

	//	Laden des Dokuments (anhand des �bergebenen Dateinamens).
	if(pDocument->LoadFile(filename.c_str()))
	{
		/*	Beim Laden ist ein Fehler passiert.
		 *	Diesen Fehler loggen wir.
		 */
		TheGame::Instance()->logError() << "StateParser::parse(): \n\t" << filename << " konnte nicht geladen werden. " << pDocument->ErrorName() << std::endl;
		
		//	Wir k�nnen nicht mit dem Parsen fortfahren. Wir geben "false" zur�ck.
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
		
		//	Wir k�nnen nicht mit dem Parsen fortfahren. Wir geben "false" zur�ck.
		return false;
	}

	/*	Wir wollen die Texturen nicht jedes Mal wenn ein Zustandswechsel erfolgt neu parsen.
	 *	Aus diesem Grunde �berpr�fen wir anhand der statischen Membervariable 'hasLoadedTextures',
	 *	ob sie bereits geparst wurden.
	 */
	if (!m_sHasLoadedTextures)
	{
		//	Aufrufen der Methode zum Laden der Texturen und �berpr�fen, ob das Laden erfolgreich war.
		if (!loadTextures(stateRoot->FirstChildElement("textures")))
		{
			TheGame::Instance()->logError() << "StateParser::parse(): \n\t" << filename << ": laden der Texturen fehlgeschlagen." << std::endl;

			//	Wir k�nnen nicht mit dem Parsen fortfahren. Wir geben "false" zur�ck.
			return false;
		}
	}

	/*	Aufrufen der Methode zum Laden der "GameObjects" und �berpr�fen, ob das Laden erfolgreich war.
	 *	Der zu ladende "GameState" wird anhand der �bergeben "stateID" ermittelt. F�r weitere Informationen siehe "GameState.h".
	 */
	if (!loadGameObjects(stateRoot->FirstChildElement(FiniteStateMachine::stateNames[stateID]), pObjects))
	{
		TheGame::Instance()->logError() << "StateParser::parse(): \n\t" << filename << ": Laden der 'GameObjects' fehlgeschlagen" << std::endl << std::endl;

		//	Das Parsen war nicht erfolgreich. Wir geben "false" zur�ck.
		return false;
	}

	//	Pointer l�schen, um einem Speicherleck vorzubeugen.
	delete pDocument;

	//	Das Parsen ist reibungsfrei abgelaufen. Wir geben "true" zur�ck.
	return true;
}

bool StateParser::loadTextures(XMLElement* pTextureNode)
{
	//	�berpr�fen, ob �berhaupt ein XML-Element mit dem Namen "textures" existiert.
	if (pTextureNode == nullptr)
	{
		/*	Die XML-Datei besitzt kein Element mit dem Namen "textures".
		 *	Diesen Fehler loggen wir. 
		 */
		TheGame::Instance()->logError() << "StateParser::loadTextures(): \n\tDie XML-Datei besitzt kein Element mit dem Namen 'textures'." << std::endl;
		
		//	Wir k�nnen nicht mit dem Laden der Texturen forfahren. Wir geben "false" zur�ck.
		return false;
	}

	//	Wir schauen ob �berhaupt Texturen vorhanden sind
	if (pTextureNode->NoChildren())
	{
		TheGame::Instance()->logError() << "StateParser::loadTextures(): \n\tDie XML-Datei besitzt keine Texturen." << std::endl;
		return false;
	}

	//	Hilfsvariable f�r die Fehlerausgabe
	int counter = 1;

	//	�ber alle Texturen iterieren
	for (XMLElement* e = pTextureNode->FirstChildElement(); e != nullptr; e = e->NextSiblingElement(), counter++)
	{

		//	Die Daten der jeweiligen Texturen aus der XML-Datei herausnehmen
		const char* id = e->Attribute("id");
		const char* path = e->Attribute("path");

#pragma region Validation
		//	Daten auf Validit�t pr�fen
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
	//	�berpr�fen, ob �berhaupt ein XML-Element mit dem Namen des gew�nschten States existiert.
	if (pCurrentStateNode == nullptr)
	{
		/*	Die XML-Datei besitzt kein Element mit dem Namen des gew�nschten States.
		*	Diesen Fehler loggen wir.
		*/
		TheGame::Instance()->logError() << "StateParser::loadTextures(): \n\tDie XML-Datei besitzt kein Element mit dem Namen des gew�nschten States" << std::endl;
		
		//	Wir k�nnen nicht mit dem Laden der "GameObjects" forfahren. Wir geben "false" zur�ck.
		return false;
	}

	//	Wir schauen ob die Liste von GameObjects vorhanden sind
	if (pCurrentStateNode->NoChildren())
	{
		TheGame::Instance()->logError() << "StateParser::loadObjects(): \n\tState ohne GameObject-Liste" << std::endl;
		return false;
	}

	/*	Aufgrund der Struktor der XML-Datei 
	 *	(state enth�lt textures; textures enth�lt einzelne texturen)
	 *	muss hier der tag mit den GameObjects geholt werden
	 */
	XMLElement* pObjectNode = pCurrentStateNode->FirstChildElement("gameObjects");
	
	//	Wir schauen ob �berhaupt Objekte vorhanden sind
	if (pObjectNode->NoChildren() || !pObjectNode)
	{
		TheGame::Instance()->logError() << "StateParser::loadObjects(): \n\tState ohne GameObjects" << std::endl;
		return false;
	}


	//	Hier werden alle Daten des GameObjects gespeichert
	ParamLoader parameters;										//	parameter Objekt f�r load()
	int x, y, width, height, numRows, numCols, animSpeed;		//	integer Daten 
	int counter = 1;											//	Hilfsvariable f�r die Fehlerausgabe

	//	�ber alle Objekte iterieren
	for (XMLElement* e = pObjectNode->FirstChildElement(); e != nullptr; e = e->NextSiblingElement(), counter++)
	{
		/*	Die Daten der jeweiligen Texturen aus der XML-Datei herausnehmen.
		 *	Gleichzeitig werden die Daten validiert (bei den int-werten), indem der R�ckgabewert 
		 *	der Funktion "Query[..]Attribute" (XMLError) gelesen wird.
		 *	
		 *	Einige Attribute m�ssen bei jedem Objekt vorhanden sein:
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

		//	Strings m�ssen extra geladen und validiert werden
		const char* type = e->Attribute("type");
		const char* textureId = e->Attribute("textureId");

#pragma region StringValidation
		//	Strings auf Validit�t pr�fen
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

		//	Ab hier sind alle Daten validiert und werden f�r das Laden der Objekte abgespeichert
		parameters.setTextureId(textureId);
		parameters.setX(x);
		parameters.setY(y);
		parameters.setWidth(width);
		parameters.setHeight(height);
		parameters.setNumCols(numCols);
		parameters.setNumRows(numRows);

		/*	Das gew�nschte Objekt von der 'GameObjectFactory'
		 *	erstellen lassen.
		 *	Anschlie�end wird es mit der Funktion 'GameObject::load()' 
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
