#include <SFML/Graphics.hpp>
#include <platformer_character.h>
#include <platform.h>
#include <utility.h>
#include <iostream>
#include <list>
#include <Box2D/Box2D.h>

//void CheckUserData(b2Contact* contact, Ball*& radarEntity, Ball*& aircraftEntity)
//{
//	b2Fixture* fixtureA = contact->GetFixtureA();
//	b2Fixture* fixtureB = contact->GetFixtureB();
//
//	//make sure only one of the fixtures was a sensor
//	bool sensorA = fixtureA->IsSensor();
//	bool sensorB = fixtureB->IsSensor();
//	if (!(sensorA ^ sensorB))
//		return false;
//
//	Ball* entityA = static_cast<Ball*>(fixtureA->GetBody()->GetUserData());
//	Ball* entityB = static_cast<Ball*>(fixtureB->GetBody()->GetUserData());
//
//	if (sensorA) { //fixtureB must be an enemy aircraft
//		radarEntity = entityA;
//		aircraftEntity = entityB;
//	}
//	else { //fixtureA must be an enemy aircraft
//		radarEntity = entityB;
//		aircraftEntity = entityA;
//	}
//}

void CheckUserData(void* userData, PlatformerCharacter** pCharPtr, Platform** platformPtr)
{	
	ContactData* data = static_cast<ContactData*>(userData);
	switch (data->contactDataType)
	{
	case ContactDataType::PLATFORM:
		*platformPtr = static_cast<Platform*>(data->data);
		break;
	case ContactDataType::PLATFORM_CHARACTER:
		*pCharPtr = static_cast<PlatformerCharacter*>(data->data);
		break;
	}
}

class MyContactListener : public b2ContactListener
{
	void BeginContact(b2Contact* contact) 
	{
		PlatformerCharacter* pChar = nullptr;
		Platform* platform = nullptr;
		if (contact->GetFixtureA()->GetUserData() != NULL)
		{
			CheckUserData(contact->GetFixtureA()->GetUserData(), &pChar, &platform);
		}
		if (contact->GetFixtureB()->GetUserData() != NULL)
		{
			CheckUserData(contact->GetFixtureB()->GetUserData(), &pChar, &platform);
		}
		if (platform != nullptr && pChar != nullptr)
		{
			std::cout << "Contact: ";
			if (contact->GetFixtureA()->GetFilterData().categoryBits == SENSOR_FOOT) {
				std::cout << "Ground\n";
				pChar->touch_ground();
			}
			if (contact->GetFixtureA()->GetFilterData().categoryBits == SENSOR_WALL_LEFT) {
				std::cout << "Wall\n";
				pChar->touch_wall(1);
			}
			if (contact->GetFixtureA()->GetFilterData().categoryBits == SENSOR_WALL_RIGHT) {
				std::cout << "Wall\n";
				pChar->touch_wall(-1);
			}
		}
	}

	void EndContact(b2Contact* contact) {

		PlatformerCharacter* pChar = nullptr;
		Platform* platform = nullptr;
		if (contact->GetFixtureA()->GetUserData() != NULL)
		{
			CheckUserData(contact->GetFixtureA()->GetUserData(), &pChar, &platform);
		}
		if (contact->GetFixtureB()->GetUserData() != NULL)
		{
			CheckUserData(contact->GetFixtureB()->GetUserData(), &pChar, &platform);
		}
		if (platform != nullptr && pChar != nullptr)
		{
			if (contact->GetFixtureA()->GetFilterData().categoryBits == SENSOR_FOOT) {
				pChar->leave_ground();
			}
			if (contact->GetFixtureA()->GetFilterData().categoryBits == SENSOR_WALL_LEFT
				|| contact->GetFixtureA()->GetFilterData().categoryBits == SENSOR_WALL_RIGHT) {
				pChar->leave_wall();
			}
		}
	}
};

int main()
{

	b2Vec2 gravity(0, 9.8f); //normal earth gravity, 9.8 m/s/s straight down!

	b2World myWorld = b2World(gravity);
	//at global scope
	MyContactListener myContactListenerInstance;

	//in FooTest constructor
	myWorld.SetContactListener(&myContactListenerInstance);

	sf::RenderWindow window(sf::VideoMode(1200, 800), "SFML works!");
	window.setFramerateLimit(framerate);
	

	float32 timeStep = 1 / framerate;      //the length of time passed to simulate (seconds)
	int32 velocityIterations = 8;   //how strongly to correct velocity
	int32 positionIterations = 3;   //how strongly to correct position

	PlatformerCharacter character(myWorld);

	std::list<Platform> platforms =
	{
		Platform(myWorld, sf::Vector2f(600.f,800.f), sf::Vector2f(1200.f,50.f)),
		Platform(myWorld, sf::Vector2f(600.f,0.f), sf::Vector2f(1200.f,50.f)),
		Platform(myWorld, sf::Vector2f(150.f,200.f), sf::Vector2f(200.f,50.f)),
		Platform(myWorld, sf::Vector2f(650.f,500.f), sf::Vector2f(200.f,50.f)),
		Platform(myWorld, sf::Vector2f(0.f,400.f), sf::Vector2f(100.f,800.f)),
		Platform(myWorld, sf::Vector2f(800.f,400.f), sf::Vector2f(150.f,550.f)),
		Platform(myWorld, sf::Vector2f(1200.f,400.f), sf::Vector2f(100.f,800.f))
	};
	float move_axis = 0.0f;

	while (window.isOpen())
	{
		bool jump_button = false;
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			move_axis = -1.0f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			move_axis = +1.0f;
		}
		myWorld.Step(timeStep, velocityIterations, positionIterations);
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			if (event.type == sf::Event::KeyReleased) {
				if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Right) {
					move_axis = 0.0f;
				}
			}
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::Space)
				{
					jump_button = true;
				}
			}
		}
		
		character.update(move_axis, jump_button);
		
		window.clear();
		character.draw(window);
		for (Platform& platform : platforms)
		{
			platform.draw(window);
		}
		window.display();
	}

	system("pause");
	return 0;
}