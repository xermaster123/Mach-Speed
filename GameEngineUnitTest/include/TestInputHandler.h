#pragma once
#include "InputHandler.h"

class TestMessageCorrect : public InputCommand
{
public:
	void execute(GameObject& gameObject) override { gameObject.OnMessage("correct"); }
};

struct TestInputHandler : public InputHandler
{
	TestInputHandler(GameObject* gameObject) : InputHandler(m_gameObject = gameObject)
	{
		/*Camera*/
		m_controlMapping[49] = new TestMessageCorrect; // 1
	}
};
