#include "ObjectRectangle.h"
#include "ParamLoader.h"
#include "Game.h"

TTF_Font* ObjectRectangle::m_font = nullptr;

ObjectRectangle::ObjectRectangle()
	: positionVector(0.0f, 0.0f),
	  width(0), height(0), 
	  m_visible(false), m_showText(false)
{
	if(!m_font)
		m_font = TTF_OpenFont("../assets/consola.ttf", 12);


	//	Default Farbe ist rot
	m_color.r = 255;
	m_color.g = 0;
	m_color.b = 0;
	m_color.a = 255;
}


ObjectRectangle::~ObjectRectangle()
{
}

void ObjectRectangle::load(const ParamLoader& params)
{
	positionVector.setX(params.getX());
	positionVector.setY(params.getY());
	width = params.getWidth();
	height = params.getHeight();

	//	Alles was hier gespeichert wird kommt im Debugmodus auf den Schirm.
	m_rectAttributes = "width: " + std::to_string(width) + "\n"
					 + "height: " + std::to_string(height) + "\n"
					 + "xPos: " + std::to_string(static_cast<int>(positionVector.getX())) + "\n"
					 + "yPos: " + std::to_string(static_cast<int>(positionVector.getY())) + "\n";
}

void ObjectRectangle::update()
{
	if (m_showText)
	{
		//	Alles was hier gespeichert wird kommt im Debugmodus auf den Schirm.
		m_rectAttributes = "width: " + std::to_string(width) + "\n"
						 + "height: " + std::to_string(height) + "\n"
						 + "xPos: " + std::to_string(static_cast<int>(positionVector.getX())) + "\n"
						 + "yPos: " + std::to_string(static_cast<int>(positionVector.getY())) + "\n";
	}
}

void ObjectRectangle::draw()
{
	//	Falls das Rechteck nicht sichtbar sein soll, muss an dieser Stelle die Methode beendet werden
	if (!m_visible)
		return;

	if (m_showText)
	{
		//	Wir bitten SDL uns eine 'SDL_Surface' aus unseren 'm_rectAttributes' zu erstellen
		SDL_Surface* pTempSurface = TTF_RenderText_Blended_Wrapped(m_font, m_rectAttributes.c_str(), m_color, 100);

		//	Die Surface wird zur Textur gemacht
		SDL_Texture* pMessage = SDL_CreateTextureFromSurface(TheGame::Instance()->getRenderer(), pTempSurface);

		//	Um ein Speicherleck zu verhindern wird die 'SDL_Surface' wieder freigegeben
		SDL_FreeSurface(pTempSurface);

		//	Breite und Höhe der Textur werden fürs Rendern in einem 'SDL_Rect' gespeichert
		int textureWidth, textureHeight;
		SDL_QueryTexture(pMessage, nullptr, nullptr, &textureWidth, &textureHeight);
		SDL_Rect messageRect = { positionVector.getX(), positionVector.getY() + height + 5, textureWidth, textureHeight };

		//	Der Text wird gerendert
		SDL_RenderCopy(TheGame::Instance()->getRenderer(), pMessage, nullptr, &messageRect);

		//	Die Textur wird wieder freigegeben, um ein Speicherleck zu verhindern
		SDL_DestroyTexture(pMessage);
	}
	/*	In den folgenden vier Zeilen wird das tatsächliche Rechteck gerendert
	 *
	 *		1. Position und Breite des Rechtecks festlegen
	 *		2. Farbe des Rechtecks festlegen
	 *		3. Rechteck malen
	 *		4. Die Farbe zum Rendern wieder neutralisieren (sonst wäre alles in der zuvor festgelegten Farbe)
	 */
	SDL_Rect rect = { positionVector.getX(), positionVector.getY(), width, height };
	SDL_SetRenderDrawColor(TheGame::Instance()->getRenderer(), m_color.r, m_color.g, m_color.b, m_color.a);
	SDL_RenderDrawRect(TheGame::Instance()->getRenderer(), &rect);
	SDL_SetRenderDrawColor(TheGame::Instance()->getRenderer(), 0, 0, 0, 255);
}
