#include <platformer_character.h>
#include <iostream>

PlatformerCharacter::PlatformerCharacter(b2World & world)
{
	//Create the rectangle
	rect.setPosition(center_position - size / 2.f);
	rect.setSize(size);
	rect.setFillColor(sf::Color::Green);
	//Create the body
	b2BodyDef myBodyDef;
	myBodyDef.type = b2_dynamicBody;
	myBodyDef.position = pixel2meter(center_position);
	myBodyDef.fixedRotation = true;
	body = world.CreateBody(&myBodyDef);

	//Create the fixtures
	b2FixtureDef box;
	b2PolygonShape box_shape;
	box_shape.SetAsBox(
		pixel2meter(size.x) / 2.f, pixel2meter(size.y) / 2.f);
	box.shape = &box_shape;
	box.friction = 0.f;

	b2FixtureDef coastleft;
	b2PolygonShape coastleft_shape;
	coastleft.isSensor = true;
	coastleft_shape.SetAsBox(
		pixel2meter(2.0f) / 2.f, pixel2meter(size.y - 4.f) / 2.f,//taille 60 pixel de largeur et 2 pixele de longueur
		b2Vec2(pixel2meter(-size.x) / 2, 0.f),
		0.f);
	coastleft.shape = &coastleft_shape;
	contactData.data = this;
	contactData.contactDataType = ContactDataType::PLATFORM_WALL_LEFT;
	coastleft.userData = &contactData;

	//b2FixtureDef coast_right;
	//b2PolygonShape coast_right_shape;
	//coast_right.isSensor = true;
	//coast_right_shape.SetAsBox(
	//	pixel2meter(2.0f) / 2.f, pixel2meter(size.y - 4.f) / 2.f,//taille 60 pixel de largeur et 2 pixele de longueur
	//	b2Vec2(pixel2meter(+size.x) / 2, 0.f),
	//	0.f);
	//coast_right.shape = &coast_right_shape;
	//contactData.contactDataType = ContactDataType::PLATFORM_WALL_RIGHT;
	//contactData.data = this;
	//coast_right.userData = &contactData;
	
	b2FixtureDef foot;
	b2PolygonShape foot_shape;
	foot.isSensor = true;
	foot_shape.SetAsBox(
		pixel2meter(size.x - 4.f) / 2.f, pixel2meter(2.0f) / 2.f,//taille 60 pixel de longeur et 2 pixel de largeur
		b2Vec2(0.f, pixel2meter(size.y)/2),//position depuis le centre
		0.f);
	foot.shape = &foot_shape;
	contactData.data = this;
	contactData.contactDataType = ContactDataType::PLATFORM_CHARACTER;
	foot.userData = &contactData;

	


	body->CreateFixture(&box);
	body->CreateFixture(&coastleft);
	body->CreateFixture(&foot);
}

PlatformerCharacter::~PlatformerCharacter()
{
}

void PlatformerCharacter::update(float move_axis, bool jump_button)
{
	//manage movements
	body->SetLinearVelocity(b2Vec2(walk_speed*move_axis, body->GetLinearVelocity().y));
	if (foot > 0 && jump_button)
	{
		body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, -jump_speed));

	}
	if (coast > 0 && jump_button)
	{
		body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().y, -jump_speed));
		
	}

	center_position = meter2pixel(body->GetPosition());
	rect.setPosition(center_position - size / 2.f);
}

void PlatformerCharacter::draw(sf::RenderWindow& window)
{
	window.draw(rect);
}

void PlatformerCharacter::touch_ground()
{
	foot++;
}

void PlatformerCharacter::leave_ground()
{
	foot--;
}


void PlatformerCharacter::touch_wall()
{
	//coast;
}

void PlatformerCharacter::leave_wall()
{
	//coast--;
}

b2Body* PlatformerCharacter::GetBody(){
	return this->body;
}