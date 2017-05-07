#pragma once

#include <SDL.h>
#include <vector>
#include "Vector2D.h"

/*	Diese Klasse, welche als Singleton implementiert wird, 
*	um diversen Problemen prophylaktisch vorzubeugen (bei Fragen an Ario wenden),
*	existiert um sämtliche Eingaben vom User (Tastatur, Maus, etc.) entgegenzunehmen,
*	damit die jeweiligen anderen Klassen (wie z.B. "Player"), etwas damit anfangen können.
*/

class InputHandler
{
private:
	//	Notwendig für Singleton-Klasse
	static InputHandler* s_pInstance;


	/*	Warum const "Uint8*"?
	*		->	Die Funktion "SDL_GetKeyboardState" gibt einen Zeiger auf ein Array 
	*			von "unsigned char"s zurück, welches sich immer an der selben Adresse im Speicher befindet. 
	*			Hierbei repräsentiert jedes Element des Arrays eine Taste auf der Tastatur 
	*			(0, wenn sie nicht gedrückt wird, und 1, wenn sie gedrückt wird). 
	*/

	const Uint8* m_aKeyboardState;					//	Array zur Speicherung des Zustands der Tastatur
	Vector2D* m_pMousePosition;						//	Ortsvektor der Mausposition
	std::vector<bool> m_mouseButtonStates;			//	Array (Vektor) zur Speicherung des Zustands der Maustasten

private:
	InputHandler();									//	Konstruktor
	~InputHandler();								//	Destruktor

	void onKeyDown();								//	Wird aufgerufen, sobald eine Taste gedrückt wird
	void onKeyUp();									//	Wird aufgerufen, sobald eine Taste losgelassen wird
	void onMouseMotion(const SDL_Event& event);			//	Wird aufgerufen, sobald die Maus bewegt wird
	void onMouseButtonDown(const SDL_Event& event);		//	Wird aufgerufen, sobald eine Maustaste gedrückt wird
	void onMouseButtonUp(const SDL_Event& event);			//	Wird aufgerufen, sobald eine Maustaste losgelassen wird

public:
	void handleInput();								//	Im Grunde werden hier nur Ereignisse gepollt

	//	getter-Funktionen
	bool isKeyDown(SDL_Scancode);					//	Prüft, ob eine bestimmte Taste gedrückt wurde
	Vector2D getMousePosition() const;					//	Gibt den Ortsvektor der Maus zurück

	std::vector<bool> getMouseButtonState() const { return m_mouseButtonStates; }

	bool getLeftMouseButtonState() const { return m_mouseButtonStates[0]; }
	bool getMiddleMouseButtonState() const { return m_mouseButtonStates[1]; }
	bool getRightMouseButtonState() const { return m_mouseButtonStates[2]; }


	/*	Wichtig für Singleton-Klasse
	*	'destroy()' ist der "Ersatz-Desktruktor"
	*/
	static InputHandler* Instance();
	static void destroy();
};

typedef InputHandler TheInputHandler;