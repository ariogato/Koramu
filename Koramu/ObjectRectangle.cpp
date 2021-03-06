#include "ObjectRectangle.h"
#include "ParamLoader.h"
#include "Game.h"
#include "InputHandler.h"

bool ObjectRectangle::s_visible = true;
TTF_Font* ObjectRectangle::s_font = nullptr;

ObjectRectangle::ObjectRectangle()
	: positionVector(0.0f, 0.0f),
	  width(0), height(0), 
	  m_visible(false), m_showText(false)
{
	if(!s_font)
		s_font = TTF_OpenFont("../assets/Fonts/consola.ttf", 12);


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
	//	Alle ObjectRectangles sollen (mit der Taste F1) getoggelt werden k�nnen
	if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_F1)) {
		//	Warten, bis die Taste losgelassen wurde
		while (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_F1))
		{
			//	Diese L�sung ist nicht ansatzweise elegant, aber es funktioniert
			TheInputHandler::Instance()->handleInput();
			TheGame::Instance()->render();
			SDL_Delay(20);
		}

		//	Die flag setzen
		s_visible = !s_visible;
	}

	//	Die Attribute sollen nur geupdatet werden, wenn sie denn auch angezeigt werden sollen
	if (m_showText)
	{
		//	Alles was hier gespeichert wird kommt im Debugmodus auf den Schirm.
		m_rectAttributes = "width: " + std::to_string(width) + "\n"
						 + "height: " + std::to_string(height) + "\n"
						 + "xPos: " + std::to_string(static_cast<int>(positionVector.getX())) + "\n"
						 + "yPos: " + std::to_string(static_cast<int>(positionVector.getY())) + "\n";
	}
}

void ObjectRectangle::draw(const Vector2D& layerPosition)
{
	//	Falls das Rechteck nicht sichtbar sein soll, muss an dieser Stelle die Methode beendet werden
	if (!m_visible || !s_visible)
		return;

	if (m_showText)
	{
		//	Wir bitten SDL uns eine 'SDL_Surface' aus unseren 'm_rectAttributes' zu erstellen
		SDL_Surface* pTempSurface = TTF_RenderText_Blended_Wrapped(s_font, m_rectAttributes.c_str(), m_color, 100);

		//	Die Surface wird zur Textur gemacht
		SDL_Texture* pMessage = SDL_CreateTextureFromSurface(TheGame::Instance()->getRenderer(), pTempSurface);

		//	Um ein Speicherleck zu verhindern wird die 'SDL_Surface' wieder freigegeben
		SDL_FreeSurface(pTempSurface);

		//	Breite und H�he der Textur werden f�rs Rendern in einem 'SDL_Rect' gespeichert
		int textureWidth, textureHeight;
		SDL_QueryTexture(pMessage, nullptr, nullptr, &textureWidth, &textureHeight);
		SDL_Rect messageRect = { positionVector.getX() + layerPosition.getX(), positionVector.getY() + layerPosition.getY() + height + 5, textureWidth, textureHeight };

		//	Der Text wird gerendert
		SDL_RenderCopy(TheGame::Instance()->getRenderer(), pMessage, nullptr, &messageRect);

		//	Die Textur wird wieder freigegeben, um ein Speicherleck zu verhindern
		SDL_DestroyTexture(pMessage);
	}
	/*	In den folgenden vier Zeilen wird das tats�chliche Rechteck gerendert
	 *
	 *		1. Position (abh�ngig von der Position des Layers) und Breite des Rechtecks festlegen
	 *		2. Farbe des Rechtecks festlegen
	 *		3. Rechteck malen
	 *		4. Die Farbe zum Rendern wieder neutralisieren (sonst w�re alles in der zuvor festgelegten Farbe)
	 */
	SDL_Rect rect = { positionVector.getX() + layerPosition.getX(), positionVector.getY() + layerPosition.getY(), width, height };
	SDL_SetRenderDrawColor(TheGame::Instance()->getRenderer(), m_color.r, m_color.g, m_color.b, m_color.a);
	SDL_RenderDrawRect(TheGame::Instance()->getRenderer(), &rect);
	SDL_SetRenderDrawColor(TheGame::Instance()->getRenderer(), 0, 0, 0, 255);
}
