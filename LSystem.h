#pragma once

#include <map>
#include <stack>
#include <vector>

#include <glm\glm.hpp>

#include "Object3D.h"

class LSystem {
public:
    LSystem();
    std::vector<Object3D> produceTree(
        std::string axiom, std::map<char, std::string> productions, int depth,
        glm::vec3 position, float stepSize, float angleIncrement
    );
    //void draw();

private:
    struct TurtleState {
        glm::vec3 position;
        glm::vec3 direction;
        float stepSize;
        glm::vec3 color;
        glm::vec3 rotation;

        TurtleState::TurtleState(
            glm::vec3 position, glm::vec3 direction, float stepSize, glm::vec3 color,
            glm::vec3 rotation
        ) :
            position(position), direction(direction), stepSize(stepSize), color(color),
            rotation(rotation)
        {
        }
    };

    std::stack<TurtleState> turtleStates;
    TurtleState currentTurtleState;

    void rotateTurtle(glm::vec3 angleIncrement);
};
