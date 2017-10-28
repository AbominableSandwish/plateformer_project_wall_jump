#ifndef PLATFORM_H
#define PLATFORM_H

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <utility.h>

class Platform
{
public:
	Platform(b2World& world, 
		sf::Vector2f position=sf::Vector2f(400.f,600.f),
		sf::Vector2f size = sf::Vector2f(800.f, 100.f));
	~Platform();

	void draw(sf::RenderWindow&);
private:
	b2Body* body;
	sf::RectangleShape rect;
	sf::Vector2f center_position;
	sf::Vector2f size;

	ContactData contactData;
};


#endif