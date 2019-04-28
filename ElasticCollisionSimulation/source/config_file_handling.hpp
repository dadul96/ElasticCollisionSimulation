#pragma once
#include "raw_config_file_handling.hpp"

struct Configuration {
	//GENERAL-Section:
	vector<int> BackgroundColorRGB;

	//BALL-Section:
	int BallCount;
	float BallRadius;
	float AbsoluteVelocity;
	vector<int> BallColorRGB;

	//constructor:
	Configuration() : BallCount(0), BallRadius(0.0), AbsoluteVelocity(0.0) {}
};

class ConfigFileHandler : public RawConfigFileHandler {
private:
	//for setting the default configuration and creating a config.ini file (only if a reading OR parsing problem occurred):
	void setConfig() {
		int i = 0;
		RawConfigData.clear();

		RawConfigData.push_back(ConfigDataStruct());
		RawConfigData[i].Section = "GENERAL";
		RawConfigData[i].Key = "BACKGROUND_COLOR_RGB";
		Configuration.BackgroundColorRGB.assign(3, 0);
		RawConfigData[i].Value.push_back(std::to_string(Configuration.BackgroundColorRGB[0]));
		RawConfigData[i].Value.push_back(std::to_string(Configuration.BackgroundColorRGB[1]));
		RawConfigData[i].Value.push_back(std::to_string(Configuration.BackgroundColorRGB[2]));
		i++;

		RawConfigData.push_back(ConfigDataStruct());
		RawConfigData[i].Section = "BALL";
		RawConfigData[i].Key = "COUNT";
		RawConfigData[i].Value.push_back(std::to_string((Configuration.BallCount = 15)));
		i++;

		RawConfigData.push_back(ConfigDataStruct());
		RawConfigData[i].Section = "BALL";
		RawConfigData[i].Key = "RADIUS";
		RawConfigData[i].Value.push_back(std::to_string((Configuration.BallRadius = 15.0f)));
		i++;

		RawConfigData.push_back(ConfigDataStruct());
		RawConfigData[i].Section = "BALL";
		RawConfigData[i].Key = "VELOCITY";
		RawConfigData[i].Value.push_back(std::to_string((Configuration.AbsoluteVelocity = 0.1f)));
		i++;

		RawConfigData.push_back(ConfigDataStruct());
		RawConfigData[i].Section = "BALL";
		RawConfigData[i].Key = "COLOR_RGB";
		Configuration.BallColorRGB.assign(3, 255);
		RawConfigData[i].Value.push_back(std::to_string(Configuration.BallColorRGB[0]));
		RawConfigData[i].Value.push_back(std::to_string(Configuration.BallColorRGB[1]));
		RawConfigData[i].Value.push_back(std::to_string(Configuration.BallColorRGB[2]));

		if (writeFile())
		{
			SuccessfulWriting = true;
		}
		else
		{
			SuccessfulWriting = false;
		}
	}

	//for checking if the configuration parameters are in certain boundaries (only if reading AND parsing was successful):
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
	//flags:
	bool SuccessfulReading;
	bool SuccessfulWriting;
	bool ConfigManipulated;

	ConfigFileHandler() :SuccessfulReading(false), SuccessfulWriting(false), ConfigManipulated(false) {}

	//for assigning the raw RawConfigData to the program configuration:
	void getConfig() {
		if (readFile())
		{
			constexpr int DesiredParameterCount = 9;
			int ParameterCount = 0;

			for (size_t i = 0; i < RawConfigData.size(); i++)
			{
				if (RawConfigData[i].Section == "GENERAL")
				{
					if (RawConfigData[i].Key == "BACKGROUND_COLOR_RGB")
					{
						for (size_t j = 0; j < RawConfigData[i].Value.size(); j++)
						{
							Configuration.BackgroundColorRGB.push_back(stoi(RawConfigData[i].Value[j]));
							ParameterCount++;
						}
					}
				}
				else if (RawConfigData[i].Section == "BALL")
				{
					if (RawConfigData[i].Key == "COUNT")
					{
						Configuration.BallCount = stoi(RawConfigData[i].Value[0]);
						ParameterCount++;
					}
					else if (RawConfigData[i].Key == "RADIUS")
					{
						Configuration.BallRadius = stof(RawConfigData[i].Value[0]);
						ParameterCount++;
					}
					else if (RawConfigData[i].Key == "VELOCITY")
					{
						Configuration.AbsoluteVelocity = stof(RawConfigData[i].Value[0]);
						ParameterCount++;
					}
					else if (RawConfigData[i].Key == "COLOR_RGB")
					{
						for (size_t j = 0; j < RawConfigData[i].Value.size(); j++)
						{
							Configuration.BallColorRGB.push_back(stoi(RawConfigData[i].Value[j]));
							ParameterCount++;
						}
					}
				}
			}
			if (DesiredParameterCount == ParameterCount)
			{
				SuccessfulReading = true;
				checkConfig(); //check if the configuration parameters are in certain boundaries
			}
			else
			{
				SuccessfulReading = false;
				setConfig(); //a new config.ini file gets created, if a problem has occurred while reading the file or parsing the data
			}
		}
		else
		{
			SuccessfulReading = false;
			setConfig();
		}
	}
};