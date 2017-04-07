#include "Fps.h"
#include <iostream>
#include <SDL.h>

void Fps::calculateFPS()
{
	//	Anzahl der Frames, aus denen ein Durchschnitt errechnet wird
	static const int NUM_SAMPLES = 10;						
	static float frameTimes[NUM_SAMPLES];
	static int currentFrame = 0;

	//	SDL_GetTicks() gibt in ms an wie viel Zeit seit der Initialisierung von SDL vergangen ist
	
	//	Vorherige Frames
	static float previousTicks = SDL_GetTicks();			

	//	Jetziger Frame
	float currentTicks;										
	currentTicks = SDL_GetTicks();							

	//	Dauer des Frames
	frameTime = currentTicks - previousTicks;				
	frameTimes[currentFrame % NUM_SAMPLES] = frameTime;		

	previousTicks = currentTicks;							

	//	Anzahl aus Frames, aus welchen die Durschschnittszahl ausgerechnet wird
	int count;												

	//	Durchschnitt aus nur den schon gezeigten Frames:
	if (currentFrame < NUM_SAMPLES)							
	{
		count = currentFrame;
	}
	else 
	{
		count = NUM_SAMPLES;								
	}

	float frameTimeAverage = 0;				
	for (int i = 0; i < count; i++)
	{
		frameTimeAverage += frameTimes[i];					
	}

	frameTimeAverage /= count;					

	//	Anzahl der Frames die pro Sekunde ausgegeben werden
	if (frameTimeAverage > 0)
	{
		fps = 1000.0f / frameTimeAverage;		 
	}
	//	Durch null teilen geht nicht, deswegen wird ein beliebeiger Wert angegeben
	else										 
	{
		fps = 60.0;								
	}
    
	//	Nächster Frame
	currentFrame++;								

	//std::cout << fps << std::endl;				//	FPS ausgeben (zum Testen)

	if (fps < 50)
		std::cout << "Low FPS! \t" << fps << std::endl;		
}

