#pragma once

class ParamLoader;

/*	Abstrakte Klasse von der jede Klasse, die etwas auf dem 
*	Bildschirm macht erbt.
*/

class GameObject
{
public:
	GameObject() {}						//	Konstruktor
	virtual ~GameObject() {}			//	Destruktor

	/*	Der Destruktor sollte virtual gesetzt werden,
	*	um  undefiniertes Verhalten zu verhindern.
	*	Für mehr Info, siehe unser Wiki (Cpp Tipps -> Abstrakte Klassen)
	*/

	//	Folgende Memberfunktionen werden erst in der abgeleiteten Klasse definiert
	virtual void load(const ParamLoader& parameters) = 0;
	virtual void update() = 0;
	virtual void draw() = 0;

	virtual void collision() = 0;
	
	//	Für Abstraktion
	virtual void destroy() = 0;
};