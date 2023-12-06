
#include "Starter.hpp"

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

const float scalingFactor = 4.0f;

class A16 : public BaseProject {
	protected:

	float Ar;

	DescriptorSetLayout DSLGubo, DSLMesh;
	
	VertexDescriptor VMesh;
	
	Pipeline PMesh;
	
	Model<VertexMesh> MCar, MApartment, MCrane, MDwellingStore1, MDwellingStore8, MDwelling1, MDwelling12, MEntertainment6, MEntertainment7, MEnv, MRoad;

	DescriptorSet DSGubo, DSCar, DSApartment, DSCrane, DSDwellingStore1, DSDwellingStore8, DSDwelling1, DSDwelling12, DSEntertainment6, DSEntertainment7, DSEnv, DSRoad;

	Texture TCity;

	MeshUniformBlock uboCar, uboApartment, uboCrane, uboDwellingStore1, uboDwellingStore8, uboDwelling1, uboDwelling12, uboEntertainment6, uboEntertainment7, uboEnv, uboRoad;

	GlobalUniformBlock gubo;


	int GameState;
	bool MoveCam;

	void setWindowParameters() {
		
		windowWidth = 800;
		windowHeight = 600;
		windowTitle = "A16";
		windowResizable = GLFW_TRUE;
		initialBackgroundColor = { 0.0f, 0.005f, 0.01f, 1.0f };

		uniformBlocksInPool = 12;
		texturesInPool = 11;

		setsInPool = 12;

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
		

		PMesh.init(this, &VMesh, "shaders/MeshVert.spv", "shaders/MeshFrag.spv", { &DSLGubo, &DSLMesh });
		
		MCar.init(this, &VMesh, "Models/transport_cool_009_transport_cool_009.001.mgcg", MGCG);
		MApartment.init(this, &VMesh, "Models/apartment_001.mgcg", MGCG);
		MCrane.init(this, &VMesh, "Models/Crane_001.mgcg", MGCG);
		MDwellingStore1.init(this, &VMesh, "Models/dwelling&store_001.mgcg", MGCG);
		MDwellingStore8.init(this, &VMesh, "Models/dwelling&store_008.mgcg", MGCG);
		MDwelling1.init(this, &VMesh, "Models/dwelling_001.mgcg", MGCG);
		MDwelling12.init(this, &VMesh, "Models/dwelling_012.mgcg", MGCG);
		MEntertainment6.init(this, &VMesh, "Models/landscape_entertainments_006.mgcg", MGCG);
		MEntertainment7.init(this, &VMesh, "Models/landscape_entertainments_007.mgcg", MGCG);
		MRoad.init(this, &VMesh, "Models/road.obj", OBJ);
		
		createEnvironment(MEnv.vertices, MEnv.indices);
		MEnv.initMesh(this, &VMesh);

		TCity.init(this, "textures/Textures_City.png");

		GameState = 0;
		MoveCam = true;
	}

