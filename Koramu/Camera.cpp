#include "Camera.h"
#include "SDL_GameObject.h"
#include "Vector2D.h"
#include "Game.h"

Camera::Camera()
	: m_positionVector(0.0f, 0.0f),
	  m_cameraWidth(0), m_cameraHeight(0),
	  m_pCenterObject(nullptr)
{
}

Camera::Camera(int width, int height)
	: m_positionVector(0.0f, 0.0f),
	  m_cameraWidth(width), m_cameraHeight(height),
	  m_pCenterObject(nullptr)
{
}

Camera::~Camera()
{
	/*	Nicht zu löschende Pointer:
	 *		m_pCenterObject:
	 *			Weil das 'ObjectLayer' das 'SDL_GameObject' besitzt und
	 *			somit für die Speicherverwaltung verantwortlich ist.
	 */
}

void Camera::centerOnGameObject(SDL_GameObject* object)
{
	/*	Wir wollen, dass die Kamera stets auf einem Objekt zentriert ist.
	 *	Zu diesem Zwecke speichern wir eine Referenz auf ein Spielobjekt.
	 *	Die Memberfunktion 'update()' errechnet dann aus der Position des Objektes
	 *	die Position der Kamera. (auf der Map)
	 *	
	 *	Es kann auch "nullptr" übergeben werden. Hier wird die Position der Kamera auf (0|0) gesetzt.
	 */
	m_pCenterObject = object;
	update();
}

void Camera::move(Vector2D& vector)
{
	//	Einfache Vektoraddition
	m_positionVector += vector;
}

void Camera::update()
{
	/*	Hier wird der Ortsvektor der Kamera so berrechnet, dass 
	 *	das zentrierte Objekt immer im Mittelpunkt der Kamera steht.
	 */
	
	//	Checken ob überhaupt ein zu zentrierdendes Objekt vorhanden ist
	if (m_pCenterObject)
	{
		/*	Berrechnung:
		 *		1. Zuerst wird der Vektor einfach übernommen.
		 *		2. Danach soll von den Komponenten jeweils die Hälfte der Breite bei x bzw. der Höhe bei y
		 *		   abgezogen werden.
		 *		3. Zuletzt soll die Hälfte der Breite des zu zentrierenden Objekts bei x bzw. der Höhe bei y zu den Komponenten addiert werden,
		 *		   damit das 'SDL_GameObject' tatsächlich ganz in der Mitte der Kamera ist. 
		 */

		Vector2D tempVector = m_pCenterObject->getPosition();
		
		tempVector.setX(tempVector.getX() - static_cast<float>(m_cameraWidth) / 2.0f);
		tempVector.setY(tempVector.getY() - static_cast<float>(m_cameraHeight) / 2.0f);

		tempVector.setX(tempVector.getX() + static_cast<float>(m_pCenterObject->getWidth()) / 2.0f);
		tempVector.setY(tempVector.getY() + static_cast<float>(m_pCenterObject->getHeight()) / 2.0f);

		//	Hier wird sichergestellt, dass die Kamera die Grenzen der Map nicht verlässt.
		
		//	Extremwerte der aktuellen Map
		int maxX = TheGame::Instance()->getCurrentState()->getCurrentMap()->getWidth();
		int maxY = TheGame::Instance()->getCurrentState()->getCurrentMap()->getHeight();

		if (tempVector.getX() <= 0)
		{
			//	Es wird verhindert, dass die Kamera die linke Grenze nicht überschreitet
			tempVector.setX(0.0f);
		}
		if(tempVector.getY() <= 0)
		{
			//	Es wird verhindet, dass die obere Grenze überschritten wird
			tempVector.setY(0.0f);
		}
		if(tempVector.getX() + m_cameraWidth >= maxX)
		{
			//	Es wird verhindert, dass die rechte Grenze überschritten wird
			tempVector.setX(maxX - m_cameraWidth);
		}
		if(tempVector.getY() + m_cameraHeight >= maxY)
		{
			//	Es wird verhindert, dass die untere Grente überschritten wird
			tempVector.setY(maxY - m_cameraHeight);
		}

		//	Der korrekte Vektor wird übernommen
		m_positionVector = tempVector;
	}
	else
	{
		//	Falls kein Objekt vorhanden ist auf (0 | 0) setzen
		m_positionVector.setX(0.0f);
		m_positionVector.setY(0.0f);
	}
}
