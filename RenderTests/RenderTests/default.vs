#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 texCoord;

struct Camera{
	mat4 view;
	mat4 projection;
	vec3 angle;
	vec3 pos;
	vec3 forward;
};

uniform Camera camera;

uniform mat4 model;

out vec2 vTexCoord;
out vec3 vNormal;
out vec3 vViewtoModel;
out vec3 vFragCoordWorld;

void main()
{
	vFragCoordWorld = (model * vec4(aPos, 1.0)).xyz;
	gl_Position = camera.projection * camera.view * model * vec4(aPos, 1.0);
	vTexCoord = texCoord;
	vNormal = normalize(vec3(model * vec4(aNormal, 0.0)));
	vViewtoModel = camera.pos - model[3].xyz;
}