#pragma once

#include "Vector2D.h"

class GameObject;

/*	Dank dieser Klasse wird nicht die gesamte Map gerendert.
 *	Stattdessen wird nur der kleine, zu der Zeit ben�tigte Teil der Map gerendert.
 *  Dieser Ausschnitt wird durch die Kamera festgelegt.
 *	Somit wird weniger CPU-Leistung ben�tigt. 
 */

class Camera
{
private:
	Vector2D m_positionVector;						//	Position des Vektors
	int m_cameraWidth;								//	Breite der Kamera
	int m_cameraHeight;								//	H�he der Kamera

public:
	Camera();										//	Konstruktor
	~Camera();										//	Destruktor
	
	void centerOnGameObject(GameObject& object);	//	Kamera auf das �bergebene Objekt zentrieren
	void move(Vector2D& vector);					//	Kamera wird mit dem �bergebenen Vektor bewegt

	//	getter-Funktionen
	Vector2D getPositionVector() const { return m_positionVector; }
	int getCameraWidth() const { return m_cameraWidth; }
	int getCameraHeight() const { return m_cameraHeight; }

	//	setter-Funktionen
	void setPositionVector(Vector2D vector) { m_positionVector = vector; }
	void setCameraWidth(int cameraWidth) { m_cameraWidth = cameraWidth; }
	void setCameraHeight(int cameraHeight) { m_cameraHeight = cameraHeight; }
};
