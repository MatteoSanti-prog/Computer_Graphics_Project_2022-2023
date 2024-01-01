#include "Checkpoint.hpp"
#include "Constants.hpp"
#include <stack>

//Checkpoint class funtions implementation

Checkpoint::Checkpoint() {}

Checkpoint::Checkpoint(glm::vec3 pos, float radius) {
    
    this->pos = pos * scalingFactor;
    this->radius = radius * scalingFactor;
    this->visible = false;
}

bool Checkpoint::isHitted(glm::vec3 CarPos) {
    if (CarPos.x > (pos.x - radius)
        && CarPos.x < (pos.x + radius)
        && CarPos.z > (pos.z - radius)
        && CarPos.z < (pos.z + radius)
        )
        return true;

    else return false;
}

bool Checkpoint::isVisible(){
    return visible;
}

void Checkpoint::changeVisibility(){
    this->visible = !this->visible;
}

glm::vec3 Checkpoint::getPos(){
    return pos;
}


//Checkpoints list - coordinates (x,y,z)


Checkpoint c1 (glm::vec3(-8.5f, 0.25f, 5.5f), 0.5f);
Checkpoint c2 (glm::vec3(-4.0f, 0.25f, 4.0f), 0.5f);
Checkpoint c3 (glm::vec3(0.0f, 0.25f, 2.5f), 0.5f);
Checkpoint c4 (glm::vec3(3.0f, 0.25f, 6.5f), 0.5f);
Checkpoint c5 (glm::vec3(8.0f, 0.25f, 10.0f), 0.5f);
Checkpoint c6 (glm::vec3(9.5f, 0.25f, 4.5f), 0.5f);
Checkpoint c7 (glm::vec3(9.0f, 0.25f, -1.0f), 0.5f);
Checkpoint c8 (glm::vec3(4.0f, 0.25f, -4.0f), 0.5f);
Checkpoint c9 (glm::vec3(-3.5f, 0.25f, -8.2f), 0.5f);
Checkpoint c10 (glm::vec3(-8.5, 0.25f, -9.5f), 0.5f);
Checkpoint c11 (glm::vec3(-9.5f, 0.25f, -5.5f), 0.5f);
Checkpoint c12 (glm::vec3(-9.5f, 0.25f, 1.0f), 0.5f);

//Checkpoint cTest1 (glm::vec3(0.0f, 0.0f, 2.5f), 0.5f);
//Checkpoint cTest2 (glm::vec3(3.0f, 0.0f, 6.5f), 0.5f);

Checkpoint currentCheckpoint;


//Checkpoint data structure

std::stack<Checkpoint> checkpoints;


//Functions implementation

void initializeCheckpoints(){
    
    checkpoints.push(c12);
    checkpoints.push(c11);
    checkpoints.push(c10);
    checkpoints.push(c9);
    checkpoints.push(c8);
    checkpoints.push(c7);
    checkpoints.push(c6);
    checkpoints.push(c5);
    checkpoints.push(c4);
    checkpoints.push(c3);
    checkpoints.push(c2);
    checkpoints.push(c1);
    
    
    currentCheckpoint = checkpoints.top();
    checkpoints.pop();
    currentCheckpoint.changeVisibility();
    
    std::cout << "\n\nCheckpoint initialization completed!\n";
    std::cout << "First Checkpoint: (" << currentCheckpoint.getPos().x << ',' << currentCheckpoint.getPos().y << ',' << currentCheckpoint.getPos().z <<")\n\n";
}



bool trackCheckpoints(glm::vec3 CarPos){
    
    //std::cout << "Car Pos: (" << CarPos.x << ',' << CarPos.y << ',' << CarPos.z <<")\n\n";
    
    if(checkpoints.empty() && !currentCheckpoint.isVisible()){
            std::cout << "\n\nFINISH!\n\n";
        return true;
    }
    
    if(currentCheckpoint.isHitted(CarPos)){
        
            std::cout << "\n\nCheckpoint with coordinates (" << currentCheckpoint.getPos().x << ',' << currentCheckpoint.getPos().y << ',' << currentCheckpoint.getPos().z <<") hitted!\n\n";
            
            if(!checkpoints.empty()){
                currentCheckpoint.changeVisibility();
                currentCheckpoint = checkpoints.top();
                checkpoints.pop();
            }
        
            currentCheckpoint.changeVisibility();
    }
    
    return false;
}


glm::vec3 getcurrentCheckpointPos(){
    return currentCheckpoint.getPos();
}

