#pragma once
#include "Layer.h"

namespace Environment
{
	/*	Diese Klasse managed unsere Animationen.
	*/
	class AnimationLayer : public Layer
	{
	private:
		//	std::vector<Anímation*> m_animations TODO: Klasse "Animation" implementieren

	public:
		AnimationLayer();
		~AnimationLayer();

		void update();							//	Aktualisiern
		void render();							//	Rendern
	};


}
