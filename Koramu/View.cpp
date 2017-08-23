#include "View.h"
#include "Camera.h"
#include "GameObject.h"

void View::centerOnGameObject(SDL_GameObject* pGameObject)
{
	//	Checken, ob nullptr übergeben wurde
	if (pGameObject)
		//	Einfach die Funktion bei Kamera aufrufen 
		m_camera.centerOnGameObject(pGameObject);
}

void View::addToView(GameObject* pGameObject)
{
	
}


