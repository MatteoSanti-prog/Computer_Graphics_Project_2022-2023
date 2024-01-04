#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "Borders.hpp"
#include "Checkpoint.hpp"
#include "Constants.hpp"
#include "Controller.hpp"
#include "math.h"

/*FREE CAM VARIABLES*/
/*Static variable used to store the current position and direction of the camera*/
static glm::vec3 FreeCamPos = FreeCamStartingPosition;
static float FreeCamYaw = 0.0f;
static float FreeCamPitch = 0.0f;
/*Static variables used to perform dumping*/
static glm::vec3 FreeCamPosOld = FreeCamStartingPosition;
static glm::vec3 FreeCamPosNew = FreeCamStartingPosition;

/*GAME LOGIC VARIABLES*/
/*Static variables used to store current position and direction both for the car and for the camera*/
static float GLLocalCarYaw = CarStartingDirection;
static glm::vec3 GLLocalCamPos, GLLocalCarPos = CarStartingPosition, GLOldCarPos = CarStartingPosition;
static float GLCamPitch = glm::radians(10.0f);
/*Static variables used to perform dumping*/
static glm::vec3 GLCamPosOld = CarStartingPosition;
static glm::vec3 GLCamPosNew;
/*Static variables used to store the current translational and rotational speed of the car*/
static float GLMovSpeed = 0.0f;
static float GLRotSpeed = glm::radians(0.0f);

/*Static variable used to store the translational speed of the car in the previous frame*/
static float GLLastMovSpeed = 0.0f;

void freeCam(float deltaT, glm::vec3 m, glm::vec3 r, glm::mat4& ViewMatrix, glm::mat4& WorldMatrix, glm::vec3& CarPos, float& CarYaw, glm::vec3& CamPos) {

    /*3D vectors which represent the unitary movement in each axis (here the direction of the camera determines these three vectors)*/
    glm::vec3 ux = glm::rotate(glm::mat4(1.0f), FreeCamYaw, glm::vec3(0, 1, 0)) * glm::vec4(1, 0, 0, 1);
    glm::vec3 uy = glm::vec3(0, 1, 0);
    glm::vec3 uz = glm::rotate(glm::mat4(1.0f), FreeCamYaw, glm::vec3(0, 1, 0)) * glm::vec4(0, 0, -1, 1);

    /*Update the direction of the camera*/
    FreeCamYaw += -FreeRotSpeed * deltaT * r.y;
    FreeCamPitch += -FreeRotSpeed * deltaT * r.x;

    /*Bind the pitch angle of the camera*/
    FreeCamPitch = FreeCamPitch < FreeMinPitch ? FreeMinPitch :
        (FreeCamPitch > FreeMaxPitch ? FreeMaxPitch : FreeCamPitch);

    /*Update the movement of the camera*/
    FreeCamPosNew += FreeMovSpeed * m.x * ux * deltaT;
    FreeCamPosNew += FreeMovSpeed * m.y * uy * deltaT;
    FreeCamPosNew += FreeMovSpeed * m.z * uz * deltaT;
    
    /*Bind the elevation of the camera*/
    FreeCamPosNew.y = FreeCamPosNew.y < 0.5f ? 0.5f : FreeCamPosNew.y;

    /*Check if the position of the camera does not collide with the objects in the scene*/
    if (!validPosition(FreeCamPosNew))
        FreeCamPosNew = FreeCamPosOld;

    /*Update the position of the camera using dumping*/
    FreeCamPos = FreeCamPosOld * exp(-lambda * deltaT) + FreeCamPosNew * (1 - exp(-lambda * deltaT));

    /*Create a View Matrix using the direction and the movement of the camera updated before. The model used is look-in*/
    ViewMatrix = glm::rotate(glm::mat4(1.0), -FreeCamPitch, glm::vec3(1, 0, 0)) *
        glm::rotate(glm::mat4(1.0), -FreeCamYaw, glm::vec3(0, 1, 0)) *
        glm::translate(glm::mat4(1.0), -FreeCamPos);

    /*Update value for dumping*/
    FreeCamPosOld = FreeCamPos;

    /*Update the output variables (car position and car direction are fixed)*/
    CamPos = FreeCamPos;
    CarPos = glm::vec3(-38.0f, 0.0, 4.0f);
    CarYaw = glm::radians(0.0f);

    /*Create a World Matrix for the car*/
    WorldMatrix = glm::translate(glm::mat4(1.0), CarPos) * glm::rotate(glm::mat4(1.0), CarYaw, glm::vec3(0, 1, 0));
}

