#define USE_GLFW 1

#ifdef USE_GLFW
#include "GLFW_EngineCore.h"
#endif

#include "LevelEditor.h"

int main(int argc, char* argv[])
{
	IEngineCore* engineCore;

#ifdef USE_GLFW
	engineCore = new GLFW_EngineCore;
#endif

	if (!engineCore->initWindow(1920, 1080, "Game Level Editor"))
		return -1;

	LevelEditor myFirstGame;

	engineCore->runEngine(myFirstGame);

	return 0;
}
