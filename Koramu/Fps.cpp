#include "Fps.h"


void Fps::calculateFPS()
{
	static const int NUM_SAMPLES = 10;						//	Anzahl der Frames, aus denen ein Durchschnitt errechnet wird
	static float frameTimes[NUM_SAMPLES];
	static int currentFrame = 0;

	//	SDL_GetTicks() gibt in ms an wie viel Zeit seit der Initialisierung von SDL vergangen ist
	
	static float previousTicks = SDL_GetTicks();			//	Vorherige Frames

	float currentTicks;										//	Jetziger Frame
	currentTicks = SDL_GetTicks();							

	frameTime = currentTicks - previousTicks;				//	Dauer des Frames
	frameTimes[currentFrame % NUM_SAMPLES] = frameTime;		//	Was macht das?

	previousTicks = currentTicks;							

	int count;												//	Anzahl aus Frames, aus welchen die Durschschnittszahl ausgerechnet wird

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

	frameTimeAverage /= count;					//	Durchschnittliche Frametime

	if (frameTimeAverage > 0)
	{
		fps = 1000.0f / frameTimeAverage;		//	Anzahl der Frames die pro Sekunde ausgegeben werden 
	}
	else										//	Durch null teilen geht nicht 
	{
		fps = 60.0;								//	Deswegen wird ein beliebeiger Wert angegeben
	}

	currentFrame++;								//	Nächster Frame

	//std::cout << fps << std::endl;				//	FPS ausgeben (zum Testen)

	if (fps < 50)
		std::cout << "Low FPS! \t" << fps << std::endl;
}

