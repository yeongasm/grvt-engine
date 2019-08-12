#version 400 core
out vec4 FragColour;

uniform vec3 colour;

void main() {
    FragColour = vec4(colour.x, colour.y, colour.z, 1.0f);
}