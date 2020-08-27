#include "GameProject.h"
#include "MainScene.h"
#include "json/json.h"
#include "PlayerCharacterObject.h"
#include "StaticEnvironmentObject.h"
#include <fstream>
#include "ModelComponent.h"
#include "GameInputHandler.h"
#include "CoinObject.h"
#include "RotateEnvironmentObject.h"
#include "DebugDrawManager.h"
#include "BgInputHandler.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include "imgui/imgui_internal.h"
#include "ParticleEmitterObject.h"
#include "BounceEnvironmentObject.h"
#include "TurretObject.h"
#include "AmmoObject.h"
#include "TrapObject.h"

GameProject::GameProject()
{
	GameProject::setupScene();
	ImGuiIO& io = ImGui::GetIO();
	//ImFont* neonize = io.Fonts->AddFontFromFileTTF("assets/fonts/neonize.ttf", 12.0f, NULL, io.Fonts->GetGlyphRangesJapanese());

	particlePtr = &m_currentScene->renderParticles;
}

GameProject::~GameProject()
{
}

void GameProject::update(float dt)
{
	if (ImGui::IsKeyDown(77)) { isMenuOpen = true; } // M

	if (m_currentScene->m_levelCompleted)
	{
		setupScene();
	}
	
	if (!isMenuOpen && !OptionsOpen && !HelpOpen)
	{
		m_currentScene->update(dt);
	}
	
}

void GameProject::render()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoScrollbar;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoNav;
	window_flags |= ImGuiWindowFlags_AlwaysAutoResize;

	m_currentScene->render();
	ImGui_ImplGlfwGL3_NewFrame();

	if (isMenuOpen)
	{
		ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_Appearing);
		ImGui::SetNextWindowPos(ImVec2(m_windowWidth*0.5f - 250, m_windowHeight - 550), ImGuiCond_Always);
		ImGui::Begin("MAIN MENU", NULL, window_flags);
		if (ImGui::Button("Resume Game", ImVec2(490, 50))) { isMenuOpen = false;}
		if (ImGui::Button("Options", ImVec2(490, 50))) { OptionsOpen = true; isMenuOpen = false; }
		if (ImGui::Button("Help", ImVec2(490, 50))) { HelpOpen = true; isMenuOpen = false; }
		if (ImGui::Button("Exit Game", ImVec2(490, 50))) { exit(0); }
		ImGui::End();
	}
	if(OptionsOpen)
	{
		ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_Appearing);
		ImGui::SetNextWindowPos(ImVec2(m_windowWidth*0.5f - 250, m_windowHeight - 550), ImGuiCond_Always);
		ImGui::Begin("OPTIONS", NULL, window_flags);
		ImGui::SliderFloat("Brightness", BrightPtr, 0, 100);
		ImGui::SliderFloat("Master Volume", mVolumePtr, 0, 100);
		ImGui::SliderFloat("Music Volume", MusicVolumePtr, 0, 100);
		ImGui::Checkbox("Particles", particlePtr);
		if (ImGui::Button("Back", ImVec2(490, 50))) { OptionsOpen = false; isMenuOpen = true; }
		ImGui::End();
	}
	if(HelpOpen)
	{
		ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_Appearing);
		ImGui::SetNextWindowPos(ImVec2(m_windowWidth*0.5f - 250, m_windowHeight - 550), ImGuiCond_Always);
		ImGui::Begin("HELP!", NULL, window_flags);
		ImGui::Text("W: Move Forward\nA: Move Left\nS: Move Back\nD: Move Right\nQ: Rotate Left\nE: Rotate Right\n\n");
		ImGui::Text("Collect all of the coins to unlock\nthe gate to progress to the\nnext level.\n");
		if(ImGui::Button("Back", ImVec2(490, 50))) { HelpOpen = false; isMenuOpen = true;}
		ImGui::End();
	}

	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiCond_Appearing);
	ImGui::SetNextWindowPos(ImVec2(m_windowWidth - 110, m_windowHeight - 40), ImGuiCond_Always);
	ImGui::Begin("Score", NULL, window_flags);
	ImGui::Value("Score", m_currentScene->m_score);
	ImGui::End();

	ImGui::Render();
	ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
}

