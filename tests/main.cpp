#include "ZConfig.h"
#include <iostream>
#include <iomanip>
#include <vector>

struct Tmp
{
    ~Tmp() { std::cout << std::endl; }
};

#define ERROR (Tmp(), std::cerr << __FILE__ << ": " << __LINE__ << ": ERROR: ")

template<typename T>
struct Helper
{
    T valueToSave;
    T valueToLoad;
    std::string name;
    std::string section;
};

Helper<int>          c1 = {   -1234567890,    0, "testSignedInt",   "section1"  };
Helper<unsigned int> c2 = {          256u,   0u, "testUnsignedInt", "section2"  };
Helper<float>        c3 = {         60.1f, 0.0f, "testFloat",       "section1"  };
Helper<std::string>  c4 = { "Hello World",   "", "testString",      "section2"  };
Helper<std::string>  c5 = { "Some value",    "", "testString",      "section1"  };

void compareValues(ZConfig* config)
{
    if (!config->getValue(c1.section, c1.name, c1.valueToLoad))
    {
        ERROR << "c1 not found";
    }
    else
    {
        if (c1.valueToLoad != c1.valueToSave)
        {
            ERROR << "value mismatch: " << c1.valueToLoad << " != " << c1.valueToSave;
        }
    }

    if (!config->getValue(c2.section, c2.name, c2.valueToLoad))
    {
        ERROR << "c2 not found";
    }
    else
    {
        if (c2.valueToLoad != c2.valueToSave)
        {
            ERROR << "value mismatch: " << c2.valueToLoad << " != " << c2.valueToSave;
        }
    }

    if (!config->getValue(c3.section, c3.name, c3.valueToLoad))
    {
        ERROR << "c3 not found";
    }
    else
    {
        if (c3.valueToLoad != c3.valueToSave)
        {
            ERROR << "value mismatch: "  << std::fixed << std::setprecision(3) << c3.valueToLoad << " != " << c3.valueToSave;
        }
    }

    if (!config->getValue(c4.section, c4.name, c4.valueToLoad))
    {
        ERROR << "c4 not found";
    }
    else
    {
        if (c4.valueToLoad != c4.valueToSave)
        {
            ERROR << "value mismatch: " << c4.valueToLoad << " != " << c4.valueToSave;
        }
    }

    if (!config->getValue(c5.section, c5.name, c5.valueToLoad))
    {
        ERROR << "c5 not found";
    }
    else
    {
        if (c5.valueToLoad != c5.valueToSave)
        {
            ERROR << "value mismatch: " << c5.valueToLoad << " != " << c5.valueToSave;
        }
    }
}

void resetReadValues()
{
    c1.valueToLoad = 0;
    c2.valueToLoad = 0u;
    c3.valueToLoad = 0.0f;
    c4.valueToLoad = "";
    c5.valueToLoad = "";
}

void testFileCreation()
{
    const std::string CONFIG_FILE = "tests/configToWrite.cfg";
    ZConfig writeConfig(CONFIG_FILE);

    // Add default values
    writeConfig.setValue(c1.section, c1.name, c1.valueToSave);
    writeConfig.setValue(c2.section, c2.name, c2.valueToSave);
    writeConfig.setValue(c3.section, c3.name, c3.valueToSave);
    writeConfig.setValue(c4.section, c4.name, c4.valueToSave);
    writeConfig.setValue(c5.section, c5.name, c5.valueToSave);

    // Check defaults
    compareValues(&writeConfig);
    resetReadValues();

    // Save twice
    if (!writeConfig.saveConfig() || !writeConfig.saveConfig())
    {
        ERROR << "ERROR: Saving config " << CONFIG_FILE << " failed!";
        return;
    }


    // Read previously saved file
    if (!writeConfig.readFromFile())
    {
        ERROR << "ERROR: Reading config " << CONFIG_FILE << " failed!";
        return;
    }

    compareValues(&writeConfig);
    resetReadValues();
}

void testReadEditedFile()
{
    const std::string CONFIG_FILE = "tests/configToRead.cfg";
    ZConfig readConfig(CONFIG_FILE);

    if (!readConfig.readFromFile())
    {
        ERROR << "ERROR: Reading config " << CONFIG_FILE << " failed!";
        return;
    }

    compareValues(&readConfig);
    resetReadValues();
}

int main()
{
    testFileCreation();
    testReadEditedFile();

    std::cout << "Tests finished\n";

    return 0;
}
