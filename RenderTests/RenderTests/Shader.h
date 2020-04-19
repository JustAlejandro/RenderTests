#pragma once
#include <GL/glew.h>
#include <vector>
#include "Mesh.h"
class Shader
{
public:
	unsigned int id;
	vector<Mesh*> meshes;
	void use() {
		glUseProgram(id);
	}
};

