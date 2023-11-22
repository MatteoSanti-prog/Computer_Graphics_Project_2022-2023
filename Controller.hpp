#define GLM_FORCE_DEPTH_ZERO_TO_ONE
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
void FreeCam(float deltaT, glm::vec3 m, glm::vec3 r, glm::mat4& ViewMatrix, glm::mat4& WorldMatrix, glm::vec3& CarPos, float& CarYaw, glm::vec3& CamPos) {
	/*Constant used for dumping*/
	const float lambda = 10.0f;

	/*Constant used for setting the initial position of the camera*/
	const glm::vec3 StartingPosition = glm::vec3(0.0f, 2.0f, 7.0f);

	/*Constants used to bind movement and rotation of the camera*/
	const float MinPitch = glm::radians(-90.0f);
	const float MaxPitch = glm::radians(90.0f);
	const float RotSpeed = glm::radians(90.0f);
	const float MovSpeed = 1.0f;

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

	/*Create a World Matrix for the car*/
	WorldMatrix = glm::translate(glm::mat4(1.0), CarPos) * glm::rotate(glm::mat4(1.0), CarYaw, glm::vec3(0, 1, 0));

	/*Update the output variables (car position and car direction are fixed)*/
	CamPos = LocalCamPos;
	CarPos = glm::vec3(0.0f);
	CarYaw = glm::radians(180.0f);
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
void GameLogic(float deltaT, glm::vec3 m, glm::vec3 r, glm::mat4& ViewMatrix, glm::mat4& WorldMatrix, glm::vec3& CarPos, float& CarYaw, glm::vec3& CamPos) {
	
	/*Constant used for dumping*/
	const float lambda = 10.0f;
	
	/*Constants used for setting the initial position and the initial direction of the car*/
	const glm::vec3 StartingPosition = glm::vec3(10.0f, 0.0, 10.0f);
	const float StartingDirection = glm::radians(0.0f);
	
	/*Constants representing the height and the distance of the camera from the car*/
	const float CamHeight = 1.0f;
	const float CamDist = 5.0f;
	
	/*Constants used to bind pitch of the camera*/
	const float MinPitch = glm::radians(10.0f);
	const float MaxPitch = glm::radians(30.0f);

	/*Constants used to bind translational speed, acceleration and deceleration of the car*/
	const float MaxSpeedForward = 15.0f;
	const float MinSpeedBackward = -3.0f;	
	const float AccFactorForward = 5.0f;
    const float AccFactorBackward = -2.5f;
	const float FrictionFactor = 3.5f;
	const float MotorBrakeFactor = 8.5f;
	
	/*Constants used to bind rotational speed, acceleration and deceleration of the car*/
	const float MaxRotSpeed = glm::radians(120.0f);
    const float RotDecFactor = 1.0f;
    const float PowerSteeringFactor = 3.5f;
    const float RotAccFactor = 4.5f;
    
	/*Static variables used to store the current transaltional and rotational speed of the camera*/
	static float MovSpeed = 0.0f;
	static float RotSpeed = glm::radians(0.0f);

	/*Floating point variables used to store translational and rotational factors of acceleration and deceleration*/
	float MovAccelerationFactor, MovDecelerationFactor, RotAccelerationFactor, RotDecelerationFactor;

	/*Floating point variable used to store acceleration*/
	float Acceleration;

	/*Floating point variable used to store normalized movement speed*/
	float NormMovSpeed;

	/*Floating point variable used to store the parameter used for interpolation*/
	float Amount;

	/*Static variables used to store current positon and direction both for the car and for the camera*/
	static float LocalCarYaw = StartingDirection;
	static glm::vec3 LocalCamPos, LocalCarPos;
	static float CamPitch = glm::radians(10.0f);

	/*Static variables used to perform dumping*/
	static glm::vec3 CamPosOld = StartingPosition, CamPosNew;

	/*3D vector used to store the point of the car followed by the camera*/
	glm::vec3 Target;

	/*3D vector which represents the unitary movement along z-axis (here the direction of the car determines this vector)*/
	glm::vec3 uz = glm::vec3(glm::rotate(glm::mat4(1), LocalCarYaw, glm::vec3(0, 1, 0)) * glm::vec4(0, 0, -1, 1));

	/*Update and bind the pitch angle of the camera*/
	CamPitch += -RotSpeed * r.x * deltaT;
	CamPitch = CamPitch < MinPitch ? MinPitch :
		(CamPitch > MaxPitch ? MaxPitch : CamPitch);
    
	/*Update the normalized translational speed (the update depends on the sign of the translational speed of the car)*/
    NormMovSpeed = MovSpeed >= 0 ? MovSpeed/MaxSpeedForward : MovSpeed/MinSpeedBackward;

	/*Update the translational factors (the update depends on the status of the key used for acceleration)*/
    MovDecelerationFactor = m.z != 0 ? -NormMovSpeed * FrictionFactor : -NormMovSpeed * (FrictionFactor + MotorBrakeFactor);
	MovAccelerationFactor = m.z > 0.0f ? AccFactorForward :
		(m.z < 0.0f ? AccFactorBackward : 0.0f);

	/*Update the acceleration of the car*/
	Acceleration = MovAccelerationFactor + MovDecelerationFactor;
    
	/*Update and bind the translational speed of the car*/
	MovSpeed = MovSpeed > MaxSpeedForward ? MaxSpeedForward :
		(MovSpeed < MinSpeedBackward ? MinSpeedBackward : MovSpeed + Acceleration * deltaT);
    
	/*Update the position of the car*/
	LocalCarPos += uz * MovSpeed * deltaT;

	/*Update the rotational factors (the update depends on the status of the key used for rotation)*/
    RotDecelerationFactor = r.y != 0 ? -RotSpeed * RotDecFactor : -RotSpeed * (RotDecFactor + PowerSteeringFactor); 
    RotAccelerationFactor = r.y * RotAccFactor + RotDecelerationFactor;
    
	/*Update and bind the rotational speed of the car*/
    RotSpeed = RotSpeed > MaxRotSpeed ? MaxRotSpeed :
        (RotSpeed < -MaxRotSpeed ? -MaxRotSpeed : RotSpeed + RotAccelerationFactor * deltaT);
    
	/*Update the parameter used for interpolation*/
    Amount = MovSpeed >= 0 ? (MovSpeed/MaxSpeedForward) : (MovSpeed/MinSpeedBackward);
    
	/*Update the direction of the car using an interpolation method in order to have a more realistic movement*/
    LocalCarYaw = glm::mix(LocalCarYaw, LocalCarYaw - RotSpeed * deltaT, Amount);

	/*Create a World Matrix for the car*/
	WorldMatrix = glm::translate(glm::mat4(1.0), LocalCarPos) * glm::rotate(glm::mat4(1.0), LocalCarYaw, glm::vec3(0, 1, 0));

	/*Update the position of the camera using dumping*/
	CamPosNew = WorldMatrix * glm::vec4(0.0f, CamHeight + CamDist * sin(CamPitch), CamDist * cos(CamPitch), 1.0f);
	Target = WorldMatrix * glm::vec4(0, 0, 0, 1.0f) + glm::vec4(0, CamHeight, 0, 0);
	LocalCamPos = CamPosOld * exp(-lambda * deltaT) + CamPosNew * (1 - exp(-lambda * deltaT));

	/*Create a View Matrix using a look at model*/
	ViewMatrix = glm::lookAt(LocalCamPos, Target, glm::vec3(0.0f, 1.0f, 0.0f));

	/*Update value for dumping*/
	CamPosOld = LocalCamPos;

	/*Create again a World Matrix for the car where the model is rotated consistently with respect to the view*/
	WorldMatrix = glm::translate(glm::mat4(1.0), LocalCarPos) * glm::rotate(glm::mat4(1.0), glm::radians(180.0f) + LocalCarYaw, glm::vec3(0, 1, 0));

	/*Update the output variables (car position and car direction are fixed)*/
	CamPos = LocalCamPos;
	CarPos = LocalCarPos;
	CarYaw = LocalCarYaw;
}
