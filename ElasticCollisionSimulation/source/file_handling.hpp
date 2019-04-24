#pragma once
#include <fstream>
#include <string>

struct Configuration {
	//GENERAL:
	int BackgroundColorRGB[3];

	//BALLS:
	int Count;
	float Radius;
	float AbsoluteVelocity;
	int ColorRGB[3];

	//Return Value:
	int Errno;
};

Configuration readFile();
int writeFile(Configuration& Configuration);