#include <iostream>
#include "SDL_Helper.h"
#include "ModelLoader.h"

SDL_Window* window;
SDL_GLContext glContext;

int windowHeight = 1080;
int windowWidth = 1920;
string nano = "C:/OneDrive/OneDrive\ -\ The\ University\ of\ Texas\ at\ Austin/Game\ Tests/nanosuit/nanosuit.obj";
string sponza = "C:/OneDrive/OneDrive\ -\ The\ University\ of\ Texas\ at\ Austin/Game\ Tests/sponza/sponza.obj";

int main(int argc, char* argv[]) {
	std::cout << "Hello World" << std::endl;

	window = SDL_Setup(windowHeight, windowWidth, glContext);

	ModelLoader loader;
	Model nanoModel = loader.loadModel(nano);
	Shader s;
	nanoModel.bindShader(s, TYPE_ALL_OBJECTS);
	nanoModel.draw();

	SDL_Delay(100);

	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}