#include "Camera.h"
#include <SDL.h>
#include <iostream>

using namespace glm;

void Camera::update()
{
	takeInput();
	rot = orientate4(axis);

	forward = vec3(normalize(rot * vec4(0.0, 0.0, -1.0, 0.0)));
	up = vec3(normalize(rot * vec4(0.0, 1.0, 0.0, 0.0)));
	right = vec3(normalize(rot * vec4(1.0, 0.0, 0.0, 0.0)));

	pos += 0.1f * (right * (float)playerX + forward * (float)playerY);
	view = lookAt(pos, pos + forward, up);
}

void Camera::takeInput() {
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			default:
				break;
			case SDLK_a:
				playerX = -1;
				break;
			case SDLK_d:
				playerX = 1;
				break;
			case SDLK_w:
				playerY = 1;
				break;
			case SDLK_s:
				playerY = -1;
				break;
			case SDLK_ESCAPE:
				quit = true;
				break;
			}
			break;
		case SDL_KEYUP:
			switch (event.key.keysym.sym) {
			default:
				break;
			case SDLK_a:
				if (playerX < 0)
					playerX = 0;
				break;
			case SDLK_d:
				if (playerX > 0)
					playerX = 0;
				break;
			case SDLK_w:
				if (playerY > 0)
					playerY = 0;
				break;
			case SDLK_s:
				if (playerY < 0)
					playerY = 0;
				break;
			}
			break;
		case SDL_MOUSEMOTION:
			axis.z -= event.motion.xrel * 0.003;
			axis.x += -event.motion.yrel * 0.003;
			break;
		default:
			break;
		}
	}
}
