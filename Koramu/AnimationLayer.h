#pragma once
#include "Layer.h"
#include "vector"

namespace Environment
{
	/*	Diese Klasse managed unsere Animationen.
	*/
	class AnimationLayer : public Layer
	{
	private:
		//	std::vector<An�mation*> m_animations TODO: Klasse "Animation" implementieren

	public:
		AnimationLayer();
		~AnimationLayer();

		void update();							//	Aktualisiern
		void render();							//	Rendern
	};


}
