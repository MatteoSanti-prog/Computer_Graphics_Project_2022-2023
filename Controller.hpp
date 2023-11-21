#define GLM_FORCE_DEPTH_ZERO_TO_ONE

void FreeCam(float deltaT, glm::vec3 m, glm::vec3 r, glm::mat4& ViewMatrix, glm::mat4& WorldMatrix, glm::vec3& CarPos, float& CarYaw, glm::vec3& CamPos) {
	const float lambda = 10.0f;
	const glm::vec3 StartingPosition = glm::vec3(0.0f, 2.0f, 7.0f);

	const float MinPitch = glm::radians(-90.0f);
	const float MaxPitch = glm::radians(90.0f);

	const float RotSpeed = glm::radians(90.0f);
	const float MovSpeed = 1.0f;

	static glm::vec3 LocalCamPos = StartingPosition;
	static glm::vec3 CamPosOld = StartingPosition, CamPosNew = StartingPosition;

	static float CamYaw = 0.0f, CamPitch = 0.0f;

	glm::vec3 ux = glm::rotate(glm::mat4(1.0f), CamYaw, glm::vec3(0, 1, 0)) * glm::vec4(1, 0, 0, 1);
	glm::vec3 uy = glm::vec3(0, 1, 0);
	glm::vec3 uz = glm::rotate(glm::mat4(1.0f), CamYaw, glm::vec3(0, 1, 0)) * glm::vec4(0, 0, -1, 1);

	CamYaw += -RotSpeed * deltaT * r.y;
	CamPitch += -RotSpeed * deltaT * r.x;
	CamPitch = CamPitch < MinPitch ? MinPitch :
		(CamPitch > MaxPitch ? MaxPitch : CamPitch);

	CamPosNew += MovSpeed * m.x * ux * deltaT;
	CamPosNew += MovSpeed * m.y * glm::vec3(0, 1, 0) * deltaT;
	CamPosNew += MovSpeed * m.z * uz * deltaT;
	
	CamPosNew.y = CamPosNew.y < 0.0f ? 0.0f : CamPosNew.y;

	LocalCamPos = CamPosOld * exp(-lambda * deltaT) + CamPosNew * (1 - exp(-lambda * deltaT));
	ViewMatrix = glm::rotate(glm::mat4(1.0), -CamPitch, glm::vec3(1, 0, 0)) *
		glm::rotate(glm::mat4(1.0), -CamYaw, glm::vec3(0, 1, 0)) *
		glm::translate(glm::mat4(1.0), -LocalCamPos);
	CamPosOld = LocalCamPos;

	CamPos = LocalCamPos;
	CarPos = glm::vec3(0.0f);
	CarYaw = glm::radians(180.0f);

	WorldMatrix = glm::translate(glm::mat4(1.0), CarPos) * glm::rotate(glm::mat4(1.0), CarYaw, glm::vec3(0, 1, 0));
}

void GameLogic(float deltaT, glm::vec3 m, glm::vec3 r, glm::mat4& ViewMatrix, glm::mat4& WorldMatrix, glm::vec3& CarPos, float& CarYaw, glm::vec3& CamPos) {
	
	const float lambda = 10.0f;
	
	const glm::vec3 StartingPosition = glm::vec3(10.0f, 0.0, 10.0f);
	const float StartingDirection = glm::radians(0.0f);
	
	const float CamHeight = 1.0f;
	const float CamDist = 5.0f;
	
	const float MinPitch = glm::radians(10.0f);
	const float MaxPitch = glm::radians(30.0f);
	const float MaxSpeedForward = 15.0f;
	const float MinSpeedBackward = -3.0f;
	
	const float AccFactorForward = 5.0f;
    const float AccFactorBackward = -1.0f;
	const float FrictionFactor = 0.5f;
	const float MotorBrakeFactor = 1.0f;
	
	static float MaxRotSpeed = glm::radians(120.0f);
    static float RotSpeed = glm::radians(0.0f);
    static float RotDecFactor = 1.0f;
    static float PowerSteeringFactor = 3.5f;
    static float RotAccFactor = 5.0f;
    
	static float MovSpeed = 0.0f;

	float AccFactor, DecFactor, Acceleration, RotAcc, RotDec;

	static float LocalCarYaw = StartingDirection;
	static glm::vec3 LocalCamPos, LocalCarPos;
	static float CamPitch = glm::radians(10.0f);
	static glm::vec3 CamPosOld = StartingPosition, CamPosNew;

	glm::vec3 Target;

	glm::vec3 ux = glm::vec3(glm::rotate(glm::mat4(1),
		LocalCarYaw, glm::vec3(0, 1, 0)) *
		glm::vec4(1, 0, 0, 1));
	glm::vec3 uy = glm::vec3(0, 1, 0);
	glm::vec3 uz = glm::vec3(glm::rotate(glm::mat4(1),
		LocalCarYaw, glm::vec3(0, 1, 0)) *
		glm::vec4(0, 0, -1, 1));

	CamPitch += -RotSpeed * r.x * deltaT;
	CamPitch = CamPitch < MinPitch ? MinPitch :
		(CamPitch > MaxPitch ? MaxPitch : CamPitch);

    DecFactor = m.z != 0 ? -MovSpeed * FrictionFactor : -MovSpeed * (FrictionFactor + MotorBrakeFactor);

	AccFactor = m.z > 0.0f ? AccFactorForward :
		(m.z < 0.0f ? AccFactorBackward : 0.0f);

	Acceleration = AccFactor + DecFactor;
    
	MovSpeed = MovSpeed > MaxSpeedForward ? MaxSpeedForward :
		(MovSpeed < MinSpeedBackward ? MinSpeedBackward : MovSpeed + Acceleration * deltaT);
    
	LocalCarPos += uz * MovSpeed * deltaT;

    RotDec = r.y != 0 ? -RotSpeed * RotDecFactor : -RotSpeed * (RotDecFactor + PowerSteeringFactor);
    
    RotAcc = r.y * RotAccFactor + RotDec;
    
    RotSpeed = RotSpeed > MaxRotSpeed ? MaxRotSpeed :
        (RotSpeed < -MaxRotSpeed ? -MaxRotSpeed : RotSpeed + RotAcc * deltaT);
    
    float a = MovSpeed >= 0 ? (MovSpeed/MaxSpeedForward) : (MovSpeed/MinSpeedBackward);
    
    LocalCarYaw = glm::mix(LocalCarYaw, LocalCarYaw - RotSpeed * deltaT, a);

	WorldMatrix = glm::translate(glm::mat4(1.0), LocalCarPos) * glm::rotate(glm::mat4(1.0), LocalCarYaw, glm::vec3(0, 1, 0));

	CamPosNew = WorldMatrix * glm::vec4(0.0f, CamHeight + CamDist * sin(CamPitch), CamDist * cos(CamPitch), 1.0f);
	Target = WorldMatrix * glm::vec4(0, 0, 0, 1.0f) + glm::vec4(0, CamHeight, 0, 0);
	LocalCamPos = CamPosOld * exp(-lambda * deltaT) + CamPosNew * (1 - exp(-lambda * deltaT));
	ViewMatrix = glm::lookAt(LocalCamPos, Target, glm::vec3(0.0f, 1.0f, 0.0f));
	CamPosOld = LocalCamPos;

	WorldMatrix = glm::translate(glm::mat4(1.0), LocalCarPos) * glm::rotate(glm::mat4(1.0), glm::radians(180.0f) + LocalCarYaw, glm::vec3(0, 1, 0));

	CamPos = LocalCamPos;
	CarPos = LocalCarPos;
	CarYaw = LocalCarYaw;
}
