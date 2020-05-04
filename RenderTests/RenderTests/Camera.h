#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "Uniform.h"
#include "SDL.h"

class Camera
{
public:
	Camera(glm::vec3 angle = glm::vec3(0.0, 0.0, 0.0), glm::vec3 pos = glm::vec3(0.0, 0.0, 3.0), float fov = glm::radians(45.0f), 
					float aspect = 16.0 / 9.0, float near = 0.1f, float far = 500.0f) : event(), rot() {
		this->axis = angle;
		this->pos = pos;
		
		this->fov = fov;
		this->aspect = aspect;
		this->near = near;
		this->far = far;

		this->projection = perspective(fov, aspect, near, far);

		uniform.set("camera.view", &this->view);
		uniform.set("camera.angle", &this->axis);
		uniform.set("camera.pos", &this->pos);
		uniform.set("camera.forward", &this->forward);
		uniform.set("camera.projection", &this->projection);
	}

	GenericUniformSet* getUniform() { return &uniform; }

	bool stillRunning() { return !quit; }

	void update();
private:
	GenericUniformSet uniform;
	glm::mat4 view;
	glm::mat4 rot;
	glm::mat4 projection;

	glm::vec3 axis;
	glm::vec3 pos;
	glm::vec3 forward = glm::vec4(0.0, 0.0, 1.0, 0.0);
	glm::vec3 up = glm::vec4(0.0, 1.0, 0.0, 0.0);
	glm::vec3 right = glm::vec4(1.0, 0.0, 0.0, 0.0);

	int playerX = 0;
	int playerY = 0;

	float fov;
	float aspect;
	float near;
	float far;

	bool quit = false;
	SDL_Event event;

	void takeInput();
};

