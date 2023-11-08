#pragma once
#include <iostream>
#include <set>
#include "objects.hpp"

#include <SFML/Graphics.hpp>

class Game {
public:
	enum GameState {
		kNotStarted = 1,
		kInProgress = 2,
		kLost = 3,
		kWon = 4,
	};

	Game();
	~Game();

	void Intialize();

	void Start();

private:
	void Draw();
	void DrawGameState();

	Ball* ball = nullptr;
	Paddle* paddle = nullptr;
	std::set<Brick*> bricks_;
	sf::RenderWindow window_;
	float speed_x_;
	float speed_y_;
	GameState state_ = GameState::kNotStarted;

	sf::Font font;
};