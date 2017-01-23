#include "Fps.h"


void Fps::calculateFPS()
{
	static const int NUM_SAMPLES = 10;			//Anzahl der Frames aus denen ein Durchschnitt errechnet wird
	static float frameTimes[NUM_SAMPLES];
	static int currentFrame = 0;

	static float previousTicks = SDL_GetTicks();			//vorherige Frames

	float currentTicks;										//jetziger Frame
	currentTicks = SDL_GetTicks();							

	frameTime = currentTicks - previousTicks;				//Dauer des Frames
	frameTimes[currentFrame % NUM_SAMPLES] = frameTime;

	previousTicks = currentTicks;							

	int count;												//Anzahl aus Frames aus welchen die Durschschnittszahl ausgerechnet wird

	if (currentFrame < NUM_SAMPLES)							//Durchschnitt aus nur den schon gezeigten Frames
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

	frameTimeAverage /= count;					//durchschnittliche Frametime

	if (frameTimeAverage > 0)
	{
		fps = 1000.0f / frameTimeAverage;		//Anzahl der Frames die per Sekunde ausgegeben werden 
	}
	else										//Durch null teilen geht nicht 
	{
		fps = 60.0;								//Deswegen wir ein beliebeiger
	}

	currentFrame++;								//nächster 

	std::cout << fps << std::endl;
}

