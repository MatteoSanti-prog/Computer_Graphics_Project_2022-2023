// This has been adapted from the Vulkan tutorial

#include "Starter.hpp"

// The uniform buffer objects data structures
// Remember to use the correct alignas(...) value
//        float : alignas(4)
//        vec2  : alignas(8)
//        vec3  : alignas(16)
//        vec4  : alignas(16)
//        mat3  : alignas(16)
//        mat4  : alignas(16)

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

// The vertices data structures
struct VertexMesh {
	glm::vec3 pos;
	glm::vec3 norm;
	glm::vec2 UV;
};

static glm::vec3 lastCamPos = glm::vec3(1.0f, 0.0f, 3.0f);

// MAIN ! 
class A16 : public BaseProject {
protected:

	// Current aspect ratio (used by the callback that resized the window
	float Ar;

	// Descriptor Layouts ["classes" of what will be passed to the shaders]
	DescriptorSetLayout DSLGubo, DSLMesh;
	// Vertex formats
	VertexDescriptor VMesh;
	// Pipelines [Shader couples]
	Pipeline PMesh;
	// Models, textures and Descriptors (values assigned to the uniforms)
	// Please note that Model objects depends on the corresponding vertex structure
	Model<VertexMesh> MBody, MHandle, MWheel, MCar;
	DescriptorSet DSGubo, DSCar;
	Texture TCity;

	// C++ storage for uniform variables
	MeshUniformBlock uboCar;
	GlobalUniformBlock gubo;

	// Other application parameters
	float CamH, CamRadius, CamPitch, CamYaw;
	int gameState;
	glm::vec3 camPos = glm::vec3(1.0f, 0.0f, 3.0f);
	bool MoveCam = true;


	// Here you set the main application parameters
	void setWindowParameters() {
		// window size, titile and initial background
		windowWidth = 800;
		windowHeight = 600;
		windowTitle = "A16";
		windowResizable = GLFW_TRUE;
		initialBackgroundColor = { 0.0f, 0.005f, 0.01f, 1.0f };

		// Descriptor pool sizes
		/* A16 */
		/* Update the requirements for the size of the pool */
		uniformBlocksInPool = 2;
		texturesInPool = 1;
		setsInPool = 2;

		Ar = (float)windowWidth / (float)windowHeight;
	}

	// What to do when the window changes size
	void onWindowResize(int w, int h) {
		Ar = (float)w / (float)h;
	}

