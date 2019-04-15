#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define PI 3.1416
#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500

void renderingThread(sf::RenderWindow* Window);

int main()
{
	sf::RenderWindow Window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Elastic Collision Simulation", sf::Style::Close);

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
	const float Radius = 10.f;
	const float AbsoluteVelocity = .1f;
	const int BallCount = 15;
	const int BallMass = 1;

	sf::Vector2f Position[BallCount];
	sf::Vector2f Velocity[BallCount];

	sf::Vector2f DeltaPosition;
	sf::Vector2f DeltaVelocity;

	float Angle = 0.f;

	sf::Time ActualTimeDelta;
	sf::Int32 IntegerTimeDelta;

	sf::CircleShape Ball[BallCount];

	srand(time(NULL));

	for (size_t i = 0; i < BallCount; i++)
	{
		Position[i].x = float(rand() % 400 + 50);
		Position[i].y = float(rand() % 400 + 50);

		Angle = float(rand() % 360);
		Angle = Angle * PI / 180.0;

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
			Position[i] = Ball[i].getPosition();

			for (size_t j = 0; j < BallCount; j++)
			{
				if (j != i)
				{
					if ((pow((Position[i].x - Position[j].x), 2) + pow((Position[i].y - Position[j].y), 2)) <= pow((2 * Radius), 2))
					{
						if (Position[i].x <= Position[j].x)
						{
							Ball[j].setPosition((Position[j].x += 1.f), Position[j].y);
						}
						if (Position[i].y <= Position[j].y)
						{
							Ball[j].setPosition(Position[j].x, (Position[j].y += 1.f));
						}
						if (Position[i].x >= Position[j].x)
						{
							Ball[j].setPosition((Position[j].x -= 1.f), Position[j].y);
						}
						if (Position[i].y >= Position[j].y)
						{
							Ball[j].setPosition(Position[j].x, (Position[j].y -= 1.f));
						}
						DeltaPosition = Position[j] - Position[i];
						DeltaVelocity = Velocity[j] - Velocity[i];
						Velocity[j] -= (2*BallMass/(BallMass+BallMass)) * ( ((DeltaVelocity.x*DeltaPosition.x)+(DeltaVelocity.y*DeltaPosition.y)) / (pow((DeltaPosition.x), 2) + pow((DeltaPosition.y), 2)) ) * DeltaPosition;
					}
				}
			}

			if (Position[i].x <= LeftBorder)
			{
				Ball[i].setPosition((Position[i].x = (LeftBorder + 1.f)), Position[i].y);
				Velocity[i].x *= -1.f;
			}
			if ((Position[i].x + (2.f * Radius)) >= RightBorder)
			{
				Ball[i].setPosition((Position[i].x = (RightBorder - ((2.f * Radius) + 1.f))), Position[i].y);
				Velocity[i].x *= -1.f;
			}
			if (Position[i].y <= TopBorder)
			{
				Ball[i].setPosition(Position[i].x, (Position[i].y = (TopBorder + 1.f)));
				Velocity[i].y *= -1.f;
			}
			if ((Position[i].y + (2.f * Radius)) >= BottomBorder)
			{
				Ball[i].setPosition(Position[i].x, (Position[i].y = (BottomBorder - ((2.f * Radius) + 1.f))));
				Velocity[i].y *= -1.f;
			}

			Ball[i].move(Velocity[i].x * IntegerTimeDelta, Velocity[i].y * IntegerTimeDelta);
			Window->draw(Ball[i]);
		}

		Window->display();
	}
}
