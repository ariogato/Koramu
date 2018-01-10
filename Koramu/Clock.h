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
	int m_timeInSeconds;														//	Die aktuelle Zeitangabe, die die Uhr macht
	const char* m_textureId;													//	Die textureId des Ziffernblattes
	bool m_isVisible;															//	Bestimmt, ob die Uhr gerendert werden soll

	int m_desiredTimeInSeconds;													//	Sobald dieser Wert von 'm_timeInSeconds' abweicht, bewegen sich die Zeiger
	int m_handVelocity;															//	Anzahl der Sekunden, die per Frame hinzugefügt werden
	
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

public:
	Clock()																		//	Konstruktor
		: m_timeInSeconds(0.0), m_textureId(nullptr), 
		  m_isVisible(false)
	{}

	void init();
	void update();
	void draw();

	void addTime(int seconds, int velocity);									//	Lässt die Zeiger der Uhr um eine bestimmte Anzahl an Sekunden wandern


	//	getter-Funktionen
	int getTimeInSeconds() const { return m_timeInSeconds; }

	//	setter-Funktionen
	void setTime(int timeInSeconds) { m_timeInSeconds = timeInSeconds; }
	void setVisible(bool v) { m_isVisible = v; }

};

