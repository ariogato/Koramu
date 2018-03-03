#pragma once

#include"Vector2D.h"
#include <SDL_ttf.h>
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
	bool m_visible;						//	Indikator, ob gerendert werden soll
	static bool s_visible;				//	Globaler Indikator (für alle Instanzen), ob gerendert werden soll

	SDL_Color m_color;					//	Farbe des Rechtecks

	std::string m_rectAttributes;		//	Attribute, die unter dem Rechteck gerendert werden.
	bool m_showText;					//	Indikator ob die 'rectAttributes' gerendert werden sollen
	static TTF_Font* s_font;			//	Vorläufige Speicherung der Schriftart (bis TextureManager auch Schriftarten händeln kann - vorläufig "static" aufgrung eines Workarounds)

public:
	ObjectRectangle();
	~ObjectRectangle();

	void load(const ParamLoader& params);
	void update();
	void draw(const Vector2D& layerPosition);

	//	getter-Funktionen
	float getX() const { return positionVector.getX(); }
	float getY() const { return positionVector.getY(); }
	int getWidth() const { return width; }
	int getHeight() const { return height; }
	bool getShowText() const { return m_showText; }
	bool getVisible() const { return m_visible; }
	
	//	setter-Funktionen
	void setColor(int r, int g, int b, int alpha) { m_color.r = r; m_color.g = g; m_color.b = b; m_color.a = alpha; }
	void setShowText(bool b) { m_showText = b; }
	void setVisible(bool b) { m_visible = b; }
};

