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
	 */

	//	Es wird gecheckt, ob 'nullptr' übergeben wurde
	if (!object)
	{
		TheGame::Instance()->logError() << "Camera::centerOnGameObject():\n\tEs wurde nullptr übergeben!" << std::endl << std::endl;
		return;
	}

	m_pCenterObject = object;
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
		 *		2. Danach soll von den Komponenten jeweils Breite bei x bzw. Höhe bei y
		 *		   abgezogen werden.
		 *		3. Zuletzt soll die Hälfte der Breite bei x bzw. Höhe bei y zu den Komponenten addiert werden,
		 *		   damit das 'SDL_GameObject' tatsächlich ganz in der Mitte der Kamera ist. 
		 */

		m_positionVector = m_pCenterObject->getPosition();
		
		m_positionVector.setX(m_positionVector.getX() - static_cast<float>(m_cameraWidth));
		m_positionVector.setY(m_positionVector.getY() - static_cast<float>(m_cameraHeight));

		m_positionVector.setX(m_positionVector.getX() + static_cast<float>(m_pCenterObject->getWidth()) / 2.0f);
		m_positionVector.setY(m_positionVector.getY() + static_cast<float>(m_pCenterObject->getHeight()) / 2.0f);
	}
	else
	{
		//	Falls kein Objekt vorhanden ist auf (0 | 0) setzen
		m_positionVector.setX(0.0f);
		m_positionVector.setY(0.0f);
	}
}
