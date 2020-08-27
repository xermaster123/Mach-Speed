#pragma once
#include "InputHandler.h"

class RotateCameraLeftCommand : public InputCommand
{
public:
	void execute(GameObject& gameObject) override { gameObject.OnMessage("rotateCameraLeft"); }
};

class RotateCameraRightCommand : public InputCommand
{
public:
	void execute(GameObject& gameObject) override { gameObject.OnMessage("rotateCameraRight"); }
};

class RotateCameraUpCommand : public InputCommand
{
public:
	void execute(GameObject& gameObject) override { gameObject.OnMessage("rotateCameraUp"); }
};

class RotateCameraDownCommand : public InputCommand
{
public:
	void execute(GameObject& gameObject) override { gameObject.OnMessage("rotateCameraDown"); }
};

class MovePlayerLeftCommand : public InputCommand
{
public:
	void execute(GameObject& gameObject) override { gameObject.OnMessage("movePlayerLeft"); }
};

class MovePlayerRightCommand : public InputCommand
{
public:
	void execute(GameObject& gameObject) override { gameObject.OnMessage("movePlayerRight"); }
};

class MovePlayerForwardCommand : public InputCommand
{
public:
	void execute(GameObject& gameObject) override { gameObject.OnMessage("movePlayerForward"); }
};

class MovePlayerBackwardCommand : public InputCommand
{
public:
	void execute(GameObject& gameObject) override { gameObject.OnMessage("movePlayerBackward"); }
};

class MovePlayerUpCommand : public InputCommand
{
public:
	void execute(GameObject& gameObject) override { gameObject.OnMessage("movePlayerUp"); }
};

class MovePlayerDownCommand : public InputCommand
{
public:
	void execute(GameObject& gameObject) override { gameObject.OnMessage("movePlayerDown"); }
};

struct EditorInputHandler : public InputHandler
{
	EditorInputHandler(GameObject* gameObject) : InputHandler(m_gameObject = gameObject)
	{
		loadCommands("config/key_mapping.json");

		/*Camera*/
		m_controlMapping[getKey("RotateCameraLeftCommand")] = std::make_shared<RotateCameraLeftCommand>(); // Q
		m_controlMapping[getKey("RotateCameraRightCommand")] = std::make_shared<RotateCameraRightCommand>(); // E
		m_controlMapping[getKey("RotateCameraUpCommand")] = std::make_shared<RotateCameraUpCommand>(); // R
		m_controlMapping[getKey("RotateCameraDownCommand")] = std::make_shared<RotateCameraDownCommand>(); // F

		m_controlMapping[getKey("MovePlayerForwardCommand")] = std::make_shared<MovePlayerForwardCommand>(); // W
		m_controlMapping[getKey("MovePlayerLeftCommand")] = std::make_shared<MovePlayerLeftCommand>(); // A
		m_controlMapping[getKey("MovePlayerBackwardCommand")] = std::make_shared<MovePlayerBackwardCommand>(); // S
		m_controlMapping[getKey("MovePlayerRightCommand")] = std::make_shared<MovePlayerRightCommand>(); // D

		m_controlMapping[getKey("MovePlayerUpCommand")] = std::make_shared<MovePlayerUpCommand>(); // X
		m_controlMapping[getKey("MovePlayerDownCommand")] = std::make_shared<MovePlayerDownCommand>(); // C
	}
};
