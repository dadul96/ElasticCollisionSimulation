#pragma once
#include <fstream>
#include <string>

class ConfigFileHandler {
	private:
	std::string FilePath;
	public:
		auto readFile();
		auto writeFile();
};