#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//Scaling factor and rotation speed of the coin
const int numberAssets = 12;
const int rectanglesPerTimer = 8;
const float scalingFactor = 4.0f;
const float coinRotSpeed = glm::radians(45.0f);

//World matrices
const glm::mat4 WorldRoad = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, -1.2f, 0.0f)) *
                            glm::scale(glm::mat4(1.0), glm::vec3(scalingFactor));

const glm::mat4 WorldEnv = glm::rotate(glm::mat4(1.0), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) *
                           glm::scale(glm::mat4(1.0), glm::vec3(scalingFactor));


const glm::mat4 WorldAsset[12] = {
        glm::translate(glm::mat4(1.0), glm::vec3(24.0f, 0.0f, 0.0f)) *
        glm::rotate(glm::mat4(1.0), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
        glm::translate(glm::mat4(1.0), glm::vec3(24.0f, 0.0f, 6.0f)) *
        glm::rotate(glm::mat4(1.0), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
        glm::translate(glm::mat4(1.0), glm::vec3(24.0f, 0.0f, 11.0f)) *
        glm::rotate(glm::mat4(1.0), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
        glm::translate(glm::mat4(1.0), glm::vec3(24.0f, 0.0f, 25.5f)) *
        glm::rotate(glm::mat4(1.0), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
        glm::translate(glm::mat4(1.0), glm::vec3(24.0f, 0.0f, 18.0f)) *
        glm::rotate(glm::mat4(1.0), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
        glm::translate(glm::mat4(1.0), glm::vec3(12.5f, 0.0f, -4.0f)) *
        glm::rotate(glm::mat4(1.0), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
        glm::translate(glm::mat4(1.0), glm::vec3(5.0f, 0.0f, -4.0f)) *
        glm::rotate(glm::mat4(1.0), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
        glm::translate(glm::mat4(1.0), glm::vec3(-2.0f, 0.0f, -4.0f)) *
        glm::rotate(glm::mat4(1.0), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
        glm::translate(glm::mat4(1.0), glm::vec3(-11.0f, 0.0f, -4.0f)) *
        glm::rotate(glm::mat4(1.0), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
        glm::translate(glm::mat4(1.0), glm::vec3(-26.0f, 0.0f, -19.0f)) *
        glm::rotate(glm::mat4(1.0), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
        glm::translate(glm::mat4(1.0), glm::vec3(-26.0f, 0.0f, -7.0f)),
        glm::translate(glm::mat4(1.0), glm::vec3(-26.0f, 0.0f, 4.0f)) *
        glm::rotate(glm::mat4(1.0), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f))
};

//Camera
const float fovy = glm::radians(45.0f);
const float nearPlane = 0.1f;
const float farPlane = 1000.0f;


//Environment
const float h = 2.0f; //colliders height


//Controller - free cam
const float lambda = 10.0f; //camera dumping

const float freeMinPitch = glm::radians(-90.0f);
const float freeMaxPitch = glm::radians(90.0f);
const float freeRotSpeed = glm::radians(90.0f);
const float freeMovSpeed = 10.0f;

const glm::vec3 freeCamStartingPosition = glm::vec3(-59.359f, 11.6599f, 59.9026f);
const float freeCamStartingYaw = -0.782541f;
const float freeCamStartingPitch = -0.0851274f;

//Controller - game logic
/*Constants used for setting the initial position and the initial direction of the car*/
const glm::vec3 carStartingPosition = glm::vec3(-38.0f, 0.1, 4.0f);
const float carStartingDirection = glm::radians(180.0f);

/*Constants representing the height and the distance of the camera from the car*/
const float gLCamHeight = 1.0f;
const float gLCamDist = 5.0f;

/*Constants used to bind pitch of the camera*/
const float glMinPitch = glm::radians(10.0f);
const float glMaxPitch = glm::radians(30.0f);
const float glRotSpeedPitch = glm::radians(20.0f);

/*Constants used to bind translational speed and acceleration of the car*/
const float carMinSpeedThreshold = 0.6f;
const float carMaxMovSpeedForward = 18.0f;
const float carMaxMovSpeedBackward = 2.0f;
const float carMovAccFactorForward = 2.5f;
const float carMovAccFactorBackward = 4.5f;

/*Constant used to implement the motor brake (during translational movement)*/
const float carMotorBrakeFactor = 0.10f;

/*Constant used to implement power steering (during rotational movement)*/
const float carPowerSteeringFactor = 3.5f;

/*Constants used to bind rotational speed and acceleration of the car*/
const float carMaxRotHighSpeedForward = glm::radians(15.0f);
const float carMaxRotLowSpeedForward = glm::radians(140.0f);
const float carMaxRotHighSpeedBackward = glm::radians(10.0f);
const float carMaxRotLowSpeedBackward = glm::radians(20.0f);
const float carRotAccFactorForward = 3.5f;
const float carRotAccFactorBackward = 1.0f;

const float carRotEpsilon = glm::radians(1.5f);


//names
const int SCREEN = 0;
const int FREE_CAMERA = 1;
const int GAME = 2;
const int INITIAL = 3;
