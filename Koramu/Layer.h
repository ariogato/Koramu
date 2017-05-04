#pragma once
#include <vector>

class GameObject;

namespace Environment
{ 
	/*	Unsere Map setzt sich aus mehreren Layern/Schichten zusammen. 
	 *	Es gibt mehrere "TileLayer" (Backgroundlayer, Collisionlayer, Bridgelayer, Interactionlayer, Foregroundlayer)
	 *	und ein "ObjectLayer", die in einer spezifischen Reihenfolge gerendert werden sollen. 
	 *	Diese Layer erben von dieser �berklasse.
	 */
	class Layer
	{
	public:
		Layer() {}
		virtual ~Layer() {}

		virtual void init() = 0;		//	Layer initialisieren
		virtual void update() = 0;		//	Layer aktualisieren
		virtual void render() = 0;		//	Layer rendern

		virtual void addContent(std::vector<GameObject*>& pObjects) = 0;		//	Inhalt hinzuf�gen
	};
}
