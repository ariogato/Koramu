#pragma once

#include <vector>
#include <map>
#include "Camera.h"
#include "Vector2D.h"


class GameObject;
class RenderObject;
class SDL_GameObject;


/*	Diese Klasse ist für sämtliche Darstellungen auf dem Bildschirm verantwortlich.
 *	Sie vereinfacht einiges, indem sie das Rendern zentralisiert. 
 *	
 *	So können viele Feautures einfacher realisiert werden, wie z.B. das Rendern
 *	mithilfe der 'Camera'.
 */

class View
{
private:
	static Camera* m_pCamera;														//	Die Kamera entscheidet welcher Teil der 'View' sichtbar ist und gerendert wird
	static std::map<unsigned long long, RenderObject*> m_renderDict;				//	Ein Dictionary der Objekte, die potentiell gerendert werden sollten
	static std::vector<unsigned long long> m_renderQueue;							//	Eine Liste, der Objekte (ID der Objekte), die gerendert werden sollen

public:
	static void centerOnGameObject(unsigned long long objectId);					//	Das ist lediglich ein Wrapper um Camera::centerOnGameObject()
	static void addToView(GameObject* pGameObject);									//	'GameObject's, die gerendert werden wollen, werden hiermit hinzugefügt
	static void removeFromView(GameObject* pGameObject);							//	'GameObject's, die nicht mehr gerendert werden sollen, können hiermit entfernt werden
	static void removeFromView(unsigned long long objectId);						//	Überladene Funktion
	static void addToRenderQueue(unsigned long long objectId);						//	Fügt der renderQueue eine ID eines Objektes hinzu
	static void addToRenderQueue(std::vector<unsigned long long>& objectIds);		//	Fügt der renderQueue mehrere IDs mehrerer Objekte hinzu
	static void addToRenderQueue(std::vector<GameObject*>& objects);				//	Fügt der renderQueue mehrere IDs mehrerer Objekte hinzu (extrahiert zuerst die IDs; nicht empfohlen!) 

	//	Unsere klassischen Funktionen
	static void update();															//	Aktualisiert Werte wie z.B. den Abstand der Objekte zur Kameraposition
	static void render();															//	Rendert final alle 'GameObject's

	//	getter-Funktionen
	static Camera* getCamera() { return m_pCamera; }

	//	setter-Funktionen
	static void setCameraPosition(Vector2D v) { m_pCamera->setPositionVector(v); }	
};