/*
GameLogic contains the logic to manage the movement and rotation of the car.
Its parameters are:
-) float deltaT = time elapsed from last frame
-) glm::vec3 m = status of the keys associated to the movement
-) glm::vec3 r = status of the keys associated to the rotation
-) glm::mat4& ViewMatrix = reference to a 4x4 View Matrix which will be one of the outputs of the function
-) glm::mat4& WorldMatrix = reference to a 4x4 World Matrix which will be one of the outputs of the function
-) glm::vec3& CarPos = reference to a 3D vector containing the position of the car and which will be one of the outputs of the function
-) float& CarYaw = reference to a floating point number containing the direction of the car and which will be one of the outputs of the function
-) glm::vec3& CamPos = reference to a 3D vector containing the position of the camera and which will be one of the outputs of the function
*/
int gameLogic(float deltaT, glm::vec3 m, glm::vec3 r, glm::mat4& ViewMatrix, glm::mat4& WorldMatrix, glm::vec3& CarPos, float& CarYaw, glm::vec3& CamPos) {

    /*Floating point variables used to store translational and rotational accelerations (and decelerations)*/
    float MovAcceleration, MovDeceleration, RotAcceleration, RotDeceleration;

    /*Floating point variable used to store normalized translational speed*/
    float NormMovSpeed;

    /*3D vector used to store the point of the car followed by the camera*/
    glm::vec3 Target;

    /*3D vector which represents the unitary movement along z-axis (here the direction of the car determines this vector)*/
    glm::vec3 uz = glm::vec3(glm::rotate(glm::mat4(1), GLLocalCarYaw, glm::vec3(0, 1, 0)) * glm::vec4(0, 0, -1, 1));

    /*Update and bind the pitch angle of the camera*/
    GLCamPitch += -GLRotSpeedPitch * r.x * deltaT;
    GLCamPitch = GLCamPitch < GLMinPitch ? GLMinPitch :
                 (GLCamPitch > GLMaxPitch ? GLMaxPitch : GLCamPitch);

    /*STEERING OF THE CAR*/

    /*Update the rotational accelerations (the update depends on the status of the key used for rotation)*/
    RotDeceleration = r.y != 0 ? 0.0f : -GLRotSpeed * CarPowerSteeringFactor;
    
    /*Update the rotational acceleration and bind the rotational speed of the car (the update depends on the status of the key used for translation*/
    RotAcceleration = m.z > 0.0f ? CarRotAccFactorForward : CarRotAccFactorBackward;
    
    /*Update the rotational speed*/
    GLRotSpeed = GLRotSpeed > CarMaxRotSpeed ? CarMaxRotSpeed :
                 (GLRotSpeed < -CarMaxRotSpeed ? -CarMaxRotSpeed : GLRotSpeed + (r.y * RotAcceleration + RotDeceleration) * deltaT);

    /*TRANSLATION OF THE CAR*/

    /*Update the normalized translational speed (the update depends on the sign of the translational speed of the car)*/
    NormMovSpeed = GLMovSpeed >= 0 ? GLMovSpeed / CarMaxMovSpeedForward : GLMovSpeed / CarMaxMovSpeedBackward;

    /*Update the translational accelerations (the update depends on the status of the key used for acceleration)*/
    MovDeceleration = m.z != 0 ? 0.0f : -GLMovSpeed * CarMotorBrakeFactor;
    MovAcceleration = m.z > 0.0f ? CarMovAccFactorForward :
                      (m.z < 0.0f ? CarMovAccFactorBackward : 0.0f);

    /*Update and bind the translational speed of the car*/
    GLMovSpeed = GLMovSpeed > CarMaxMovSpeedForward ? CarMaxMovSpeedForward :
                 (GLMovSpeed < -CarMaxMovSpeedBackward ? -CarMaxMovSpeedBackward : GLMovSpeed + (m.z * MovAcceleration + MovDeceleration) * deltaT);

    /*If the translational speed reaches a certain threshold, its value becomes zero*/
    GLMovSpeed = GLMovSpeed < CarMinSpeedThreshold && GLLastMovSpeed >= CarMinSpeedThreshold ? 0.0f :
                 (GLMovSpeed > -CarMinSpeedThreshold && GLLastMovSpeed <= -CarMinSpeedThreshold ? 0.0f : GLMovSpeed);

    /*Update the translational speed of the previous frame*/
    GLLastMovSpeed = GLMovSpeed;
    
    /*Update the direction of the car using an interpolation method in order to have a more realistic movement*/
    GLLocalCarYaw = glm::mix(GLLocalCarYaw, GLLocalCarYaw - GLRotSpeed * deltaT, NormMovSpeed);

    /*Update the position of the car*/
    GLLocalCarPos += uz * GLMovSpeed * deltaT;
    if (!validPosition(GLLocalCarPos)) {
        GLLocalCarPos = GLOldCarPos;
        GLMovSpeed = 0.0f;
    }

    /*Check the checkpoints*/
    if(trackCheckpoints(GLLocalCarPos)){
        GLLocalCarPos = CarStartingPosition;
        GLLocalCarYaw = CarStartingDirection;
        GLMovSpeed = 0.0f;
        
        initializeCheckpoints();
        resetFreeCam();
        return 0; //Reset GameState to 0 (SplashArt)
    }
        
    /*Create a World Matrix for the car*/
    WorldMatrix = glm::translate(glm::mat4(1.0), GLLocalCarPos) * glm::rotate(glm::mat4(1.0), GLLocalCarYaw, glm::vec3(0, 1, 0));

    /*Update the position of the camera using dumping*/
    GLCamPosNew = WorldMatrix * glm::vec4(0.0f, GLCamHeight + GLCamDist * sin(GLCamPitch), GLCamDist * cos(GLCamPitch), 1.0f);
    Target = WorldMatrix * glm::vec4(0, 0, 0, 1.0f) + glm::vec4(0, GLCamHeight, 0, 0);
    GLLocalCamPos = GLCamPosOld * exp(-lambda * deltaT) + GLCamPosNew * (1 - exp(-lambda * deltaT));
    if (!validPosition(GLLocalCamPos))
        GLLocalCamPos = GLCamPosOld;

    /*Create a View Matrix using a look at model*/
    ViewMatrix = glm::lookAt(GLLocalCamPos, Target, glm::vec3(0.0f, 1.0f, 0.0f));

    /*Update value for dumping*/
    GLCamPosOld = GLLocalCamPos;

    /*Update the output variables (car position and car direction are fixed)*/
    CamPos = GLLocalCamPos;
    CarPos = GLLocalCarPos;
    CarYaw = GLLocalCarYaw;

    /*Create again a World Matrix for the car where the model is rotated consistently with respect to the view*/
    WorldMatrix = glm::translate(glm::mat4(1.0), GLLocalCarPos) * glm::rotate(glm::mat4(1.0), glm::radians(180.0f) + GLLocalCarYaw, glm::vec3(0, 1, 0));

    GLOldCarPos = GLLocalCarPos;
    
    return 2;
}

void resetFreeCam(){
    FreeCamPosOld = FreeCamStartingPosition;
    FreeCamPosNew = FreeCamStartingPosition;
    FreeCamYaw = 0.0f;
    FreeCamPitch = 0.0f;
}
