#pragma once

#include <string>

/*	Diese Klasse erfüllt den Zweck, dass wir nicht immer bei der Initialisierung eines
*	GameObject's alle Parameter (xPos, yPos, ...) einzeln übergeben, sondern es mit einem 
*	einzigen Objekt erledigen können.
*
*	Falls später weitere Parameter benötigt werden, einfach ein Attribut anlegen und 
*	sowohl die dazugehörige get- als auch set-Funktion schreiben
*
*	Das Ganze wird so ablaufen, dass man zuerst ein Objekt dieser Klasse erstellt,
*	welches mit Anfangswerten wie '0' gefüllt ist, und die gewünschten Werte
*	anschließend mit den set-Funktionen setzt.
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
	int m_tileId;
	std::string m_uniqueId;
	std::string m_textureId;
	std::string m_mapId;
	std::string m_callbackId;

public:
	ParamLoader()									//	Konstruktor
		:	m_xPos(0.0f), m_yPos(0.0f),
			m_width(0), m_height(),
			m_numCols(0), m_numRows(0),
			m_animSpeed(1),
			m_tileId(0)
	{}

	//	getter-Funktionen
	float getX() const { return m_xPos; }
	float getY() const { return m_yPos; }
	int getWidth() const { return m_width; }
	int getHeight() const { return m_height; }
	int getNumCols() const { return m_numCols; }
	int getNumRows() const { return m_numRows; }
	int getAnimSpeed() const { return m_animSpeed; }
	int getTileId() const { return m_tileId; }
	std::string getUniqueId() const { return m_uniqueId; }
	std::string getTextureId() const { return m_textureId; }
	std::string getMapId() const { return m_mapId; }
	std::string getCallbackId() const { return m_callbackId; }

	//	setter-Funktionen
	void setX(float x) { m_xPos = x; }
	void setY(float y) { m_yPos = y; }
	void setWidth(int width) { m_width = width; }
	void setHeight(int height) { m_height = height; }
	void setNumCols(int num) { m_numCols = num; }
	void setNumRows(int num) { m_numRows = num; }
	void setAnimSpeed(int animspeed) { m_animSpeed = animspeed; }
	void setTileId(int tileId) { m_tileId = tileId; }
	void setUniqueId(std::string id) { m_uniqueId = id; }
	void setTextureId(std::string id) { m_textureId = id; }
	void setMapId(std::string id) { m_mapId = id; }
	void setCallbackId(std::string id) { m_callbackId = id; }
};