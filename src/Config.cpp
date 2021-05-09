#include "..\include\Config.h"
#include <fstream>
#include <algorithm>
#include <cctype>

Config::Config(const std::string& filename): m_filename(filename)
{
}

Config::~Config()
{
}

bool Config::readFromFile()
{
	std::ifstream file(m_filename.c_str());
	std::string line;

	if (file)
	{
		while (std::getline(file, line))
		{
			parseLine(line);
		}

		file.close();
		return true;
	}

	return false;
}

bool Config::saveConfig()
{
	std::ofstream file(m_filename.c_str());

	if (file)
	{
		auto it = m_entries.begin();

		while (it != m_entries.end())
		{
			std::string key(it->first);
			std::string value(it->second);

			key.erase(std::remove_if(key.begin(), key.end(), std::isspace), key.end());
			value.erase(std::remove_if(value.begin(), value.end(), std::isspace), value.end());

			file << key << " = " << value << "\n";

			++it;
		}

		file.close();
		return true;
	}

	return false;
}

void Config::parseLine(std::string & line)
{
	auto current = line.begin();
	auto end = std::remove_if(line.begin(), line.end(), std::isspace);

	std::string key = "";
	std::string value = "";
	bool foundEqual = false;

	while (current != end)
	{
		if (*current == '#')
		{
			break;
		}

		if (!foundEqual && *current != '=')
		{
			key += *current;
		}
		else if (*current == '=')
		{
			foundEqual = true;
		}
		else
		{
			value += *current;
		}

		++current;
	}

	if (isValidKey(key) && value.length() > 0)
	{
		m_entries[key] = value;
	}
}

bool Config::isValidKey(const std::string key)
{
	return (key.length() > 0 && m_entries.find(key) != m_entries.end());
}
