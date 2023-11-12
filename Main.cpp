#include "Starter.hpp"
#include "Controller.hpp"


struct MeshUniformBlock {
	alignas(4) float amb;
	alignas(4) float gamma;
	alignas(16) glm::vec3 sColor;
	alignas(16) glm::mat4 mvpMat;
	alignas(16) glm::mat4 mMat;
	alignas(16) glm::mat4 nMat;
};

struct GlobalUniformBlock {
	alignas(16) glm::vec3 DlightDir;
	alignas(16) glm::vec3 DlightColor;
	alignas(16) glm::vec3 AmbLightColor;
	alignas(16) glm::vec3 eyePos;
};


struct VertexMesh {
	glm::vec3 pos;
	glm::vec3 norm;
	glm::vec2 UV;
};

struct VertexVColor {
	glm::vec3 pos; /**/
	glm::vec3 norm;
	glm::vec3 color;
    };

class A16;
void FreeCam(float deltaT, glm::vec3 m, glm::vec3 r, glm::mat4& ViewMatrix, glm::mat4& WorldMatrix, glm::vec3& CarPos, float& CarYaw, glm::vec3& CamPos);
void GameLogic(float deltaT, glm::vec3 m, glm::vec3 r, glm::mat4& ViewMatrix, glm::mat4& WorldMatrix, glm::vec3& CarPos, float& CarYaw, glm::vec3& CamPos);

class A16 : public BaseProject {
protected:

	float Ar;

	DescriptorSetLayout DSLGubo, DSLMesh;
	
	VertexDescriptor VMesh;
	
	Pipeline PMesh;
	
	Model<VertexMesh> MCar, MApartment;

	DescriptorSet DSGubo, DSCar, DSApartment;

	Texture TCity;

	MeshUniformBlock uboCar, uboApartment;
	Model<VertexVColor> MRoad; /**/
	VertexDescriptor VVColor; /**/
	MeshUniformBlock uboRoad; /**/
	Pipeline PVColor; /**/
	DescriptorSetLayout DSLVColor; /**/
	DescriptorSet DSRoad;
	GlobalUniformBlock gubo;

	int GameState;
	bool MoveCam;

	void setWindowParameters() {
		
		windowWidth = 800;
		windowHeight = 600;
		windowTitle = "A16";
		windowResizable = GLFW_TRUE;
		initialBackgroundColor = { 0.0f, 0.005f, 0.01f, 1.0f };

		uniformBlocksInPool = 10; //3,2,3 /**/
		texturesInPool = 10;
		setsInPool = 10;

		Ar = (float)windowWidth / (float)windowHeight;
	}

	void onWindowResize(int w, int h) {
		Ar = (float)w / (float)h;
	}

