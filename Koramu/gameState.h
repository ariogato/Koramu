#pragma once

namespace FiniteStateMachine 
{
	enum class GameStateIDs
	{
		menuState, playState, pauseState
	};


	class GameState
	{
	private:
		GameStateIDs stateID;
	public:
		GameState();
		virtual ~GameState();

		virtual void onEnter() = 0;
		virtual void onExit() = 0;
		virtual void handleInput() = 0;
		virtual void update() = 0;
		virtual void render() = 0;
		virtual void getState() = 0;
	};

	
};