#pragma once
#include "ObjectRectangle.h"


/*	Die Uhr wird in der oberen linken Ecke des Bildschirmes angezeigt.
 *	Sie wird in die Story eingebaut. Sie bleibt jedoch nicht permanent sichtbar,
 *	sondern wird von einem Skript bei Bedarf sichtbar und wieder unsichtbar gemacht.
 *	
 */
class Clock
{
private:
	ObjectRectangle m_rect;														//	In dieses Rechteck wird  die Uhr gerendert
	double m_timeInSeconds;														//	Die aktuelle Zeitangabe, die die Uhr macht
	const char* m_textureId;													//	Die textureId des Ziffernblattes
	bool m_isVisible;															//	Bestimmt, ob die Uhr gerendert werden soll

	int m_desiredTimeInSeconds;													//	Sobald dieser Wert von 'm_timeInSeconds' abweicht, bewegen sich die Zeiger
	double m_handVelocity;														//	Anzahl der Sekunden, die per Frame hinzugefügt werden
	
	/*	Diese Speicherstruktur enthält alle Informationen zu einem Uhrzeiger.
	*	Z.B. die Ausrichtung, die bei jedem Aufruf von update() der 'Clock' anhand
	*	von 'm_timeInSeconds' neu berechnet wird.
	*
	*	Hiervon braucht die Uhr drei Instanzen (Sekunden, Minuten, Stunden)
	*/
	struct ClockHand
	{
		int width;
		int height;
		int rotationAngle;
		const char* textureId;

		ClockHand() 
			: width(0), height(0), rotationAngle(0),
			  textureId("") {}

	}m_secondsHand, m_minutesHand, m_hoursHand;
	ObjectRectangle m_tinyDisplayRect;											//	Die Box, in die die Kleine Anzeige gerendert wird
	double m_tinyDisplay;														//	Die kleine Zeitanzeige (Stunden)
	const char* m_tinyDispayTextureId;											//	Die textureId der kleinen Anzeige
	int m_desiredTinyDisplay;													//	siehe m_desiredTimeSeconds
	double m_displayVelocity;

public:
	Clock()																		//	Konstruktor
		: m_timeInSeconds(0), m_textureId(nullptr),
		  m_isVisible(false), m_desiredTimeInSeconds(0), m_handVelocity(0),
		  m_tinyDisplay(0), m_tinyDispayTextureId(nullptr), m_desiredTinyDisplay(0), 
		  m_displayVelocity(0)
	{
	}

	void init();
	void update();
	void draw();

	void addTime(int seconds, double velocity);									//	Lässt die Zeiger der Uhr um eine bestimmte Anzahl an Sekunden wandern
	void addTinyDisplayTime(int hours, double velocity);						//	Inkrementiert/Dekrementiert die kleine Anzeige


	//	getter-Funktionen
	int getTimeInSeconds() const { return m_timeInSeconds; }
	int getTinyDisplayTimeInHours() const { return m_tinyDisplay; }
	
	//	setter-Funktionen
	void setTime(int timeInSeconds) { m_timeInSeconds = m_desiredTimeInSeconds = timeInSeconds; }
	void setVisible(bool v) { m_isVisible = v; }
	void setTinyDisplayTime(int timeInHours) { m_tinyDisplay = m_desiredTinyDisplay = timeInHours; }

};

