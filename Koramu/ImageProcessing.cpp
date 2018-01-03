#include "ImageProcessing.h"
#include <SDL.h>
#include <vector>

inline Uint32 ImageProcessing::getPixel(SDL_Surface* pSurface, int x, int y)
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

inline void ImageProcessing::setPixel(SDL_Surface* pSurface, int x, int y, Uint32 pixel)
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
			//	Die Farbwerte von 5 benachbarten Pixeln (in x & y Richtung) werden hier gespeichert
			Pixel pixelMatrix[5][5] = { 0 };
			//bool invalid[5][5] = { false };


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

			 //	Der gewichtete Mittelwert wird errechnet und gespeichert
			Pixel sumPixel = { 0, 0, 0 };
			int num = 0;
			for (int y = 0; y < 5; y++)
			{
				for (int x = 0; x < 5; x++)
				{
					int accessX = j + x - 2;
					int accessY = i + y - 2;

					if (accessX < 0 || accessX > surface->w
						|| accessY < 0 ||accessY > surface->h)
					{
						//	Es soll nichts gemacht werden
					}
					else
					{
						//	Die Variablen, in denen die Farbwerte als 8 bit unsigned int gespeichert werden
						Uint8 r;
						Uint8 g;
						Uint8 b;

						//	Die Werte werden gelesen
						SDL_GetRGB(getPixel(surface, accessX, accessY), surface->format, &r, &g, &b);

						//	Die Werte werden gespeichert
						sumPixel.r += static_cast<unsigned int>(r) * kernel[x][y] * 25;
						sumPixel.g += static_cast<unsigned int>(g) * kernel[x][y] * 25;
						sumPixel.b += static_cast<unsigned int>(b) * kernel[x][y] * 25;
						num++;
					}

				}
			}
			sumPixel.r /= num;
			sumPixel.g /= num;
			sumPixel.b /= num;
			rgbPixels[i][j] = sumPixel;
		}
	}

	//	Es wird wieder über jedes Pixel iteriert
	for (int i = 0; i < surface->h; i++)
	{
		for (int j = 0; j < surface->w; j++)
		{
			setPixel(surface, j, i, SDL_MapRGB(surface->format, (Uint8)rgbPixels[i][j].r, (Uint8)rgbPixels[i][j].g, (Uint8)rgbPixels[i][j].b));
		}
	}
}

//	Muss ungerade sein
#define KERNEL_SIZE 5

void ImageProcessing::gaussianBlurFast(SDL_Surface* pSurface)
{
	/*	Die Operation hier ist im Grunde genommen ein einfacher eindimensionaler Tiefpassfilter.
	*	Die Faltung erfolgt mit der Gausschen Normalverteilung. Dadurch werden
	*	hochfrequente Bildanteile (also Kanten) herausgefiltert, was ein weiches
	*	Bild zur Folge hat.
	*
	*	Die Faltung erfolgt dreimal (einmal für jeden Farbwert).
	*	Hierbei wird die 2D Operation in zwei 1D Operationen aufgeteilt
	*	-> einmal horizontal und einmal vertikal.
	*/
	
	//	Der Kernel, mit dem die Faltung durchgeführt wird
	/*double kernel[KERNEL_SIZE  = 11] = {
		0.089823, 
		0.090472, 
		0.09098, 
		0.091344, 
		0.091564, 
		0.091637, 
		0.091564,
		0.091344,
		0.09098,
		0.090472,
		0.089823,
	};*/

	double kernel[KERNEL_SIZE] = {
		0.19968,
		0.20016,
		0.20032,
		0.20016,
		0.19968,
	};

	//double kernel[KERNEL_SIZE] = { 1.0, 1.0, 1.0, 1.0, 1.0 };


	//	Eine Speicherstruktur für die Farbwerte eines Pixels
	struct Pixel
	{
		unsigned int r;
		unsigned int g;
		unsigned int b;
	};

	//	Hier werden alle neuen Pixel gespeichert
	std::vector<std::vector<Pixel>> rgbPixels(pSurface->h, std::vector<Pixel>(pSurface->w));

	//	Es wird über jedes Pixel iteriert
	for (int i = 0; i < pSurface->h; i++)
	{
		for (int j = 0; j < pSurface->w; j++)
		{
			//	Die Farbwerte von KERNEL_SIZE benachbarten Pixeln (in x & y Richtung) werden hier gespeichert
			Pixel pixelMatrix[KERNEL_SIZE][KERNEL_SIZE] = { 0 };


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

			//	Der gewichtete Mittelwert wird errechnet und gespeichert
			Pixel sumPixel = { 0, 0, 0 };
			int num = 0;
			
			for (int x = 0; x < KERNEL_SIZE; x++)
			{
				int accessX = j + x - 2;

				if (accessX < 0 || accessX >= pSurface->w)
				{
					//	Es soll nichts gemacht werden
				}
				else
				{
					//	Die Variablen, in denen die Farbwerte als 8 bit unsigned int gespeichert werden
					Uint8 r;
					Uint8 g;
					Uint8 b;

					//	Die Werte werden gelesen
					SDL_GetRGB(getPixel(pSurface, accessX, i), pSurface->format, &r, &g, &b);

					//	Die Werte werden gespeichert
					sumPixel.r += static_cast<unsigned int>(r) * kernel[x] * KERNEL_SIZE;
					sumPixel.g += static_cast<unsigned int>(g) * kernel[x] * KERNEL_SIZE;
					sumPixel.b += static_cast<unsigned int>(b) * kernel[x] * KERNEL_SIZE;
					num++;
				}

			}
			sumPixel.r /= num;
			sumPixel.g /= num;
			sumPixel.b /= num;
			rgbPixels[i][j] = sumPixel;
		}
	}

	/*	Der horizontale Durchlauf braucht wieder einen vector zum Schreiben 
	 *	und einen zum Lesen, weshalb rgbPixels hier kopiert wird.
	 */
	std::vector<std::vector<Pixel>> tempRgbPixels(rgbPixels);
	
	//	Es wird wieder über jedes Pixel iteriert
	for (int i = 0; i < pSurface->h; i++)
	{
		for (int j = 0; j < pSurface->w; j++)
		{
			//	Horizontaler Durchlauf
			Pixel sumPixel = { 0, 0, 0 };
			int num = 0;

			for (int x = 0; x < KERNEL_SIZE; x++)
			{
				int accessY = i + x - 2;

				if (accessY < 0 || accessY >= pSurface->h)
				{
					//	Es soll nichts gemacht werden
				}
				else
				{
					//	Die Variablen, in denen die Farbwerte als 8 bit unsigned int gespeichert werden
					Uint8 r;
					Uint8 g;
					Uint8 b;

					//	Die Werte werden gespeichert
					sumPixel.r += tempRgbPixels[accessY][j].r * kernel[x] * KERNEL_SIZE;
					sumPixel.g += tempRgbPixels[accessY][j].g * kernel[x] * KERNEL_SIZE;
					sumPixel.b += tempRgbPixels[accessY][j].b * kernel[x] * KERNEL_SIZE;
					num++;
				}

			}
			sumPixel.r /= num;
			sumPixel.g /= num;
			sumPixel.b /= num;
			rgbPixels[i][j] = sumPixel;

			setPixel(pSurface, j, i, SDL_MapRGB(pSurface->format, (Uint8)rgbPixels[i][j].r, (Uint8)rgbPixels[i][j].g, (Uint8)rgbPixels[i][j].b));
		}
	}
}


