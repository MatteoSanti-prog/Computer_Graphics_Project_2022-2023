#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "Borders.hpp"
#include "Checkpoint.hpp"
#include "Constants.hpp"
#include "Controller.hpp"
#include "math.h"

/*FREE CAM VARIABLES*/
/*Static variable used to store the current position and direction of the camera*/
static glm::vec3 freeCamPos = freeCamStartingPosition;
static float freeCamYaw = freeCamStartingYaw;
static float freeCamPitch = freeCamStartingPitch;
/*Static variables used to perform dumping*/
static glm::vec3 freeCamPosOld = freeCamStartingPosition;
static glm::vec3 freeCamPosNew = freeCamStartingPosition;

/*GAME LOGIC VARIABLES*/
/*Static variables used to store current position and direction both for the car and for the camera*/
static float glLocalCarYaw = carStartingDirection;
static glm::vec3 glLocalCamPos, glLocalCarPos = carStartingPosition, glOldCarPos = carStartingPosition;
static float glCamPitch = glm::radians(10.0f);
/*Static variables used to perform dumping*/
static glm::vec3 glCamPosOld = carStartingPosition;
static glm::vec3 glCamPosNew;
/*Static variables used to store the current translational and rotational speed of the car*/
static float glMovSpeed = 0.0f;
static float glRotSpeed = glm::radians(0.0f);

/*Static variable used to store the translational speed of the car in the previous frame*/
static float glLastMovSpeed = 0.0f;

void freeCam(float deltaT, glm::vec3 m, glm::vec3 r, glm::mat4& ViewMatrix, glm::mat4& WorldMatrix, glm::vec3& CarPos, float& CarYaw, glm::vec3& CamPos) {

    /*3D vectors which represent the unitary movement in each axis (here the direction of the camera determines these three vectors)*/
    glm::vec3 ux = glm::rotate(glm::mat4(1.0f), freeCamYaw, glm::vec3(0, 1, 0)) * glm::vec4(1, 0, 0, 1);
    glm::vec3 uy = glm::vec3(0, 1, 0);
    glm::vec3 uz = glm::rotate(glm::mat4(1.0f), freeCamYaw, glm::vec3(0, 1, 0)) * glm::vec4(0, 0, -1, 1);

    /*Update the direction of the camera*/
    freeCamYaw += -freeRotSpeed * deltaT * r.y;
    freeCamPitch += -freeRotSpeed * deltaT * r.x;

    /*Bind the pitch angle of the camera*/
    freeCamPitch = freeCamPitch < freeMinPitch ? freeMinPitch :
                   (freeCamPitch > freeMaxPitch ? freeMaxPitch : freeCamPitch);

    /*Update the movement of the camera*/
    freeCamPosNew += freeMovSpeed * m.x * ux * deltaT;
    freeCamPosNew += freeMovSpeed * m.y * uy * deltaT;
    freeCamPosNew += freeMovSpeed * m.z * uz * deltaT;

    /*Bind the elevation of the camera*/
    freeCamPosNew.y = freeCamPosNew.y < 0.5f ? 0.5f : freeCamPosNew.y;

    /*Check if the position of the camera does not collide with the objects in the scene*/
    if (!validPosition(freeCamPosNew))
        freeCamPosNew = freeCamPosOld;

    /*Update the position of the camera using dumping*/
    freeCamPos = freeCamPosOld * exp(-lambda * deltaT) + freeCamPosNew * (1 - exp(-lambda * deltaT));

    /*Create a View Matrix using the direction and the movement of the camera updated before. The model used is look-in*/
    ViewMatrix = glm::rotate(glm::mat4(1.0), -freeCamPitch, glm::vec3(1, 0, 0)) *
                 glm::rotate(glm::mat4(1.0), -freeCamYaw, glm::vec3(0, 1, 0)) *
                 glm::translate(glm::mat4(1.0), -freeCamPos);

    /*Update value for dumping*/
    freeCamPosOld = freeCamPos;

    /*Update the output variables (car position and car direction are fixed)*/
    CamPos = freeCamPos;
    CarPos = carStartingPosition;
    CarYaw = glm::radians(0.0f);

    /*Create a World Matrix for the car*/
    WorldMatrix = glm::translate(glm::mat4(1.0), CarPos) * glm::rotate(glm::mat4(1.0), CarYaw, glm::vec3(0, 1, 0));
}

