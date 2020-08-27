#pragma once
#include "Component.h"

#define COLOUR_ADJUSTMENT_VALUE 0.01f

class ColourComponent : public Component
{
public:
	ColourComponent() : m_colourValue(0) { }

	float m_colourValue;
	void OnUpdate(float dt) override { }

	void OnMessage(const std::string m) override
	{
		if (m == "increase")
		{
			m_colourValue += COLOUR_ADJUSTMENT_VALUE;
		}
		
		if (m == "decrease")
		{
			m_colourValue -= COLOUR_ADJUSTMENT_VALUE;
		}

		if (m_colourValue < 0)
		{
			m_colourValue = 0;
			//std::cout << "Value cannot go below 0!\n";
		}
		else if (m_colourValue > 1)
		{
			m_colourValue = 1;
			//std::cout << "Value cannot go over 1!\n";
		}
		else
		{
			std::cout << "New colour value: " << m_colourValue << "\n";
		}
	}
};

class RedComponent : public ColourComponent
{
public:
	RedComponent();
	RedComponent(float val) { m_colourValue = val; }
};

class GreenComponent : public ColourComponent
{
public:
	GreenComponent();
	GreenComponent(float val) { m_colourValue = val; }
};

class BlueComponent : public ColourComponent
{
public:
	BlueComponent();
	BlueComponent(float val) { m_colourValue = val; }
};