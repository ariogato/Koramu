#include "Logger.h"

Logger::Logger(std::ostream& os)
	: m_outputStream(&os), heap(false)
{
	/*	Der Funktion kann ein ostream Objekt �bergeben werden.
	*	Wenn nichts �bergeben wird, wird einfach std::cout verwendet.
	*	(Default-Parameter)
	*
	*	Danach wird unser Stream mit dem �bergebenen initialisiert
	*/
}

Logger::Logger(std::string filename)
{
	/*	Wir brauchen ein neues ofstream Objekt (ein Objekt zum Schreiben in Dateien),
	*	denn wir wollen auch checken ob die Datei ge�ffnet wurde.
	*
	*	Der zweite Parameter beschreibt den Modus:
	*		out : Wir wollen in die Datei schreiben
	*		trunc : Die Datei wird vor dem �ffnen gel�scht
	*
	*	Das '|' ist der bin�re UND Operator, der einfach die Modi kombiniert
	*/
	std::ofstream* ofs = new std::ofstream(filename.c_str(), std::fstream::out | std::fstream::trunc);

	//	Checken ob die Datei ge�ffnet wurde
	if (!ofs->is_open())
	{
		//	Eine Fehlermeldung ausgeben
		std::cerr << "Logger::Logger(std::string):\nDie Log-Datei \"" << filename << "\" konnte nicht ge�ffnet werden!"
				  << std::endl << "std::cout wird nun als Logersatz verwendet!" << std::endl;

		//	outputStream mit std::cout initialisieren
		m_outputStream = &std::cout;

		//	Auch speichern, dass outputStream mit std::cout initialisiert wurde
		heap = false;
	}

	/*	Ab hier ist klar, dass die Datei ge�ffnet wurde.
	*	Nun m�ssen wir outputStream mit ofs initialisieren.
	*/
	m_outputStream = ofs;

	//	Speichern, dass outputStream selber bef�llt wurde
	heap = true;
}

Logger::~Logger()
{
	/*	outputStream freigeben, falls wir ihn selber alloziiert haben 
	*	(d.h. wenn wir in eine Datei geschrieben haben).
	*	Sonst zeigt outputStream auf cout und um cout k�mmert sich schon jemand anderes
	*/

	if (heap) delete m_outputStream;
}

template<typename T>
Logger& operator<<(Logger& log, const T& msg)
{
	*(log.m_outputStream) << msg;
	return log;
}
