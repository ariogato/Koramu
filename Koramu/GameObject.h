#pragma once
#include <vector>
#include "Vector2D.h"
#include "ObjectRectangle.h"
#include <string>

class ParamLoader;
class Player;

/*	Abstrakte Klasse von der jede Klasse, die etwas auf dem 
*	Bildschirm macht erbt.
*/

class GameObject
{
protected:
	std::string m_uniqueId;						//	Einzigartige ID, die manuell gesetzt und danach unveränderbar ist
	bool m_isVisible;									//	Indikator, ob das Objekt gerendert werden soll

public:
	GameObject()										//	Konstruktor
		: m_isVisible(true)
	{}

	virtual ~GameObject() {}							//	Destruktor

	/*	Der Destruktor sollte virtual gesetzt werden,
	*	um  undefiniertes Verhalten zu verhindern.
	*	Für mehr Info, siehe unser Wiki (Cpp Tipps -> Abstrakte Klassen)
	*/

	//	Folgende Memberfunktionen werden erst in der abgeleiteten Klasse definiert
	virtual void load(const ParamLoader& parameters) = 0;
	virtual void update() = 0;
	virtual void draw(const Vector2D& layerPosition) = 0;

	virtual void collision() = 0;
	virtual void onCreate() = 0;
	virtual void interact(Player* pPlayer) = 0;

	//	ssshhh (Workaround) - Implementierung in "SDL_GameObject"
	virtual Vector2D getVelocity() const = 0;
	virtual Vector2D getPosition() const = 0;
	virtual int getWidth() const = 0;
	virtual int getHeight() const = 0;
	virtual std::string getTextureId() const = 0;
	virtual std::vector<ObjectRectangle> getCollisionRects() const = 0;
	
	//	getter-Funktionen
	std::string getUniqueId() const { return m_uniqueId; }
	bool isVisible() const { return m_isVisible; }

	//	setter-Funktionen
	void setVisibility(bool b) { m_isVisible = b; }

	//	Für Abstraktion
	virtual void destroy() = 0;
};