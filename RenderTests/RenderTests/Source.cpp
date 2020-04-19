#include <iostream>
#include "SDL_Helper.h"
#include "ModelLoader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

SDL_Window* window;
SDL_GLContext glContext;

int windowHeight = 1080;
int windowWidth = 1920;
string defaultDir = "C:/OneDrive/OneDrive\ -\ The\ University\ of\ Texas\ at\ Austin/Game\ Tests/";
string shaderDir = defaultDir + "RenderTests/RenderTests/RenderTests/";
string nano = defaultDir + "nanosuit/nanosuit.obj";
string sponza = defaultDir + "sponza/sponza.obj";

int main(int argc, char* argv[]) {
	std::cout << "Hello World" << std::endl;

	window = SDL_Setup(windowHeight, windowWidth, glContext);

	ModelLoader loader;
	Model nanoModel = loader.loadModel(nano);
	Shader s((shaderDir + "default.vs").c_str(), (shaderDir + "default.fs").c_str());
	nanoModel.bindShader(s, TYPE_ALL_OBJECTS);

	// view/projection transformations
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)windowWidth / (float)windowHeight, 0.1f, 500.0f);
	glm::mat4 view = lookAt(vec3(0.0,0.0,3.0), vec3(0.0, 0.0, 2.0), vec3(0.0, 1.0f, 0.0f));

	UniformSet us;
	us.set("projection", projection);
	us.set("view", view);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
	us.set("model", model);

	nanoModel.bindUniform(&us, TYPE_ALL_OBJECTS);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	nanoModel.draw();

	SDL_GL_SwapWindow(window);
	SDL_Delay(5000);

	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}