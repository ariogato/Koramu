#include "Notebook.h"
#include "Game.h"
#include "TextureManager.h"

Notebook::Notebook(int pages)
	: m_currentLeftPage(0)
{
	//	Id der Textur gehardcodet
	m_textureId = "notebook";

	//	Seitenzahl setzen und sicherstellen, dass es immmer eine gerade Anzahl an Seiten gibt (linke und rechte Seite)
	int pageNumber = pages + (pages % 2) * 1;

	//	Parameter des "ObjectRectangle"s setzen
	m_rect.height = 640;
	m_rect.width = 14 * 64;
	m_rect.setVisible(true);

	//	Entsprechende Anzahl an Seiten erstellen
	for (int i = 0; i < pageNumber; i++)
	{
		//	Zwischen rechten und linken Seiten unterscheiden
		if(i % 2 == 0)
		{
			//	Neue linke Seite erstellen (an die Maße des Notizbuchs angepasst) und einfügen
			Page newLeftPage;
			newLeftPage.setPosition(m_rect.getX(), m_rect.getY());
			newLeftPage.setSize(m_rect.getWidth() / 2, m_rect.getHeight());
			m_pages.push_back(newLeftPage);
		}
		else
		{
			//	Neue rechte Seite erstellen (an die Maße des Notizbuchs angepasst) und einfügen
			Page newRightPage;
			newRightPage.setPosition(m_rect.getX() + m_rect.getWidth() / 2, m_rect.getY());
			newRightPage.setSize(m_rect.getWidth() / 2, m_rect.getHeight());
			m_pages.push_back(newRightPage);
		}
	}
}

Notebook::~Notebook()
{
}

void Notebook::update()
{
}

void Notebook::draw()
{
	//	Textur des Notizbuches zeichnen
	TheTextureManager::Instance()->draw(m_textureId,
		m_rect.positionVector.getX(), m_rect.positionVector.getY(),
		m_rect.width, m_rect.height);

	//	Es sollen immer die aktuelle linke Seite und die dazugehörige rechte Seite gezeichnet werden (hier wird der Text gezeichnet(
	m_pages[m_currentLeftPage].draw();
	m_pages[m_currentLeftPage + 1].draw();

}

void Notebook::browseRight()
{
	//	Überprüfen, ob noch nach rechts geblättert werden kann
	if(m_currentLeftPage + 2 < m_pages.size())
		//	Index der nun zu zeichnenden linken Seite festlegen
		m_currentLeftPage += 2;
}

void Notebook::browseLeft()
{
	//	Überprüfen, ob noch nach links geblättert werden kann
	if(m_currentLeftPage - 2 >= 0)
		//	Index der nun zu zeichnenden rechten Seite festlegen
		m_currentLeftPage -= 2;
}

void Notebook::clear()
{
	/*	Das Notizbuch muss beim Verlassen des "PlayState"s geleert werden, da es beim erneuten betreten 
	 *	mit den gespeicherten Notizen (in "save.xml") befüllt wird und andernfalls Notizen gedoppelt enthalten wären.
	 */

	//	Über alle Seiten iterieren
	for (int i = 0; i < m_pages.size(); i++)
	{
		//	Einzelene Seite leeren
		m_pages[i].clear();
	}
}

void Notebook::align()
{
	//	Notizbuch zentrieren
	m_rect.positionVector.setX((TheGame::Instance()->getGameWidth() - m_rect.width) / 2);
	m_rect.positionVector.setY((TheGame::Instance()->getGameHeight() - m_rect.height) / 2);
	for(int i = 0; i <= m_pages.size(); i++)
	{
		//	Seiten, der Position des Notizbuches entsprechend positionieren
		//	Es muss zwischen linker und rechter Seite unterschieden werden
		if(i % 2 == 0)
			//	Linke Seite
			m_pages[i].setPosition(m_rect.positionVector.getX(), m_rect.positionVector.getY());
		else
			//	Rechte Seite
			m_pages[i].setPosition(m_rect.positionVector.getX() + m_rect.getWidth() / 2, m_rect.positionVector.getY());
	}
}

void Notebook::addNote(std::string text)
{
	//	Übergebenen Text zum Notizbuch hinzufügen. Die Notiz wird automatisch auf der richtigen Seite hinzugefügt

	//	Über alle Seiten iterieren
	for (int i = 0; i < m_pages.size(); i++)
	{
		//	Überprüfen, ob die aktuell betrachtete Seite bereits voll ist
		if (!m_pages[i].isFull())
		{
			//	Die Seite ist nicht voll. Die Notiz wird zu dieser Seite hinzugefügt
			m_pages[i].addText(text);
			//	Fertig
			return;
		}
	}

	//	Wenn wir hier landen, ist bereits jede Seite des Notizbuches voll
	TheGame::Instance()->logError() << "Notizbuch ist voll!" << std::endl << std::endl;
}