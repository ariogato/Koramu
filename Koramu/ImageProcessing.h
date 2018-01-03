#pragma once
#include <SDL_stdinc.h>

struct SDL_Surface;

/*	Diese Klasse ist einfach eine Ansammlung an statischen Funktionen, 
 *	die Bilder (SDL_Surfaces) verarbeiten.
 *
 *	Die meisten Methoden sind mit einem beachtlichen Overhead verbunden.
 */

class ImageProcessing
{
private:
	static Uint32 getPixel(SDL_Surface* pSurface, int x, int y);
	static void setPixel(SDL_Surface* pSurface, int x, int y, Uint32 pixel);

public:
	static void gaussianBlur(SDL_Surface* pSurface);
	static void gaussianBlurFast(SDL_Surface* pSurface);
};
