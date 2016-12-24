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
	std::ostream& outputStream;		//	Der Stream, dem die Nachricht übergeben wird (kann auch ofstream sein)

public:
	Logger();						//	Konstruktor ohne Parameter
	Logger(std::string);						//	Konstruktor mit Parametern
	~Logger();						//	Destruktor
};