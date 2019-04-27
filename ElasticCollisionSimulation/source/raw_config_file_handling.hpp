#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using std::endl;
using std::cout;
using std::cin;
using std::string;
using std::ifstream;
using std::vector;


struct ConfigDataStruct {
	string Section;
	string Key;
	vector<string> Value;
};


class RawConfigFileHandler {
private:
	string FilePath;

protected:
	vector<ConfigDataStruct> ConfigData;

	bool readFile() {
		bool SuccessfulReadingRaw = false;
		ifstream myfile(FilePath);

		if (myfile.is_open())
		{
			constexpr char Char1 = '=';
			constexpr char Char2 = ',';
			constexpr char Char3 = '[';
			string Input;
			string Section;
			int i = 0;

			while (getline(myfile, Input))
			{
				int Char1Pos = -1;
				int Char2Pos = -1;

				string Key;
				string RawValue;
				vector<string> Value;

				if ((Char1Pos = Input.find(Char1)) > 0)
				{
					Key = RawValue = Input;
					Key.erase(Char1Pos, Key.size());
					RawValue.erase(0, (Char1Pos + 1));
					while ((Char2Pos = RawValue.find(Char2)) > 0)
					{
						Value.push_back(RawValue.substr(0, Char2Pos));
						RawValue.erase(0, Char2Pos + 1);
					}
					if ((Char2Pos < 0) || (Char2Pos == string::npos))
					{
						Value.push_back(RawValue);
					}
					ConfigData.push_back(ConfigDataStruct());
					ConfigData[i].Section = Section;
					ConfigData[i].Key = Key;
					ConfigData[i].Value = Value;

					i++;
				}
				else if (Input.find(Char3) != string::npos)
				{
					Section.clear();
					Section = Input;
					Section.pop_back();
					Section.erase(0, 1);
				}
			}
			myfile.close();
			return SuccessfulReadingRaw = true;
		}
		else
		{
			return SuccessfulReadingRaw = false;
		}
	}

	bool writeFile() {
		bool SuccessfulWritingRaw = false;
		//###########################
		return SuccessfulWritingRaw = false;
	}

public:
	RawConfigFileHandler() : FilePath("config.ini") {}

	void setFilePath(string pFilePath) {
		FilePath = pFilePath;
	}

	string getFilePath() {
		return FilePath;
	}

	void printConfigData() {
		for (size_t i = 0; i < ConfigData.size(); i++)
		{
			cout << "[" << ConfigData[i].Section << "]" << endl;
			cout << ConfigData[i].Key << "=";
			for (size_t j = 0; j < ConfigData[i].Value.size(); j++)
			{
				if (j > 0)
				{
					cout << ",";
				}
				cout << ConfigData[i].Value[j];
			}
			cout << endl << endl;
		}
			
	}
};
