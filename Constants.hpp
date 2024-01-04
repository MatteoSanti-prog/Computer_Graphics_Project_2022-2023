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

//Controller - game logic
/*Constants used for setting the initial position and the initial direction of the car*/
const glm::vec3 CarStartingPosition = glm::vec3(-38.0f, 0.0, 4.0f);
const float CarStartingDirection = glm::radians(180.0f);

/*Constants representing the height and the distance of the camera from the car*/
const float GLCamHeight = 1.0f;
const float GLCamDist = 5.0f;

/*Constants used to bind pitch of the camera*/
const float GLMinPitch = glm::radians(10.0f);
const float GLMaxPitch = glm::radians(30.0f);
const float GLRotSpeedPitch = glm::radians(20.0f);

/*Constants used to bind translational speed and acceleration of the car*/
const float CarMinSpeedThreshold = 0.3f;
const float CarMaxMovSpeedForward = 15.0f;
const float CarMaxMovSpeedBackward = 6.0f;
const float CarMovAccFactorForward = 2.5f;
const float CarMovAccFactorBackward = 2.0f;

/*Constant used to implement the motor brake (during translational movement)*/
const float CarMotorBrakeFactor = 0.15f;

/*Constant used to implement power steering (during rotational movement)*/
const float CarPowerSteeringFactor = 3.5f;

/*Constants used to bind rotational speed and acceleration of the car*/
const float CarMaxRotSpeed = glm::radians(80.0f);
const float CarRotAccFactorForward = 3.5f;
const float CarRotAccFactorBackward = 1.0f;
