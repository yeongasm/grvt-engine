#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangents;
layout (location = 4) in vec3 aBitangents;

layout (std140, binding = 0) uniform ProjView {
    mat4 projview;
};

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;

/**
* TODO(Afiq):
* There is a better way to calculate the Normal matrix.
* Check out Eric's Blog website.
*/
void main() {
    TexCoords   = aTexCoords;
    FragPos     = vec3(model * vec4(aPos, 1.0f));
    Normal      = mat3(transpose(inverse(model))) * aNormals;

    gl_Position = projview * model * vec4(aPos, 1.0f);
}