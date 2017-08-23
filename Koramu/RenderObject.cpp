#include "RenderObject.h"
#include "SDL_GameObject.h"
#include "View.h"

RenderObject::RenderObject(SDL_GameObject* pRefObj)
	: m_pReferenceObject(pRefObj),
	  m_renderPosition(pRefObj->getPosition()), 
	  m_referenceDistance(0.0f, 0.0f)
{}

RenderObject::~RenderObject()
{
	/*	Nicht zu löschende Pointer:
	*		m_pReferenceObject:
	*			Es ist lediglich eine Referenz auf einen Member,
	*			der das Eigentum vom seinem jeweiligen Layer ist und somit
	*			auch von selbigem gelöscht wird
	*/
}

RenderObject::RenderObject(const RenderObject& r)
	: m_pReferenceObject(r.m_pReferenceObject),
	  m_renderPosition(r.m_renderPosition), 
	  m_referenceDistance(r.m_referenceDistance)
{
	/*	Wir wollen hierbei expilizit, dass der Pointer
	*	'm_pReferenceObject' bei beiden Instanzen ("Kopie & Original") auf
	*	das selbe 'SDL_GameObject' zeigt.
	*/
}

void RenderObject::calculateReferenceDistance(Vector2D& referencePoint)
{
	
}

void RenderObject::update()
{
	//	Den Ortsvektor der Kamera zwischenspeichern
	Vector2D cameraPosition = View::getCamera()->getPositionVector();
	
	//	Die Distanz zum Ortsvektor der Kamera berechnen (+ speichern)
	calculateReferenceDistance(cameraPosition);
}

void RenderObject::render()
{
	m_pReferenceObject->draw();
}

