#include "Clock.h"
#include "TextureManager.h"
#include "FontManager.h"

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

	//	Attribute der kleinen Anzeige leider auch...
	m_tinyDispayTextureId = "clockTinyDisplay";
	m_tinyDisplayRect.width = 96;
	m_tinyDisplayRect.height = 16;
	m_tinyDisplayRect.positionVector.setX(m_rect.width / 2 - m_tinyDisplayRect.width / 2 + 10);
	m_tinyDisplayRect.positionVector.setY(m_rect.height / 2 + 32);
	m_tinyDisplay = 999999;

	m_tinyDisplayRect.setShowText(false);
	m_tinyDisplayRect.setColor(255, 130, 28, 255);
	m_tinyDisplayRect.setVisible(true);
}

void Clock::update()
{
	//	Wenn eine andere Zeit angezeigt werden soll, werden die Zeiger entsprechend bewegt
	if (m_desiredTimeInSeconds != m_timeInSeconds)
	{
		//	Herausfinden, ob der Zähler inkrementiert oder dekrementiert werden muss
		int signum = (m_desiredTimeInSeconds - m_timeInSeconds < 0) ? -1 : 1;

		/*	Hier soll die Zählervariable so oft um 'm_handVelocity' inkrementiert werden, bis die Werte gleich sind.
		 *	Bzw. dekrementiert, wenn die Different negativ ist.
		 */
		m_timeInSeconds += signum * m_handVelocity;

		//	Hier wird nach einem overshoot gecheckt (kann passieren, da m_handVelocity > 1 sein kann)
		if (signum < 0 && m_timeInSeconds < m_desiredTimeInSeconds || signum > 0 && m_timeInSeconds > m_desiredTimeInSeconds)
		{
			m_timeInSeconds = m_desiredTimeInSeconds;
		}
	}

	//	Eine Änderung in der kleinen Anzeige
	if (m_desiredTinyDisplay != m_tinyDisplay)
	{
		//	Beschreibung siehe oben

		int signum = (m_desiredTinyDisplay - m_tinyDisplay < 0) ? -1 : 1;

		m_tinyDisplay += signum * m_displayVelocity;

		if (signum < 0 && m_tinyDisplay < m_desiredTinyDisplay || signum > 0 && m_tinyDisplay > m_desiredTinyDisplay)
		{
			m_tinyDisplay = m_desiredTinyDisplay;
		}
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

	//	Die Position, an der die kleinere Anzeige gezeichnet werden soll

	//	Ziffernblatt zeichnen
	TheTextureManager::Instance()->draw(m_textureId, m_rect.getX(), m_rect.getY(), m_rect.getWidth(), m_rect.getHeight());

	//	Kleine Anzeige zeichnen
	TheTextureManager::Instance()->draw(m_tinyDispayTextureId, m_tinyDisplayRect.getX(), m_tinyDisplayRect.getY() - 2, m_tinyDisplayRect.width, m_tinyDisplayRect.height);
	
	//	Die Zahl wir in einen String umgewandelt und dann mit dem Fontmanager gezeichnet
	std::string tinyDis = std::to_string(static_cast<int>(m_tinyDisplay));

	//	Der String wird gepaddet (d.h. Es werden Nullen hinzugefügt bis eine Ziffernanzahl von 6 erreicht wird)
	tinyDis.insert(0, 6 - tinyDis.length(), '0');

	TheFontManager::Instance()->drawText(tinyDis, m_tinyDisplayRect.getX(), m_tinyDisplayRect.getY(), 16);

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
	//m_rect.draw(Vector2D(0.0f, 0.0f));
	//m_tinyDisplayRect.draw(Vector2D(0.0f, 0.0f));
}

void Clock::addTime(int seconds, double velocity)
{
	m_desiredTimeInSeconds = m_timeInSeconds + seconds;
	m_handVelocity = velocity;
}

void Clock::addTinyDisplayTime(int hours, double velocity)
{
	m_desiredTinyDisplay = m_tinyDisplay + hours;
	m_displayVelocity = velocity;
}