	void localInit() {
		
		DSLMesh.init(this, {
			{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS},
			{1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}
			});

		DSLGubo.init(this, {
					{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS}
			});

		VMesh.init(this, {
			{0, sizeof(VertexMesh), VK_VERTEX_INPUT_RATE_VERTEX}
			}, {
				{0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexMesh, pos),
					   sizeof(glm::vec3), POSITION},
				{0, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexMesh, norm),
					   sizeof(glm::vec3), NORMAL},
				{0, 2, VK_FORMAT_R32G32_SFLOAT, offsetof(VertexMesh, UV),
					   sizeof(glm::vec2), UV}
			});
		
		DSLVColor.init(this, { /**/
					{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS}
			});
		VVColor.init(this, { /**/
			{0, sizeof(VertexVColor), VK_VERTEX_INPUT_RATE_VERTEX}
			}, {
			    {0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexVColor, pos),
					   sizeof(glm::vec3), POSITION},
			    {0, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexVColor, norm),
					   sizeof(glm::vec3), NORMAL},
			    {0, 2, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexVColor, color),
					   sizeof(glm::vec3), COLOR}
			});
		PVColor.init(this, &VVColor, "shaders/VColorVert.spv", "shaders/VColorFrag.spv", { &DSLGubo, &DSLVColor }); /**/
		
		PMesh.init(this, &VMesh, "shaders/MeshVert.spv", "shaders/MeshFrag.spv", { &DSLGubo, &DSLMesh });
		
		MCar.init(this, &VMesh, "Models/transport_cool_009_transport_cool_009.001.mgcg", MGCG);
		MApartment.init(this, &VMesh, "Models/apartment_001.mgcg", MGCG);
		MRoad.init(this, &VVColor, "Models/road.obj", OBJ); /**/
		TCity.init(this, "textures/Textures_City.png");

		GameState = 0;
		MoveCam = true;
	}

	void pipelinesAndDescriptorSetsInit() {
		
		PMesh.create();
		PVColor.create(); /**/
		
		DSCar.init(this, &DSLMesh, {
					{0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
					{1, TEXTURE, 0, &TCity}
			});
		DSApartment.init(this, &DSLMesh, {
					{0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
					{1, TEXTURE, 0, &TCity}
			});
		
		DSGubo.init(this, &DSLGubo, {
					{0, UNIFORM, sizeof(GlobalUniformBlock), nullptr}
			});
			
		DSRoad.init(this, &DSLVColor, { /**/
					{0, UNIFORM, sizeof(MeshUniformBlock), nullptr}
			});
	}

	
	void pipelinesAndDescriptorSetsCleanup() {
		
		PMesh.cleanup();
		PVColor.cleanup(); /**/
		DSCar.cleanup();
		DSApartment.cleanup();
		DSRoad.cleanup();  /**/
		DSGubo.cleanup();
	}

	void localCleanup() {
		
		TCity.cleanup();

		MCar.cleanup();
		MApartment.cleanup();
		MRoad.cleanup(); /**/
		DSLMesh.cleanup();
		DSLGubo.cleanup();

		PMesh.destroy();
		PVColor.destroy(); /**/
	}

	void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage) {

		DSGubo.bind(commandBuffer, PMesh, 0, currentImage);

		PMesh.bind(commandBuffer);

		MCar.bind(commandBuffer);
		DSCar.bind(commandBuffer, PMesh, 1, currentImage);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(MCar.indices.size()), 1, 0, 0, 0);

		MApartment.bind(commandBuffer);
		DSApartment.bind(commandBuffer, PMesh, 1, currentImage);
		
		DSGubo.bind(commandBuffer, PVColor, 0, currentImage); /**/
		PVColor.bind(commandBuffer);
		MRoad.bind(commandBuffer); /**/
		DSRoad.bind(commandBuffer, PMesh, 1, currentImage);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(MRoad.indices.size()), 1, 0, 0, 0); /**/

		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(MApartment.indices.size()), 1, 0, 0, 0);
	}

	void updateUniformBuffer(uint32_t currentImage) {
		bool fire = false;
		float deltaT;
		glm::vec3 m = glm::vec3(0.0f), r = glm::vec3(0.0f);
		glm::vec3 CarPos, CamPos;
		float CarYaw;
		glm::mat4 World, View, Prj;

		const float FOVy = glm::radians(45.0f);
		const float nearPlane = 0.1f;
		const float farPlane = 100.0f;

		if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		getSixAxis(deltaT, m, r, fire);

		switch (GameState) {
		case 0:
		    if (glfwGetKey(window, GLFW_KEY_P)) {
			    MoveCam = false;
			    GameState = 1;
		    }
			break;
		case 1:
			break;
		}

		if (MoveCam) {		
			FreeCam(deltaT, m, r, View, World, CarPos, CarYaw, CamPos);
		}
		else {
			GameLogic(deltaT, m, r, View, World, CarPos, CarYaw, CamPos);
		}

		Prj = glm::perspective(FOVy, Ar, nearPlane, farPlane);
		Prj[1][1] *= -1;

		gubo.DlightDir = glm::normalize(glm::vec3(1, 2, 3));
		gubo.DlightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		gubo.AmbLightColor = glm::vec3(0.1f);
		gubo.eyePos = CamPos;

		DSGubo.map(currentImage, &gubo, sizeof(gubo), 0);

		uboCar.amb = 1.0f; uboCar.gamma = 180.0f; uboCar.sColor = glm::vec3(1.0f);
		uboCar.mvpMat = Prj * View * World;
		uboCar.mMat = World;
		uboCar.nMat = glm::inverse(glm::transpose(World));
		DSCar.map(currentImage, &uboCar, sizeof(uboCar), 0);

		World = glm::translate(glm::mat4(1.0), glm::vec3(10.0f, 0.0f, 10.0f));
		uboApartment.amb = 1.0f; uboApartment.gamma = 180.0f; uboApartment.sColor = glm::vec3(1.0f);
		uboApartment.mvpMat = Prj * View * World;
		uboApartment.mMat = World;
		uboApartment.nMat = glm::inverse(glm::transpose(World));
		DSApartment.map(currentImage, &uboApartment, sizeof(uboApartment), 0);
		
		World = glm::mat4(1); /**/
		uboRoad.amb = 1.0f; uboApartment.gamma = 180.0f; uboApartment.sColor = glm::vec3(1.0f);
		uboRoad.mvpMat = Prj * View * World; /**/
		uboRoad.mMat = World;
		uboRoad.nMat = glm::inverse(glm::transpose(World));
		DSRoad.map(currentImage, &uboRoad, sizeof(uboRoad), 0);
	}
};


int main() {
	A16 app;

	try {
		app.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}