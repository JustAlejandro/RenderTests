#version 330 core
struct Material{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_normal1;
};

uniform Material material;

in VS_OUT {
	vec3 FragPos;
	vec2 TexCoord;
	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangentFragPos;
} vs_out;

out vec4 FragColor;

void main()
{
    vec3 normal = texture(material.texture_normal1, vs_out.TexCoord).rgb;

    FragColor = vec4(normal, 1.0);
}