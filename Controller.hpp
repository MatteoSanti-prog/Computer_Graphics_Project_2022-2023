#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "Borders.hpp"
/*
FreeCam contains the logic to manage the movement and rotation of the camera in the initial part of the application.
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
void A16::freeCam(float deltaT, glm::vec3 m, glm::vec3 r, glm::mat4& ViewMatrix, glm::mat4& WorldMatrix, glm::vec3& CarPos, float& CarYaw, glm::vec3& CamPos) {
	/*Constant used for dumping*/
	const float lambda = 10.0f;

	/*Constant used for setting the initial position of the camera*/
	const glm::vec3 StartingPosition = glm::vec3(0.0f, 2.0f, 7.0f);

	/*Constants used to bind movement and rotation of the camera*/
	const float MinPitch = glm::radians(-90.0f);
	const float MaxPitch = glm::radians(90.0f);
	const float RotSpeed = glm::radians(90.0f);
	const float MovSpeed = 10.0f;

	/*Static variable used to store the current position of the camera*/
	static glm::vec3 LocalCamPos = StartingPosition;

	/*Static variables used to store the current direction of the camera*/
	static float CamYaw = 0.0f;
	static float CamPitch = 0.0f;

	/*Static variables used to perform dumping*/
	static glm::vec3 CamPosOld = StartingPosition;
	static glm::vec3 CamPosNew = StartingPosition;

	/*3D vectors which represent the unitary movement in each axis (here the direction of the camera determines these three vectors)*/
	glm::vec3 ux = glm::rotate(glm::mat4(1.0f), CamYaw, glm::vec3(0, 1, 0)) * glm::vec4(1, 0, 0, 1);
	glm::vec3 uy = glm::vec3(0, 1, 0);
	glm::vec3 uz = glm::rotate(glm::mat4(1.0f), CamYaw, glm::vec3(0, 1, 0)) * glm::vec4(0, 0, -1, 1);

	/*Update the direction of the camera*/
	CamYaw += -RotSpeed * deltaT * r.y;
	CamPitch += -RotSpeed * deltaT * r.x;

	/*Bind the pitch angle of the camera*/
	CamPitch = CamPitch < MinPitch ? MinPitch :
		(CamPitch > MaxPitch ? MaxPitch : CamPitch);

	/*Update the movement of the camera*/
	CamPosNew += MovSpeed * m.x * ux * deltaT;
	CamPosNew += MovSpeed * m.y * glm::vec3(0, 1, 0) * deltaT;
	CamPosNew += MovSpeed * m.z * uz * deltaT;
	
	/*Bind the elevation of the camera*/
	CamPosNew.y = CamPosNew.y < 0.0f ? 0.0f : CamPosNew.y;

	/*Update the position of the camera using dumping*/
	LocalCamPos = CamPosOld * exp(-lambda * deltaT) + CamPosNew * (1 - exp(-lambda * deltaT));

	/*Create a View Matrix using the direction and the movement of the camera updated before. The model used is look-in*/
	ViewMatrix = glm::rotate(glm::mat4(1.0), -CamPitch, glm::vec3(1, 0, 0)) *
		glm::rotate(glm::mat4(1.0), -CamYaw, glm::vec3(0, 1, 0)) *
		glm::translate(glm::mat4(1.0), -LocalCamPos);

	/*Update value for dumping*/
	CamPosOld = LocalCamPos;

	/*Update the output variables (car position and car direction are fixed)*/
	CamPos = LocalCamPos;
	CarPos = glm::vec3(0.0f, 0.0, 50.0f);
	CarYaw = glm::radians(180.0f);

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
void A16::gameLogic(float deltaT, glm::vec3 m, glm::vec3 r, glm::mat4& ViewMatrix, glm::mat4& WorldMatrix, glm::vec3& CarPos, float& CarYaw, glm::vec3& CamPos) {
	
	/*Constant used for dumping*/
	const float lambda = 10.0f;
	
	/*Constants used for setting the initial position and the initial direction of the car*/
	const glm::vec3 StartingPosition = glm::vec3(0.0f);
	const float StartingDirection = glm::radians(0.0f);
	
	/*Constants representing the height and the distance of the camera from the car*/
	const float CamHeight = 1.0f;
	const float CamDist = 5.0f;
	
	/*Constants used to bind pitch of the camera*/
	const float MinPitch = glm::radians(10.0f);
	const float MaxPitch = glm::radians(30.0f);
	const float RotSpeedPitch = glm::radians(20.0f);

	/*Constants used to bind translational speed and acceleration of the car*/
	const float SpeedThreshold = 0.12f;
	const float MaxMovSpeedForward = 15.0f;
	const float MaxMovSpeedBackward = 6.0f;	
	const float MovAccFactorForward = 5.0f;
    const float MovAccFactorBackward = 2.5f;

	/*Constant used to implement the motor brake (during translational movement)*/
	const float MotorBrakeFactor = 0.6f;

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
	static glm::vec3 LocalCamPos, LocalCarPos = glm::vec3(0.0f, 0.0, 50.0f), OldCarPos = glm::vec3(0.0f, 0.0, 50.0f);
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

	/*QUI POSSIAMO PENSARE DI AGGIUNGERE FRIZIONE LATERALE*/
	/*Del tipo: LateralFriction = r.y * LateralFrictionFactor (poi gestisci il segno della frizione laterale perchè deve sempre
	essere sotttratto alla velocità corrente)*/

	/*If the translational speed reaches a certain threshold, its value becomes zero*/
	MovSpeed = MovSpeed < SpeedThreshold && LastMovSpeed >= SpeedThreshold ? 0.0f :
		(MovSpeed > -SpeedThreshold && LastMovSpeed <= -SpeedThreshold ? 0.0f : MovSpeed);

	/*Update the translational speed of the previous frame*/
	LastMovSpeed = MovSpeed;
    
	/*Update the direction of the car using an interpolation method in order to have a more realistic movement*/
    LocalCarYaw = glm::mix(LocalCarYaw, LocalCarYaw - RotSpeed * deltaT, NormMovSpeed);

	/*Update the position of the car*/
	LocalCarPos += uz * MovSpeed * deltaT;
	if (!validPosition(LocalCarPos, scalingFactor))
		LocalCarPos = OldCarPos;

	/*Create a World Matrix for the car*/
	WorldMatrix = glm::translate(glm::mat4(1.0), LocalCarPos) * glm::rotate(glm::mat4(1.0), LocalCarYaw, glm::vec3(0, 1, 0));

	/*Update the position of the camera using dumping*/
	CamPosNew = WorldMatrix * glm::vec4(0.0f, CamHeight + CamDist * sin(CamPitch), CamDist * cos(CamPitch), 1.0f);
	Target = WorldMatrix * glm::vec4(0, 0, 0, 1.0f) + glm::vec4(0, CamHeight, 0, 0);
	LocalCamPos = CamPosOld * exp(-lambda * deltaT) + CamPosNew * (1 - exp(-lambda * deltaT));
	if (!validPosition(LocalCamPos, scalingFactor))
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
}
