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
	box.friction = .0f;

	b2FixtureDef side_left;
	b2PolygonShape side_left_shape;
	side_left.isSensor = true;
	side_left_shape.SetAsBox(
		pixel2meter(2.0f) / 2.f, pixel2meter(size.y - 4.f) / 2.f,//taille 60 pixel de largeur et 2 pixele de longueur
		b2Vec2(pixel2meter(-size.x) / 2, 0.f),
		0.f);
	side_left.shape = &side_left_shape;
	contactData.data = this;
	contactData.contactDataType = ContactDataType::PLATFORM_CHARACTER;
	
	side_left.userData = &contactData;
	side_left.filter.categoryBits = SENSOR_WALL_LEFT;
	side_left.filter.maskBits = PLATEFORM;//radar only collides with aircraft

	b2FixtureDef side_right;
	b2PolygonShape side_right_shape;
	side_right.isSensor = true;
	side_right_shape.SetAsBox(
		pixel2meter(2.0f) / 2.f, pixel2meter(size.y - 4.f) / 2.f,//taille 60 pixel de largeur et 2 pixele de longueur
		b2Vec2(pixel2meter(+size.x) / 2, 0.f),
		0.f);
	side_right.shape = &side_right_shape;
	contactData.data = this;
	contactData.contactDataType = ContactDataType::PLATFORM_CHARACTER;

	side_right.userData = &contactData;
	side_right.filter.categoryBits = SENSOR_WALL_RIGHT;
	side_right.filter.maskBits = PLATEFORM;//radar only collides with aircraft

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
	foot.filter.categoryBits = SENSOR_FOOT;
	foot.filter.maskBits = PLATEFORM;//radar only collides with aircraft

	


	body->CreateFixture(&box);
	body->CreateFixture(&side_left);
	body->CreateFixture(&side_right);
	body->CreateFixture(&foot);
}

PlatformerCharacter::~PlatformerCharacter()
{
}

void PlatformerCharacter::update(float move_axis, bool jump_button)
{
	//manage movements
	if (etat_wall_jump != 0) {
		body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity()));
		
		etat_wall_jump--;
	}
	else {
		body->SetLinearVelocity(b2Vec2((walk_speed*move_axis), body->GetLinearVelocity().y));
		//body->ApplyLinearImpulse(b2Vec2((walk_speed*move_axis), body->GetLinearVelocity().y), body->GetWorldCenter(), true);
	}
	
	if (foot > 0 && jump_button)
	{
		body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, -jump_speed));
	}
	if (side != 0 && jump_button)
	{
		if (foot != 1) {
			etat_wall_jump = 45;
			body->SetLinearVelocity(b2Vec2(side*(jump_speed / 2), -jump_speed/2));
			//body->ApplyLinearImpulse(b2Vec2(side*(jump_speed / 2), -jump_speed / 2), body->GetWorldCenter(), true);
			//body->ApplyAngularImpulse((side*(jump_speed / 2), -jump_speed / 2), true);
			//body->ApplyForce(b2Vec2(side*(jump_speed / 2), -jump_speed / 2), body->GetWorldCenter(), true);
		}
	}

	if (side != 0 && move_axis != 0) {
		if (move_axis < 0) {
			//body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, -jump_speed / 50000));
			body->ApplyForce(b2Vec2(0, -8), body->GetWorldCenter(), true);
		}
		if (move_axis > 0) {
			body->ApplyForce(b2Vec2(0, -8), body->GetWorldCenter(), true);
		}
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


void PlatformerCharacter::touch_wall(int _direction)
{
	side=_direction;
}

void PlatformerCharacter::leave_wall()
{
	side=0;
}

b2Body* PlatformerCharacter::GetBody(){
	return this->body;
}