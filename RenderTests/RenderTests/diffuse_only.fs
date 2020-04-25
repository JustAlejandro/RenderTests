#version 330 core
struct Material{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_normal1;
};

in vec2 vTexCoord;
in vec3 vNormal;

out vec4 FragColor;

uniform Material material;

void main()
{
    FragColor = texture(material.texture_diffuse1, vTexCoord);
}