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
	int m_numCols;
	int m_numRows;
	int m_animSpeed;
	std::string m_textureId;

public:

	//	getter-Funktionen
	float getX() const { return m_xPos; }
	float getY() const { return m_yPos; }
	int getWidth() const { return m_width; }
	int getHeight() const { return m_height; }
	int getNumCols() const { return m_numCols; }
	int getNumRows() const { return m_numRows; }
	int getAnimSpeed() const { return m_animSpeed; }
	std::string getTextureId() const { return m_textureId; }

	//	setter-Funktionen
	void setX(float x) { m_xPos = x; }
	void setY(float y) { m_yPos = y; }
	void setWidth(int width) { m_width = width; }
	void setHeight(int height) { m_height = height; }
	void setNumCols(int num) { m_numCols = num; }
	void setNumRows(int num) { m_numRows = num; }
	void setAnimSpeed(int animspeed) { m_animSpeed = animspeed; }
	void setTextureId(std::string id) { m_textureId = id; }
};