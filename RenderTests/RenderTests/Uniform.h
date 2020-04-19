#pragma once
#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <SDL_opengl.h>
#include <map>
#include <string>

using namespace glm;
using namespace std;


class UniformSet
{
private:
	map<string, vec3> vec3Uni;
	map<string, vec4> vec4Uni;
	map<string, mat4> mat4Uni;
	map<string, int> intUni;

public:
	void set(string s, vec3 input) {
		vec3Uni[s] = input;
	}
	void set(string s, vec4 input) {
		vec4Uni[s] = input;
	}
	void set(string s, mat4 input) {
		mat4Uni[s] = input;
	}
	void set(string s, int input) {
		intUni[s] = input;
	}
	void apply(unsigned int shaderID) {
		for (pair<string, vec3> i : vec3Uni) {
			glUniform3fv(glGetUniformLocation(shaderID, i.first.c_str()), 1, (const GLfloat*)&i.second[0]);
		}
		for (pair<string, vec4> i : vec4Uni) {
			glUniform4fv(glGetUniformLocation(shaderID, i.first.c_str()), 1, (const GLfloat*)&i.second[0]);
		}
		for (pair<string, mat4> i : mat4Uni) {
			glUniformMatrix4fv(glGetUniformLocation(shaderID, i.first.c_str()), 1, GL_FALSE, (const GLfloat*)&i.second);
		}
		for (pair<string, int> i : intUni) {
			glUniform1i(glGetUniformLocation(shaderID, i.first.c_str()), i.second);
		}
	}
};

