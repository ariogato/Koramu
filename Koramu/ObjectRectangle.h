#pragma once

#include"Vector2D.h"
#include "SDL_ttf.h"
#include <string>

class ParamLoader;

/*	Instanzen der Klasse "ObjectRectangle" legen die 
 *	Position, Breite und Höhe von Spielobjekten fest.	
 *	Im Debugmodus werden sie mitgerendert und die Attribute angezeigt.
 */

class ObjectRectangle
{
public:
	Vector2D positionVector;			//	Der Ortsvektor zum Rechteck
	int width;							//	Die Breite des Rechtecks
	int height;							//	Die Höhe des Rechtecks

private:
	std::string m_rectAttributes;		//	Attribute, die unter dem Rechteck gerendert werden.

	TTF_Font* m_font;					//	Vorläufige Speicherung der Schriftart (bis TextureManager auch Schriftarten händeln kann)

public:
	ObjectRectangle();
	~ObjectRectangle();

	void load(const ParamLoader& params);
	void update();
	void draw();

	//	getter-Funktionen
	float getX() const { return positionVector.getX(); }
	float getY() const { return positionVector.getY(); }
	int getWidth() const { return width; }
	int getHeight() const { return height; }
	
};

