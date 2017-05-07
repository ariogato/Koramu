#pragma once

#include <SDL.h>
#include <vector>
#include "Vector2D.h"

/*	Diese Klasse, welche als Singleton implementiert wird, 
*	um diversen Problemen prophylaktisch vorzubeugen (bei Fragen an Ario wenden),
*	existiert um s�mtliche Eingaben vom User (Tastatur, Maus, etc.) entgegenzunehmen,
*	damit die jeweiligen anderen Klassen (wie z.B. "Player"), etwas damit anfangen k�nnen.
*/

class InputHandler
{
private:
	//	Notwendig f�r Singleton-Klasse
	static InputHandler* s_pInstance;


	/*	Warum const "Uint8*"?
	*		->	Die Funktion "SDL_GetKeyboardState" gibt einen Zeiger auf ein Array 
	*			von "unsigned char"s zur�ck, welches sich immer an der selben Adresse im Speicher befindet. 
	*			Hierbei repr�sentiert jedes Element des Arrays eine Taste auf der Tastatur 
	*			(0, wenn sie nicht gedr�ckt wird, und 1, wenn sie gedr�ckt wird). 
	*/

	const Uint8* m_aKeyboardState;					//	Array zur Speicherung des Zustands der Tastatur
	Vector2D* m_pMousePosition;						//	Ortsvektor der Mausposition
	std::vector<bool> m_mouseButtonStates;			//	Array (Vektor) zur Speicherung des Zustands der Maustasten

private:
	InputHandler();									//	Konstruktor
	~InputHandler();								//	Destruktor

	void onKeyDown();								//	Wird aufgerufen, sobald eine Taste gedr�ckt wird
	void onKeyUp();									//	Wird aufgerufen, sobald eine Taste losgelassen wird
	void onMouseMotion(const SDL_Event& event);			//	Wird aufgerufen, sobald die Maus bewegt wird
	void onMouseButtonDown(const SDL_Event& event);		//	Wird aufgerufen, sobald eine Maustaste gedr�ckt wird
	void onMouseButtonUp(const SDL_Event& event);			//	Wird aufgerufen, sobald eine Maustaste losgelassen wird

public:
	void handleInput();								//	Im Grunde werden hier nur Ereignisse gepollt

	//	getter-Funktionen
	bool isKeyDown(SDL_Scancode);					//	Pr�ft, ob eine bestimmte Taste gedr�ckt wurde
	Vector2D getMousePosition() const;					//	Gibt den Ortsvektor der Maus zur�ck

	std::vector<bool> getMouseButtonState() const { return m_mouseButtonStates; }

	bool getLeftMouseButtonState() const { return m_mouseButtonStates[0]; }
	bool getMiddleMouseButtonState() const { return m_mouseButtonStates[1]; }
	bool getRightMouseButtonState() const { return m_mouseButtonStates[2]; }


	/*	Wichtig f�r Singleton-Klasse
	*	'destroy()' ist der "Ersatz-Desktruktor"
	*/
	static InputHandler* Instance();
	static void destroy();
};

typedef InputHandler TheInputHandler;