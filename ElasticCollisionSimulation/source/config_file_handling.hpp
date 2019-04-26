#pragma once
#include "raw_config_file_handling.hpp"

struct Configuration {
	//GENERAL:
	vector<int> BackgroundColorRGB;

	//BALL:
	int BallCount;
	float BallRadius;
	float AbsoluteVelocity;
	vector<int> BallColorRGB;
};

class ConfigFileHandler : RawConfigFileHandler {
public:
	Configuration Configuration;

	void getConfig() {
		readFile();
		for (size_t i = 0; i < ConfigData.size(); i++)
		{
			if (ConfigData[i].Section == "GENERAL")
			{
				if (ConfigData[i].Key == "BACKGROUND_COLOR_RGB")
				{
					for (size_t j = 0; j < ConfigData[i].Value.size(); j++)
					{
						Configuration.BackgroundColorRGB.push_back(stoi(ConfigData[i].Value[j]));
					}
				}
			}
			else if (ConfigData[i].Section == "BALL")
			{
				if (ConfigData[i].Key == "COUNT")
				{
					Configuration.BallCount = stoi(ConfigData[i].Value[0]);
				}
				else if (ConfigData[i].Key == "RADIUS")
				{
					Configuration.BallRadius = stof(ConfigData[i].Value[0]);
				}
				else if (ConfigData[i].Key == "VELOCITY")
				{
					Configuration.AbsoluteVelocity = stof(ConfigData[i].Value[0]);
				}
				else if (ConfigData[i].Key == "COLOR_RGB")
				{
					for (size_t j = 0; j < ConfigData[i].Value.size(); j++)
					{
						Configuration.BallColorRGB.push_back(stoi(ConfigData[i].Value[j]));
					}
				}
			}
		}
	}

};