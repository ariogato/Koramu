#pragma once

#include "Vector2D.h"

class SDL_GameObject;

/*	Dank dieser Klasse wird nicht die gesamte Map gerendert.
 *	Stattdessen wird nur der kleine, zu der Zeit benötigte Teil der Map gerendert.
 *  Dieser Ausschnitt wird durch die Kamera festgelegt.
 *	Somit wird weniger CPU-Leistung benötigt. 
 */

class Camera
{
private:
	Vector2D m_positionVector;						//	Position der Kamera
	int m_cameraWidth;								//	Breite der Kamera
	int m_cameraHeight;								//	Höhe der Kamera
	SDL_GameObject* m_pCenterObject;				//	Objekt auf dem die Kamera zentriert ist

public:
	Camera();										//	Konstruktor
	Camera(int width, int height);
	~Camera();										//	Destruktor
	
	void centerOnGameObject(SDL_GameObject* object);	//	Kamera wird auf das übergebene Objekt zentriert
	void move(Vector2D& vector);						//	Kamera wird mit dem übergebenen Vektor bewegt
	void update();										//	Die Position der Kamera wird, abhängig von 'm_pCenterObject' errechnet

	//	getter-Funktionen
	Vector2D getPositionVector() const { return m_positionVector; }
	int getCameraWidth() const { return m_cameraWidth; }
	int getCameraHeight() const { return m_cameraHeight; }

	//	setter-Funktionen
	void setPositionVector(Vector2D vector) { m_positionVector = vector; }
	void setCameraWidth(int cameraWidth) { m_cameraWidth = cameraWidth; }
	void setCameraHeight(int cameraHeight) { m_cameraHeight = cameraHeight; }
};
