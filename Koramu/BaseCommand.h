#pragma once

class SDL_GameObject;

/*	Jeder Befehl hat einen Typ (z.B. MoveCommand, ...), damit man ihn identifizieren kann.
 *	Alle m�glichen Typen sind in dieser enumeration festgehalten. 
 *	
 *	Der Array speichert die Namen der Befehlstypen f�r eventuelle Bildschirmausgaben oder �hnlichem.
 */

enum COMMAND_TYPE
{
	COMMAND_MOVE = 0,
};


static const char* COMMAND_TYPE_STRING[] =
{ "COMMAND_MOVE", };

/*	Diese Klasse beschreibt einen beliebigen Befehl f�r ein Objekt.
 *	Damit man beispielsweise nicht bei jedem Frame die Position aktualisieren muss, gibt es
 *	die Commands, die man einmal (per Script) setzt, die dann das Objekt Frame f�r Frame
 *	bewegen, bis die Zielposition erreicht wurde.
 *	
 *	Dies hier ist die Basisklasse, von der alle Befehle erben. Sie alle m�ssen die 
 *	Funktion 'update()' implementieren.
 */
class BaseCommand
{
protected:
	COMMAND_TYPE m_type;								//	Typ des Befehls
	bool m_done;										//	Flag, die gesetzt wird sobald der Befehl ausgef�hrt wurde (soll von CommandQueue gelesen werden)

public:
	BaseCommand()								//	Konstruktor	
		: m_type(COMMAND_MOVE), m_done(false)
	{}
	virtual ~BaseCommand() {}							//	Destruktor

	virtual void update(SDL_GameObject& object) = 0;	//	Diese Funktion legt im Grunde genommen fest, was f�r ein Befehl vorliegt

	//	getter-Funktionen
	COMMAND_TYPE getType() const { return m_type; }
	bool isDone() const { return m_done; }
};
