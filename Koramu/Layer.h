#pragma once

namespace Environment
{ 
	/*	Unsere Map setzt sich aus mehreren Layern/Schichten zusammen. 
	 *	Es gitb mehrere "TileLayer" (Backgroundlayer, Collisionlayer, Bridgelayer, Interactionlayer, Foregroundlayer)
	 *	und ein "ObjectLayer", die in einer spezifischen Reihenfolge gerendert werden sollen. 
	 *	Diese Layer erben von dieser Überklasse.
	 */
	class Layer
	{
	public:
		Layer() {}
		virtual ~Layer() {}

		virtual void update() = 0;		//	Layer aktualisieren
		virtual void render() = 0;		//	Layer rendern
	};
}
