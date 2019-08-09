#version 400 core 
layout (location = 0) in vec3 aPos;

uniform mat4 projection;

void main() {
    glPosition = projection * vec4(aPos, 1.0f);
}