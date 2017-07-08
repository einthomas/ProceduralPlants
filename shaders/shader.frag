#version 330 core

out vec4 outColor;

in vec3 vertNormal;
in vec3 vertFragPosition;
in vec2 vertUV;

uniform vec3 cameraPosition;
uniform vec3 color;

void main() {
    vec3 ambient = vec3(0.1f) * color;

    vec3 lightPosition = cameraPosition;
    vec3 lightDirection = normalize(lightPosition - vertFragPosition);
    vec3 diffuse = max(dot(lightDirection, vertNormal), 0.0f) * color;

    outColor = vec4(ambient + diffuse, 1.0f);
}
