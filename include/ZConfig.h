#pragma once
#include <string>
#include <map>
#include <sstream>


class ZConfig
{
public:

	// All settings are read from and written in the same file.
	ZConfig(const std::string& filename);
	~ZConfig();

	// Read all settings from file. Returns false if reading fails (e.g. file does not exist).
	// Checks for valid entries: each entry must have a default value set with setValue before reading.
	// All whitespace is ignored (e.g. "My Key" => "MyKey").
	bool readFromFile();

	// Save all settings in file (old file is overwritten). Returns false if writing to file fails.
	// Whitespace in entries is ignored (e.g. "My Value" => "MyValue").
	bool saveConfig();

	// Set config entry: name = value.
	template<typename T>
	void setValue(const std::string& name, const T& value)
	{
		std::ostringstream os;
		os << value;
		m_entries[name] = os.str();
	}

	// Get config entry: name = value. Returns false if entry is not found.
	template<typename T>
	bool getValue(const std::string& name, T& value)
	{
		if (m_entries.find(name) != m_entries.end())
		{
			std::stringstream is(m_entries[name]);
			is >> value;
			return true;
		}
		return false;
	}

private:
	void parseLine(std::string& line);
	bool isValidKey(const std::string key);

private:
	std::string m_filename;
	std::map<std::string, std::string> m_entries;
};
