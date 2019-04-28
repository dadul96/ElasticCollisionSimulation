/*
Title: Elastic Collision Simulation
Description: This program simulates elastic collisions of balls in a boxed area.
Author: Daniel Duller
Version: 0.2.0
Creation date: 14.04.2019
Last change: 26.04.2019
*/

#include <SFML/Graphics.hpp>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <thread>
#include "config_file_handling.hpp"


void rendering(sf::RenderWindow* pWindow, const int& pWINDOW_WIDTH, const int& pWINDOW_HEIGHT, const ConfigFileHandler& pConfigHandler);

int main() {
	constexpr int WindowWidth = 500;
	constexpr int WindowHeight = 500;

	ConfigFileHandler ConfigHandler;
	ConfigHandler.setFilePath("config.ini");
	ConfigHandler.getConfig();

	sf::ContextSettings Settings;
	Settings.antialiasingLevel = 4;

	sf::RenderWindow Window(sf::VideoMode(WindowWidth, WindowHeight), "Elastic Collision Simulation", sf::Style::Close, Settings);
	Window.setActive(false);

	std::thread renderingThread(&rendering, &Window, WindowWidth, WindowHeight, ConfigHandler);

	while (Window.isOpen())
	{
		sf::Event event;
		while (Window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				Window.close();
				renderingThread.join();
			}
		}
	}
	return 0;
}

void rendering(sf::RenderWindow* pWindow, const int& pWINDOW_WIDTH, const int& pWINDOW_HEIGHT, const ConfigFileHandler& pConfigHandler) {
	pWindow->setActive(true);
	pWindow->setVerticalSyncEnabled(true);

	constexpr float PI = 3.1415f;
	const float BottomBorder = static_cast<float>(pWINDOW_HEIGHT);
	constexpr float TopBorder = 0.f;
	constexpr float LeftBorder = 0.f;
	const float RightBorder = static_cast<float>(pWINDOW_WIDTH);
	const unsigned int BallMass = 1;

	float Radius = pConfigHandler.Configuration.BallRadius;
	float Diameter = 2.f * Radius;
	float AbsoluteVelocity = pConfigHandler.Configuration.AbsoluteVelocity;
	unsigned int BallCount = pConfigHandler.Configuration.BallCount;
	
	float Angle = 0.f;
	float AbsoluteDistance = 0.f;
	float OverlappingDistance = 0.f;

	sf::Color BackgroundColor(pConfigHandler.Configuration.BackgroundColorRGB[0], pConfigHandler.Configuration.BackgroundColorRGB[1], pConfigHandler.Configuration.BackgroundColorRGB[2]);
	sf::Color BallColor(pConfigHandler.Configuration.BallColorRGB[0], pConfigHandler.Configuration.BallColorRGB[1], pConfigHandler.Configuration.BallColorRGB[2]);

	sf::Vector2f TemplateVector2f;
	vector<sf::Vector2f> Position;
	vector<sf::Vector2f> Velocity;

	sf::Vector2f DeltaPosition;
	sf::Vector2f UnitVectorDeltaPosition;
	sf::Vector2f DeltaVelocity;

	sf::Time ActualTimeDelta;
	sf::Int32 IntegerTimeDelta;

	sf::CircleShape TemplateCircleShape;
	vector<sf::CircleShape> Ball;

	srand(static_cast<unsigned int>(time(NULL)));

	for (size_t i = 0; i < BallCount; i++)
	{
		Position.push_back(TemplateVector2f);
		Position[i].x = static_cast<float>(rand() % 400 + 50);
		Position[i].y = static_cast<float>(rand() % 400 + 50);

		Angle = static_cast<float>(rand() % 360);
		Angle = (Angle * PI) / 180.f;

		Velocity.push_back(TemplateVector2f);
		Velocity[i].x = AbsoluteVelocity * cosf(Angle);
		Velocity[i].y = AbsoluteVelocity * sinf(Angle);

		Ball.push_back(TemplateCircleShape);
		Ball[i].setRadius(Radius);
		Ball[i].setFillColor(BallColor);
		Ball[i].setPosition(Position[i].x - Radius, Position[i].y - Radius);
	}

	sf::Clock Clock;

	while (pWindow->isOpen())
	{
		pWindow->clear(BackgroundColor);

		ActualTimeDelta = Clock.restart();
		IntegerTimeDelta = ActualTimeDelta.asMilliseconds();

		for (size_t i = 0; i < BallCount; i++)
		{
			for (size_t j = i + 1; j < BallCount; j++)
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
			pWindow->draw(Ball[i]);
		}

		pWindow->display();
	}
}
