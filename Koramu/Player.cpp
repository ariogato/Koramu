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
}

void Player::update()
{
#pragma region testStuff
	if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_RIGHT))
	{
		//	Die Funktion der Klasse "SDL_GameObject" wird aufgerufen, um für uns den Job zu erledigen 
		SDL_GameObject::update();

		m_currentRow = 2;

		Vector2D temp(1.0f, 0.0f);

		m_positionVector += temp;
	}
	else if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_LEFT))
	{
		//	Die Funktion der Klasse "SDL_GameObject" wird aufgerufen, um für uns den Job zu erledigen 
		SDL_GameObject::update();

		m_currentRow = 3;

		Vector2D temp(-1.0f, 0.0f);

		m_positionVector += temp;
	}
	else if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_UP))
	{
		//	Die Funktion der Klasse "SDL_GameObject" wird aufgerufen, um für uns den Job zu erledigen 
		SDL_GameObject::update();

		m_currentRow = 1;

		Vector2D temp(0.0f, -1.0f);

		m_positionVector += temp;
	}
	else if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_DOWN))
	{
		//	Die Funktion der Klasse "SDL_GameObject" wird aufgerufen, um für uns den Job zu erledigen 
		SDL_GameObject::update();

		m_currentRow = 0;

		Vector2D temp(0.0f, 1.0f);

		m_positionVector += temp;
	}
#pragma endregion
}

void Player::draw()
{
	//	Die Funktion der Klasse "SDL_GameObject" wird aufgerufen, um für uns den Job zu erledigen 
	SDL_GameObject::draw();
}
