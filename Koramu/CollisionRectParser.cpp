#include "CollisionRectParser.h"
#include "tinyxml2.h"
#include "Game.h"


using namespace tinyxml2;

CollisionRectParser::CollisionRectParser()
{
}

CollisionRectParser::~CollisionRectParser()
{
}

bool CollisionRectParser::parse(std::string filename)
{
#pragma region Document
	//	Ein neues Dokument wird erstellt und geöffnet
	XMLDocument* pDocument = new XMLDocument();

	//	Laden des Dokuments (anhand des übergebenen Dateinamens).
	if (pDocument->LoadFile(filename.c_str()))
	{
		/*	Beim Laden ist ein Fehler passiert.
		*	Diesen Fehler loggen wir.
		*/
		TheGame::Instance()->logError() << "CollisionRectParser::parse(): \n\t" << filename << " konnte nicht geladen werden. " << pDocument->ErrorName() << std::endl << std::endl;

		//	Wir können nicht mit dem Parsen fortfahren. Wir geben "false" zurück.
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
		TheGame::Instance()->logError() << "CollisionRectParser::parse(): \n\t" << filename << " hat kein <object>-Element." << std::endl << std::endl;

		//	Wir können nicht mit dem Parsen fortfahren. Wir geben "false" zurück.
		return false;
	}
#pragma endregion 

//	Über die einzelnen Objekttypen in <objects> iterieren
for(XMLElement* o = pObjectRoot->FirstChildElement(); o != nullptr; o = o->NextSiblingElement())
{
	//	Über die "collisionRects" des aktuellen Objekttypen iterieren
	for(XMLElement* c = o->FirstChildElement(); c != nullptr; c = c->NextSiblingElement())
	{
		
	}
}

}
