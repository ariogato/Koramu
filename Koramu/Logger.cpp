#include "Logger.h"

Logger::Logger(std::ostream& os)
	: m_outputStream(&os), heap(false)
{
	/*	Der Funktion kann ein ostream Objekt übergeben werden.
	*	Wenn nichts übergeben wird, wird einfach std::cout verwendet.
	*	(Default-Parameter)
	*
	*	Danach wird unser Stream mit dem übergebenen initialisiert
	*/
}

Logger::Logger(std::string filename)
{
	/*	Wir brauchen ein neues ofstream Objekt (ein Objekt zum Schreiben in Dateien),
	*	denn wir wollen auch checken ob die Datei geöffnet wurde.
	*
	*	Der zweite Parameter beschreibt den Modus:
	*		out : Wir wollen in die Datei schreiben
	*		trunc : Die Datei wird vor dem Öffnen gelöscht
	*
	*	Das '|' ist der binäre UND Operator, der einfach die Modi kombiniert
	*/
	std::ofstream* ofs = new std::ofstream(filename.c_str(), std::fstream::out | std::fstream::trunc);

	//	Checken ob die Datei geöffnet wurde
	if (!ofs->is_open())
	{
		//	Eine Fehlermeldung ausgeben
		std::cerr << "Logger::Logger(std::string):\nDie Log-Datei \"" << filename << "\" konnte nicht geöffnet werden!"
				  << std::endl << "std::cout wird nun als Logersatz verwendet!" << std::endl;

		//	outputStream mit std::cout initialisieren
		m_outputStream = &std::cout;

		//	Auch speichern, dass outputStream mit std::cout initialisiert wurde
		heap = false;
	}

	/*	Ab hier ist klar, dass die Datei geöffnet wurde.
	*	Nun müssen wir outputStream mit ofs initialisieren.
	*/
	m_outputStream = ofs;

	//	Speichern, dass outputStream selber befüllt wurde
	heap = true;
}

Logger::~Logger()
{
	/*	outputStream freigeben, falls wir ihn selber alloziiert haben 
	*	(d.h. wenn wir in eine Datei geschrieben haben).
	*	Sonst zeigt outputStream auf cout und um cout kümmert sich schon jemand anderes
	*/

	if (heap) delete m_outputStream;
}

template<typename T>
Logger& operator<<(Logger& log, const T& msg)
{
	*(log.m_outputStream) << msg;
	return log;
}
