#pragma once

#include "UniqueIdGenerator.h"

class ParamLoader;

/*	Abstrakte Klasse von der jede Klasse, die etwas auf dem 
*	Bildschirm macht erbt.
*/

class GameObject
{
protected:
	unsigned long long m_uniqueId;						//	Einzigartige ID, die beim Erstellen gesetzt und danach unveränderbar ist
	bool m_isVisible;									//	Indikator, ob das Objekt gerendert werden soll

public:
	GameObject()										//	Konstruktor
		: m_uniqueId(UniqueIdGenerator::newID()),
		  m_isVisible(true)
	{}

	virtual ~GameObject() {}							//	Destruktor

	/*	Der Destruktor sollte virtual gesetzt werden,
	*	um  undefiniertes Verhalten zu verhindern.
	*	Für mehr Info, siehe unser Wiki (Cpp Tipps -> Abstrakte Klassen)
	*/

	//	Folgende Memberfunktionen werden erst in der abgeleiteten Klasse definiert
	virtual void load(const ParamLoader& parameters) = 0;
	virtual void update() = 0;
	virtual void draw() = 0;

	virtual void collision() = 0;
	
	//	getter-Funktionen
	unsigned long long getUniqueId() const { return m_uniqueId; }
	bool isVisible() const { return m_isVisible; }

	//	setter-Funktionen
	void setVisibility(bool b) { m_isVisible = b; }

	//	Für Abstraktion
	virtual void destroy() = 0;
};