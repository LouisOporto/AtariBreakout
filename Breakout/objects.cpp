#include "objects.hpp"
#include <algorithm>
#include <SFML/Graphics.hpp>

bool Object::CollidesWith (const Object* other) const {
	if (other->GetWidth() == 0 || other->GetHeight() == 0 || GetWidth() == 0 || GetHeight() == 0) return false;
	// Origin must be bigger than bottom right of other object
	if (GetX() < other->GetX() + other->GetWidth() && 
		GetX() + GetWidth() > other->GetX() && 
		GetY() < other->GetY() + other->GetHeight() && 
		GetY() + GetHeight() > other->GetY()) return true;
	return false;
}

// Need to improve code for collision on the sides of bricks.
bool Object::CollidesWithX  (const Object* other) const {
	if (other->GetWidth() == 0 || other->GetHeight() == 0 || GetWidth() == 0 || GetHeight() == 0) return false;
	// Origin must be bigger than bottom right of other object
	float left = std::abs(other->GetX() - GetX() - GetWidth());
	float right = std::abs(other->GetX() + other->GetWidth() - GetX());
	float bot = std::abs(other->GetY() + other->GetHeight() - GetY());
	float top = std::abs(other->GetY() - GetY() - GetHeight());
	
	float wall = std::min(left, right);
	float level = std::min(top, bot);
	std::cout << "Collision Flip\n";
	//std::cout << "Wall:" << wall << " & " << "Level:" << level << '\n';
	if (wall < level) { // Ball is on the side of object
		return true;
	}
	else { // Ball is either on top our below the object
		return false;
	}
}

void Brick::Draw() const {
	sf::RectangleShape brick(sf::Vector2f(GetWidth(), GetHeight()));
	brick.setPosition(sf::Vector2f(GetX(), GetY()));
	brick.setFillColor(color_);

	GetWindow()->draw(brick);
}

void Ball::Draw() const {
	sf::CircleShape ball(GetWidth() / 2.f);
	ball.setPosition(sf::Vector2f(GetX(), GetY()));
	ball.setFillColor(sf::Color(210, 25, 25));

	GetWindow()->draw(ball);
}