	void pipelinesAndDescriptorSetsInit() {
		
		PMesh.create();
		
		DSCar.init(this, &DSLMesh, {
					{0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
					{1, TEXTURE, 0, &TCity}
			});
		DSApartment.init(this, &DSLMesh, {
					{0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
					{1, TEXTURE, 0, &TCity}
			});
		DSCrane.init(this, &DSLMesh, {
					{0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
					{1, TEXTURE, 0, &TCity}
			});
		DSDwellingStore1.init(this, &DSLMesh, {
					{0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
					{1, TEXTURE, 0, &TCity}
			});
		DSDwellingStore8.init(this, &DSLMesh, {
					{0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
					{1, TEXTURE, 0, &TCity}
			});
		DSDwelling1.init(this, &DSLMesh, {
					{0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
					{1, TEXTURE, 0, &TCity}
			});
		DSDwelling12.init(this, &DSLMesh, {
					{0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
					{1, TEXTURE, 0, &TCity}
			});
		DSEntertainment6.init(this, &DSLMesh, {
					{0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
					{1, TEXTURE, 0, &TCity}
			});
		DSEntertainment7.init(this, &DSLMesh, {
					{0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
					{1, TEXTURE, 0, &TCity}
			});
		
		DSEnv.init(this, &DSLMesh, {
					{0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
					{1, TEXTURE, 0, &TCity}
			});

		DSRoad.init(this, &DSLMesh, {
					{0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
					{1, TEXTURE, 0, &TCity}
			});

		DSGubo.init(this, &DSLGubo, {
					{0, UNIFORM, sizeof(GlobalUniformBlock), nullptr}
			});
	}

	
	void pipelinesAndDescriptorSetsCleanup() {
		
		PMesh.cleanup(); 
		DSCar.cleanup();
		DSApartment.cleanup();
		DSCrane.cleanup();
		DSDwellingStore1.cleanup();
		DSDwellingStore8.cleanup();
		DSDwelling1.cleanup();
		DSDwelling12.cleanup();
		DSEntertainment6.cleanup();
		DSEntertainment7.cleanup();
		DSRoad.cleanup();
		DSEnv.cleanup();
		DSGubo.cleanup();	
	}

	void localCleanup() {
		
		TCity.cleanup();

		MCar.cleanup();
		MApartment.cleanup();
		MCrane.cleanup();
		MDwellingStore1.cleanup();
		MDwellingStore8.cleanup();
		MDwelling1.cleanup();
		MDwelling12.cleanup();
		MEntertainment6.cleanup();
		MEntertainment7.cleanup();
		MRoad.cleanup();
		MEnv.cleanup();

		DSLMesh.cleanup();
		DSLGubo.cleanup();

		PMesh.destroy();		
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
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(MApartment.indices.size()), 1, 0, 0, 0);

		MCrane.bind(commandBuffer);
		DSCrane.bind(commandBuffer, PMesh, 1, currentImage);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(MCrane.indices.size()), 1, 0, 0, 0);

		MDwellingStore1.bind(commandBuffer);
		DSDwellingStore1.bind(commandBuffer, PMesh, 1, currentImage);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(MDwellingStore1.indices.size()), 1, 0, 0, 0);

		MDwellingStore8.bind(commandBuffer);
		DSDwellingStore8.bind(commandBuffer, PMesh, 1, currentImage);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(MDwellingStore8.indices.size()), 1, 0, 0, 0);

		MDwelling1.bind(commandBuffer);
		DSDwelling1.bind(commandBuffer, PMesh, 1, currentImage);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(MDwelling1.indices.size()), 1, 0, 0, 0);

		MDwelling12.bind(commandBuffer);
		DSDwelling12.bind(commandBuffer, PMesh, 1, currentImage);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(MDwelling12.indices.size()), 1, 0, 0, 0);

		MEntertainment6.bind(commandBuffer);
		DSEntertainment6.bind(commandBuffer, PMesh, 1, currentImage);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(MEntertainment6.indices.size()), 1, 0, 0, 0);

		MEntertainment7.bind(commandBuffer);
		DSEntertainment7.bind(commandBuffer, PMesh, 1, currentImage);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(MEntertainment7.indices.size()), 1, 0, 0, 0);

		MRoad.bind(commandBuffer);
		DSRoad.bind(commandBuffer, PMesh, 1, currentImage);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(MRoad.indices.size()), 1, 0, 0, 0);

		MEnv.bind(commandBuffer);
		DSEnv.bind(commandBuffer, PMesh, 1, currentImage);		
		vkCmdDrawIndexed(commandBuffer,
				static_cast<uint32_t>(MEnv.indices.size()), 1, 0, 0, 0);
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

		if (MoveCam)		
			freeCam(deltaT, m, r, View, World, CarPos, CarYaw, CamPos);
		else
			gameLogic(deltaT, m, r, View, World, CarPos, CarYaw, CamPos);

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

		/*
		World = glm::translate(glm::mat4(1.0), glm::vec3(-25.0f, 0.0f, -10.0f));
		uboApartment.amb = 1.0f; uboApartment.gamma = 180.0f; uboApartment.sColor = glm::vec3(1.0f);
		uboApartment.mvpMat = Prj * View * World;
		uboApartment.mMat = World;
		uboApartment.nMat = glm::inverse(glm::transpose(World));
		DSApartment.map(currentImage, &uboApartment, sizeof(uboApartment), 0);

		World = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, 20.0f));
		uboCrane.amb = 1.0f; uboCrane.gamma = 180.0f; uboCrane.sColor = glm::vec3(1.0f);
		uboCrane.mvpMat = Prj * View * World;
		uboCrane.mMat = World;
		uboCrane.nMat = glm::inverse(glm::transpose(World));
		DSCrane.map(currentImage, &uboCrane, sizeof(uboCrane), 0);

		World = glm::translate(glm::mat4(1.0), glm::vec3(-13.5f, 0.0f, -10.0f));
		uboDwellingStore1.amb = 1.0f; uboDwellingStore1.gamma = 180.0f; uboDwellingStore1.sColor = glm::vec3(1.0f);
		uboDwellingStore1.mvpMat = Prj * View * World;
		uboDwellingStore1.mMat = World;
		uboDwellingStore1.nMat = glm::inverse(glm::transpose(World));
		DSDwellingStore1.map(currentImage, &uboDwellingStore1, sizeof(uboDwellingStore1), 0);

		World = glm::translate(glm::mat4(1.0), glm::vec3(13.5f, 0.0f, -10.0f));
		uboDwellingStore8.amb = 1.0f; uboDwellingStore8.gamma = 180.0f; uboDwellingStore8.sColor = glm::vec3(1.0f);
		uboDwellingStore8.mvpMat = Prj * View * World;
		uboDwellingStore8.mMat = World;
		uboDwellingStore8.nMat = glm::inverse(glm::transpose(World));
		DSDwellingStore8.map(currentImage, &uboDwellingStore8, sizeof(uboDwellingStore8), 0);

		World = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -12.5f));
		uboDwelling1.amb = 1.0f; uboDwelling1.gamma = 180.0f; uboDwelling1.sColor = glm::vec3(1.0f);
		uboDwelling1.mvpMat = Prj * View * World;
		uboDwelling1.mMat = World;
		uboDwelling1.nMat = glm::inverse(glm::transpose(World));
		DSDwelling1.map(currentImage, &uboDwelling1, sizeof(uboDwelling1), 0);

		World = glm::translate(glm::mat4(1.0), glm::vec3(25.0f, 0.0f, -10.0f));
		uboDwelling12.amb = 1.0f; uboDwelling12.gamma = 180.0f; uboDwelling12.sColor = glm::vec3(1.0f);
		uboDwelling12.mvpMat = Prj * View * World;
		uboDwelling12.mMat = World;
		uboDwelling12.nMat = glm::inverse(glm::transpose(World));
		DSDwelling12.map(currentImage, &uboDwelling12, sizeof(uboDwelling12), 0);

		World = glm::translate(glm::mat4(1.0), glm::vec3(-20.0f, 0.0f, 10.0f)) * glm::rotate(glm::mat4(1.0), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		uboEntertainment6.amb = 1.0f; uboEntertainment6.gamma = 180.0f; uboEntertainment6.sColor = glm::vec3(1.0f);
		uboEntertainment6.mvpMat = Prj * View * World;
		uboEntertainment6.mMat = World;
		uboEntertainment6.nMat = glm::inverse(glm::transpose(World));
		DSEntertainment6.map(currentImage, &uboEntertainment6, sizeof(uboEntertainment6), 0);

		World = glm::translate(glm::mat4(1.0), glm::vec3(20.0f, 0.0f, 10.0f)) * glm::rotate(glm::mat4(1.0), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		uboEntertainment7.amb = 1.0f; uboEntertainment7.gamma = 180.0f; uboEntertainment7.sColor = glm::vec3(1.0f);
		uboEntertainment7.mvpMat = Prj * View * World;
		uboEntertainment7.mMat = World;
		uboEntertainment7.nMat = glm::inverse(glm::transpose(World));
		DSEntertainment7.map(currentImage, &uboEntertainment7, sizeof(uboEntertainment7), 0);
		*/

		World = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, -2.2f, 0.0f)) * glm::scale(glm::mat4(1.0), glm::vec3(scalingFactor));
		uboRoad.amb = 1.0f; uboRoad.gamma = 180.0f; uboRoad.sColor = glm::vec3(1.0f);
		uboRoad.mvpMat = Prj * View * World;
		uboRoad.mMat = World;
		uboRoad.nMat = glm::inverse(glm::transpose(World));
		DSRoad.map(currentImage, &uboRoad, sizeof(uboRoad), 0);

		World = glm::rotate(glm::mat4(1.0), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0), glm::vec3(scalingFactor));
		uboEnv.amb = 1.0f; uboEnv.gamma = 180.0f; uboEnv.sColor = glm::vec3(1.0f);
		uboEnv.mvpMat = Prj * View * World; 
		uboEnv.mMat = World;
		uboEnv.nMat = glm::inverse(glm::transpose(World));
		DSEnv.map(currentImage, &uboEnv, sizeof(uboEnv), 0);
	}

	void createEnvironment(std::vector<VertexMesh>& vPos, std::vector<uint32_t>& vIdx);
	void freeCam(float deltaT, glm::vec3 m, glm::vec3 r, glm::mat4& ViewMatrix, glm::mat4& WorldMatrix, glm::vec3& CarPos, float& CarYaw, glm::vec3& CamPos);
	void gameLogic(float deltaT, glm::vec3 m, glm::vec3 r, glm::mat4& ViewMatrix, glm::mat4& WorldMatrix, glm::vec3& CarPos, float& CarYaw, glm::vec3& CamPos);
};

#include "Environment.hpp"
#include "Controller.hpp"

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