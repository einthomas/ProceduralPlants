#include "LSystem.h"

LSystem::LSystem() :
    currentTurtleState(glm::vec3(0.0f), glm::vec3(0.0f), 0.0f, glm::vec3(0.0f), glm::vec3(0.0f))
{
}

std::vector<Object3D> LSystem::produceTree(
    std::string axiom, std::map<char, std::string> productions, int depth,
    glm::vec3 position, float stepSize, float angleIncrement
) {
    while (!turtleStates.empty()) {
        turtleStates.pop();
    }

    currentTurtleState.position = position;
    currentTurtleState.direction = glm::vec3(0.0f, 1.0f, 0.0f);
    currentTurtleState.stepSize = stepSize;
    currentTurtleState.color = glm::vec3(0.0f, 0.0f, 1.0f);
    currentTurtleState.rotation = glm::vec3(0.0f);

    std::vector<Object3D> object3Ds;

    std::string word;
    for (int i = 0; i < depth; i++) {
        word = "";
        for (int k = 0; k < axiom.length(); k++) {
            if (productions.count(axiom[k])) {
                word += productions[axiom[k]];
            } else {
                word += axiom[k];
            }
        }
        axiom = word;
    }

    for (int i = 0; i < axiom.length(); i++) {
        if (axiom[i] == 'F') {
            // move forward by stepSize and draw
            object3Ds.push_back(Object3D::makeCube(
                currentTurtleState.position, glm::vec3(0.01f, currentTurtleState.stepSize, 0.01f),
                currentTurtleState.color, currentTurtleState.rotation
            ));
            currentTurtleState.position += currentTurtleState.direction * stepSize;
        } else if (axiom[i] == 'f') {
            // move forward by stepSize
            currentTurtleState.position += glm::vec3(0.0f, stepSize, 0.0f);
        } else if (axiom[i] == '+') {
            rotateTurtle(glm::vec3(0.0f, angleIncrement, 0.0f));
        } else if (axiom[i] == '-') {
            rotateTurtle(glm::vec3(0.0f, -angleIncrement, 0.0f));
        } else if (axiom[i] == '\\') {
            rotateTurtle(glm::vec3(0.0f, 0.0f, angleIncrement));
        } else if (axiom[i] == '/') {
            rotateTurtle(glm::vec3(0.0f, 0.0f, -angleIncrement));
        } else if (axiom[i] == '&') {
            rotateTurtle(glm::vec3(-angleIncrement, 0.0f, 0.0f));
        } else if (axiom[i] == '^') {
            rotateTurtle(glm::vec3(angleIncrement, 0.0f, 0.0f));
        } else if (axiom[i] == '|') {
            rotateTurtle(glm::vec3(0.0f, glm::radians(180.0f), 0.0f));
        } else if (axiom[i] == '[') {
            turtleStates.push(currentTurtleState);
        } else if (axiom[i] == ']') {
            currentTurtleState = turtleStates.top();
            turtleStates.pop();
        }
    }

    return object3Ds;
}

void LSystem::rotateTurtle(glm::vec3 angleIncrement) {
    currentTurtleState.rotation += angleIncrement;
    glm::mat4 modelMatrix;
    modelMatrix = glm::rotate(modelMatrix, currentTurtleState.rotation.x, glm::vec3(1, 0, 0));
    modelMatrix = glm::rotate(modelMatrix, currentTurtleState.rotation.y, glm::vec3(0, 1, 0));
    modelMatrix = glm::rotate(modelMatrix, currentTurtleState.rotation.z, glm::vec3(0, 0, 1));
    currentTurtleState.direction = modelMatrix * glm::vec4(glm::vec3(0.0f, 1.0f, 0.0f), 1.0f);
    currentTurtleState.direction = glm::normalize(currentTurtleState.direction);
}
