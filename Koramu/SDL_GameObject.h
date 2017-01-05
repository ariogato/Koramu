#pragma once

#include <string>
#include "GameObject.h"

/*	Diese Klasse erbt von der abstrakten Klasse GameObject.
*	Andere Klassen wie der Player wiederum erben von SDL_GameObject
*
*	Die Klasse beschreibt alle Objekte, die mit Texturen repräsentiert werden.
*	Trotzdem wird kaum eine Instanz von SDL_GameObject vorhanden sein, weil 
*	fast alles mehr Memberfunktionen und -variablen braucht als hier vorhanden.
*
*	Bsp.:
*		- Player braucht z.B. Methode walk()
*		- Ein Button braucht eine Methode onClick()
*/

class SDL_GameObject : protected GameObject
{
private:
	//	TODO: Vector2D als Ortsvektor
	int m_numFrames;								//	Die Anzahl der Frames auf dem Spritesheet (in einer Reihe)
	int m_animSpeed;								//	Die Animationsgeschwindigkeit (Wie schnell die Bilder hintereinander abgespielt werden)
	std::string m_textureId;						//	Die Id, unter der die Textur im TextureManager gespeichert wurde

public:


};