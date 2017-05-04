#pragma once
#include <vector>
#include "Layer.h"

class GameObject;

namespace Environment
{
	/*	Diese Klasse managed unsere Animationen.
	 *	
	 *	Ein Array ('std::vector') hört sich vielleicht sinnvoll an,
	 *	aber dadurch, dass 'addContent' einen Array aus 'GameObject's bekommt, ist es unmöglich 
	 *	die 'GameObject's als 'Animation's zu speichern.
	 *	Nichtsdestotrotz (wusstet ihr, dass das ein einziges Wort ist? -> http://www.duden.de/rechtschreibung/nichtsdestotrotz)
	 *	werden Instanzen der Klasse 'Animation' gespeichert. (Dies ist möglich, weil 'Animation' von 'SDL_GameObject' 
	 *	und das wiederum von 'GameObject' erbt)
	*/
	class AnimationLayer : public Layer
	{
	private:
		std::vector<GameObject*> m_animations;

		void addAnimations(std::vector<GameObject*>& pAnimations);

	public:
		AnimationLayer();
		~AnimationLayer();

		void init();							//	Initialisieren
		void update();							//	Aktualisiern
		void render();							//	Rendern

		void addContent(std::vector<GameObject*>& pObjects);		//	Inhalt hinzufügen
	};


}
