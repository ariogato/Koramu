#pragma once

#include "Vector2D.h"

class SDL_GameObject;

/*	Diese Klasse ist im Grunde genommen ein Wrapper um 'GameObject'.
 *	Sie existiert, um das Rendern und Updaten der 'View' Klasse
 *	zu vereinfachen. Dies wird bewerkstelligt, indem einige Rechenoperationen
 *	beim RenderObject stattfiden, statt direkt bei der 'View' Klasse selbst.
 */

class RenderObject
{
private:
	SDL_GameObject* m_pReferenceObject;										//	Zeiger auf das Referenz 'SDL_GameObject'
	Vector2D m_renderPosition;												//	Position (auf dem Koordinatensystem des Fensters) an der das Objekt gerendert werden soll
	Vector2D m_referenceDistance;											//	Distanz zu einem Referenzpunkt (meist Ortsvektor der Kamera)

	//	Utility-Funktionen
	void calculateReferenceDistance(Vector2D& referencePoint);				//	Rechnet die Distanz des Objektes zum übergebenen Punkt aus (meist zur 'Camera')

public:
	explicit RenderObject(SDL_GameObject* pRefObj);							//	Konstruktor
	~RenderObject();														//	Destruktor
	RenderObject(const RenderObject& r);									//	Kopierkonstruktor

	
	//	Die klassischen Funktionen
	void update();															//	Aktualisieren der Werte
	void render();															//	Die 'render()'-Funktion ist wieder lediglich ein Wrapper

	//	getter-Funktionen
	SDL_GameObject* getGameObject() { return m_pReferenceObject; }
	Vector2D getReferenceDistance() const { return m_referenceDistance; }
	Vector2D getRenderPosition() const { return m_renderPosition; }
};
