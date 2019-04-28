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

class ConfigFileHandler : public RawConfigFileHandler {
private:
	void setConfig() {
		int i = 0;
		ConfigData.clear();

		ConfigData.push_back(ConfigDataStruct());
		ConfigData[i].Section = "GENERAL";
		ConfigData[i].Key = "BACKGROUND_COLOR_RGB";
		Configuration.BackgroundColorRGB.assign(3, 0);
		ConfigData[i].Value.push_back(std::to_string(Configuration.BackgroundColorRGB[0]));
		ConfigData[i].Value.push_back(std::to_string(Configuration.BackgroundColorRGB[1]));
		ConfigData[i].Value.push_back(std::to_string(Configuration.BackgroundColorRGB[2]));
		i++;

		ConfigData.push_back(ConfigDataStruct());
		ConfigData[i].Section = "BALL";
		ConfigData[i].Key = "COUNT";
		ConfigData[i].Value.push_back(std::to_string((Configuration.BallCount = 15)));
		i++;

		ConfigData.push_back(ConfigDataStruct());
		ConfigData[i].Section = "BALL";
		ConfigData[i].Key = "RADIUS";
		ConfigData[i].Value.push_back(std::to_string((Configuration.BallRadius = 15.0f)));
		i++;

		ConfigData.push_back(ConfigDataStruct());
		ConfigData[i].Section = "BALL";
		ConfigData[i].Key = "VELOCITY";
		ConfigData[i].Value.push_back(std::to_string((Configuration.AbsoluteVelocity = 0.1f)));
		i++;

		ConfigData.push_back(ConfigDataStruct());
		ConfigData[i].Section = "BALL";
		ConfigData[i].Key = "COLOR_RGB";
		Configuration.BallColorRGB.assign(3, 255);
		ConfigData[i].Value.push_back(std::to_string(Configuration.BallColorRGB[0]));
		ConfigData[i].Value.push_back(std::to_string(Configuration.BallColorRGB[1]));
		ConfigData[i].Value.push_back(std::to_string(Configuration.BallColorRGB[2]));

		if (writeFile())
		{
			SuccessfulWriting = true;
		}
		else
		{
			SuccessfulWriting = false;
		}
	}

	void checkConfig() {
		for (size_t i = 0; i < Configuration.BackgroundColorRGB.size(); i++)
		{
			if (Configuration.BackgroundColorRGB[i] < 0)
			{
				Configuration.BackgroundColorRGB[i] = 0;
				ConfigManipulated = true;
			}
			else if (Configuration.BackgroundColorRGB[i] > 255)
			{
				Configuration.BackgroundColorRGB[i] = 255;
				ConfigManipulated = true;
			}
		}

		if (Configuration.BallCount < 1)
		{
			Configuration.BallCount = 1;
			ConfigManipulated = true;
		}
		else if (Configuration.BallCount > 200)
		{
			Configuration.BallCount = 200;
			ConfigManipulated = true;
		}

		if (Configuration.BallRadius < 1.f)
		{
			Configuration.BallRadius = 1.f;
			ConfigManipulated = true;
		}
		else if (Configuration.BallRadius > 50.f)
		{
			Configuration.BallRadius = 50.f;
			ConfigManipulated = true;
		}

		if (Configuration.AbsoluteVelocity < 0.01f)
		{
			Configuration.AbsoluteVelocity = 0.01f;
			ConfigManipulated = true;
		}
		else if (Configuration.AbsoluteVelocity > 0.8f)
		{
			Configuration.AbsoluteVelocity = 0.8f;
			ConfigManipulated = true;
		}

		for (size_t i = 0; i < Configuration.BallColorRGB.size(); i++)
		{
			if (Configuration.BallColorRGB[i] < 0)
			{
				Configuration.BallColorRGB[i] = 0;
				ConfigManipulated = true;
			}
			else if (Configuration.BallColorRGB[i] > 255)
			{
				Configuration.BallColorRGB[i] = 255;
				ConfigManipulated = true;
			}
		}
	}

public:
	Configuration Configuration;
	bool SuccessfulReading;
	bool SuccessfulWriting;
	bool ConfigManipulated;

	ConfigFileHandler() :SuccessfulReading(false), SuccessfulWriting(false), ConfigManipulated(false) {}

	void getConfig() {
		if (readFile())
		{
			constexpr int DesiredParameterCount = 9;
			int ParameterCount = 0;

			for (size_t i = 0; i < ConfigData.size(); i++)
			{
				if (ConfigData[i].Section == "GENERAL")
				{
					if (ConfigData[i].Key == "BACKGROUND_COLOR_RGB")
					{
						for (size_t j = 0; j < ConfigData[i].Value.size(); j++)
						{
							Configuration.BackgroundColorRGB.push_back(stoi(ConfigData[i].Value[j]));
							ParameterCount++;
						}
					}
				}
				else if (ConfigData[i].Section == "BALL")
				{
					if (ConfigData[i].Key == "COUNT")
					{
						Configuration.BallCount = stoi(ConfigData[i].Value[0]);
						ParameterCount++;
					}
					else if (ConfigData[i].Key == "RADIUS")
					{
						Configuration.BallRadius = stof(ConfigData[i].Value[0]);
						ParameterCount++;
					}
					else if (ConfigData[i].Key == "VELOCITY")
					{
						Configuration.AbsoluteVelocity = stof(ConfigData[i].Value[0]);
						ParameterCount++;
					}
					else if (ConfigData[i].Key == "COLOR_RGB")
					{
						for (size_t j = 0; j < ConfigData[i].Value.size(); j++)
						{
							Configuration.BallColorRGB.push_back(stoi(ConfigData[i].Value[j]));
							ParameterCount++;
						}
					}
				}
			}
			if (DesiredParameterCount == ParameterCount)
			{
				SuccessfulReading = true;
				checkConfig();
			}
			else
			{
				SuccessfulReading = false;
				setConfig();
			}
		}
		else
		{
			SuccessfulReading = false;
			setConfig();
		}
	}
};