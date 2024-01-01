#pragma once

#include <glm/glm.hpp>

// Camera
const float FOVy = glm::radians(45.0f);
const float nearPlane = 0.1f;
const float farPlane = 1000.0f;


//Environment
const float h = 2.0f; //colliders height


//
const float scalingFactor = 4.0f;
const float coinRotSpeed = glm::radians(45.0f);


//Controller - free cam
const float lambda = 10.0f; //camera dumping

const float FreeMinPitch = glm::radians(-90.0f);
const float FreeMaxPitch = glm::radians(90.0f);
const float FreeRotSpeed = glm::radians(90.0f);
const float FreeMovSpeed = 10.0f;

const glm::vec3 FreeCamStartingPosition = glm::vec3(0.0f, 2.0f, 7.0f);
