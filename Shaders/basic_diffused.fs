#version 400 core
out vec4 FragColour;

in vec2 TexCoords;

uniform sampler2D diffuseTexture;

void main() {
    FragColour = texture(diffuseTexture, TexCoords);
}