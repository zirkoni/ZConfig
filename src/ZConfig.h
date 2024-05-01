#pragma once
#include <string>
#include <map>
#include <sstream>


class ZConfig
{
    static constexpr char SECTION_START = '[';
    static constexpr char SECTION_END   = ']';

    static constexpr char COMMENT_MARKER = '#';

    const char* WHITESPACE = " \t\r";

public:

    // All settings are read from and written in the same file.
    ZConfig(const std::string& filename);
    ~ZConfig();

    // Read all settings from file. Returns false if file does not exist.
    bool readFromFile();

    // Save all settings in file (old file is overwritten). Returns false if writing to file fails.
    bool saveConfig();

    // Set config entry.
    template<typename T>
    void setValue(const std::string& section, const std::string& name, const T& value)
    {
        std::ostringstream os;
        os << value;
        m_config[section][name] = os.str();
    }

    // Get config entry. Returns false if entry is not found.
    template<typename T>
    bool getValue(const std::string& section, const std::string& name, T& value)
    {
        for(const auto& [sectionName, entries] : m_config)
        {
            if(sectionName == section)
            {
                for(const auto& [key, val] : entries)
                {
                    if(key == name)
                    {
                        if constexpr(std::is_same<T, std::string>::value)
                        {
                            value = val;
                        } else
                        {
                            std::stringstream is(val);
                            is >> value;
                        }

                        return true;
                    }
                }
            }
        }

        return false;
    }

private:
    void parseLine(std::string& line);
    bool isValidKey(const std::string& key);
    bool isSection(const std::string& section);
    bool isComment(const std::string& line);

    std::string& ltrim(std::string& s, const char* t);
    std::string& rtrim(std::string& s, const char* t);
    std::string& trim(std::string& s, const char* t);

    // Member variables
    std::string m_filename;
    std::map<std::string, std::map<std::string, std::string>> m_config;
    std::map<std::string, std::map<std::string, std::string>>::iterator m_lastSection;
};
