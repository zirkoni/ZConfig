#include "ZConfig.h"
#include <fstream>

ZConfig::ZConfig(const std::string& filename): m_filename(filename)
{
    m_lastSection = m_config.end();
}

ZConfig::~ZConfig()
{
}

bool ZConfig::readFromFile()
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

bool ZConfig::saveConfig()
{
    std::ofstream file(m_filename.c_str());

    if (file)
    {
        for(const auto& [section, entries] : m_config)
        {
            file << SECTION_START << section << SECTION_END << "\n";

            for(const auto& [key, value] : entries)
            {
                file << key << " = " << value << "\n";
            }
        }

        file.close();
        return true;
    }

    return false;
}

void ZConfig::parseLine(std::string & line)
{
    // Remove leading/trailing whitespace
    trim(line, WHITESPACE);

    if(isComment(line))
    {
        return;
    } else if(isSection(line))
    {
        // Remove section markers
        ltrim(line, std::string(1, SECTION_START).c_str());
        rtrim(line, std::string(1, SECTION_END).c_str());

        auto iter = m_config.find(line);

        // Init empty section
        if(iter == m_config.end())
        {
            std::map<std::string, std::string> emptyEntry;
            m_lastSection = m_config.insert(m_config.begin(), std::make_pair(line, emptyEntry));
        }

    } else if(m_lastSection != m_config.end())
    {

        std::size_t equalSignPosition = line.find("=");

        if(equalSignPosition != std::string::npos)
        {
            std::string key = line.substr(0, equalSignPosition);
            std::string value = line.erase(0, equalSignPosition + 1);
            trim(key, WHITESPACE);
            trim(value, WHITESPACE);
            m_lastSection->second[key] = value;
        }   
    }
}

std::string& ZConfig::ltrim(std::string& s, const char* t)
{
    s.erase(0, s.find_first_not_of(t));
    return s;
}

std::string& ZConfig::rtrim(std::string& s, const char* t)
{
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}

std::string& ZConfig::trim(std::string& s, const char* t)
{
    return ltrim(rtrim(s, t), t);
}

bool ZConfig::isSection(const std::string& section)
{
    if(section.length() > 1)
    {
        return (*section.begin() == SECTION_START && *section.rbegin() == SECTION_END);
    }

    return false;
}

bool ZConfig::isComment(const std::string& line)
{
    return (!line.empty() && line.at(0) == COMMENT_MARKER);
}
