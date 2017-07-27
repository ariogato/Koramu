#include "Player.h"
#include "ParamLoader.h"
#include "InputHandler.h"

Player::Player()
{}

Player::~Player()
{}

void Player::load(const ParamLoader& params)
{
	//	Die Funktion der Klasse "SDL_GameObject" wird aufgerufen, um für uns den Job zu erledigen 
	SDL_GameObject::load(params);


	/*	Hier wird die Kollisionsbox des 'Player's definiert.
	 *	Die offset Variablen beschreiben die Position der Kollisionsbox
	 *	innerhalb des 'Player'-Objekts in Abhängigkeit von der oberen linken Ecke.
	 */

	
	ParamLoader collisionParams;
	float offsetX, offsetY;
	int collisionWidth, collisionHeight;

	collisionWidth = 70;
	collisionHeight = 30;
	offsetX = params.getWidth() - collisionWidth;
	offsetY = params.getHeight() - collisionHeight;

	collisionParams.setX(params.getX() + offsetX);
	collisionParams.setY(params.getY() + offsetY);
	collisionParams.setWidth(collisionWidth);
	collisionParams.setHeight(collisionHeight);

	ObjectRectangle tempRectangle;
	tempRectangle.load(collisionParams);
	m_collisionRects.push_back(tempRectangle);

	/*	Die Beschreibung der Box soll nicht angzeigt werden, 
	 *	während die Box selber durchaus angezeigt werden soll
	 */
	m_collisionRects[0].setVisible(true);
	m_collisionRects[0].setShowText(false);
	 
	
}

void Player::update()
{
#pragma region testStuff
	m_currentCol = 0;
	m_velocity.setX(0.0f);
	m_velocity.setY(0.0f);
	if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_RIGHT))
	{
		m_currentRow = 2;

		Vector2D temp(2.0f, 0.0f);

		m_velocity += temp;

		//	Die Funktion der Klasse "SDL_GameObject" wird aufgerufen, um für uns den Job zu erledigen 
		SDL_GameObject::update();
	}
	else if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_LEFT))
	{

		m_currentRow = 3;

		Vector2D temp(-2.0f, 0.0f);

		m_velocity += temp;

		//	Die Funktion der Klasse "SDL_GameObject" wird aufgerufen, um für uns den Job zu erledigen 
		SDL_GameObject::update();
	}
	else if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_UP))
	{

		m_currentRow = 1;

		Vector2D temp(0.0f, -2.0f);

		m_velocity += temp;

		//	Die Funktion der Klasse "SDL_GameObject" wird aufgerufen, um für uns den Job zu erledigen 
		SDL_GameObject::update();
	}
	else if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_DOWN))
	{

		m_currentRow = 0;

		Vector2D temp(0.0f, 2.0f);

		m_velocity += temp;

		//	Die Funktion der Klasse "SDL_GameObject" wird aufgerufen, um für uns den Job zu erledigen 
		SDL_GameObject::update();
	}
#pragma endregion
}

void Player::draw()
{
	//	Die Funktion der Klasse "SDL_GameObject" wird aufgerufen, um für uns den Job zu erledigen 
	SDL_GameObject::draw();
}
