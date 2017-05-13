#pragma once

#include "Vector2D.h"

namespace Environment
{ 
	/*	Unsere Map setzt sich aus mehreren Layern/Schichten zusammen. 
	 *	Es gibt mehrere "TileLayer" (Backgroundlayer, Collisionlayer, Bridgelayer, Interactionlayer, Foregroundlayer)
	 *	und ein "ObjectLayer", die in einer spezifischen Reihenfolge gerendert werden sollen. 
	 *	Diese Layer erben von dieser Überklasse.
	 */
	class Layer
	{
	protected:
		Vector2D m_positionVector;
	public:
		Layer()
			: m_positionVector(0.0f, 0.0f)
		{}
		virtual ~Layer() {}

		virtual void update() = 0;		//	Layer aktualisieren
		virtual void render() = 0;							//	Layer rendern
	};
}
