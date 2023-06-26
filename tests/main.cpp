#include "Config.h"
#include <iostream>
#include <iomanip>
#include <vector>

struct Tmp {
	~Tmp() { std::cout << std::endl; }
};

#define ERROR (Tmp(), std::cerr << __FILE__ << ": " << __LINE__ << ": ")

template<typename T>
struct Helper
{
	T valueToSave;
	T valueToLoad;
	std::string name;
};

Helper<int>          c1 = {   -1234567890,    0, "testSignedInt"   };
Helper<unsigned int> c2 = {          256u,   0u, "testUnsignedInt" };
Helper<float>        c3 = {         60.1f, 0.0f, "testFloat"       };
Helper<std::string>  c4 = { "Hello_World",   "", "testString"      };

void compareValues(Config* config)
{
	if (!config->getValue(c1.name, c1.valueToLoad))
	{
		ERROR << "ERROR: Config::getValue " << c1.name << " failed!\n";
	}
	else
	{
		if (c1.valueToLoad != c1.valueToSave)
		{
			ERROR << "ERROR: Config::getValue mismatch " << c1.name << ": " << c1.valueToLoad << ", " << c1.valueToSave << "!\n";
		}
	}

	if (!config->getValue(c2.name, c2.valueToLoad))
	{
		ERROR << "ERROR: Config::getValue " << c2.name << " failed!\n";
	}
	else
	{
		if (c2.valueToLoad != c2.valueToSave)
		{
			ERROR << "ERROR: Config::getValue mismatch " << c2.name << ": " << c2.valueToLoad << ", " << c2.valueToSave << "!\n";
		}
	}

	if (!config->getValue(c3.name, c3.valueToLoad))
	{
		ERROR << "ERROR: Config::getValue " << c3.name << " failed!\n";
	}
	else
	{
		if (c3.valueToLoad != c3.valueToSave)
		{
			ERROR << "ERROR: Config::getValue mismatch " << c3.name << std::fixed << std::setprecision(3) << ": " << c3.valueToLoad << ", " << c3.valueToSave << "!\n";
		}
	}

	if (!config->getValue(c4.name, c4.valueToLoad))
	{
		ERROR << "ERROR: Config::getValue " << c4.name << " failed!\n";
	}
	else
	{
		if (c4.valueToLoad != c4.valueToSave)
		{
			ERROR << "ERROR: Config::getValue mismatch " << c4.name << ": " << c4.valueToLoad << ", " << c4.valueToSave << "!\n";
		}
	}
}

void resetReadValues()
{
	c1.valueToLoad = 0;
	c2.valueToLoad = 0u;
	c3.valueToLoad = 0.0f;
	c4.valueToLoad = "";
}

void testWriteSave()
{
	const std::string CONFIG_FILE = "configToWrite.cfg";
	Config writeConfig(CONFIG_FILE);

	// Add default values
	writeConfig.setValue<int>(c1.name, c1.valueToSave);
	writeConfig.setValue<unsigned int>(c2.name, c2.valueToSave);
	writeConfig.setValue<float>(c3.name, c3.valueToSave);
	writeConfig.setValue<std::string>(c4.name, c4.valueToSave);

	if (!writeConfig.saveConfig() || !writeConfig.saveConfig())
	{
		ERROR << "ERROR: Saving config " << CONFIG_FILE << " failed!";
		resetReadValues();
		return;
	}

	if (!writeConfig.readFromFile())
	{
		ERROR << "ERROR: Reading config " << CONFIG_FILE << " failed!";
		resetReadValues();
		return;
	}

	compareValues(&writeConfig);
	resetReadValues();
}

void testReadLoad()
{
	const std::string CONFIG_FILE = "configToRead.cfg";
	Config readConfig(CONFIG_FILE);

	readConfig.setValue<int>(c1.name, c1.valueToLoad);
	readConfig.setValue<unsigned int>(c2.name, c2.valueToLoad);
	readConfig.setValue<float>(c3.name, c3.valueToLoad);
	readConfig.setValue<std::string>(c4.name, c4.valueToLoad);

	if (!readConfig.readFromFile())
	{
		ERROR << "ERROR: Reading config " << CONFIG_FILE << " failed!";
	}
	else
	{
		compareValues(&readConfig);
	}
}


int main()
{
	testWriteSave();

	testReadLoad();

	return 0;
}
