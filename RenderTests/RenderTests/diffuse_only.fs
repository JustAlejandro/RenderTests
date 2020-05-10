#version 330 core
#define MAX_LIGHTS 10

struct Material{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_normal1;
};

struct DirectionalLight{
    vec3 direction;
};

struct PointLight{
    vec3 position;
};

struct SpotLight{
    vec3 direction;
    vec3 position;
};

uniform int pointLightCount;
uniform int spotLightCount;
uniform int directionalLightCount;

uniform DirectionalLight directionalLights[MAX_LIGHTS];
uniform PointLight pointLights[MAX_LIGHTS];
uniform SpotLight spotLights[MAX_LIGHTS];

in vec2 vTexCoord;
in vec3 vNormal;
in vec3 vFragCoordWorld;

out vec4 FragColor;

uniform Material material;

float calculatePointLights();

void main() {
    float diffuseFactor = min(calculatePointLights(), 1.0);
    float ambient = 0.05f;
    float lightFactor = min(diffuseFactor + ambient, 1.0);

    FragColor = vec4(lightFactor * texture(material.texture_diffuse1, vTexCoord).xyz, 1.0);
}

float calculatePointLights(){
    float pointLightFactor = 0.0f;
    for(int i = 0; i < pointLightCount; i++) {
        pointLightFactor += max(dot(normalize(vNormal), normalize(pointLights[i].position - vFragCoordWorld)), 0.0);
    }
    return min(pointLightFactor, 1.0f);
}