#include "Object3D.h"

#include <glm/gtc/matrix_transform.hpp>

GLuint Object3D::cubeVAO = static_cast<GLuint>(-1);

static GLfloat boxVertices[] = {
    // Positions          // Texture Coords // Normals
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,        0.0f,  0.0f, -1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,        0.0f,  0.0f, -1.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 0.0f,        0.0f,  0.0f, -1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,        0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,        0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,        0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,        0.0f,  0.0f,  1.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,        0.0f,  0.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,        0.0f,  0.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,        0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,        0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,        0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,       -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,       -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,       -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,       -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,       -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,       -1.0f,  0.0f,  0.0f,

    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,        1.0f,  0.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,        1.0f,  0.0f,  0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,        1.0f,  0.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,        1.0f,  0.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,        1.0f,  0.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, 0.0f,        1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,        0.0f, -1.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 1.0f,        0.0f, -1.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,        0.0f, -1.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,        0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,        0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,        0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,        0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,        0.0f,  1.0f,  0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,        0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,        0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,        0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,        0.0f,  1.0f,  0.0f,
};

Object3D::Object3D(
    const glm::vec3 &position, const glm::vec3 &scale, const glm::vec3 &color,
    const glm::vec3 &rotation
) : 
    position(position), scale(scale), color(color), rotation(rotation)
{
}

Object3D Object3D::makeCube(
    const glm::vec3 &position, const glm::vec3 &scale, const glm::vec3 &color,
    const glm::vec3 &rotation
) {
    if (cubeVAO == static_cast<GLuint>(-1)) {
        // create and bind VAO
        glGenVertexArrays(1, &cubeVAO);
        glBindVertexArray(cubeVAO);

        // create and bind VBO
        GLuint VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        // copy vertices array to VBO
        glBufferData(
            GL_ARRAY_BUFFER, sizeof(boxVertices), boxVertices, GL_STATIC_DRAW
        );

        // set vertex attribute pointers
        glVertexAttribPointer(
            0, 3, GL_FLOAT, GL_FALSE,
            8 * sizeof(GLfloat), static_cast<GLvoid*>(0)
        );
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
            1, 2, GL_FLOAT, GL_FALSE,
            8 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat))
        );
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(
            2, 3, GL_FLOAT, GL_FALSE,
            8 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(5 * sizeof(GLfloat))
        );
        glEnableVertexAttribArray(2);

        // unbind VAO
        glBindVertexArray(0);
    }

    return Object3D(position, scale, color, rotation);
}

void Object3D::draw(const Shader &shader, const RenderInfo &renderInfo) {
    shader.use();
    shader.setMatrix4("model", calculateModelMatrix());
    shader.setMatrix4("view", renderInfo.viewMatrix);
    shader.setMatrix4("projection", renderInfo.projectionMatrix);
    shader.setVector3f("cameraPosition", renderInfo.cameraPosition);
    shader.setVector3f("color", color);

    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

glm::mat4 Object3D::calculateModelMatrix() {
    glm::mat4 modelMatrix;
    modelMatrix = glm::translate(modelMatrix, position);
    //modelMatrix = glm::translate(modelMatrix, -glm::vec3(0.0f, scale.y / 2.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, rotation.x, glm::vec3(1, 0, 0));
    modelMatrix = glm::rotate(modelMatrix, rotation.y, glm::vec3(0, 1, 0));
    modelMatrix = glm::rotate(modelMatrix, rotation.z, glm::vec3(0, 0, 1));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, scale.y / 2.0f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, scale);

    return modelMatrix;
}
