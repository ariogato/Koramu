#pragma once

#include "Camera.h"


class GameObject;
class SDL_GameObject;


/*	Diese Klasse ist für sämtliche Darstellungen auf dem Bildschirm verantwortlich.
 *
 */

class View
{
private:
	static Camera m_camera;													//	Die Kamera entscheidet welcher Teil der 'View' sichtbar ist und gerendert wird

public:
	static void centerOnGameObject(SDL_GameObject* pGameObject);				//	Das ist lediglich ein Wrapper um Camera::centerOnGameObject()
	static void addToView(GameObject* pGameObject);								//	Alle 'GameObject's, die gerendert werden wollen
};
