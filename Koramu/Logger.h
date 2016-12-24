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
	std::ostream* m_outputStream;			//	Der Stream, dem die Nachricht übergeben wird (kann auch ofstream sein)
	bool heap;								//	Gibt an ob wir outputStream selber befüllt haben oder nicht (wichtig für Destruktor)

public:
	Logger(std::ostream& = std::cout);						//	Konstruktor für cout
	Logger(std::string);						//	Konstruktor für eine Logdatei
	~Logger();									//	Destruktor
	
	template<typename T>
	friend Logger& operator<<(Logger&, const T&);

};

template<typename T>
Logger& operator<<(Logger&, const T&);