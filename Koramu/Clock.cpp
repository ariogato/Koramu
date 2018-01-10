#include "Clock.h"
#include "TextureManager.h"

void Clock::init()
{
	//	Sämtliche Attribute werden leider hardgecodet
	m_secondsHand.textureId = "clockHandSeconds";
	m_minutesHand.textureId = "clockHandMinutes";
	m_hoursHand.textureId = "clockHandHours";
	
	m_secondsHand.width = 85;
	m_secondsHand.height = 2;

	m_minutesHand.width = 80;
	m_minutesHand.height = 4;

	m_hoursHand.width = 60;
	m_hoursHand.height = 4;

	m_secondsHand.rotationAngle = m_minutesHand.rotationAngle = m_hoursHand.rotationAngle = 0;
	

	//	Attribute des Ziffernblattes werden ebenfalls hardgecodet
	m_textureId = "clockFace";
	m_rect.positionVector.setX(10.0f);
	m_rect.positionVector.setY(10.0f);
	m_rect.width = m_rect.height = 192;

	m_rect.setShowText(false);
	m_rect.setColor(255, 130, 28, 255);
	m_rect.setVisible(true);
}

void Clock::update()
{
	//	Bei einer 'm_velocity' > 1 kann es sein, dass über den Wert hinausgeschossen wird.
	if (m_desiredTimeInSeconds < m_timeInSeconds)
	{
		m_desiredTimeInSeconds = m_desiredTimeInSeconds;
	}
	else if (m_desiredTimeInSeconds > m_timeInSeconds)
	{
		//	Hier soll die Zählervariable so oft um 'm_handVelocity' inkrementiert werden, bis die Werte gleich sind 
		m_timeInSeconds += m_handVelocity;
	}
	

	/*	Hier wird der Rotationswinkel der einzelnen Zeiger, basierend auf 'm_timeInSeconds' berechnet.
	 *
	 *	Hierzu wird die Modulo Operation genutzt, um zu errechnen, um wie viele Einheiten der Zeiger 
	 *	vom Nullpunkt (Winkel = 90° bzw. 270°) entfernt ist.
	 */

	
	double surplusSecs = static_cast<double>(m_timeInSeconds) / 60.0 ;
	double surplusMinutes = static_cast<double>(m_timeInSeconds) / 3600.0;
	double surplusHours = static_cast<double>(m_timeInSeconds) / (3600.0 * 12.0);
	
	m_secondsHand.rotationAngle = static_cast<int>(270 + surplusSecs * 360.0) % 360;
	m_minutesHand.rotationAngle = static_cast<int>(270 + surplusMinutes * 360.0) % 360;
	m_hoursHand.rotationAngle = static_cast<int>(270 + surplusHours * 360.0) % 360;
}

void Clock::draw()
{
	//	Die Uhr soll nur gezeichnet werden, wenn die Flag auf true gesetzt wird
	if (!m_isVisible)
	{
		return;
	}

	//	Zum zeichnen brauchen wir das Zentrum des Ziffernblattes, weil jeder Zeiger von da ausgeht
	int centerX = m_rect.getX() + m_rect.getWidth() / 2;
	int centerY = m_rect.getY() + m_rect.getHeight() / 2;

	//	Der TextureManager braucht außerdem

	//	Ziffernblatt zeichnen
	TheTextureManager::Instance()->draw(m_textureId, m_rect.getX(), m_rect.getY(), m_rect.getWidth(), m_rect.getHeight());
	
	//	Stundenzeiger zeichnen
	TheTextureManager::Instance()->draw(
		m_hoursHand.textureId,
		centerX, centerY - m_hoursHand.height / 2, 
		m_hoursHand.width, m_hoursHand.height,
		0, m_hoursHand.height / 2, 
		m_hoursHand.rotationAngle);

	//	Minutenzeiger zeichnen
	TheTextureManager::Instance()->draw(
		m_minutesHand.textureId,
		centerX, centerY - m_minutesHand.height / 2,
		m_minutesHand.width, m_minutesHand.height,
		0, m_minutesHand.height / 2,
		m_minutesHand.rotationAngle);

	//	Sekundenzeiger zeichnen
	TheTextureManager::Instance()->draw(
		m_secondsHand.textureId,
		centerX, centerY - m_secondsHand.height / 2,
		m_secondsHand.width, m_secondsHand.height,
		0, m_secondsHand.height / 2,
		m_secondsHand.rotationAngle);

	//	Das ObjectRectangle zeichnen
	m_rect.draw(Vector2D(0.0f, 0.0f));
}

void Clock::addTime(int seconds, int velocity)
{
	m_desiredTimeInSeconds = m_timeInSeconds + seconds;
	m_handVelocity = velocity;
}
