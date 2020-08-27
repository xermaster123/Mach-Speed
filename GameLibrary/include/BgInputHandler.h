#pragma once
#include "InputHandler.h"
#include "ColourComponent.h"

class IncreaseRedCommand : public InputCommand
{
public:
	void execute(GameObject& gameObject) override { if (gameObject.getComponent<RedComponent>() != nullptr) gameObject.getComponent<RedComponent>()->OnMessage("increase"); }
};

class DecreaseRedCommand : public InputCommand
{
public:
	void execute(GameObject& gameObject) override { if (gameObject.getComponent<RedComponent>() != nullptr) gameObject.getComponent<RedComponent>()->OnMessage("decrease"); }
};

class IncreaseGreenCommand : public InputCommand
{
public:
	void execute(GameObject& gameObject) override { if (gameObject.getComponent<GreenComponent>() != nullptr) gameObject.getComponent<GreenComponent>()->OnMessage("increase"); }
};

class DecreaseGreenCommand : public InputCommand
{
public:
	void execute(GameObject& gameObject) override { if (gameObject.getComponent<GreenComponent>() != nullptr) gameObject.getComponent<GreenComponent>()->OnMessage("decrease"); }
};

class IncreaseBlueCommand : public InputCommand
{
public:
	void execute(GameObject& gameObject) override { if (gameObject.getComponent<BlueComponent>() != nullptr) gameObject.getComponent<BlueComponent>()->OnMessage("increase"); }
};

class DecreaseBlueCommand : public InputCommand
{
public:
	void execute(GameObject& gameObject) override { if (gameObject.getComponent<BlueComponent>() != nullptr) gameObject.getComponent<BlueComponent>()->OnMessage("decrease"); }
};

struct BgInputHandler : public InputHandler
{
	BgInputHandler(GameObject* gameObject) : InputHandler(m_gameObject = gameObject)
	{
		loadCommands("config/key_mapping.json");

		/*Camera*/
		m_controlMapping[getKey("IncreaseRedCommand")] = std::make_shared<IncreaseRedCommand>(); // R
		m_controlMapping[getKey("DecreaseRedCommand")] = std::make_shared<DecreaseRedCommand>(); // T

		m_controlMapping[getKey("IncreaseGreenCommand")] = std::make_shared<IncreaseGreenCommand>(); // F
		m_controlMapping[getKey("DecreaseGreenCommand")] = std::make_shared<DecreaseGreenCommand>(); // G

		m_controlMapping[getKey("IncreaseBlueCommand")] = std::make_shared<IncreaseBlueCommand>(); // V
		m_controlMapping[getKey("DecreaseBlueCommand")] = std::make_shared<DecreaseBlueCommand>(); // B
	}
};
