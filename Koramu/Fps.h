#pragma once
#include <iostream>
#include <SDL.h>

/*	Dies Klasse dafür da, die FPS (Frames Per Second) zu berechnen. Es ist einfach wunderbar, 
*	wenn man die FPS als Spieler angezeigt bekommt :).
*/
class Fps
{
private: 
	float fps;					//	Frames Per Second	
	float frameTime;			//	Dauer eines Frames

public:
	void calculateFPS();		//	FPS berechnen
};