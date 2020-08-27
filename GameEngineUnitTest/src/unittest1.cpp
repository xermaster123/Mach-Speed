#include "stdafx.h"
#include "CppUnitTest.h"
#include "TestGame.h"
#include "TestGameObject.h"
#include "TestInputHandler.h"
#include "GLFW_EngineCore.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GameEngineUnitTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TransformComponentTest)
		{
			// ARRANGE
			TestGame game;
			TestGameObject* object = new TestGameObject();
			object->addComponent(new TransformComponent());
			bool success = false;

			// ACTION
			object->getComponent<TransformComponent>()->translate(1, 0, 0);
			if (object->getComponent<TransformComponent>()->position().x == 1)
			{
				success = true;
			}
			// ASSERT
			Assert::IsTrue(success);
			//// SECOND ACTION
			object->getComponent<TransformComponent>()->translate(0, 1, 0);
			if (object->getComponent<TransformComponent>()->position().y == 1)
			{
				success = true;
			}
			// SECOND ASSERT
			Assert::IsTrue(success);
		}

		TEST_METHOD(CameraComponentTest)
		{
			// ARRANGE
			TestGame game;
			TestGameObject* object = new TestGameObject();
			object->addComponent(new CameraComponent());
			bool success = false;
			// ACTION
			object->getComponent<CameraComponent>()->translate(1, 0, 0);
			if (object->getComponent<CameraComponent>()->position().x == 1)
			{
				success = true;
			}
			// ASSERT
			Assert::IsTrue(success);
			//// SECOND ACTION
			object->getComponent<CameraComponent>()->translate(0, 1, 0);
			if (object->getComponent<CameraComponent>()->position().y == 1)
			{
				success = true;
			}
			// SECOND ASSERT
			Assert::IsTrue(success);
		}

		TEST_METHOD(GameObjectMessageTest)
		{
			// ARRANGE
			TestGame game;
			TestGameObject* object = new TestGameObject();

			// ACTION
			object->OnMessage("test");
			bool success = object->m_success;
			// ASSERT
			Assert::IsFalse(success);
			// SECOND ACTION
			object->OnMessage("correct");
			success = object->m_success;
			// SECOND ASSERT
			Assert::IsTrue(success);
		}

		TEST_METHOD(InputHandlerTest)
		{
			// ARRANGE
			TestGame game;
			TestGameObject* object = new TestGameObject();
			game.m_currentScene->v_gameObjects.push_back(object);
			game.m_inputHandler.push_back(new TestInputHandler(game.m_currentScene->v_gameObjects.at(0)));

			// make space for the keybuffer
			static std::vector<bool> m_keyBuffer;

			m_keyBuffer.resize(400);
			std::fill(m_keyBuffer.begin(), m_keyBuffer.end(), false);

			// ACTION
			m_keyBuffer[50] = true;
			for (InputHandler* element : game.m_inputHandler)
			{
				element->handleInputs(m_keyBuffer);
			}
			bool success = object->m_success;
			m_keyBuffer[50] = false;
			
			// ASSERT
			Assert::IsFalse(success);
			
			// SECOND ACTION
			m_keyBuffer[49] = true;
			for (InputHandler* element : game.m_inputHandler)
			{
				element->handleInputs(m_keyBuffer);
			}
			success = object->m_success;
			m_keyBuffer[49] = true;

			// SECOND ASSERT
			Assert::IsTrue(success);
		}

		TEST_METHOD(KeyMappingLoadTest)
		{
			// ARRANGE
			TestGame game;
			TestGameObject* object = new TestGameObject();
			game.m_currentScene->v_gameObjects.push_back(object);
			game.m_inputHandler.push_back(new TestInputHandler(game.m_currentScene->v_gameObjects.at(0)));

			bool success = false;

			// ACTION
			success = game.m_inputHandler[0]->loadCommands("incorrectpath");

			// ASSERT
			Assert::IsFalse(success);

			// SECOND ACTION
			success = game.m_inputHandler[0]->loadCommands("config/key_mapping.json");

			// SECOND ASSERT
			Assert::IsTrue(success);
		}

	};
}