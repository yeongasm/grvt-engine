#version 400 core
out vec4 FragColour;

#define MAX_LIGHTS 100;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform vec3        viewPos;
uniform mat4        lights;
uniform Material    material;

vec3 CalcDirLight(mat4 Light, vec3 Normal, vec3 ViewDirection);

void main() {
    vec3 normal     = normalize(Normal);
    vec3 viewDir    = normalize(viewPos - FragPos);
    vec3 result     = vec3(0.0f);

    result = CalcDirLight(lights, normal, viewDir);

    FragColour = vec4(result, 1.0f);
}


vec3 CalcDirLight(mat4 Light, vec3 Normal, vec3 ViewDirection) {
    vec3 lightColour    = vec3(Light[2][0], Light[2][1], Light[2][2]);
    vec3 lightPosition  = vec3(Light[1][0], Light[1][1], Light[1][2]);
    vec3 lightDirection = normalize(lightPosition - FragPos);

    vec3 ambient = material.ambient * lightColour;

    float diff = max(dot(lightDirection, Normal), 0.0f);
    vec3 diffuse = lightColour * (diff * material.diffuse);

    vec3 reflectDirection = reflect(-lightDirection, Normal);
    float spec = pow(max(dot(ViewDirection, reflectDirection), 0.0f), material.shininess);
    vec3 specular = lightColour * (spec * material.specular);

    return ambient + diffuse + specular;
}