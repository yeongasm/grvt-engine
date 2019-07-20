#version 400 core
out vec4 FragColour;

#define MAX_LIGHT 1000

layout (std140) uniform Lights {
    int         total;
    mat4        light[MAX_LIGHT];
};

struct Material {
    vec3        ambient;
    vec3        diffuse;
    vec3        specular;
    float       shininess;
};

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform vec3        viewPos;
uniform Material    material;

vec3 CalcDirLight(mat4 Light, vec3 Normal, vec3 ViewDirection);
vec3 CalcPointLight(mat4 Light, vec3 Normal, vec3 FragmentPosition, vec3 ViewDirection);

void main() {
    vec3 normal     = normalize(Normal);
    vec3 viewDir    = normalize(viewPos - FragPos);
    vec3 result     = vec3(0.0f);

    for (int i = 0; i < 1000; i++) {
        if (light[i][0][0] == 0.0f)
            result = CalcDirLight(light[i], normal, viewDir);
        else
            result = CalcPointLight(light[i], normal, FragPos, viewDir);
    }

    FragColour = vec4(result, 1.0f);
}


vec3 CalcDirLight(mat4 Light, vec3 Normal, vec3 ViewDirection) {
    vec3 lightPosition  = vec3(Light[1][0], Light[1][1], Light[1][2]);
    vec3 lightColour    = vec3(Light[2][0], Light[2][1], Light[2][2]);
    vec3 lightDirection = normalize(lightPosition - FragPos);

    vec3 ambient = material.ambient * lightColour;

    float diff = max(dot(lightDirection, Normal), 0.0f);
    vec3 diffuse = lightColour * (diff * material.diffuse);

    vec3 reflectDirection = reflect(-lightDirection, Normal);
    float spec = pow(max(dot(ViewDirection, reflectDirection), 0.0f), material.shininess);
    vec3 specular = lightColour * (spec * material.specular);

    return ambient + diffuse + specular;
}


vec3 CalcPointLight(mat4 Light, vec3 Normal, vec3 FragmentPosition, vec3 ViewDirection) {
    vec3 lightPosition  = vec3(Light[1][0], Light[1][1], Light[1][2]);
    vec3 lightColour    = vec3(Light[2][0], Light[2][1], Light[2][2]);
    vec3 lightDirection = normalize(lightPosition - FragmentPosition);
    vec3 reflectDir     = reflect(-lightDirection, Normal);

    float diff  = max(dot(lightDirection, Normal), 0.0f);
    float spec  = pow(max(dot(reflectDir, ViewDirection), 0.0f), material.shininess);

    float d     = length(lightPosition - FragmentPosition);
    float att   = 1.0f /  (Light[3][0] + (Light[3][1] * d) + (Light[3][2] * (d * d)));

    vec3 ambient    = lightColour * material.ambient;
    vec3 diffuse    = lightColour * material.diffuse * diff;
    vec3 specular   = lightColour * material.specular * spec;
    
    ambient     *= att;
    diffuse     *= att;
    specular    *= att;

    return ambient + diffuse + specular;
}