/*GameLogic contains the logic to manage the movement and rotation of the car and return the state of the game*/
int gameLogic(float deltaT, glm::vec3 m, glm::vec3 r, glm::mat4& ViewMatrix, glm::mat4& WorldMatrix, glm::vec3& CarPos, float& CarYaw, glm::vec3& CamPos, int& brakeOnOff) {

    /*Floating point variables used to store translational and rotational accelerations (and decelerations)*/
    float movAcceleration, movDeceleration, motAcceleration, rotDeceleration;

    /*Floating point variable used to store normalized translational speed*/
    float normMovSpeed;

    float carMaxRotSpeed;

    /*3D vector used to store the point of the car followed by the camera*/
    glm::vec3 Target;

    /*3D vector which represents the unitary movement along z-axis (here the direction of the car determines this vector)*/
    glm::vec3 uz = glm::vec3(glm::rotate(glm::mat4(1), glLocalCarYaw, glm::vec3(0, 1, 0)) * glm::vec4(0, 0, -1, 1));

    /*Update and bind the pitch angle of the camera*/
    glCamPitch += -glRotSpeedPitch * r.x * deltaT;
    glCamPitch = glCamPitch < glMinPitch ? glMinPitch :
                 (glCamPitch > glMaxPitch ? glMaxPitch : glCamPitch);


    /*TRANSLATION OF THE CAR*/

    /*Update the normalized translational speed (the update depends on the sign of the translational speed of the car)*/
    normMovSpeed = glMovSpeed >= 0 ? abs(glMovSpeed / carMaxMovSpeedForward) : abs(glMovSpeed / carMaxMovSpeedBackward);

    /*Update the translational accelerations (the update depends on the status of the key used for acceleration)*/
    movDeceleration = m.z != 0 ? 0.0f : -glMovSpeed * carMotorBrakeFactor;
    movAcceleration = m.z > 0.0f ? carMovAccFactorForward :
                      (m.z < 0.0f ? carMovAccFactorBackward : 0.0f);

    /*Update and bind the translational speed of the car*/
    glMovSpeed = glMovSpeed > carMaxMovSpeedForward ? carMaxMovSpeedForward :
                 (glMovSpeed < -carMaxMovSpeedBackward ? -carMaxMovSpeedBackward : glMovSpeed + (m.z * movAcceleration + movDeceleration) * deltaT);

    /*If the translational speed reaches a certain threshold, its value becomes zero*/
    glMovSpeed = glMovSpeed < carMinSpeedThreshold && glLastMovSpeed >= carMinSpeedThreshold ? 0.0f :
                 (glMovSpeed > -carMinSpeedThreshold && glLastMovSpeed <= -carMinSpeedThreshold ? 0.0f : glMovSpeed);


    /*STEERING OF THE CAR*/

    /*Update the rotational accelerations (the update depends on the status of the key used for rotation)*/
    rotDeceleration = r.y != 0 ? 0.0f : -glRotSpeed * carPowerSteeringFactor;

    /*Update the rotational acceleration and bind the rotational speed of the car (the update depends on the status of the key used for translation*/
    motAcceleration = m.z > 0.0f ? carRotAccFactorForward : carRotAccFactorBackward;

    /*Update the maximum rotation speed of the car on the basis of the translational speed*/
    carMaxRotSpeed = glMovSpeed >= 0 ? carMaxRotHighSpeedForward * normMovSpeed + carMaxRotLowSpeedForward * (1 - normMovSpeed) :
                     carMaxRotHighSpeedBackward * normMovSpeed + carMaxRotLowSpeedBackward * (1 - normMovSpeed);

    /*Update the rotational speed*/
    glRotSpeed = glRotSpeed > carMaxRotSpeed + carRotEpsilon ? carMaxRotSpeed :
                 (glRotSpeed < -(carMaxRotSpeed + carRotEpsilon) ? -carMaxRotSpeed : glRotSpeed + (r.y * motAcceleration + rotDeceleration) * deltaT);

    /*Update the translational speed of the previous frame*/
    glLastMovSpeed = glMovSpeed;

    /*Update the direction of the car using an interpolation method in order to have a more realistic movement*/
    glLocalCarYaw = glm::mix(glLocalCarYaw, glLocalCarYaw - glRotSpeed * deltaT, normMovSpeed);

    /*Update the position of the car*/
    glLocalCarPos += uz * glMovSpeed * deltaT;
    if (!validPosition(glLocalCarPos)) {
        glLocalCarPos = glOldCarPos;
        glMovSpeed = 0.0f;
    }

    /*Check the checkpoints*/
    if(trackCheckpoints(glLocalCarPos)){
        glLocalCarPos = carStartingPosition;
        glLocalCarYaw = carStartingDirection;
        glMovSpeed = 0.0f;

        initializeCheckpoints();
        resetFreeCam();
        return SCREEN; //Reset gameState to 0 (SplashArt)
    }

    /*Update flag in order to know which color to use*/
    if (m.z < 0 && glMovSpeed > 0) {
        brakeOnOff = 0;
    }
    else if (m.z < 0 && glMovSpeed < 0) {
        brakeOnOff = 1;
    }
    else {
        brakeOnOff = 2;
    }

    /*Create a World Matrix for the car*/
    WorldMatrix = glm::translate(glm::mat4(1.0), glLocalCarPos) * glm::rotate(glm::mat4(1.0), glLocalCarYaw, glm::vec3(0, 1, 0));

    /*Update the position of the camera using dumping*/
    glCamPosNew = WorldMatrix * glm::vec4(0.0f, gLCamHeight + gLCamDist * sin(glCamPitch), gLCamDist * cos(glCamPitch), 1.0f);
    Target = WorldMatrix * glm::vec4(0, 0, 0, 1.0f) + glm::vec4(0, gLCamHeight, 0, 0);
    glLocalCamPos = glCamPosOld * exp(-lambda * deltaT) + glCamPosNew * (1 - exp(-lambda * deltaT));
    if (!validPosition(glLocalCamPos))
        glLocalCamPos = glCamPosOld;

    /*Create a View Matrix using a look at model*/
    ViewMatrix = glm::lookAt(glLocalCamPos, Target, glm::vec3(0.0f, 1.0f, 0.0f));

    /*Update value for dumping*/
    glCamPosOld = glLocalCamPos;

    /*Update the output variables (car position and car direction are fixed)*/
    CamPos = glLocalCamPos;
    CarPos = glLocalCarPos;
    CarYaw = glLocalCarYaw;

    /*Create again a World Matrix for the car where the model is rotated consistently with respect to the view*/
    WorldMatrix = glm::translate(glm::mat4(1.0), glLocalCarPos) * glm::rotate(glm::mat4(1.0), glm::radians(180.0f) + glLocalCarYaw, glm::vec3(0, 1, 0));

    glOldCarPos = glLocalCarPos;

    return GAME;
}

/*Reset the game to the initial state when all the checkpoints have been collected*/
void resetFreeCam(){
    freeCamPosOld = freeCamStartingPosition;
    freeCamPosNew = freeCamStartingPosition;
    freeCamYaw = freeCamStartingYaw;
    freeCamPitch = freeCamStartingPitch;
}
