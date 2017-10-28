#include "utility.h"


const float pixel_per_meter = 64.f;

float pixel2meter(float pixel)
{
	return pixel/pixel_per_meter;
}

float meter2pixel(float meter)
{
	return meter*pixel_per_meter;
}

b2Vec2 pixel2meter(sf::Vector2f pixels)
{
	return b2Vec2(pixel2meter(pixels.x), pixel2meter(pixels.y));
}

sf::Vector2f meter2pixel(b2Vec2 meters)
{
	return sf::Vector2f(meter2pixel(meters.x), meter2pixel(meters.y));
}