void GameProject::setupScene()
{
	if (m_currentLevel == 0)
	{
		m_currentScene = new MainScene();
		loadLevel("assets/levels/level1.json");
	}
	else
	{
		IEngineCore* engine = m_currentScene->m_engineInterfacePtr;

		auto nextLevelPath = m_currentScene->getNextLevelPath();
		if (nextLevelPath == "")
		{
			std::cout << "Failed to acces : " << nextLevelPath << std::endl;
			return;
		}

		std::cout << "Accessing : " << nextLevelPath << std::endl;

		removeLevel();
		m_currentScene = new MainScene();
		m_currentScene->m_engineInterfacePtr = engine;
		loadLevel(nextLevelPath);
	}

	m_currentLevel++;
}

bool GameProject::loadLevel(std::string levelFile)
{
	std::cout << "Loading level data..." << std::endl;

	std::fstream jsonFile;
	Json::Value root;
	Json::Reader reader;

	jsonFile.open(levelFile.c_str());

	if (!reader.parse(jsonFile, root))
	{
		std::cout << "Failed to parse data from: " << levelFile << ". " << reader.getFormattedErrorMessages() << "\n";
		return false;
	}

	const Json::Value gameObjects = root["GameObjects"];

	for (auto gameObject : gameObjects)
	{
		ModelProperties properties;
		const string type = gameObject["type"].asString();
		float x, y, z;

		if (!m_currentScene->m_modelManager->loadModel(gameObject["model"].asString()))
		{
			std::cout << "Failed to load " + gameObject["model"].asString() << std::endl;
			continue;
		}

		properties.m_filePath = gameObject["model"].asString();

		const Json::Value posNode = gameObject["position"];
		if (posNode.type() != Json::nullValue)
		{
			x = posNode[0].asFloat();
			y = posNode[1].asFloat();
			z = posNode[2].asFloat();

			properties.m_position = glm::vec3(x, y, z);
		}

		const Json::Value oriNode = gameObject["orientation"];
		if (oriNode.type() != Json::nullValue)
		{
			x = oriNode[0].asFloat();
			y = oriNode[1].asFloat();
			z = oriNode[2].asFloat();
			float w = oriNode[3].asFloat(); //Angle axis

			properties.m_orientation = glm::quat(w, x, y, z);

			if (w > 1 || w < -1)
			{
				std::cout << gameObject["model"].asString() << " : " << type << " has invalid orientation w[0]\n";
			}

			if (x > 1 || x < -1)
			{
				std::cout << gameObject["model"].asString() << " : " << type << " has invalid orientation x[1]\n";
			}

			if (y > 1 || y < -1)
			{
				std::cout << gameObject["model"].asString() << " : " << type << " has invalid orientation y[2]\n";
			}

			if (y > 1 || y < -1)
			{
				std::cout << gameObject["model"].asString() << " : " << type << " has invalid orientation z[3]\n";
			}
		}

		const Json::Value scaleNode = gameObject["scale"];
		if (scaleNode.type() != Json::nullValue)
		{
			x = scaleNode[0].asFloat();
			y = scaleNode[1].asFloat();
			z = scaleNode[2].asFloat();

			properties.m_scale = glm::vec3(x, y, z);
		}

		// data for the particle effects
		std::string particleFilePath;
		glm::vec3 particleOffset;
		glm::vec4 particleColour;
		float particleSize, particleMaxR, particleMinR, particleExpire;
		bool particleFalling;
		const Json::Value particleNode = gameObject["particle"];
		if (particleNode.type() != Json::nullValue) {
			// texture file
			const Json::Value pTextureNode = gameObject["particle"]["filepath"];
			if (pTextureNode.type() != Json::nullValue) {
				particleFilePath = pTextureNode.asString();
			}
			// particle position in scene
			const Json::Value pPositionNode = gameObject["particle"]["position"];
			if (pPositionNode.type() != Json::nullValue) {
				particleOffset.x = pPositionNode[0].asFloat();
				particleOffset.y = pPositionNode[1].asFloat();
				particleOffset.z = pPositionNode[2].asFloat();
			}
			// particle colour
			const Json::Value pColourNode = gameObject["particle"]["colour"];
			if (pColourNode.type() != Json::nullValue) {
				particleColour.x = pColourNode[0].asFloat();
				particleColour.y = pColourNode[1].asFloat();
				particleColour.z = pColourNode[2].asFloat();
				particleColour.w = pColourNode[3].asFloat();
			}
			// size of particle
			const Json::Value pSizeNode = gameObject["particle"]["size"];
			if (pSizeNode.type() != Json::nullValue) {
				particleSize = pSizeNode.asFloat();
			}
			// range in x and z where particles will randomly spawn between
			const Json::Value pRangeNode = gameObject["particle"]["range"];
			if (pRangeNode.type() != Json::nullValue) {
				particleMaxR = pRangeNode[0].asFloat();
				particleMinR = pRangeNode[1].asFloat();
			}
			// is the particle falling? if false, assume its going up
			const Json::Value pFallingNode = gameObject["particle"]["falling"];
			if (pFallingNode.type() != Json::nullValue) {
				particleFalling = pFallingNode.asBool();
			}
			// particle expiration time limit
			const Json::Value pExpireNode = gameObject["particle"]["expiration"];
			if (pExpireNode.type() != Json::nullValue) {
				particleExpire = pExpireNode.asFloat();
			}
		}

		if (type == "PlayerCharacter")
		{
			m_currentScene->m_player = std::make_shared<PlayerCharacterObject>(properties, gameObject["speed"].asFloat());

			//setObjectProperties(m_currentScene->m_player, properties, false);
			m_currentScene->m_camera = m_currentScene->m_player->getComponent<CameraComponent>();
			m_currentScene->m_player->OnUpdate(0);
		}
		else if (type == "StaticEnvironmentObject")
		{
			auto object = std::make_shared<StaticEnvironmentObject>();
			setObjectProperties(object, properties, false);
			m_currentScene->v_staticObjects.push_back(object);
		}
		else if (type == "CoinObject")
		{
			auto object = std::make_shared<CoinObject>(gameObject["value"].asInt());
			setObjectProperties(object, properties);
		}
		else if (type == "RotateEnvironmentObject")
		{
			auto object = std::make_shared<RotateEnvironmentObject>();
			setObjectProperties(object, properties);
		}
		else if (type == "BounceEnvironmentObject")
		{
			auto object = std::make_shared<BounceEnvironmentObject>();
			setObjectProperties(object, properties);
		}
		else if (type == "LevelGateObject")
		{
			m_currentScene->m_gate = std::make_shared<LevelGateObject>(gameObject["nextLevel"].asString(), gameObject["collide_distance"].asFloat(), gameObject["score"].asFloat());
			setObjectProperties(m_currentScene->m_gate, properties);
		}
		else if (type == "ParticleEmitterObject") {
			auto object = std::make_shared<ParticleEmitterObject>(particleFilePath, particleOffset, particleColour,
				particleSize, particleMaxR, particleMinR, particleFalling, particleExpire);
			setObjectProperties(object, properties, false);
			m_currentScene->v_particles.push_back(object);
		}
		else if (type == "TurretObject")
		{
			if (m_currentScene->m_modelManager->loadModel(gameObject["bullet_model"].asString()))
			{
				auto object = make_shared<TurretObject>();
				object->getComponent<ShootComponent>()->m_modelPath = gameObject["bullet_model"].asString();
				setObjectProperties(object, properties);
			}
		}
		else if (type == "TrapObject")
		{
			auto object = make_shared<TrapObject>();
			setObjectProperties(object, properties);
		}
		else
		{
			std::cout << type << ": unknown type\n";
			continue; // not an object we can create
		}

		std::cout << gameObject["model"].asString() << " : " << type << " loaded\n";
	}

	if (m_currentScene->m_player != nullptr)
	{
		// TODO Possibly move this to the scene class so each scene
		// could have it's own handler so you can have different handlers
		m_inputHandler.push_back(GameInputHandler(m_currentScene->m_player.get()));
	}

	m_currentScene->m_background = GameObject();
	m_currentScene->m_background.addComponent(new RedComponent(0.53f));
	m_currentScene->m_background.addComponent(new GreenComponent(0.81f));
	m_currentScene->m_background.addComponent(new BlueComponent(0.98f));
	m_inputHandler.push_back(BgInputHandler(&m_currentScene->m_background));

	return true;
}

void GameProject::removeLevel()
{
	//delete m_currentScene->m_background;
	delete m_currentScene;
	m_currentScene = nullptr;
	m_inputHandler.clear();
}

void GameProject::setObjectProperties(std::shared_ptr<GameObject> object, ModelProperties properties, bool pushBack) const
{
	object->getComponent<TransformComponent>()->translate(properties.m_position);
	object->getComponent<TransformComponent>()->scale(properties.m_scale);
	object->getComponent<TransformComponent>()->setOrientation(properties.m_orientation);
	object->getComponent<ModelComponent>()->m_modelProperties = properties;
	if (pushBack) m_currentScene->v_gameObjects.push_back(object);
}