#pragma once
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

#define OPENGL_MAJOR_VERSION 2
#define OPENGL_MINOR_VERSION 1


SDL_Window* SDL_Setup(int height, int width, SDL_GLContext& context) {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cerr << "SDL2 Failed to Initialize: " << SDL_GetError() << std::endl;
		exit(-1);
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OPENGL_MAJOR_VERSION);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OPENGL_MINOR_VERSION);

	SDL_Window* display = SDL_CreateWindow("RenderTests", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);

	if (display == nullptr) {
		std::cerr << "Couldn't Make SDL Window: " << SDL_GetError() << std::endl;
		exit(-1);
	}

	context = SDL_GL_CreateContext(display);

	if (context == nullptr) {
		std::cerr << "Couldn't create OpenGL Context: " << SDL_GetError() << std::endl;
		exit(-1);
	}
	
	if (glGetString(GL_VERSION) == nullptr) {
		std::cerr << "Error with OpenGL Config: " << SDL_GetError() << std::endl;
		exit(-1);
	}

	SDL_GL_MakeCurrent(display, context);

	//Initialize GLEW
	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK) {
		std::cerr << "Error initializing GLEW! %s\n" << glewGetErrorString(glewError) << std::endl;
		exit(-1);
	}

	//Sometimes glew just throws errors, so we'll clear them
	glGetError();

	glViewport(0, 0, width, height);

	glEnable(GL_DEPTH_TEST);

	return display;
}