#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;
uniform vec3 viewPos;

out VS_OUT {
	vec3 FragPos;
	vec2 TexCoord;
	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangentFragPos;
} vs_out;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	vs_out.TexCoord = texCoord;
	vs_out.FragPos = aPos;

	vec3 Tan = normalize(vec3(model * vec4(aTangent, 0.0)));
	vec3 Bit = normalize(vec3(model * vec4(aBitangent, 0.0)));
	vec3 Norm = normalize(vec3(model * vec4(aNormal, 0.0)));
	mat3 worldToTanSpace = transpose(mat3(Tan, Bit, Norm));

	vs_out.TangentLightPos = worldToTanSpace * lightPos;
	vs_out.TangentViewPos = worldToTanSpace * viewPos;
	vs_out.TangentFragPos = worldToTanSpace * vec3(model * vec4 (aPos, 1.0));
}