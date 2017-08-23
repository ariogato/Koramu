#include "UniqueIdGenerator.h"
#include "Game.h"

//	Den statischen Member global initialisieren
unsigned long long UniqueIdGenerator::m_currentTopNumber = 0;

UniqueIdGenerator::UniqueIdGenerator()
{}


UniqueIdGenerator::~UniqueIdGenerator()
{}


unsigned long long UniqueIdGenerator::newID()
{
	/*	Checken ob der maximale ID Wert überschritten wird (unwahrscheinlich)
	*	Zu diesem Zeitpunkt habe ich noch keine gescheite Prozedur, die mit diesem Fehler umgehen kann,
	*	weshalb der Fehler lediglich geloggt wird
	*/
	if (++m_currentTopNumber == 0)
	{
		TheGame::Instance()->logError() << "UniqueIdGenerator::newID(): \n\tDer maximale ID Wert wurde überschritten!" << std::endl << std::endl;
	}

	//	Der Variablenwert wurde bereits im if-statement inkrementiert
	return m_currentTopNumber;
}


