#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

class Object {
public:
	Object(int width, int height, sf::RenderWindow* window) : width_(width), height_(height), window_(window) {}

	void SetX(float x) { x_ = x; }
	void SetY(float y) { y_ = y; }

	float GetX() const { return x_; }
	float GetY() const { return y_; }
	int GetWidth() const { return width_; }
	int GetHeight() const { return height_; }

	virtual void Draw() const = 0; // Pure Function

	bool CollidesWith(const Object* other) const;
	bool CollidesWithX(const Object* other) const;

protected:
	sf::RenderWindow* GetWindow() const { return window_; }

private:
	int width_;
	int height_;
	float x_;
	float y_;
	sf::RenderWindow* window_;
};

class Brick : public Object {
public:
	Brick(int width, int height, sf::Color color, sf::RenderWindow* window) : Object(width, height, window), color_(color) {}

	void Draw() const override;
private:
	sf::Color color_;
};

class Paddle : public Brick {
public:
	Paddle(int width, int height, sf::RenderWindow* window) : Brick(width, height, sf::Color(50, 50, 50), window) {}
};

class Ball : public Object {
public:
	Ball(int diameter, sf::RenderWindow* window) : Object(diameter, diameter, window) {}

	void Draw() const override;
};