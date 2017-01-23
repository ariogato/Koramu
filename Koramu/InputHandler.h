#pragma once

#include <SDL.h>
#include "Game.h"

/*	Diese Klasse, welche als Singleton implementiert wird 
*	(um diverse Probleme prophylaktisch auszurotten. Bei Fragen an Ario wenden),
*	existiert um s�mtliche Eingaben vom User (Tastatur, Maus, etc.) entgegenzunehmen,
*	damit die jeweiligen anderen Klassen (wie z.B. "Player"), etwas damit anfangen k�nnen.
*/

class InputHandler
{
private:
	//	Notwendig f�r Singleton-Klasse
	static InputHandler* s_pInstance;


	/*	Warum const "Uint8*"?
	*		->	Die Funktion "SDL_GetKeyboardState" gibt einen Zeiger auf einen Array 
	*			von "unsigned char"s zur�ck, welcher sich immer an der selben Adresse im Speicher befindet. 
	*			Hierbei repr�sentiert jedes Element des Arrays eine Taste auf der Tastatur 
	*			(0 wenn sie nicht gedr�ckt wird & 1 wenn sie gedr�ckt wird). 
	*/

	const Uint8* m_aKeyboardState;					//	Array zur Speicherung des Zustands der Tastatur

private:
	InputHandler();									//	Konstruktor
	~InputHandler();								//	Destruktor

public:
	bool isKeyDown(SDL_Scancode);					//	Wird aufgerufen um zu pr�fen ob eine bestimmte Taste gedr�ckt wurde

	void handleInput();								//	

	/*	Sowohl "onKeyDown" als auch "onKeyUp" speichern den Zustand der Tastatur,
	*	aber trotzdem haben beide Funktionen eine Daseinsberechtigung, da wir in 
	*	Zukunft vielleicht noch mehr machen wollen, wenn eine Taste gedr�ckt wurde.
	*
	*	Beide Funktionen werden in "handleInput" aufgerufen falls eine Taste
	*	gedr�ckt bzw. losgelassen wurde.
	*/

	void onKeyDown();
	void onKeyUp();

	/*	Wichtig f�r Singleton-Klasse
	*	'destroy()' ist der "Ersatz-Desktruktor"
	*/
	static InputHandler* Instance();
	static void destroy();
};

typedef InputHandler TheInputHandler;