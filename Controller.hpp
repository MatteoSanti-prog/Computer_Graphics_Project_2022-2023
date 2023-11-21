#define GLM_FORCE_DEPTH_ZERO_TO_ONE

void FreeCam(float deltaT, glm::vec3 m, glm::vec3 r, glm::mat4& ViewMatrix, glm::vec3& CarPos, float& CarYaw, glm::vec3& CamPos) {

	const float MinPitch = glm::radians(-90.0f);
	const float MaxPitch = glm::radians(90.0f);

	const float RotSpeed = glm::radians(90.0f);
	const float MovSpeed = 1.0f;

	static glm::vec3 LocalCamPos = glm::vec3(0.0f, 2.0f, 7.0f);

	static float CamYaw = 0.0f, CamPitch = 0.0f;

	glm::vec3 ux = glm::rotate(glm::mat4(1.0f), CamYaw, glm::vec3(0, 1, 0)) * glm::vec4(1, 0, 0, 1);
	glm::vec3 uy = glm::vec3(0, 1, 0);
	glm::vec3 uz = glm::rotate(glm::mat4(1.0f), CamYaw, glm::vec3(0, 1, 0)) * glm::vec4(0, 0, -1, 1);

	CamYaw += -RotSpeed * deltaT * r.y;
	CamPitch += -RotSpeed * deltaT * r.x;
	CamPitch = CamPitch < MinPitch ? MinPitch :
		(CamPitch > MaxPitch ? MaxPitch : CamPitch);

	LocalCamPos += MovSpeed * m.x * ux * deltaT;
	LocalCamPos += MovSpeed * m.y * glm::vec3(0, 1, 0) * deltaT;
	LocalCamPos += MovSpeed * m.z * uz * deltaT;

	LocalCamPos.y = LocalCamPos.y < 0.0f ? 0.0f : LocalCamPos.y;

	ViewMatrix = glm::rotate(glm::mat4(1.0), -CamPitch, glm::vec3(1, 0, 0)) *
		glm::rotate(glm::mat4(1.0), -CamYaw, glm::vec3(0, 1, 0)) *
		glm::translate(glm::mat4(1.0), -LocalCamPos);

	CamPos = LocalCamPos;
	CarPos = glm::vec3(0.0f);
	CarYaw = 0.0f;
}

void GameLogic(float deltaT, glm::vec3 m, glm::vec3 r, glm::mat4& ViewMatrix, glm::vec3& CarPos, float& CarYaw, glm::vec3& CamPos) {
	// Parameters
	const float lambda = 10.0f;

	// Car starting point
	const glm::vec3 StartingPosition = glm::vec3(10.0f, 0.0, 10.0f);
	const float StartingDirection = glm::radians(0.0f);

	// Camera target height and distance
	const float CamHeight = 1.0f;
	const float CamDist = 5.0f;
	// Camera Pitch limits
	const float MinPitch = glm::radians(-8.75f);
	const float MaxPitch = glm::radians(60.0f);
	// Rotation and motion speed
	const float RotSpeed = glm::radians(120.0f);
	const float MovSpeed = 2.0f;

	// Game Logic implementation

	// yaw, pitch sono relativi alla telecamera, mentre yawPlayer è lo yaw del personaggio
	static float LocalCarYaw = StartingDirection;
	static glm::vec3 LocalCamPos, LocalCarPos;
	static float CamYaw = StartingDirection, CamPitch;
	static glm::vec3 CamPosOld = StartingPosition, CamPosNew;

	glm::vec3 Target;

	/*
	Qui vincolo le direzioni del movimento del player alla visuale della telecamera (ad esempio, il movimento in
	avanti del player corrsisponde al movimento verso la direzione nella quale la camera sta puntando).
	Non a caso è lo yaw della camera a determinare queste direzioni.
	Inoltre, controllando la rotazione della telecamera durante il movimento del personaggio, siamo in grado di far
	girare il modello nella direzione desiderata
	*/
	glm::vec3 ux = glm::vec3(glm::rotate(glm::mat4(1),
		CamYaw, glm::vec3(0, 1, 0)) *
		glm::vec4(1, 0, 0, 1));
	glm::vec3 uy = glm::vec3(0, 1, 0);
	glm::vec3 uz = glm::vec3(glm::rotate(glm::mat4(1),
		CamYaw, glm::vec3(0, 1, 0)) *
		glm::vec4(0, 0, -1, 1));

	/*
	Qui gestisco gli angoli e la posizione del personaggio
	*/
	CamPitch += RotSpeed * r.x * deltaT;
	CamPitch = CamPitch < MinPitch ? MinPitch :
		(CamPitch > MaxPitch ? MaxPitch : CamPitch);

	CamYaw += -RotSpeed * r.y * deltaT;

	if (m.z == 1) {
		LocalCarPos += uz * MovSpeed * m.z * deltaT;
		LocalCarYaw = CamYaw + glm::radians(180.0f);
	}

	/*Qui creo la View Matrix*/
	/*
	Notare che non moltiplico c e a per la World Matrix perchè voglio che la telecamera sia svincolata dalla direzione in
	cui guarda il player, ma voglio che sia vincolata solo alla sua posizione. Perciò moltiplico solo per la traslazione rispetto a Pos
	*/
	CamPosNew = glm::translate(glm::mat4(1.0), LocalCarPos) * glm::vec4(sin(CamYaw), CamHeight + CamDist * sin(CamPitch), CamDist * cos(CamPitch) * cos(CamYaw), 1.0f);
	Target = glm::translate(glm::mat4(1.0), LocalCarPos) * glm::vec4(0, 0, 0, 1.0f) + glm::vec4(0, CamHeight, 0, 0);
	LocalCamPos = CamPosOld * exp(-lambda * deltaT) + CamPosNew * (1 - exp(-lambda * deltaT));
	ViewMatrix = glm::lookAt(LocalCamPos, Target, glm::vec3(0.0f, 1.0f, 0.0f));
	CamPosOld = LocalCamPos;

	CamPos = LocalCamPos;
	CarPos = LocalCarPos;
	CarYaw = LocalCarYaw;
}
