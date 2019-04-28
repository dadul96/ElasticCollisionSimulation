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
using std::ofstream;
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
	vector<ConfigDataStruct> RawConfigData;

	bool readFile() {
		bool SuccessfulReadingRaw = false;
		ifstream inFile(FilePath);

		if (inFile.is_open())
		{
			constexpr char Char1 = '=';
			constexpr char Char2 = ',';
			constexpr char Char3 = '[';
			string Input;
			string Section;
			int i = 0;

			while (getline(inFile, Input))
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
					RawConfigData.push_back(ConfigDataStruct());
					RawConfigData[i].Section = Section;
					RawConfigData[i].Key = Key;
					RawConfigData[i].Value = Value;

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
			inFile.close();
			return (SuccessfulReadingRaw = true);
		}
		else
		{
			return (SuccessfulReadingRaw = false);
		}
	}

	bool writeFile() {
		bool SuccessfulWritingRaw = false;
		ofstream outFile(FilePath);
		if (outFile.is_open())
		{
			for (size_t i = 0; i < RawConfigData.size(); i++)
			{
				if (i == 0)
				{
					outFile << "[" << RawConfigData[i].Section << "]" << "\n";
				}
				else if (RawConfigData[i].Section != RawConfigData[(i - 1)].Section)
				{
					outFile << "\n";
					outFile << "[" << RawConfigData[i].Section << "]" << "\n";
				}
				outFile << RawConfigData[i].Key << "=";
				for (size_t j = 0; j < RawConfigData[i].Value.size(); j++)
				{
					if (j == 0)
					{
						outFile << RawConfigData[i].Value[j];
					}
					else
					{
						outFile << "," << RawConfigData[i].Value[j];
					}
				}
				outFile << "\n";
			}
			outFile.close();
			return (SuccessfulWritingRaw = true);
		}
		else
		{
			return (SuccessfulWritingRaw = false);
		}
	}

public:
	RawConfigFileHandler() : FilePath("config.ini") {}

	void setFilePath(string pFilePath) {
		FilePath = pFilePath;
	}

	string getFilePath() {
		return FilePath;
	}

	void printRawConfigData() {
		for (size_t i = 0; i < RawConfigData.size(); i++)
		{
			cout << "[" << RawConfigData[i].Section << "]" << endl;
			cout << RawConfigData[i].Key << "=";
			for (size_t j = 0; j < RawConfigData[i].Value.size(); j++)
			{
				if (j > 0)
				{
					cout << ",";
				}
				cout << RawConfigData[i].Value[j];
			}
			cout << endl << endl;
		}

	}
};
