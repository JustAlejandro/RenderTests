#include <iostream>
#include "SDL_Helper.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include <glm/gtx/string_cast.hpp>
#include "Model.h"
#include "LightModel.h"
#include "Scene.h"


SDL_Window* window;
SDL_GLContext glContext;

int windowHeight = 720;
int windowWidth = 1280;
string defaultDir = "C:/OneDrive/OneDrive\ -\ The\ University\ of\ Texas\ at\ Austin/Game\ Tests/";
string shaderDir = defaultDir + "RenderTests/RenderTests/RenderTests/";
string nanoDir = defaultDir + "nanosuit/nanosuit.obj";
string sponza = defaultDir + "sponza/sponza.obj";
string cubeDir = defaultDir + "cube/source/cube.obj";

int main(int argc, char* argv[]) {
	window = SDL_Setup(windowHeight, windowWidth, glContext);

	Scene scene;

	Shader s((shaderDir + "default.vs").c_str(), (shaderDir + "diffuse_only.fs").c_str());
	Shader blankShader((shaderDir + "default.vs").c_str(), (shaderDir + "default.fs").c_str());

	string nano = "nanosuit";
	string cube = "cube";
	string cube2 = "cube2";

	scene.loadModel(nano, nanoDir);
	scene.bindShader(nano, s, TYPE_ALL_OBJECTS);
	scene.warpModel(nano, vec3(0.0f, -4.0f, -10.0f));
	scene.scaleModel(nano, 0.4f);

	scene.loadLightModel(cube, cubeDir, LIGHT_TYPE_POINT, vec3(0), vec3(-3.0, 3.0, -5.0));
	scene.bindShader(cube, blankShader, TYPE_ALL_OBJECTS);
	scene.warpModel(cube, vec3(-3.0f, 3.0f, -5.0f));
	scene.scaleModel(cube, 0.05f);

	scene.loadLightModel(cube2, cubeDir, LIGHT_TYPE_POINT, vec3(0), vec3(3.0, 3.0, -15.0));
	scene.bindShader(cube2, blankShader, TYPE_ALL_OBJECTS);
	scene.warpModel(cube2, vec3(3.0f, 3.0f, -15.0f));
	scene.scaleModel(cube2, 0.05f);

	scene.bindBasicUniforms();


	Shader normalNoSpecularShader((shaderDir + "normal_no_specular.vs").c_str(), (shaderDir + "normal_no_specular.fs").c_str());
	//nanoModel.bindShader(normalNoSpecularShader, TYPE_DIFFUSE_BUMP_SPECULAR);

	while (scene.stillRunning()) {
		scene.update();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		scene.draw();

		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}