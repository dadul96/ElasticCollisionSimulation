/*
Title: Elastic Collision Simulation
Description: This program simulates elastic collisions of balls in an boxed area.
Author: Daniel Duller
Version: 0.1.0
Creation date: 14.04.2019
Last change: 20.04.2019
*/

#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <time.h>
#include <math.h>

const float PI = 3.1415f;
const int WINDOW_WIDTH = 500;
const int WINDOW_HEIGHT = 500;

void renderingThread(sf::RenderWindow* Window);

int main()
{
	sf::ContextSettings Settings;
	Settings.antialiasingLevel = 4;

	sf::RenderWindow Window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Elastic Collision Simulation", sf::Style::Close, Settings);

	Window.setActive(false);
	sf::Thread thread(&renderingThread, &Window);
	thread.launch();

	while (Window.isOpen())
	{
		sf::Event event;
		while (Window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				Window.close();
			}
		}
	}
	return 0;
}

void renderingThread(sf::RenderWindow* Window)
{
	Window->setActive(true);
	Window->setVerticalSyncEnabled(true);

	const float BottomBorder = float(WINDOW_HEIGHT);
	const float TopBorder = 0.f;
	const float LeftBorder = 0.f;
	const float RightBorder = float(WINDOW_WIDTH);
	const float Radius = 15.f;
	const float Diameter = 2.f * Radius;
	const float AbsoluteVelocity = .1f;
	const int BallCount = 15;
	const int BallMass = 1;

	float Angle = 0.f;
	float AbsoluteDistance = 0.f;
	float OverlappingDistance = 0.f;

	sf::Vector2f Position[BallCount];
	sf::Vector2f Velocity[BallCount];

	sf::Vector2f DeltaPosition;
	sf::Vector2f UnitVectorDeltaPosition;
	sf::Vector2f DeltaVelocity;

	sf::Time ActualTimeDelta;
	sf::Int32 IntegerTimeDelta;

	sf::CircleShape Ball[BallCount];

	srand(static_cast<unsigned int>(time(NULL)));

	for (size_t i = 0; i < BallCount; i++)
	{
		Position[i].x = static_cast<float>(rand() % 400 + 50);
		Position[i].y = static_cast<float>(rand() % 400 + 50);

		Angle = static_cast<float>(rand() % 360);
		Angle = (Angle * PI) / 180.f;

		Velocity[i].x = AbsoluteVelocity * cosf(Angle);
		Velocity[i].y = AbsoluteVelocity * sinf(Angle);

		Ball[i].setRadius(Radius);
		Ball[i].setFillColor(sf::Color::Green);
		Ball[i].setPosition(Position[i].x - Radius, Position[i].y - Radius);
	}

	sf::Clock Clock;

	while (Window->isOpen())
	{
		Window->clear(sf::Color::Black);

		ActualTimeDelta = Clock.restart();
		IntegerTimeDelta = ActualTimeDelta.asMilliseconds();

		for (size_t i = 0; i < BallCount; i++)
		{
			for (size_t j = 0; j < BallCount; j++)
			{
				if (j != i)
				{
					DeltaPosition = Position[j] - Position[i];
					DeltaVelocity = Velocity[j] - Velocity[i];
					AbsoluteDistance = sqrt((DeltaPosition.x * DeltaPosition.x) + (DeltaPosition.y * DeltaPosition.y));
					UnitVectorDeltaPosition.x = DeltaPosition.x / AbsoluteDistance;
					UnitVectorDeltaPosition.y = DeltaPosition.y / AbsoluteDistance;

					if (AbsoluteDistance < Diameter)
					{
						Velocity[j] -= (2 * BallMass / (BallMass + BallMass)) * (((DeltaVelocity.x * DeltaPosition.x) + (DeltaVelocity.y * DeltaPosition.y)) / (pow((DeltaPosition.x), 2) + pow((DeltaPosition.y), 2))) * DeltaPosition;
						DeltaPosition = (-DeltaPosition);
						DeltaVelocity = (-DeltaVelocity);
						Velocity[i] -= (2 * BallMass / (BallMass + BallMass)) * (((DeltaVelocity.x * DeltaPosition.x) + (DeltaVelocity.y * DeltaPosition.y)) / (pow((DeltaPosition.x), 2) + pow((DeltaPosition.y), 2))) * DeltaPosition;

						Position[j].x += (Velocity[j].x * IntegerTimeDelta);
						Position[j].x += (Velocity[j].y * IntegerTimeDelta);
						Position[i].x += (Velocity[i].x * IntegerTimeDelta);
						Position[i].x += (Velocity[i].y * IntegerTimeDelta);

						OverlappingDistance = Diameter - AbsoluteDistance;

						Ball[j].setPosition((Position[j].x += (OverlappingDistance * UnitVectorDeltaPosition.x)), (Position[j].y += OverlappingDistance * UnitVectorDeltaPosition.y));
						Ball[i].setPosition((Position[i].x -= (OverlappingDistance * UnitVectorDeltaPosition.x)), (Position[i].y -= OverlappingDistance * UnitVectorDeltaPosition.y));
					}
				}
			}

			if (Position[i].x < LeftBorder)
			{
				Ball[i].setPosition((Position[i].x = LeftBorder), Position[i].y);
				Velocity[i].x *= -1.f;
			}
			if ((Position[i].x + Diameter) > RightBorder)
			{
				Ball[i].setPosition((Position[i].x = (RightBorder - Diameter)), Position[i].y);
				Velocity[i].x *= -1.f;
			}
			if (Position[i].y < TopBorder)
			{
				Ball[i].setPosition(Position[i].x, (Position[i].y = TopBorder));
				Velocity[i].y *= -1.f;
			}
			if ((Position[i].y + Diameter) > BottomBorder)
			{
				Ball[i].setPosition(Position[i].x, (Position[i].y = (BottomBorder - Diameter)));
				Velocity[i].y *= -1.f;
			}

			Ball[i].setPosition((Position[i].x += (Velocity[i].x * IntegerTimeDelta)), (Position[i].y += (Velocity[i].y * IntegerTimeDelta)));
			Window->draw(Ball[i]);
		}

		Window->display();
	}
}
