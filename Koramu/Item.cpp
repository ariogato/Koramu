#include "Item.h"
#include "Vector2D.h"
#include "TextureManager.h"
#include "FontManager.h"
#include "Game.h"

void Item::draw(Vector2D& v, int num)
{
	/*	Das Item wird in eine Zeile gezeichnet. Die Reihenfolge ist:
	 *		[Textur] [Beschreibung] [Anzahl]
	 *
	 *	Der Eingabevektor zeigt auf die obere linke Ecke der Zeile. 
	 *	Des Weiteren hat jede Zeile eine höhe von 50 Pixeln.
	 */

	//	Der x-Wert des Teils, der aktuell gerendert wird
	int x = v.getX();

	//	Die Textur zeichnen
	TheTextureManager::Instance()->drawScaledFrame(m_textureId, x, v.getY(), 50, 50, 0, 0, 1, 1);

	//	x-Wert aktualisieren
	x += 50 + 20;

	//	In diesem ObjectRectangle wird der Text gerendert
	ObjectRectangle rect;
	rect.width = 280;
	rect.height = 40;
	rect.positionVector = Vector2D(x, v.getY() + 10);
	TheFontManager::Instance()->drawTextBox(m_itemName, rect);
	
	rect.draw(Vector2D(0.0f, 0.0f));

	//	Das ObjectRectangle wird für die Anzahl recyclet (hardgecodeter Abstand vom rechten Rand :-( )
	rect.width = 130;
	rect.positionVector.setX(TheGame::Instance()->getGameWidth() - rect.width - 25 - 10);

	rect.draw(Vector2D(0.0f, 0.0f));

	//	Nun wird die Anzahl gerendert
	TheFontManager::Instance()->drawTextBox(std::to_string(num), rect);
}
