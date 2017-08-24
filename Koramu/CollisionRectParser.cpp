#include "CollisionRectParser.h"
#include "tinyxml2.h"
#include "Game.h"
#include "ParamLoader.h"


using namespace tinyxml2;

CollisionRectParser::CollisionRectParser()
{
}

CollisionRectParser::~CollisionRectParser()
{
}

bool CollisionRectParser::parse(std::string filename, std::vector<GameObject*>* pObjects)
{
#pragma region Document
	//	Ein neues Dokument wird erstellt und ge�ffnet
	XMLDocument* pDocument = new XMLDocument();

	//	Laden des Dokuments (anhand des �bergebenen Dateinamens).
	if (pDocument->LoadFile(filename.c_str()))
	{
		/*	Beim Laden ist ein Fehler passiert.
		*	Diesen Fehler loggen wir.
		*/
		TheGame::Instance()->logError() << "CollisionRectParser::parse(): \n\t" << filename << " konnte nicht geladen werden. " << pDocument->ErrorName() << std::endl << std::endl;

		//	Wir k�nnen nicht mit dem Parsen fortfahren. Wir geben "false" zur�ck.
		return false;
	}
#pragma endregion 

#pragma region StateRoot
	//	Ermitteln des Wurzelelementes
	XMLElement* pObjectRoot = pDocument->RootElement();
	if (!pObjectRoot)
	{
		/*	Die XML-Datei besitzt kein Wurzelelement und ist demnach leer.
		*	Diesen Fehler loggen wir.
		*/
		TheGame::Instance()->logError() << "CollisionRectParser::parse(): \n\t" << filename << " hat kein <objects>-Element." << std::endl << std::endl;

		//	Wir k�nnen nicht mit dem Parsen fortfahren. Wir geben "false" zur�ck.
		return false;
	}
#pragma endregion 

	//	Hier werden alle Daten des GameObjects gespeichert
	ParamLoader parameters;										//	Parameter Objekt f�r load()
	int x, y, width, height;									//	Integer Daten 
	int counter = 1;											//	Hilfsvariable f�r die Fehlerausgabe

	//	Vector zur Zwischenspeicherung der "collisionRects"
	std::vector<ObjectRectangle> tempCollisionRects;

	//	�ber die einzelnen Objekttypen in <objects> iterieren
	for(XMLElement* o = pObjectRoot->FirstChildElement(); o != nullptr; o = o->NextSiblingElement())
	{
		//	�ber die "collisionRects" des aktuellen Objekttypen iterieren
		for(XMLElement* c = o->FirstChildElement(); c != nullptr; c = c->NextSiblingElement())
		{
			/*	Die Daten des jeweiligen "collisionRects" aus der XML-Datei herausnehmen.
			 *	Gleichzeitig werden die Daten validiert (bei den int-werten), indem der R�ckgabewert
			 *	der Funktion "Query[..]Attribute" (XMLError) gelesen wird.
			 */

			//	xPos
			if (c->QueryAttribute("xPos", &x))
			{
				TheGame::Instance()->logError() << "CollisionRectParser::parse(): \n\tDas " << counter << ". 'collisonRect' besitzt keine xPos" << std::endl << std::endl;
				return false;
			}
			//	yPos
			if (c->QueryAttribute("yPos", &y))
			{
				TheGame::Instance()->logError() << "CollisionRectParser::parse(): \n\tDas " << counter << ". 'collisonRect' besitzt keine yPos" << std::endl << std::endl;
				return false;
			}
			//	width
			if (c->QueryAttribute("width", &width))
			{
				TheGame::Instance()->logError() << "CollisionRectParser::parse(): \n\tDas " << counter << ". 'collisonRect' besitzt keine width" << std::endl << std::endl;
				return false;
			}
			//	height
			if (c->QueryAttribute("height", &height))
			{
				TheGame::Instance()->logError() << "CollisionRectParser::parse(): \n\tDas " << counter << ". 'collisonRect' besitzt keine height" << std::endl << std::endl;
				return false;
			}

			//	Parameter des "ObjectRectangle"s abspeichern
			parameters.setX(x);
			parameters.setY(y);
			parameters.setWidth(width);
			parameters.setHeight(height);
			
			//	Erstellen des "ObjectRectangle"s mit den gew�nschten Attributen ("parameters")
			ObjectRectangle tempRectangle;
			tempRectangle.load(parameters);
			tempRectangle.setVisible(true);
			tempRectangle.setShowText(false);
			tempRectangle.setColor(0, 0, 255, 255);

			//	Hinzuf�gen des "collisionRect"s in den Vector mit den anderen "collisionRects"s des Objekttypen
			tempCollisionRects.push_back(tempRectangle);
		}

		//	�ber die �bergebenen "GameObject"s iterieren
		for (auto gameObject : *pObjects)
		{
			//	�berpr�fen, ob "gameObject" den selben Typ (entspricht der TextureID!) hat, wie das Objekt, das gerade geparst wird 
			if (gameObject->getTextureId() == o->Attribute("name"))
			{
				//	Casten, um die "collisionRect"s zum SDL_GameObject hinzuf�gen zu k�nnen
				SDL_GameObject* gameObjectSDL = dynamic_cast<SDL_GameObject*>(gameObject);

				/*	�ber die geparsten "collisionRect"s iterieren und die Position des Objektes zum Ortsvektor des "collisionRect"s addieren.
				 *	Dies muss getan werden, da die Position in der xml-Datei logischerweise relativ zum Objekt angegeben wird.
				 */	
				for(int i = 0; i < tempCollisionRects.size(); i++)
				{
					tempCollisionRects[i].positionVector.setX(tempCollisionRects[i].getX() + gameObject->getPosition().getX());
					tempCollisionRects[i].positionVector.setY(tempCollisionRects[i].getY() + gameObject->getPosition().getY());
				}

				//	"collsionRect"s zum "SDL_GameObject" hinzuf�gen
				gameObjectSDL->addCollisionRects(tempCollisionRects);

				/*	�ber die geparsten "collisionRect"s iterieren und die Position des Objektes wieder vom Ortsvektor des "collisionRect"s subtrahieren.
				*	Dies muss getan werden, damit die Position wieder relativ zum Objekt gespeichert ist 
				*	und f�r das n�chste Objekt korrekt berechnet wird.
				*/
				for (int i = 0; i < tempCollisionRects.size(); i++)
				{
					tempCollisionRects[i].positionVector.setX(tempCollisionRects[i].getX() - gameObject->getPosition().getX());
					tempCollisionRects[i].positionVector.setY(tempCollisionRects[i].getY() - gameObject->getPosition().getY());
				}
			}
		}
	}
	//	Es wurde erfolgreich geparst
	return true;
}
