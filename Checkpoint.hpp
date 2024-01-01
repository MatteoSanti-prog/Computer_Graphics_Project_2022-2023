#pragma once


#include <glm/glm.hpp>
#include <iostream>


class Checkpoint {
    
    private:
    
        glm::vec3 pos;
        float radius;
        bool visible;

    
    public:
    
        Checkpoint();
        Checkpoint(glm::vec3 pos, float radius);
        bool isHitted(glm::vec3 CarPos);
        bool isVisible();
        void changeVisibility();
        glm::vec3 getPos();
    
};

// Function declarations
void initializeCheckpoints();
bool trackCheckpoints(glm::vec3 CarPos);
glm::vec3 getcurrentCheckpointPos();
