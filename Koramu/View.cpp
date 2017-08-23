#include "View.h"
#include <iterator>
#include "Game.h"
#include "Camera.h"
#include "GameObject.h"
#include "RenderObject.h"

//	Die statischen Member global initialisieren
Camera* View::m_pCamera = new Camera(												//	Camera
	TheGame::Instance()->getGameWidth(),
	TheGame::Instance()->getGameHeight()
);

std::map<unsigned long long, RenderObject*> View::m_renderDict;						//	renderDict
std::vector<unsigned long long> View::m_renderQueue;								//	renderQueue


void View::centerOnGameObject(unsigned long long objectId)
{
	//	Checken, ob das Objekt mit der übergebenen ID exisitiert
	if (!m_renderDict.count(objectId))
		return;

	//	Einfach die Funktion bei Kamera aufrufen
	m_pCamera->centerOnGameObject(m_renderDict[objectId]->getGameObject());
}

void View::addToView(GameObject* pGameObject)
{
	//	Checken, ob nullptr übergeben wurde
	if (!pGameObject)
		return;
	//	Cast 'GameObject' zu 'SDL_GameObject'
	if (SDL_GameObject* s = dynamic_cast<SDL_GameObject*>(pGameObject))
	{
		//	das Objekt zum Dictionary hinzufügen
		m_renderDict.insert(std::pair<unsigned long long, RenderObject*>(s->getUniqueId(), new RenderObject(s)));
	}

}

void View::removeFromView(GameObject* pGameObject)
{
	/*	Hier wird lediglich die überladene Version dieser Funtion
	 *	mit der ID der übergebenen 'GameObject's aufgerufen.
	 */

	//	Checken, ob nullptr übergeben wurde
	if (pGameObject)
		removeFromView(pGameObject->getUniqueId());
}

void View::removeFromView(unsigned long long objectId)
{
	//	Checken, ob das Objekt mit der übergeben ID existiert
	if (!m_renderDict.count(objectId))
		return;

	//	Die ID suchen
	std::vector<unsigned long long>::iterator it = std::find(m_renderQueue.begin(), m_renderQueue.end(), objectId);
	
	//	Die ID muss nur entfernt werden, falls sie gefunden wurde
	if (it != m_renderQueue.end())
		m_renderQueue.erase(it);
	
	//	Das Objekt vom renderDict entfernen
	m_renderDict.erase(objectId);
}

void View::addToRenderQueue(unsigned long long objectId)
{
	//	checken, ob sich ein 'GameObject' mit der übergebeben ID im renderDict befindet
	if (!m_renderDict.count(objectId))
		return;

	//	Checken, ob die objectId sich bereits in der renderQueue befindet
	if (std::find(m_renderQueue.begin(), m_renderQueue.end(), objectId) != m_renderQueue.end())
		return;

	//	Ab hier kann die objectId sicher der renderQueue hinzugefügt werden
	m_renderQueue.push_back(objectId);
}

void View::addToRenderQueue(std::vector<unsigned long long>& objectIds)
{
	//	Über alle IDs iterieren und zur renderQueue hinzufügen
	for (auto i : objectIds)
		addToRenderQueue(i);
}

void View::addToRenderQueue(std::vector<GameObject*>& objects)
{
	//	Über alle Objekte iterieren und zur renderQueue hinzufügen
	for (auto i : objects)
	{
		//	Checken, ob das Objekt nullptr ist
		if (i)
		{
			//	Die ID zur renderQueue hinzufügen
			addToRenderQueue(i->getUniqueId());
		}
	}
}

void View::update()
{
	
}


void View::render()
{
	//	Es wird über jede ID in der renderQueue iteriert, wobei die entsprechenden 'GameObject's gerendert werden
	for (auto i : m_renderQueue)
	{
		//	Checken, ob das Objekt mit dieser ID existiert
		if (m_renderDict.count(i))
			m_renderDict[i]->render();
	}

	//	Die renderQueue für den nächsten Frame leeren
	m_renderQueue.clear();
}

