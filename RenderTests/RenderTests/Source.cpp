#include <iostream>
#include "SDL_Helper.h"
#include "ModelLoader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

SDL_Window* window;
SDL_GLContext glContext;

int windowHeight = 480;
int windowWidth = 720;
string defaultDir = "C:/OneDrive/OneDrive\ -\ The\ University\ of\ Texas\ at\ Austin/Game\ Tests/";
string shaderDir = defaultDir + "RenderTests/RenderTests/RenderTests/";
string nano = defaultDir + "nanosuit/nanosuit.obj";
string sponza = defaultDir + "sponza/sponza.obj";
string cube = defaultDir + "cube/source/cube.obj";

int main(int argc, char* argv[]) {
	window = SDL_Setup(windowHeight, windowWidth, glContext);

	ModelLoader loader;
	Model nanoModel = loader.loadModel(nano);
	Shader s((shaderDir + "default.vs").c_str(), (shaderDir + "diffuse_only.fs").c_str());
	nanoModel.bindShader(s, TYPE_ALL_OBJECTS);

	Model cubeModel = loader.loadModel(cube);
	Shader defaultShader((shaderDir + "default.vs").c_str(), (shaderDir + "default.fs").c_str());
	cubeModel.bindShader(defaultShader, TYPE_ALL_OBJECTS);

	Shader normalNoSpecularShader((shaderDir + "normal_no_specular.vs").c_str(), (shaderDir + "normal_no_specular.fs").c_str());
	nanoModel.bindShader(normalNoSpecularShader, TYPE_DIFFUSE_BUMP_SPECULAR);

	// view/projection transformations
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)windowWidth / (float)windowHeight, 0.1f, 500.0f);
	glm::mat4 view = lookAt(vec3(0.0,0.0,3.0), vec3(0.0, 0.0, 2.0), vec3(0.0, 1.0f, 0.0f));

	GenericUniformSet camera;
	camera.set("projection", &projection);
	camera.set("view", &view);

	GenericUniformSet nanoUniform;
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -4.0f, -10.0f)); // translate it down so it's at the center of the scene
	model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));	// it's a bit too big for our scene, so scale it down
	nanoUniform.set("model", &model);

	GenericUniformSet cubeUniform;
	glm::mat4 cubeModelMatrix = glm::mat4(1.0f);
	cubeModelMatrix = glm::translate(cubeModelMatrix, vec3(-3.0f, 3.0f, -5.0f));
	cubeModelMatrix = glm::scale(cubeModelMatrix, glm::vec3(0.05f, 0.05f, 0.05f));
	cubeUniform.set("model", &cubeModelMatrix);

	nanoModel.bindUniform(&camera, TYPE_ALL_OBJECTS);
	nanoModel.bindUniform(&nanoUniform, TYPE_ALL_OBJECTS);

	cubeModel.bindUniform(&camera, TYPE_ALL_OBJECTS);
	cubeModel.bindUniform(&cubeUniform, TYPE_ALL_OBJECTS);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	nanoModel.draw();
	cubeModel.draw();

	SDL_GL_SwapWindow(window);
	SDL_Delay(1000);

	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}