#include <platform.h>

Platform::Platform(b2World & world, sf::Vector2f position, sf::Vector2f size)
{
	center_position = position;
	this->size = size;
	//Create the rectangle
	rect.setPosition(center_position - size / 2.f);
	rect.setSize(size);
	rect.setFillColor(sf::Color::Red);
	//Create the body
	b2BodyDef myBodyDef;
	myBodyDef.type = b2_staticBody;
	myBodyDef.position = pixel2meter(center_position);
	body = world.CreateBody(&myBodyDef);

	//Create the fixtures
	b2FixtureDef box;
	b2PolygonShape box_shape;
	box_shape.SetAsBox(pixel2meter(size.x) / 2.f, pixel2meter(size.y) / 2.f);
	box.shape = &box_shape;
	contactData.contactDataType = ContactDataType::PLATFORM;
	contactData.data = this;
	box.userData = &contactData;
	body->CreateFixture(&box);
}

Platform::~Platform()
{
}

void Platform::draw(sf::RenderWindow & window)
{
	window.draw(rect);
}
