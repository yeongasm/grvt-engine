#version 400 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangents;
layout (location = 4) in vec3 aBitangents;

out vec2 TexCoords;

uniform mat4 composite;

void main() {
    TexCoords = aTexCoords;
    gl_Position = composite * vec4(aPos, 1.0f);
}