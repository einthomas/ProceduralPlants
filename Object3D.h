#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Shader.h"
#include "RenderInfo.h"

class Object3D {
public:
    glm::vec3 position, rotation, scale, color;
    static GLuint cubeVAO;

    Object3D(
        const glm::vec3 &color, const glm::vec3 &position, const glm::vec3 &scale,
        const glm::vec3 &rotation
    );
    void draw(const Shader &shader, const RenderInfo &renderInfo);
    static Object3D makeCube(
        const glm::vec3 &position, const glm::vec3 &scale, const glm::vec3 &color,
        const glm::vec3 &rotation
    );
    glm::mat4 Object3D::calculateModelMatrix();

private:
};
