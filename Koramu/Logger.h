#pragma once

#include <iostream>
#include <fstream>
#include <string>

/*	Eine Klasse, die geschrieben wurde um den
*	Logging-Prozess zu optimieren.
*	Um von Consolen-Output zu eine Logdatei zu wechseln, muss
*	man den Logger nur mit dem Dateipfad initialisieren.
*/

class Logger
{
private:
	std::ostream* m_pOutputStream;			//	Der Stream, dem die Nachricht �bergeben wird (kann auch ofstream sein)
	bool m_heap;							//	Gibt an ob wir outputStream selber bef�llt haben oder nicht (wichtig f�r Destruktor)

public:
	Logger(std::ostream& = std::cout);						//	Konstruktor f�r cout
	Logger(std::string);						//	Konstruktor f�r eine Logdatei
	~Logger();									//	Destruktor
	

	/*	Ab hier wird der Stream Operator �berladen.
	*	D.h. wir k�nnen in Zukunft unser log Objekt wie std::cout verwenden.
	*	Bsp.:
	*		TheGame::Instance()->logError() << 12 << std::endl << "geil!" << std::endl;
	*
	*	Das Ganze muss in der Headerdatei gemacht werden, weil templates sonst meckern
	*/
	template<class T>
	Logger& operator<<(const T& msg)								//	Hier wird der Operator f�r alle klassischen Datentypen �berladen.
	{
		//	die message wird in dem Stream (Datei oder cout) �bergeben
		*(this->m_pOutputStream) << msg;

		/*	damit Verkettungen funktionieren
		*	Bsp.:
		*		... << 12 << 13 << 14 << 15 << 16 << ...;
		*/
		return *this;
	}


	std::ostream& operator<<(std::ostream& (*f)(std::ostream&))		//	Hier wird der Operator f�r z.B. std::endl �berladen
	{
		//	std::endl ist eigentlich eine Funktion, die bei std::cout auch immer aufgerufen wird
		return f(*m_pOutputStream);
	}

};

