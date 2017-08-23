#pragma once

/*	Diese Klasse erstellt eine einzigartige ID,
*	welche lediglich eine fortlaufende Nummer im Zahlenbereich
*	0 - 18.446.744.073.709.551.615 (64-Bit) ist.
*/

class UniqueIdGenerator
{
private:
	static unsigned long long m_currentTopNumber;		//	Die fortlaufende Nummer

public:
	UniqueIdGenerator();							//	Konstruktor
	~UniqueIdGenerator();							//	Destruktor

	static unsigned long long newID();				//	Gibt eine neue einzigartige ID heraus
};
