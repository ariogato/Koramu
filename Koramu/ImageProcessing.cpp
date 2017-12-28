#include "ImageProcessing.h"
#include <SDL.h>
#include <iostream>
#include <vector>

Uint32 ImageProcessing::getPixel(SDL_Surface* pSurface, int x, int y)
{
	/*	Die Pixel von der Surface speichern.
	 *	Die Pixel werden in einem Array gespeichert. (void*)
	 *	Ein cast muss von void* auf Uint32* durchgeführt werden.
	 */
	Uint32* pPixels = static_cast<Uint32*>(pSurface->pixels);

	/*	Da die Pixel in einem eindimensionales Array gespeichert werden, 
	 *	erfolgt der Zugriff auf ein einzelnes Pixel so:
	 */
	return pPixels[y * pSurface->w + x];
}

void ImageProcessing::setPixel(SDL_Surface* pSurface, int x, int y, Uint32 pixel)
{
	//	siehe getPixel()
	Uint32* pPixels = static_cast<Uint32*>(pSurface->pixels);

	pPixels[y * pSurface->w + x] = pixel;
}

void ImageProcessing::gaussianBlur(SDL_Surface* surface)
{
	/*	Die Operation hier ist im Grunde genommen ein einfacher Tiefpassfilter.
	 *	Die Faltung erfolgt mit der Gausschen Normalverteilung. Dadurch werden
	 *	hochfrequente Bildanteile (also Kanten) herausgefiltert, was ein weiches 
	 *	Bild zur Folge hat.
	 *	
	 *	Die Faltung erfolgt dreimal (einmal für jeden Farbwert).
	 *	Hierbei wird die 2D Operation in zwei 1D Operationen aufgeteilt 
	 *	-> einmal horizontal und einmal vertikal.
	 */

	//	Der Kernel, mit dem die Pixelwerte multipliziert werden.
	/*double kernel[5][5] =
	{
		{ 0.003765, 0.015019, 0.023792, 0.015019, 0.003765 },
		{ 0.015019, 0.059912, 0.094907, 0.059912, 0.015019 },
		{ 0.023792, 0.094907, 0.150342, 0.094907, 0.023792 },
		{ 0.015019, 0.059912, 0.094907, 0.059912, 0.015019 },
		{ 0.003765, 0.015019, 0.023792, 0.015019, 0.003765 }
	};*/

	/*double kernel[5][5] =
	{
		{ 0.000874, 0.006976, 0.01386, 0.006976, 0.000874 },
		{ 0.006976, 0.0557, 0.110656, 0.0557, 0.006976 },
		{ 0.01386, 0.110656, 0.219833, 0.110656, 0.01386 },
		{ 0.006976, 0.0557, 0.110656, 0.0557, 0.006976 },
		{ 0.000874, 0.006976, 0.01386, 0.006976, 0.000874 }
	};*/
	double kernel[5][5] =
	{
		{ 0.039206, 0.039798, 0.039997, 0.039798, 0.039206 },
		{ 0.039798, 0.040399, 0.040601, 0.040399, 0.039798 },
		{ 0.039997, 0.040601, 0.040804, 0.040601, 0.039997 },
		{ 0.039798, 0.040399, 0.040601, 0.040399, 0.039798 },
		{ 0.039206, 0.039798, 0.039997, 0.039798, 0.039206 }
	};

	//	Eine Speicherstruktur für die Farbwerte eines Pixels
	struct Pixel
	{
		unsigned int r;
		unsigned int g;
		unsigned int b;
	};

	//	Hier werden alle neuen Pixel gespeichert
	std::vector<std::vector<Pixel>> rgbPixels(surface->h, std::vector<Pixel>(surface->w));

	//	Es wird über jedes Pixel iteriert
	for (int i = 0; i < surface->h; i++)
	{
		for (int j = 0; j < surface->w; j++)
		{
			//	Die Farbwerte von 5 benachbarten Pixeln (in x & y Richtung) werden hier 
			Pixel pixelMatrix[5][5];
			bool invalid[5][5];

			//	Die Arrays werden initialisiert
			for (int i = 0; i < 5; i++)
			{
				for (int j = 0; j < 5; j++)
				{

					pixelMatrix[i][j].r = 0;
					pixelMatrix[i][j].g = 0;
					pixelMatrix[i][j].b = 0;
					invalid[i][j] = false;
					
				}

			}

			/*	Hier wird über die umliegenden Pixel iteriert
			 *	
			 *	Es werden immer zwei Pixel in x und y Richtung (positiv und negativ)
			 *	betrachtet.
			 *	So hat man am Ende eine Matrix mit allen umliegenden Pixeln
			 *	
			 *	Diese Matrix wird dann mit dem Kernel gewichtet.
			 *	
			 *	Das passiert für jede Farbe
			 */
			int countY = 0;
			for (int y = -2; y <= 2; y++, countY++)
			{
				int countX = 0;
				for (int x = -2; x <= 2; x++, countX++)
				{

					if (j + x < 0 || j + x > surface->w
						|| i + y < 0 ||i + y > surface->h)
					{
						invalid[countX][countY] = true;
					}
					else
					{
						//	Die Variablen, in denen die Farbwerte als 8 bit unsigned int gespeichert werden
						Uint8 r;
						Uint8 g;
						Uint8 b;

						//	Die Werte werden gelesen
						SDL_GetRGB(getPixel(surface, j + x, i + y), surface->format, &r, &g, &b);

						//	Die Werte werden in unsigned int gecastet und gespeichert
						pixelMatrix[countX][countY] = 
						{
							static_cast<unsigned int>(r), 
							static_cast<unsigned int>(g), 
							static_cast<unsigned int>(b) 
						};
						/*std::cout << countX << " " << countY << " : ";
						std::cout << pixelMatrix[countX][countY].r << " " << pixelMatrix[countX][countY].g << " " << pixelMatrix[countX][countY].b << std::endl;*/
					}
				}
			}


			//	Der gewichtete Mittelwert wird errechnet und gespeichert
			Pixel sumPixel = {0, 0, 0};
			int num = 0;
			for (int y = 0; y < 5; y++)
			{
				for (int x = 0; x < 5; x++)
				{
					//	Wenn der Eintraf invalid war, soll er nicht ins gewicht fallen
					if (invalid[x][y])
					{
						continue;
					}

					sumPixel.r += pixelMatrix[x][y].r * kernel[x][y] * 25;
					sumPixel.g += pixelMatrix[x][y].g * kernel[x][y] * 25;
					sumPixel.b += pixelMatrix[x][y].b * kernel[x][y] * 25;
					//std::cout << pixelMatrix[x][y].r << " " << pixelMatrix[x][y].g << " " << pixelMatrix[x][y].b << std::endl;
					num++;
				}
			}
			sumPixel.r /= num;
			sumPixel.g /= num;
			sumPixel.b /= num;
			rgbPixels[i][j] = sumPixel;
			//std::cout << rgbPixels[i][j].r << " " << rgbPixels[i][j].g << " " << rgbPixels[i][j].b << std::endl;
			
		}
	}

	//	Es wird wieder über jedes Pixel iteriert
	for (int i = 0; i < surface->h; i++)
	{
		for (int j = 0; j < surface->w; j++)
		{
			setPixel(surface, j, i, SDL_MapRGB(surface->format, (Uint8)rgbPixels[i][j].r, (Uint8)rgbPixels[i][j].g, (Uint8)rgbPixels[i][j].b));
			//setPixel(surface, j, i, SDL_MapRGB(surface->format, 255, 0, 0));
		}
	}
}
