#include "Dialog.h"
#include <sstream>
#include "ParamLoader.h"
#include "Game.h"
#include "TextureManager.h"
#include "FontManager.h"
#include "InputHandler.h"


Dialog::Dialog()
	: m_isVisible(false)
{
	//	Die Parameter f�r das ObjectRectangle
	ParamLoader rectParams;

	//	Die Box soll immer am unteren Bildschirmrand zentriert sein
	rectParams.setWidth(TheGame::Instance()->getGameWidth() - 200);
	rectParams.setHeight(200);
	rectParams.setX((TheGame::Instance()->getGameWidth() - rectParams.getWidth()) / 2);
	rectParams.setY(TheGame::Instance()->getGameHeight() - rectParams.getHeight());

	//	m_rect laden
	m_rect.load(rectParams);
	m_rect.setShowText(false);
	m_rect.setVisible(true);
	m_rect.setColor(185, 49, 198, 255);

	//	Die Attribute von m_rect f�r m_textBox �bernehmen
	m_textBox = m_rect;

	//	Einen Rand von 20px in die Dialogbox einbauen
	int margin = 20;
	m_textBox.positionVector += Vector2D(static_cast<float>(margin), static_cast<float>(margin));
	m_textBox.height -= margin * 2;
	m_textBox.width -= margin * 2;
		
	//	Die textureId setzen
	m_boxTextureId = "dialogBox";
}

Dialog::~Dialog()
{
}

void Dialog::update()
{
	m_rect.update();

	//	Es soll nichts gemacht werden, falls kein Text vorhanden ist
	if (!m_isVisible)
		return;

	/*	Falls die Interaktionstaste gedr�ckt wird, soll die erste Zeile in m_message gel�scht werden.
	 *	Dadurch wird immer die n�chste Zeile gerendert, da 'draw()' immer nur die erste Zeile in
	 *	m_message rendert.
	 */

	 //	Checken, ob die Interaktionstaste bedient wurde
	if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_SPACE))
	{
		//	Warten, bis die Taste losgelassen wurde
		while (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_SPACE))
		{
			//	Diese L�sung ist nicht ansatzweise elegant, aber es funktioniert
			TheInputHandler::Instance()->handleInput();
			TheGame::Instance()->render();
			SDL_Delay(20);
		}
		
		//	Die n�chste Zeile soll erst angezeigt werden, wenn die aktuelle fertig ist
		if (!m_lineFinished)
			return;

		//	Das erste Element l�schen
		m_message.erase(m_message.begin());

		//	Checken, ob das letzte Element gel�scht wurde (falls ja, soll die box nicht mehr gerendert werden)
		if (m_message.empty())
			m_isVisible = false;

		//	Die flag zur�cksetzen
		m_lineFinished = false;
	}

}

void Dialog::draw()
{
	//	Falls kein Text vorhanden ist, soll nicht gerendert werden
	if (!m_isVisible)
		return;

	/*
	 *	Der Text soll nicht sofort auf dem Bildschirm erscheinen, 
	 *	sondern per Animation St�ck f�r St�ck geschrieben werden.
	 *	
	 *	Zu diesem Zwecke wird immer ein Teil des Gesamttextes gerendert, welcher immer gr��er 
	 *	wird. Dies simuliert ein Schreiben. 
	 */

	//	Die L�nge des Teiltextes ( / animSpeed)
	static unsigned int part = 0;
	
	//	Die Schnelligkeit, in der der Text geschrieben wird (kleiner = schneller)
	unsigned int charPerSec = 1;

	//	Teil der Zeile, der gerendert werden soll  (falls die Animation abgeschlossen ist, einfach alles rendern)
	std::string text = (m_lineFinished) ? (m_message[0]) : m_message[0].substr(0, (part / charPerSec));

	//	Die Dialogbox rendern
	TheTextureManager::Instance()->draw(m_boxTextureId,
		m_rect.positionVector.getX(), m_rect.positionVector.getY(),
		m_rect.width, m_rect.height);

	//	Es soll immer die aktuelle Zeile gezeichnet werden, bis die Interaktionstaste gedr�ckt wird (siehe 'update()')
	TheFontManager::Instance()->drawTextBox(text, m_textBox);

	//	Die L�nge des Teiltextes inkrementieren, wenn das Ende noch nicht erreicht wurde, ansonsten die flag setzen und part zur�cksetzen 
	if (part / charPerSec == m_message[0].size() || m_lineFinished)
	{
		part = 0;
		m_lineFinished = true;
	}
	else
	{
		part++;
	}

	//	F�rs debugging das Rechteck zeichnen
	m_rect.draw(Vector2D(0.0f, 0.0f));
	//m_textBox.draw(Vector2D(0.0f, 0.0f));
}

void Dialog::addMessage(std::string text)
{
	/*	Diese Methode f�gt einen Text an einen eventuell schon vorhandenen 
	 *	an.
	 */

	//	Checken, ob ein leerer string �bergeben wurde
	if (text.empty())
		return;

	//	Falls der string bef�llt ist und m_messsage leer, soll die Textbox mit geg. Text sichtbar sein
	m_isVisible = true;

	//	Der Array wird mit den einzelnen Zeilen aus dem Text bef�llt ('\n' als Trennzeichen)

	//	Ein stream objekt mit dem text erstellen (f�r std::getline)
	std::stringstream textStream(text);

	//	Zwischenspeicher f�r die einzelnen Zeilen
	std::string tempString;

	//	Durch den string gehen und die Zeilen extrahieren
	while (getline(textStream, tempString, '\n'))
		m_message.push_back(tempString);

}

void Dialog::setSize(int width, int height)
{
	//	H�he und Breite der Box festlegen
	m_rect.width = width;
	m_rect.height = height;


	//	Die Attribute von m_rect f�r m_textBox �bernehmen
	m_textBox = m_rect;

	//	Einen Rand von 20px in die Dialogbox einbauen
	int margin = 20;
	m_textBox.height -= margin * 2;
	m_textBox.width -= margin * 2;
}

void Dialog::setPosition(int x, int y)
{
	m_rect.positionVector.setX(x);
	m_rect.positionVector.setY(y);

	//	Die Attribute von m_rect f�r m_textBox �bernehmen
	m_textBox = m_rect;

	//	Den Rand von 20px beibehalten
	int margin = 20;
	m_textBox.positionVector += Vector2D(static_cast<float>(margin), static_cast<float>(margin));
}
