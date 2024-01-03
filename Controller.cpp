#include "Controller.hpp"
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "Borders.hpp"
#include "Checkpoint.hpp"
#include "Constants.hpp"

/*Static variable used to store the current position and direction of the camera*/
static glm::vec3 FreeCamPos = FreeCamStartingPosition;
static float FreeCamYaw = 0.0f;
static float FreeCamPitch = 0.0f;

void freeCam(float deltaT, glm::vec3 m, glm::vec3 r, glm::mat4& ViewMatrix, glm::mat4& WorldMatrix, glm::vec3& CarPos, float& CarYaw, glm::vec3& CamPos) {
    
    /*Static variables used to perform dumping*/
    static glm::vec3 CamPosOld = FreeCamStartingPosition;
    static glm::vec3 CamPosNew = FreeCamStartingPosition;

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
    CamPosNew += FreeMovSpeed * m.x * ux * deltaT;
    CamPosNew += FreeMovSpeed * m.y * glm::vec3(0, 1, 0) * deltaT;
    CamPosNew += FreeMovSpeed * m.z * uz * deltaT;
    
    /*Bind the elevation of the camera*/
    CamPosNew.y = CamPosNew.y < 0.2f ? 0.2f : CamPosNew.y;

    /*Check if the position of the camera does not collide with the objects in the scene*/
    if (!validPosition(CamPosNew))
        CamPosNew = CamPosOld;

    /*Update the position of the camera using dumping*/
    FreeCamPos = CamPosOld * exp(-lambda * deltaT) + CamPosNew * (1 - exp(-lambda * deltaT));

    /*Create a View Matrix using the direction and the movement of the camera updated before. The model used is look-in*/
    ViewMatrix = glm::rotate(glm::mat4(1.0), -FreeCamPitch, glm::vec3(1, 0, 0)) *
        glm::rotate(glm::mat4(1.0), -FreeCamYaw, glm::vec3(0, 1, 0)) *
        glm::translate(glm::mat4(1.0), -FreeCamPos);

    /*Update value for dumping*/
    CamPosOld = FreeCamPos;

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
    
    /*Constants used for setting the initial position and the initial direction of the car*/
    const glm::vec3 StartingPosition = glm::vec3(-38.0f, 0.0, 4.0f);
    const float StartingDirection = glm::radians(180.0f);
    
    /*Constants representing the height and the distance of the camera from the car*/
    const float CamHeight = 1.0f;
    const float CamDist = 5.0f;
    
    /*Constants used to bind pitch of the camera*/
    const float MinPitch = glm::radians(10.0f);
    const float MaxPitch = glm::radians(30.0f);
    const float RotSpeedPitch = glm::radians(20.0f);

    /*Constants used to bind translational speed and acceleration of the car*/
    const float MinSpeedThreshold = 0.3f;
    const float MaxMovSpeedForward = 15.0f;
    const float MaxMovSpeedBackward = 6.0f;
    const float MovAccFactorForward = 2.5f;
    const float MovAccFactorBackward = 2.0f;

    /*Constant used to implement the motor brake (during translational movement)*/
    const float MotorBrakeFactor = 0.15f;

    /*Constant used to implement power steering (during rotational movement)*/
    const float PowerSteeringFactor = 3.5f;
    
    /*Constants used to bind rotational speed and acceleration of the car*/
    const float MaxRotSpeed = glm::radians(80.0f);
    const float RotAccFactorForward = 3.5f;
    const float RotAccFactorBackward = 1.0f;
    
    /*Static variables used to store the current transaltional and rotational speed of the car*/
    static float MovSpeed = 0.0f;
    static float RotSpeed = glm::radians(0.0f);

    /*Static variable used to store the transaltional speed of the car in the previous frame*/
    static float LastMovSpeed = 0.0f;

    /*Floating point variables used to store translational and rotational accelerations (and decelerations)*/
    float MovAcceleration, MovDeceleration, RotAcceleration, RotDeceleration;

    /*Floating point variable used to store normalized translational speed*/
    float NormMovSpeed;

    /*Static variables used to store current positon and direction both for the car and for the camera*/
    static float LocalCarYaw = StartingDirection;
    static glm::vec3 LocalCamPos, LocalCarPos = StartingPosition, OldCarPos = StartingPosition;
    static float CamPitch = glm::radians(10.0f);

    /*Static variables used to perform dumping*/
    static glm::vec3 CamPosOld = StartingPosition;
    static glm::vec3 CamPosNew;

    /*3D vector used to store the point of the car followed by the camera*/
    glm::vec3 Target;

    /*3D vector which represents the unitary movement along z-axis (here the direction of the car determines this vector)*/
    glm::vec3 uz = glm::vec3(glm::rotate(glm::mat4(1), LocalCarYaw, glm::vec3(0, 1, 0)) * glm::vec4(0, 0, -1, 1));

    /*Update and bind the pitch angle of the camera*/
    CamPitch += -RotSpeedPitch * r.x * deltaT;
    CamPitch = CamPitch < MinPitch ? MinPitch :
        (CamPitch > MaxPitch ? MaxPitch : CamPitch);

    /*STEERING OF THE CAR*/

    /*Update the rotational accelerations (the update depends on the status of the key used for rotation)*/
    RotDeceleration = r.y != 0 ? 0.0f : -RotSpeed * PowerSteeringFactor;
    
    /*Update the rotational acceleration and bind the rotational speed of the car (the update depends on the status of the key used for translation*/
    RotAcceleration = m.z > 0.0f ? RotAccFactorForward : RotAccFactorBackward;
    
    /*Update the rotational speed*/
    RotSpeed = RotSpeed > MaxRotSpeed ? MaxRotSpeed :
        (RotSpeed < -MaxRotSpeed ? -MaxRotSpeed : RotSpeed + (r.y * RotAcceleration + RotDeceleration) * deltaT);

    /*TRANSLATION OF THE CAR*/

    /*Update the normalized translational speed (the update depends on the sign of the translational speed of the car)*/
    NormMovSpeed = MovSpeed >= 0 ? MovSpeed / MaxMovSpeedForward : MovSpeed / MaxMovSpeedBackward;

    /*Update the translational accelerations (the update depends on the status of the key used for acceleration)*/
    MovDeceleration = m.z != 0 ? 0.0f : -MovSpeed * MotorBrakeFactor;
    MovAcceleration = m.z > 0.0f ? MovAccFactorForward :
        (m.z < 0.0f ? MovAccFactorBackward : 0.0f);

    /*Update and bind the translational speed of the car*/
    MovSpeed = MovSpeed > MaxMovSpeedForward ? MaxMovSpeedForward :
        (MovSpeed < -MaxMovSpeedBackward ? -MaxMovSpeedBackward : MovSpeed + (m.z * MovAcceleration + MovDeceleration) * deltaT);

    /*If the translational speed reaches a certain threshold, its value becomes zero*/
    MovSpeed = MovSpeed < MinSpeedThreshold && LastMovSpeed >= MinSpeedThreshold ? 0.0f :
        (MovSpeed > -MinSpeedThreshold && LastMovSpeed <= -MinSpeedThreshold ? 0.0f : MovSpeed);

    /*Update the translational speed of the previous frame*/
    LastMovSpeed = MovSpeed;
    
    /*Update the direction of the car using an interpolation method in order to have a more realistic movement*/
    LocalCarYaw = glm::mix(LocalCarYaw, LocalCarYaw - RotSpeed * deltaT, NormMovSpeed);

    /*Update the position of the car*/
    LocalCarPos += uz * MovSpeed * deltaT;
    if (!validPosition(LocalCarPos)) {
        LocalCarPos = OldCarPos;
        MovSpeed = 0.0f;
    }

    /*Check the checkpoints*/
    if(trackCheckpoints(LocalCarPos)){
        LocalCarPos = StartingPosition;
        LocalCarYaw = StartingDirection;
        MovSpeed = 0.0f;
        
        initializeCheckpoints();
        resetFreeCam();
        return 0; //Reset GameState to 0 (SplashArt)
    }
        
    /*Create a World Matrix for the car*/
    WorldMatrix = glm::translate(glm::mat4(1.0), LocalCarPos) * glm::rotate(glm::mat4(1.0), LocalCarYaw, glm::vec3(0, 1, 0));

    /*Update the position of the camera using dumping*/
    CamPosNew = WorldMatrix * glm::vec4(0.0f, CamHeight + CamDist * sin(CamPitch), CamDist * cos(CamPitch), 1.0f);
    Target = WorldMatrix * glm::vec4(0, 0, 0, 1.0f) + glm::vec4(0, CamHeight, 0, 0);
    LocalCamPos = CamPosOld * exp(-lambda * deltaT) + CamPosNew * (1 - exp(-lambda * deltaT));
    if (!validPosition(LocalCamPos))
        LocalCamPos = CamPosOld;

    /*Create a View Matrix using a look at model*/
    ViewMatrix = glm::lookAt(LocalCamPos, Target, glm::vec3(0.0f, 1.0f, 0.0f));

    /*Update value for dumping*/
    CamPosOld = LocalCamPos;

    /*Update the output variables (car position and car direction are fixed)*/
    CamPos = LocalCamPos;
    CarPos = LocalCarPos;
    CarYaw = LocalCarYaw;

    /*Create again a World Matrix for the car where the model is rotated consistently with respect to the view*/
    WorldMatrix = glm::translate(glm::mat4(1.0), LocalCarPos) * glm::rotate(glm::mat4(1.0), glm::radians(180.0f) + LocalCarYaw, glm::vec3(0, 1, 0));

    OldCarPos = LocalCarPos;
    
    return 2;
}

void resetFreeCam(){
    FreeCamPos = FreeCamStartingPosition;
}
