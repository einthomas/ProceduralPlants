#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vertNormal;
out vec3 vertFragPosition;
out vec2 vertUV;

void main() {
    vec4 worldPosition = model * vec4(position, 1.0f);
    gl_Position = projection * view * worldPosition;
    vertNormal = normalize(vec3(model * vec4(normal, 0.0f)).xyz);
    vertFragPosition = vec3(worldPosition);
    vertUV = uv;
}
