#version 450 core
out vec4 FragColour;

#define MAX_LIGHT 1000

layout (std140, binding = 1) uniform Lights {
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
    mat4 pLight     = mat4(0.0f);

    for (int i = 0; i < total; i++) {
        pLight = light[i];

        if (pLight[0][0] == 0.0f)
            result = CalcDirLight(pLight, normal, viewDir);
        else
            result = CalcPointLight(pLight, normal, FragPos, viewDir);
    }

    FragColour = vec4(result, 1.0f);
}


vec3 CalcDirLight(mat4 Light, vec3 Normal, vec3 ViewDirection) {
    float brightness    = Light[0][1];
    vec3 lightPosition  = vec3(Light[1][0], Light[1][1], Light[1][2]);
    vec3 lightColour    = vec3(Light[2][0], Light[2][1], Light[2][2]) * brightness;
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
    float brightness    = Light[0][1];
    vec3 lightPosition  = vec3(Light[1][0], Light[1][1], Light[1][2]);
    vec3 lightColour    = vec3(Light[2][0], Light[2][1], Light[2][2]) * brightness;
    vec3 lightDirection = normalize(lightPosition - FragmentPosition);
    vec3 reflectDir     = reflect(-lightDirection, Normal);

    float diff  = max(dot(lightDirection, Normal), 0.0f);
    float spec  = pow(max(dot(ViewDirection, reflectDir), 0.0f), material.shininess);

    float d     = length(lightPosition - FragmentPosition);
    float kC    = Light[3][0];
    float kL    = Light[3][1];
    float kQ    = Light[3][2];
    float att   = 1.0f /  (kC + kL * d + kQ * (d * d));

    vec3 ambient    = lightColour * material.ambient;
    vec3 diffuse    = lightColour * diff * material.diffuse;
    vec3 specular   = lightColour * spec * material.specular;
    
    ambient     *= att;
    diffuse     *= att;
    specular    *= att;

    return ambient + diffuse + specular;
}