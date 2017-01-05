#pragma once

#include <string>

/*	Diese Klasse erf�llt den Zweck, dass wir nicht immer bei der Initialisierung eines
*	GameObject's alle Parameter (xPos, yPos, ...) einzeln �bergeben, sondern es mit einem 
*	einzigen Objekt erledigen k�nnen.
*
*	Falls sp�ter weitere Parameter ben�tigt werden, einfach ein Attribut anlegen und 
*	sowohl die dazugeh�rige get- als auch set-Funktion schreiben
*
*	Das Ganze wird so ablaufen, dass man zuerst ein Objekt dieser Klasse erstellt,
*	welches mit Anfangswerten wie '0' gef�llt ist, und die gew�nschten Werte
*	anschlie�end mit den set-Funktionen setzt.
*/

class ParamLoader
{
private:
	float m_xPos;
	float m_yPos;
	int m_width;
	int m_height;
	int m_numFrames;
	int m_animSpeed;
	std::string m_textureId;

public:

	//	get-Funktionen
	float getX() const { return m_xPos; }
	float getY() const { return m_yPos; }
	int getWidth() const { return m_width; }
	int getHeight() const { return m_height; }
	int getNumFrames() const { return m_numFrames; }
	int getAnimSpeed() const { return m_animSpeed; }
	std::string getTextureId() const { return m_textureId; }

	//	set-Funktionen
	void setX(float x) { m_xPos = x; }
	void setY(float y) { m_yPos = y; }
	void setWidth(int width) { m_width = width; }
	void setHeight(int height) { m_height = height; }
	void setNumFrames(int num) { m_numFrames = num; }
	void setAnimSpeed(int animspeed) { m_animSpeed = animspeed; }
	void setTextureId(std::string id) { m_textureId = id; }
};