	// Here you load and setup all your Vulkan Models and Texutures.
	// Here you also create your Descriptor set layouts and load the shaders for the pipelines
	void localInit() {
		// Descriptor Layouts [what will be passed to the shaders]
		DSLMesh.init(this, {
			// this array contains the bindings:
			// first  element : the binding number
			// second element : the type of element (buffer or texture)
			//                  using the corresponding Vulkan constant
			// third  element : the pipeline stage where it will be used
			//                  using the corresponding Vulkan constant
			{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS},
			{1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}
			});

		DSLGubo.init(this, {
					{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS}
			});

		// Vertex descriptors
		VMesh.init(this, {
			// this array contains the bindings
			// first  element : the binding number
			// second element : the stride of this binging
			// third  element : whether this parameter change per vertex or per instance
			//                  using the corresponding Vulkan constant
			{0, sizeof(VertexMesh), VK_VERTEX_INPUT_RATE_VERTEX}
			}, {
				// this array contains the location
				// first  element : the binding number
				// second element : the location number
				// third  element : the offset of this element in the memory record
				// fourth element : the data type of the element
				//                  using the corresponding Vulkan constant
				// fifth  elmenet : the size in byte of the element
				// sixth  element : a constant defining the element usage
				//                   POSITION - a vec3 with the position
				//                   NORMAL   - a vec3 with the normal vector
				//                   UV       - a vec2 with a UV coordinate
				//                   COLOR    - a vec4 with a RGBA color
				//                   TANGENT  - a vec4 with the tangent vector
				//                   OTHER    - anything else
				//
				// ***************** DOUBLE CHECK ********************
				//    That the Vertex data structure you use in the "offsetoff" and
				//	in the "sizeof" in the previous array, refers to the correct one,
				//	if you have more than one vertex format!
				// ***************************************************
				{0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexMesh, pos),
					   sizeof(glm::vec3), POSITION},
				{0, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexMesh, norm),
					   sizeof(glm::vec3), NORMAL},
				{0, 2, VK_FORMAT_R32G32_SFLOAT, offsetof(VertexMesh, UV),
					   sizeof(glm::vec2), UV}
			});

		// Pipelines [Shader couples]
		// The second parameter is the pointer to the vertex definition
		// Third and fourth parameters are respectively the vertex and fragment shaders
		// The last array, is a vector of pointer to the layouts of the sets that will
		// be used in this pipeline. The first element will be set 0, and so on..
		PMesh.init(this, &VMesh, "shaders/MeshVert.spv", "shaders/MeshFrag.spv", { &DSLGubo, &DSLMesh });
		// Models, textures and Descriptors (values assigned to the uniforms)

		// Create models
		// The second parameter is the pointer to the vertex definition for this model
		// The third parameter is the file name
		// The last is a constant specifying the file type: currently only OBJ or GLTF
		MCar.init(this, &VMesh, "Models/transport_cool_009_transport_cool_009.001.mgcg", MGCG);
		

        /*
		// Creates a mesh with direct enumeration of vertices and indices
		MKey.vertices = { {{-0.8f, 0.6f}, {0.0f,0.0f}}, {{-0.8f, 0.95f}, {0.0f,1.0f}},
						 {{ 0.8f, 0.6f}, {1.0f,0.0f}}, {{ 0.8f, 0.95f}, {1.0f,1.0f}} };
		MKey.indices = { 0, 1, 2,    1, 2, 3 };
		MKey.initMesh(this, &VOverlay);

		// Creates a mesh with direct enumeration of vertices and indices
		MSplash.vertices = { {{-1.0f, -0.58559f}, {0.0102f, 0.0f}}, {{-1.0f, 0.58559f}, {0.0102f,0.85512f}},
						 {{ 1.0f,-0.58559f}, {1.0f,0.0f}}, {{ 1.0f, 0.58559f}, {1.0f,0.85512f}} };
		MSplash.indices = { 0, 1, 2,    1, 2, 3 };
		MSplash.initMesh(this, &VOverlay);
		*/

		// Create the textures
		// The second parameter is the file name
		TCity.init(this, "textures/Textures_City.png");

		// Init local variables
		CamH = 1.0f;
		CamRadius = 3.0f;
		CamPitch = glm::radians(15.f);
		CamYaw = glm::radians(30.f);
		gameState = 0;
	}

	// Here you create your pipelines and Descriptor Sets!
	void pipelinesAndDescriptorSetsInit() {
		// This creates a new pipeline (with the current surface), using its shaders
		PMesh.create();
		
		DSCar.init(this, &DSLMesh, {
					{0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
					{1, TEXTURE, 0, &TCity}
			});
		
		DSGubo.init(this, &DSLGubo, {
					{0, UNIFORM, sizeof(GlobalUniformBlock), nullptr}
			});
	}

	// Here you destroy your pipelines and Descriptor Sets!
	// All the object classes defined in Starter.hpp have a method .cleanup() for this purpose
	void pipelinesAndDescriptorSetsCleanup() {
		// Cleanup pipelines
		PMesh.cleanup();
		DSCar.cleanup();
		DSGubo.cleanup();
	}

	// Here you destroy all the Models, Texture and Desc. Set Layouts you created!
	// All the object classes defined in Starter.hpp have a method .cleanup() for this purpose
	// You also have to destroy the pipelines: since they need to be rebuilt, they have two different
	// methods: .cleanup() recreates them, while .destroy() delete them completely
	void localCleanup() {
		// Cleanup textures
		TCity.cleanup();

		// Cleanup models
		MCar.cleanup();

		// Cleanup descriptor set layouts
		DSLMesh.cleanup();
		DSLGubo.cleanup();

		// Destroys the pipelines
		PMesh.destroy();
	}

	// Here it is the creation of the command buffer:
	// You send to the GPU all the objects you want to draw,
	// with their buffers and textures

	void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage) {
		// sets global uniforms (see below fro parameters explanation)
		DSGubo.bind(commandBuffer, PMesh, 0, currentImage);

		// binds the pipeline
		PMesh.bind(commandBuffer);

		MCar.bind(commandBuffer);
		DSCar.bind(commandBuffer, PMesh, 1, currentImage);
		vkCmdDrawIndexed(commandBuffer,
			static_cast<uint32_t>(MCar.indices.size()), 1, 0, 0, 0);
	}

	// Here is where you update the uniforms.
	// Very likely this will be where you will be writing the logic of your application.
	void updateUniformBuffer(uint32_t currentImage) {
		static float debounce = false;
		static int curDebounce = 0;
		// Standard procedure to quit when the ESC key is pressed
		if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		// Integration with the timers and the controllers
		float deltaT;
		glm::vec3 m = glm::vec3(0.0f), r = glm::vec3(0.0f);
		bool fire = false;
		getSixAxis(deltaT, m, r, fire);
		static bool wasFire = false;
		bool handleFire = (wasFire && (!fire));
		wasFire = fire;
		/*
		if (glfwGetKey(window, GLFW_KEY_P)) {
			if (!debounce) {
				MoveCam = !MoveCam;
				camPos = lastCamPos;
				debounce = true;
				curDebounce = GLFW_KEY_P;
			}
		}
		else {
			if ((curDebounce == GLFW_KEY_P) && debounce) {
				debounce = false;
				curDebounce = 0;
			}
		}
		*/
		

		// Parameters
		// Camera FOV-y, Near Plane and Far Plane
		const float FOVy = glm::radians(45.0f);
		const float nearPlane = 0.1f;
		const float farPlane = 100.0f;
		const float rotSpeed = glm::radians(90.0f);
		const float movSpeed = 1.0f;

		glm::mat4 Prj = glm::perspective(FOVy, Ar, nearPlane, farPlane);
		Prj[1][1] *= -1;

		glm::mat4 View;
		glm::vec3 camTarget;

		//if (MoveCam) {
			CamYaw = CamYaw - rotSpeed * deltaT * r.y;
			CamPitch = CamPitch - rotSpeed * deltaT * r.x;
			CamPitch = CamPitch < glm::radians(-90.0f) ? glm::radians(-90.0f) :
				(CamPitch > glm::radians(90.0f) ? glm::radians(90.0f) : CamPitch);

			glm::mat3 CamDir = glm::rotate(glm::mat4(1.0f), CamYaw, glm::vec3(0, 1, 0)) *
				glm::rotate(glm::mat4(1.0f), CamPitch, glm::vec3(1, 0, 0));

			glm::vec3 ux = glm::rotate(glm::mat4(1.0f), CamYaw, glm::vec3(0, 1, 0)) * glm::vec4(1, 0, 0, 1);
			glm::vec3 uz = glm::rotate(glm::mat4(1.0f), CamYaw, glm::vec3(0, 1, 0)) * glm::vec4(0, 0, -1, 1);
			camPos = camPos + movSpeed * m.x * ux * deltaT;
			camPos = camPos + movSpeed * m.y * glm::vec3(0, 1, 0) * deltaT;
			camPos = camPos + movSpeed * m.z * uz * deltaT;

			View = glm::rotate(glm::mat4(1.0), -CamPitch, glm::vec3(1, 0, 0)) *
				glm::rotate(glm::mat4(1.0), -CamYaw, glm::vec3(0, 1, 0)) *
				glm::translate(glm::mat4(1.0), -camPos);
		//}
		/*
		else {
			CamH += m.z * movSpeed * deltaT;
			CamRadius -= m.x * movSpeed * deltaT;
			CamPitch -= r.x * rotSpeed * deltaT;
			CamYaw += r.y * rotSpeed * deltaT;

			camTarget = glm::vec3(0, CamH, 0);
			camPos = camTarget +
				CamRadius * glm::vec3(cos(CamPitch) * sin(CamYaw),
					sin(CamPitch),
					cos(CamPitch) * cos(CamYaw));
			View = glm::lookAt(camPos, camTarget, glm::vec3(0, 1, 0));

			lastCamPos = camPos;
		}
		*/

		gubo.DlightDir = glm::normalize(glm::vec3(1, 2, 3));
		gubo.DlightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		gubo.AmbLightColor = glm::vec3(0.1f);
		gubo.eyePos = camPos;

		// Writes value to the GPU
		DSGubo.map(currentImage, &gubo, sizeof(gubo), 0);
		// the .map() method of a DataSet object, requires the current image of the swap chain as first parameter
		// the second parameter is the pointer to the C++ data structure to transfer to the GPU
		// the third parameter is its size
		// the fourth parameter is the location inside the descriptor set of this uniform block

		glm::mat4 World = glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, 0.93f, -0.15f));
		uboCar.amb = 1.0f; uboCar.gamma = 180.0f; uboCar.sColor = glm::vec3(1.0f);
		uboCar.mvpMat = Prj * View * World;
		uboCar.mMat = World;
		uboCar.nMat = glm::inverse(glm::transpose(World));
		DSCar.map(currentImage, &uboCar, sizeof(uboCar), 0);
	}
};


// This is the main: probably you do not need to touch this!
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