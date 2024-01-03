#pragma once

#include <glm/glm.hpp>

class Collider {
private:
	float right;
	float left;
	float bottom;
	float top;
	float height;
public:
    Collider();
    Collider(float right, float left, float top, float bottom, float height);
    bool isInside(glm::vec3 pos);
};

bool validPosition(glm::vec3 pos);
