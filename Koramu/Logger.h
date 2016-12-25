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
	std::ostream* m_pOutputStream;			//	Der Stream, dem die Nachricht übergeben wird (kann auch ofstream sein)
	bool heap;								//	Gibt an ob wir outputStream selber befüllt haben oder nicht (wichtig für Destruktor)

public:
	Logger(std::ostream& = std::cout);						//	Konstruktor für cout
	Logger(std::string);						//	Konstruktor für eine Logdatei
	~Logger();									//	Destruktor
	

	/*	Ab hier wird der Stream Operator überladen.
	*	D.h. wir können in Zukunft unser log Objekt wie std::cout verwenden.
	*	Bsp.:
	*		TheGame::Instance()->logError() << 12 << std::endl << "geil!" << std::endl;
	*/
	template<class T>
	Logger& operator<<(const T& msg)								//	Hier wird der Operator für alle klassischen Datentypen überladen.
	{
		*(this->m_pOutputStream) << msg;
		return *this;
	}


	std::ostream& operator<<(std::ostream& (*f)(std::ostream&))		//	Hier wird der Operator für z.B. std::endl überladen
	{
		return f(*m_pOutputStream);
	}

};

