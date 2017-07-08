#pragma once

#include <glm/glm.hpp>

struct RenderInfo {

    const glm::mat4 &viewMatrix;
    const glm::mat4 &projectionMatrix;
    const glm::vec3 &cameraPosition;

    RenderInfo(
        const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix,
        const glm::vec3 &cameraPosition
    ) :
        viewMatrix(viewMatrix),
        projectionMatrix(projectionMatrix),
        cameraPosition(cameraPosition)
    {
    }
};
