#version 400 core
out vec4 FragColour;

in vec3 TexCoords;

uniform samplerCube skybox;
uniform vec4 colour;

void main() {
    FragColour = texture(skybox, TexCoords) * colour;
}