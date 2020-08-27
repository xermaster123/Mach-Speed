#pragma once
#include <map>
#include "GameObject.h"
#include <vector>
#include "TransformComponent.h"
#include <fstream>
#include "json/json.h"
#include <memory>

class InputCommand
{
public:
	virtual ~InputCommand() {}
	virtual void execute(GameObject& gameObject) = 0;
};

struct InputHandler
{
	GameObject* m_gameObject;
	std::map<int, std::shared_ptr<InputCommand>> m_controlMapping;
	std::map<std::string, int> m_keyMap;

	InputHandler(GameObject* gameObject) : m_gameObject(gameObject) { }

	void handleInputs(const std::vector<bool>& keyBuffer)
	{
		for (const auto& mapEntry : m_controlMapping)
		{
			/*for (size_t i = 0; i < keyBuffer.size(); i++)
			{
			if (keyBuffer[i])
			{
			std::cout << "Key Press: " << i << "\n";
			}
			}*/

			if (keyBuffer[mapEntry.first])
			{
				mapEntry.second->execute(*m_gameObject);
			}
		}
	}

	bool loadCommands(std::string filePath)
	{
		std::cout << "Loading key mapping data..." << std::endl;

		std::fstream jsonFile;
		Json::Value root;
		Json::Reader reader;
		size_t size = 0;

		jsonFile.open(filePath.c_str());

		// Check if the file is valid
		if (!reader.parse(jsonFile, root))
		{
			std::cout << "Failed to parse data from: " << filePath << ". " << reader.getFormattedErrorMessages() << "\n";
			return false;
		}

		// Read root array
		const Json::Value keyCommands = root["Commands"];

		for (auto keyCommand : keyCommands)
		{
			std::string key_name;
			int key_ascii;

			const std::string command = keyCommand["command"].asString();
			if (command != "")
			{
				key_name = command;
			}

			const Json::Int key = keyCommand["key"].asInt();
			if (key != Json::nullValue)
			{
				key_ascii = key;
			}

			m_keyMap[key_name] = key_ascii;
		}

		return true;
	}

	// Check to see if the command exists and return the key for it
	int getKey(std::string command)
	{
		int key;

		try
		{
			key = m_keyMap.at(command);
		}
		catch (const std::exception&)
		{
			return 0;
		}

		std::cout << key << " : " << command << " key set" << std::endl;

		return key;
	}
};
