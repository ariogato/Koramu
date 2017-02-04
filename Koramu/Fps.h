#pragma once
#include <iostream>
#include <SDL.h>

/*	Diese Klasse ist dafür da, die FPS (Frames Per Second) zu berechnen. Es ist einfach wunderbar, 
*	wenn man die FPS als Spieler angezeigt bekommt :).
*/
class Fps
{
private: 
	float fps;					//	Frames Per Second	
	float frameTime;			//	Dauer eines Frames

public:
	void calculateFPS();		//	Methode um FPS zu berechnen

	//	getter-Funktionen
	int getFPS() { return fps; }
};