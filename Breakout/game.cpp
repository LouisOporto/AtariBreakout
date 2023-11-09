#include "game.hpp"

#include <iostream>
#include <set>
#include <vector>

const float kSize = 500;
const int kBrickHeight = 20;
const int kPaddleWidth = 80;
const int kBrickWidth = 48;
const int kBrickSpacing = 2;
const int kBallDiameter = 18;
const float kInitialSpeed = 5;
const int kNumRows = 5;
const sf::Color red(255, 0, 0);
const sf::Color orange(255, 125, 0);
const sf::Color green(0, 200, 100);
const sf::Color teal(0, 125, 255);
const sf::Color blue(0, 0, 255);
const sf::Color white(255, 255, 255);

Game::Game() : speed_x_(kInitialSpeed), speed_y_(kInitialSpeed) {
	font.loadFromFile("arial.ttf");
}

Game::~Game() {
	delete ball;
	delete paddle;
	for (auto brick : bricks_) {
		delete brick;
	}
	bricks_.clear();
}

void Game::Intialize() {
	// Create game window
	window_.create(sf::VideoMode(kSize, kSize), "Atari Breakout");

	// Create paddle, balls, and bricks for the game
	paddle = new Paddle(kPaddleWidth, kBrickHeight, &window_);
	paddle->SetX(kSize / 2 - kPaddleWidth / 2);
	paddle->SetY(kSize - kBrickHeight * 3);

	ball = new Ball(kBallDiameter, &window_);
	ball->SetX(kSize / 2 - kBallDiameter / 2);
	ball->SetY(kSize / 2 - kBallDiameter / 2);

	std::vector<sf::Color> colors;
	colors.push_back(red);
	colors.push_back(orange);
	colors.push_back(green);
	colors.push_back(teal);
	colors.push_back(blue);

	//Make it scalable by getting size / (brick width + brick spacing)
	for (int i = 0; i < kSize / (kBrickWidth + kBrickSpacing); i++) {
		for (int j = 0; j < kNumRows; j++) {
			Brick* brick = new Brick(kBrickWidth, kBrickHeight, colors[j % kNumRows], &window_);
			brick->SetX(i * (kBrickWidth + kBrickSpacing));
			brick->SetY(j * (kBrickHeight + kBrickSpacing));
			bricks_.insert(brick);
		}
	}
	Draw();
}

void Game::Start() {
	sf::Clock clock;
	float accumalator = 0.f;
	float dt = 0.f;
	bool pause = false;

	while (window_.isOpen()) {
		dt = clock.restart().asSeconds();

		sf::Event event;
		if (window_.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window_.close();
			}
			if (event.type == sf::Event::MouseMoved) paddle->SetX(event.mouseMove.x);
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
				if(state_ == GameState::kNotStarted) state_ = GameState::kInProgress;
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P) {
				pause = !pause;
			}
		}

		if (!pause) {
			// Has to be inside when the game is running
			accumalator += dt;
			if (state_ == GameState::kInProgress) {
				//Run game logic here
				if (ball->GetX() <= 0 || ball->GetX() + ball->GetWidth() >= kSize) speed_x_ *= -1;
				if (ball->GetY() <= 0) speed_y_ *= -1;
				else if (ball->GetY() + ball->GetHeight() >= kSize) state_ = GameState::kLost;

				if (ball->CollidesWith(paddle)) {
					if (ball->CollidesWithX(paddle)) speed_x_ *= -1;
					else speed_y_ *= -1;
				}
				for (auto it = bricks_.begin(); it != bricks_.end();) {
					if (ball->CollidesWith(*it)) {
						if (ball->CollidesWithX(*it)) speed_x_ *= -1;
						else speed_y_ *= -1;
						it = bricks_.erase(it);
					}
					else it++;
				}

				if (bricks_.size() == 0) state_ = GameState::kWon;

				ball->SetX(ball->GetX() + speed_x_ * 60.0f * dt);
				ball->SetY(ball->GetY() + speed_y_ * 60.0f * dt);
			}

			//Draw here after a set amount of time has elapsed
			if (accumalator > 1.0 / 60.0) {
				Draw();
				accumalator -= 1.0 / 60.0;
			}
		}
	}
}

void Game::Draw() {
	// Do other stuff in this area of code dealing with drawing all the objects in the game
	window_.clear(white);
	ball->Draw();
	paddle->Draw();
	for (auto brick : bricks_) {
		brick->Draw();
	}
	DrawGameState();
	window_.display();
}

void Game::DrawGameState() {
	
	/*
		kNotStarted = 1,
		kInProgress = 2,
		kLost = 3,
		kWon = 4,
	*/
	if (state_ == GameState::kInProgress) return;

	std::string message;
	if (state_ == GameState::kNotStarted) message = "Click to play!";
	else if (state_ == GameState::kLost) message = "GAME OVER";
	else if (state_ == GameState::kWon) message = "YOU WON";

	sf::Text text;
	text.setString(message);
	text.setFont(font);
	text.setCharacterSize(kBrickHeight * 2);
	text.setFillColor(red);

	// We use textRect to get bounds of string due to the difference in strings used
	// Dynamic method to ensure the text is centered
	sf::FloatRect textRect = text.getGlobalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	text.setPosition(kSize / 2, kSize / 2 - kBrickWidth);

	window_.draw(text);
}