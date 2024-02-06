#include "Starter.hpp"
#include "Constants.hpp"
#include "Controller.hpp"
#include "Environment.hpp"
#include "Structs.hpp"
#include "Checkpoint.hpp"
#include <chrono> //timer


class GoldenRoad : public BaseProject {
protected:

    float Ar;

    //timer variables /**/
    float timer = 0;
    //timer /**/
    Model<VertexOverlay> M1timer[8];
    Model<VertexOverlay> M2timer[8];
    float w = 0.004; //segment wide
    float timer_pos_x[8] = {-0.87f,-0.87f,-0.83f,-0.83f,-0.85f,-0.85f,-0.85f,-0.85f}; /**/
    float timer_pos_y[8] = {-0.87f,-0.83f,-0.87f,-0.83f,-0.89f,-0.85f,-0.81f,-0.85f}; /**/
    float delta_x[8] = {w,     w,     w,     w,     0.025f,0.025f,0.025f,w};
    float delta_y[8] = {0.025f,0.025f,0.025f,0.025f,w,     w,     w,     0.045f};
    std::string x1 = "00000000";
    std::string x2 = "00000000";
    float digit_distance = 0.08f;
    DescriptorSet DS1Timer[8], DS2Timer[8]; /**/

    DescriptorSetLayout DSLDay, DSLNight, DSLMesh, DSLOverlay;

    VertexDescriptor VMesh, VOverlay;

    Pipeline PMeshDay, PMeshNight, POverlay;

    Model<VertexMesh> MCar, MApartment1, MApartment2, MApartment3, MBank1, MDwellingStore1, MDwellingStore2, MDwellingStore8,
            MDwelling1, MDwelling12, MEntertainment6, MEnv, MRoad, MCoin;
    Model<VertexOverlay> MSplash;

    DescriptorSet DSDay, DSNight, DSCar, DSApartment1, DSApartment2, DSApartment3, DSApartment4, DSBank1, DSDwellingStore1,
            DSDwellingStore2, DSDwellingStore8, DSDwelling1, DSDwelling12, DSDwelling13, DSEntertainment6, DSEnv, DSRoad, DSSplash,
            DSCoin;

    Texture TCity, TSplash, TGrass, TMetalRoughness, TMarbleRoughness, TTimer;

    MeshUniformBlock uboCar, uboApartment1, uboApartment2, uboApartment3, uboApartment4, uboBank1, uboDwellingStore1, uboDwellingStore2, uboDwellingStore8, uboDwelling1, uboDwelling12, uboDwelling13, uboEntertainment6, uboEnv, uboRoad, uboCoin;
    OverlayUniformBlock uboSplash;

    GlobalUniformBlockNight globalUniformBlockNight;
    GlobalUniformBlockDay globalUniformBlockDay;

    int gameState;
    int brakeOnOff;

    void setWindowParameters() {

        windowWidth = 800;
        windowHeight = 600;
        windowTitle = "GoldenRoad";
        windowResizable = GLFW_TRUE;
        initialBackgroundColor = { 0.0f, 0.4f, 1.0f, 1.0f };

        uniformBlocksInPool = 50; //19 17 19 /**/
        texturesInPool = 50;
        setsInPool = 50;

        Ar = (float)windowWidth / (float)windowHeight;
    }

    void onWindowResize(int w, int height) {
        Ar = (float)w / (float)height;
    }

    void localInit() {

        DSLMesh.init(this, {
                {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS},
                {1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT},
                {2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}
        });

        DSLOverlay.init(this, {
                {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS},
                {1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}
        });

        DSLDay.init(this, {
                {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS}
        });

        DSLNight.init(this, {
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

        VOverlay.init(this, {
                {0, sizeof(VertexOverlay), VK_VERTEX_INPUT_RATE_VERTEX}
        }, {
                              {0, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(VertexOverlay, pos),
                                      sizeof(glm::vec2), OTHER},
                              {0, 1, VK_FORMAT_R32G32_SFLOAT, offsetof(VertexOverlay, UV),
                                      sizeof(glm::vec2), UV}
                      });


        PMeshDay.init(this, &VMesh, "shaders/MeshVert.spv", "shaders/MeshFragDay.spv", {&DSLDay, &DSLMesh });
        PMeshNight.init(this, &VMesh, "shaders/MeshVert.spv", "shaders/MeshFragNight.spv", {&DSLNight, &DSLMesh });
        POverlay.init(this, &VOverlay, "shaders/OverlayVert.spv", "shaders/OverlayFrag.spv", { &DSLOverlay });
        POverlay.setAdvancedFeatures(VK_COMPARE_OP_LESS_OR_EQUAL, VK_POLYGON_MODE_FILL,
                                     VK_CULL_MODE_NONE, false);

        MCar.init(this, &VMesh, "Models/transport_cool_009_transport_cool_009.001.mgcg", MGCG);
        MApartment1.init(this, &VMesh, "Models/apartment_001.mgcg", MGCG);
        MApartment2.init(this, &VMesh, "Models/apartment_002.mgcg", MGCG);
        MApartment3.init(this, &VMesh, "Models/apartment_003.mgcg", MGCG);
        MBank1.init(this, &VMesh, "Models/bank_001.mgcg", MGCG);
        MDwellingStore1.init(this, &VMesh, "Models/dwelling&store_001.mgcg", MGCG);
        MDwellingStore2.init(this, &VMesh, "Models/dwelling&store_002.mgcg", MGCG);
        MDwellingStore8.init(this, &VMesh, "Models/dwelling&store_008.mgcg", MGCG);
        MDwelling1.init(this, &VMesh, "Models/dwelling_001.mgcg", MGCG);
        MDwelling12.init(this, &VMesh, "Models/dwelling_012.mgcg", MGCG);
        MEntertainment6.init(this, &VMesh, "Models/landscape_entertainments_006.mgcg", MGCG);
        MRoad.init(this, &VMesh, "Models/road.obj", OBJ);

        MSplash.vertices = { {{-1.0f, -1.0f}, {0.001f, 0.001f}}, {{-1.0f, 1.0f}, {0.001f,0.999f}},
                             {{ 1.0f,-1.0f}, {0.999f,0.001f}}, {{ 1.0f, 1.0f}, {0.999f,0.999f}} };
        MSplash.indices = { 0, 1, 2,    1, 2, 3 };
        MSplash.initMesh(this, &VOverlay);

        for(int i = 0; i < 8; i++){ /**/
            M1timer[i].vertices = {{{ timer_pos_x[i] - delta_x[i], timer_pos_y[i] - delta_y[i]}, {0.001f, 0.001f}},
                                   {{timer_pos_x[i]-delta_x[i], timer_pos_y[i]+delta_y[i]},      {0.001f, 0.999f}},
                                   {{timer_pos_x[i]+delta_x[i], timer_pos_y[i]-delta_y[i]},      {0.999f, 0.001f}},
                                   {{ timer_pos_x[i]+delta_x[i], timer_pos_y[i]+delta_y[i]},     {0.999f, 0.999f}} };
            M1timer[i].indices = {0, 1, 2, 1, 2, 3 };
            M1timer[i].initMesh(this, &VOverlay);

            M2timer[i].vertices = {{{ timer_pos_x[i] - delta_x[i]+digit_distance, timer_pos_y[i] - delta_y[i]}, {0.001f, 0.001f}},
                                   {{timer_pos_x[i]-delta_x[i]+digit_distance, timer_pos_y[i]+delta_y[i]},      {0.001f, 0.999f}},
                                   {{timer_pos_x[i]+delta_x[i]+digit_distance, timer_pos_y[i]-delta_y[i]},      {0.999f, 0.001f}},
                                   {{ timer_pos_x[i]+delta_x[i]+digit_distance, timer_pos_y[i]+delta_y[i]},     {0.999f, 0.999f}} };
            M2timer[i].indices = {0, 1, 2, 1, 2, 3 };
            M2timer[i].initMesh(this, &VOverlay);
        }/**/

        createEnvironment(MEnv.vertices, MEnv.indices);
        MEnv.initMesh(this, &VMesh);

        createCoin(MCoin.vertices, MCoin.indices);
        MCoin.initMesh(this, &VMesh);

        TCity.init(this, "textures/Textures_City.png");
        TSplash.init(this, "textures/initial_screen.png");
        TGrass.init(this, "textures/grass.png");
        TMetalRoughness.init(this, "textures/Metals_10_met_rough_ao.png", VK_FORMAT_R8G8B8A8_UNORM);
        TMarbleRoughness.init(this, "textures/Marble_08_met_rough_ao.png", VK_FORMAT_R8G8B8A8_UNORM);
        TTimer.init(this, "textures/red.png"); /**/

        gameState = SCREEN;

        initializeCheckpoints();
    }

    void pipelinesAndDescriptorSetsInit() {

        PMeshDay.create();
        PMeshNight.create();
        POverlay.create();

        DSCar.init(this, &DSLMesh, {
                {0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
                {1, TEXTURE, 0, &TCity},
                {2, TEXTURE, 0, &TMetalRoughness}
        });
        DSApartment1.init(this, &DSLMesh, {
                {0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
                {1, TEXTURE, 0, &TCity},
                {2, TEXTURE, 0, &TMarbleRoughness}
        });
        DSApartment2.init(this, &DSLMesh, {
                {0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
                {1, TEXTURE, 0, &TCity},
                {2, TEXTURE, 0, &TMarbleRoughness}
        });
        DSApartment3.init(this, &DSLMesh, {
                {0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
                {1, TEXTURE, 0, &TCity},
                {2, TEXTURE, 0, &TMarbleRoughness}
        });
        DSApartment4.init(this, &DSLMesh, {
                {0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
                {1, TEXTURE, 0, &TCity},
                {2, TEXTURE, 0, &TMarbleRoughness}
        });
        DSBank1.init(this, &DSLMesh, {
                {0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
                {1, TEXTURE, 0, &TCity},
                {2, TEXTURE, 0, &TMarbleRoughness}
        });
        DSDwellingStore1.init(this, &DSLMesh, {
                {0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
                {1, TEXTURE, 0, &TCity},
                {2, TEXTURE, 0, &TMarbleRoughness}
        });
        DSDwellingStore2.init(this, &DSLMesh, {
                {0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
                {1, TEXTURE, 0, &TCity},
                {2, TEXTURE, 0, &TMarbleRoughness}
        });
        DSDwellingStore8.init(this, &DSLMesh, {
                {0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
                {1, TEXTURE, 0, &TCity},
                {2, TEXTURE, 0, &TMarbleRoughness}
        });
        DSDwelling1.init(this, &DSLMesh, {
                {0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
                {1, TEXTURE, 0, &TCity},
                {2, TEXTURE, 0, &TMarbleRoughness}
        });
        DSDwelling12.init(this, &DSLMesh, {
                {0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
                {1, TEXTURE, 0, &TCity},
                {2, TEXTURE, 0, &TMarbleRoughness}
        });
        DSDwelling13.init(this, &DSLMesh, {
                {0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
                {1, TEXTURE, 0, &TCity},
                {2, TEXTURE, 0, &TMarbleRoughness}
        });
        DSEntertainment6.init(this, &DSLMesh, {
                {0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
                {1, TEXTURE, 0, &TCity},
                {2, TEXTURE, 0, &TMarbleRoughness}
        });

        DSEnv.init(this, &DSLMesh, {
                {0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
                {1, TEXTURE, 0, &TGrass},
                {2, TEXTURE, 0, &TGrass}
        });

        DSRoad.init(this, &DSLMesh, {
                {0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
                {1, TEXTURE, 0, &TCity},
                {2, TEXTURE, 0, &TMarbleRoughness}
        });

        DSCoin.init(this, &DSLMesh, {
                {0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
                {1, TEXTURE, 0, &TCity},
                {2, TEXTURE, 0, &TMetalRoughness}
        });

        DSSplash.init(this, &DSLOverlay, {
                {0, UNIFORM, sizeof(OverlayUniformBlock), nullptr},
                {1, TEXTURE, 0, &TSplash}
        });
        /**/
        for(int i = 0; i < 8; i++){
            DS1Timer[i].init(this, &DSLOverlay, {
                    {0, UNIFORM, sizeof(OverlayUniformBlock), nullptr},
                    {1, TEXTURE, 0, &TTimer}
            });
            DS2Timer[i].init(this, &DSLOverlay, {
                    {0, UNIFORM, sizeof(OverlayUniformBlock), nullptr},
                    {1, TEXTURE, 0, &TTimer}
            });
        }
        /**/

        DSDay.init(this, &DSLDay, {
                {0, UNIFORM, sizeof(GlobalUniformBlockDay), nullptr}
        });

        DSNight.init(this, &DSLNight, {
                {0, UNIFORM, sizeof(GlobalUniformBlockNight), nullptr}
        });
    }


    void pipelinesAndDescriptorSetsCleanup() {

        PMeshDay.cleanup();
        PMeshNight.cleanup();
        POverlay.cleanup();

        DSCar.cleanup();
        DSApartment1.cleanup();
        DSApartment2.cleanup();
        DSApartment3.cleanup();
        DSApartment4.cleanup();
        DSBank1.cleanup();
        DSDwellingStore1.cleanup();
        DSDwellingStore2.cleanup();
        DSDwellingStore8.cleanup();
        DSDwelling1.cleanup();
        DSDwelling12.cleanup();
        DSDwelling13.cleanup();
        DSEntertainment6.cleanup();
        DSRoad.cleanup();
        DSCoin.cleanup();
        DSEnv.cleanup();
        DSSplash.cleanup();
        DSDay.cleanup();
        DSNight.cleanup();
    }

    void localCleanup() {

        TCity.cleanup();
        TTimer.cleanup(); /**/
        TSplash.cleanup();
        TGrass.cleanup();
        TMetalRoughness.cleanup();
        TMarbleRoughness.cleanup();

        MCar.cleanup();
        MApartment1.cleanup();
        MApartment2.cleanup();
        MApartment3.cleanup();
        MBank1.cleanup();
        MDwellingStore1.cleanup();
        MDwellingStore2.cleanup();
        MDwellingStore8.cleanup();
        MDwelling1.cleanup();
        MDwelling12.cleanup();
        MEntertainment6.cleanup();
        MRoad.cleanup();
        MCoin.cleanup();
        MEnv.cleanup();
        MSplash.cleanup();
        for(int i = 0; i < 8; i++) { /**/
            M1timer[i].cleanup();
            M2timer[i].cleanup();
        }/**/

        DSLMesh.cleanup();
        DSLOverlay.cleanup();
        DSLDay.cleanup();
        DSLNight.cleanup();

        PMeshDay.destroy();
        PMeshNight.destroy();
        POverlay.destroy();
    }

    void switchPipeline(VkCommandBuffer commandBuffer, int currentImage, Pipeline currPipeline, DescriptorSet currDs) {

        currDs.bind(commandBuffer, currPipeline, 0, currentImage);

        currPipeline.bind(commandBuffer);

        MCar.bind(commandBuffer);
        DSCar.bind(commandBuffer, currPipeline, 1, currentImage);
        vkCmdDrawIndexed(commandBuffer,
                         static_cast<uint32_t>(MCar.indices.size()), 1, 0, 0, 0);

        MApartment1.bind(commandBuffer);
        DSApartment1.bind(commandBuffer, currPipeline, 1, currentImage);
        vkCmdDrawIndexed(commandBuffer,
                         static_cast<uint32_t>(MApartment1.indices.size()), 1, 0, 0, 0);

        MApartment2.bind(commandBuffer);
        DSApartment2.bind(commandBuffer, currPipeline, 1, currentImage);
        vkCmdDrawIndexed(commandBuffer,
                         static_cast<uint32_t>(MApartment2.indices.size()), 1, 0, 0, 0);
        DSApartment4.bind(commandBuffer, currPipeline, 1, currentImage);
        vkCmdDrawIndexed(commandBuffer,
                         static_cast<uint32_t>(MApartment2.indices.size()), 1, 0, 0, 0);

        MApartment3.bind(commandBuffer);
        DSApartment3.bind(commandBuffer, currPipeline, 1, currentImage);
        vkCmdDrawIndexed(commandBuffer,
                         static_cast<uint32_t>(MApartment3.indices.size()), 1, 0, 0, 0);


        MBank1.bind(commandBuffer);
        DSBank1.bind(commandBuffer, currPipeline, 1, currentImage);
        vkCmdDrawIndexed(commandBuffer,
                         static_cast<uint32_t>(MBank1.indices.size()), 1, 0, 0, 0);

        MDwellingStore1.bind(commandBuffer);
        DSDwellingStore1.bind(commandBuffer, currPipeline, 1, currentImage);
        vkCmdDrawIndexed(commandBuffer,
                         static_cast<uint32_t>(MDwellingStore1.indices.size()), 1, 0, 0, 0);

        MDwellingStore2.bind(commandBuffer);
        DSDwellingStore2.bind(commandBuffer, currPipeline, 1, currentImage);
        vkCmdDrawIndexed(commandBuffer,
                         static_cast<uint32_t>(MDwellingStore2.indices.size()), 1, 0, 0, 0);

        MDwellingStore8.bind(commandBuffer);
        DSDwellingStore8.bind(commandBuffer, currPipeline, 1, currentImage);
        vkCmdDrawIndexed(commandBuffer,
                         static_cast<uint32_t>(MDwellingStore8.indices.size()), 1, 0, 0, 0);

        MDwelling1.bind(commandBuffer);
        DSDwelling1.bind(commandBuffer, currPipeline, 1, currentImage);
        vkCmdDrawIndexed(commandBuffer,
                         static_cast<uint32_t>(MDwelling1.indices.size()), 1, 0, 0, 0);

        MDwelling12.bind(commandBuffer);
        DSDwelling12.bind(commandBuffer, currPipeline, 1, currentImage);
        vkCmdDrawIndexed(commandBuffer,
                         static_cast<uint32_t>(MDwelling12.indices.size()), 1, 0, 0, 0);
        DSDwelling13.bind(commandBuffer, currPipeline, 1, currentImage);
        vkCmdDrawIndexed(commandBuffer,
                         static_cast<uint32_t>(MDwelling12.indices.size()), 1, 0, 0, 0);

        MEntertainment6.bind(commandBuffer);
        DSEntertainment6.bind(commandBuffer, currPipeline, 1, currentImage);
        vkCmdDrawIndexed(commandBuffer,
                         static_cast<uint32_t>(MEntertainment6.indices.size()), 1, 0, 0, 0);

        MRoad.bind(commandBuffer);
        DSRoad.bind(commandBuffer, currPipeline, 1, currentImage);
        vkCmdDrawIndexed(commandBuffer,
                         static_cast<uint32_t>(MRoad.indices.size()), 1, 0, 0, 0);

        MCoin.bind(commandBuffer);
        DSCoin.bind(commandBuffer, currPipeline, 1, currentImage);
        vkCmdDrawIndexed(commandBuffer,
                         static_cast<uint32_t>(MCoin.indices.size()), 1, 0, 0, 0);

        MEnv.bind(commandBuffer);
        DSEnv.bind(commandBuffer, currPipeline, 1, currentImage);
        vkCmdDrawIndexed(commandBuffer,
                         static_cast<uint32_t>(MEnv.indices.size()), 1, 0, 0, 0);
    }

    void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage) {

        POverlay.bind(commandBuffer);
        MSplash.bind(commandBuffer);
        DSSplash.bind(commandBuffer, POverlay, 0, currentImage);
        vkCmdDrawIndexed(commandBuffer,
                         static_cast<uint32_t>(MSplash.indices.size()), 1, 0, 0, 0);

        for(int i = 0; i < 8; i++){ /**/
            M1timer[i].bind(commandBuffer);
            DS1Timer[i].bind(commandBuffer, POverlay, 0, currentImage);
            vkCmdDrawIndexed(commandBuffer,
                             static_cast<uint32_t>(M2timer[i].indices.size()), 1, 0, 0, 0);
            M2timer[i].bind(commandBuffer);
            DS2Timer[i].bind(commandBuffer, POverlay, 0, currentImage);
            vkCmdDrawIndexed(commandBuffer,
                             static_cast<uint32_t>(M2timer[i].indices.size()), 1, 0, 0, 0);

        }/**/

        /*Depending on the state of the game we choose which pipeline to use*/
        if (gameState == FREE_CAMERA) {
            switchPipeline(commandBuffer, currentImage, PMeshDay, DSDay);
        }
        if (gameState == GAME) {
            switchPipeline(commandBuffer, currentImage, PMeshNight, DSNight);
        }
    }

    //associate a sequence of bits for each digit from 0 to 9
    //the value of each bit switch on or off the segments that compose the timer on screen
    //t is the value of the timer, v indicate which digit of the timer need to be translated
    void codeTime(std::string *str, int t, int v){
        std::string code;
        int x = 0;
        switch(v){
            case 2:
                x = t % 10;
                break;
            case 1:
                x = ((t % 100) - (t % 10))/10; //take the second digit
                break;
            default:
                x = 0;
        }

        switch(x) {
            case 0:
                *str = "11111010";
                return;
            case 1:
                *str = "00000001";
                return;
            case 2:
                *str = "01101110";
                return;
            case 3:
                *str = "00111110";
                return;
            case 4:
                *str = "10110100";
                return;
            case 5:
                *str = "10011110";
                return;
            case 6:
                *str = "11011110";
                return;
            case 7:
                *str = "00111000";
                return;
            case 8:
                *str = "11111110";
                return;
            case 9:
                *str = "10111110";
                return;
        }
    }

    void updateUniformBuffer(uint32_t currentImage) {
        bool fire = false;
        float deltaT;
        glm::vec3 m = glm::vec3(0.0f), r = glm::vec3(0.0f);
        glm::vec3 CarPos, CamPos;
        float CarYaw;
        static float CoinYaw = glm::radians(0.0f);
        glm::mat4 World, View, Prj;
        // timer: delta time single frame
        auto delta = std::chrono::system_clock::now();


        if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        getSixAxis(deltaT, m, r, fire);

        static bool wasFire = false;
        bool handleFire = (wasFire && (!fire));
        wasFire = fire;

        switch (gameState) {
            case SCREEN:
                if (handleFire) {
                    gameState = FREE_CAMERA;
                    initialBackgroundColor = { 0.0f, 0.4f, 1.0f, 1.0f };
                    RebuildPipeline();
                }
                break;
            case FREE_CAMERA:
                timer = 0; /**/
                freeCam(deltaT, m, r, View, World, CarPos, CarYaw, CamPos);
                globalUniformBlockDay.lightDir = glm::normalize(glm::vec3(3, 5, 6));
                globalUniformBlockDay.lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                globalUniformBlockDay.eyePos = CamPos;
                DSDay.map(currentImage, &globalUniformBlockDay, sizeof(globalUniformBlockDay), 0);
                if (handleFire) {
                    gameState = GAME;
                    initialBackgroundColor = { 0.0f, 0.005f, 0.01f, 1.0f };
                    RebuildPipeline();
                }
                break;
            case GAME:
                /**/
                //update timer
                timer += std::chrono::duration_cast<std::chrono::duration<float>>(delta.time_since_epoch()).count() *std::pow(10, -11);
                //std::cout << "timer: " << timer << std::endl;
                /**/

                gameState = gameLogic(deltaT, m, r, View, World, CarPos, CarYaw, CamPos, brakeOnOff);

                globalUniformBlockNight.lightPos = CarPos + glm::vec3(- sin(CarYaw) * 0.4f, 0.6f, - cos(CarYaw) * 0.4f);
                globalUniformBlockNight.lightDir = glm::vec3(sin(CarYaw), 0.3f, cos(CarYaw));
                globalUniformBlockNight.lightColorBrakes = brakeOnOff == 0 ? glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) :
                                                           (brakeOnOff == 1) ? glm::vec4(1.0f) : glm::vec4(0.0f);
                globalUniformBlockNight.eyePos = CamPos;
                DSNight.map(currentImage, &globalUniformBlockNight, sizeof(globalUniformBlockNight), 0);
                break;
        }

        Prj = glm::perspective(fovy, Ar, nearPlane, farPlane);
        Prj[1][1] *= -1;

        if(gameState != SCREEN){

            uboCar.mvpMat = Prj * View * World;
            uboCar.mMat = World;
            uboCar.nMat = glm::inverse(glm::transpose(World));
            DSCar.map(currentImage, &uboCar, sizeof(uboCar), 0);

            /*Assets in the GFHE section*/
            World = glm::translate(glm::mat4(1.0), glm::vec3(24.0f, 0.0f, 0.0f)) * glm::rotate(glm::mat4(1.0), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            uboApartment1.mvpMat = Prj * View * World;
            uboApartment1.mMat = World;
            uboApartment1.nMat = glm::inverse(glm::transpose(World));
            DSApartment1.map(currentImage, &uboApartment1, sizeof(uboApartment1), 0);

            World = glm::translate(glm::mat4(1.0), glm::vec3(24.0f, 0.0f, 6.0f)) * glm::rotate(glm::mat4(1.0), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            uboDwellingStore1.mvpMat = Prj * View * World;
            uboDwellingStore1.mMat = World;
            uboDwellingStore1.nMat = glm::inverse(glm::transpose(World));
            DSDwellingStore1.map(currentImage, &uboDwellingStore1, sizeof(uboDwellingStore1), 0);

            World = glm::translate(glm::mat4(1.0), glm::vec3(24.0f, 0.0f, 11.0f)) * glm::rotate(glm::mat4(1.0), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            uboDwellingStore8.mvpMat = Prj * View * World;
            uboDwellingStore8.mMat = World;
            uboDwellingStore8.nMat = glm::inverse(glm::transpose(World));
            DSDwellingStore8.map(currentImage, &uboDwellingStore8, sizeof(uboDwellingStore8), 0);

            World = glm::translate(glm::mat4(1.0), glm::vec3(24.0f, 0.0f, 25.5f)) * glm::rotate(glm::mat4(1.0), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            uboDwellingStore2.mvpMat = Prj * View * World;
            uboDwellingStore2.mMat = World;
            uboDwellingStore2.nMat = glm::inverse(glm::transpose(World));
            DSDwellingStore2.map(currentImage, &uboDwellingStore2, sizeof(uboDwellingStore2), 0);

            World = glm::translate(glm::mat4(1.0), glm::vec3(24.0f, 0.0f, 18.0f)) * glm::rotate(glm::mat4(1.0), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            uboDwelling12.mvpMat = Prj * View * World;
            uboDwelling12.mMat = World;
            uboDwelling12.nMat = glm::inverse(glm::transpose(World));
            DSDwelling12.map(currentImage, &uboDwelling12, sizeof(uboDwelling12), 0);

            /*Assets in the RJQG section*/
            World = glm::translate(glm::mat4(1.0), glm::vec3(12.5f, 0.0f, -4.0f)) * glm::rotate(glm::mat4(1.0), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            uboApartment3.mvpMat = Prj * View * World;
            uboApartment3.mMat = World;
            uboApartment3.nMat = glm::inverse(glm::transpose(World));
            DSApartment3.map(currentImage, &uboApartment3, sizeof(uboApartment3), 0);

            World = glm::translate(glm::mat4(1.0), glm::vec3(5.0f, 0.0f, -4.0f)) * glm::rotate(glm::mat4(1.0), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            uboApartment2.mvpMat = Prj * View * World;
            uboApartment2.mMat = World;
            uboApartment2.nMat = glm::inverse(glm::transpose(World));
            DSApartment2.map(currentImage, &uboApartment2, sizeof(uboApartment2), 0);

            World = glm::translate(glm::mat4(1.0), glm::vec3(-2.0f, 0.0f, -4.0f)) * glm::rotate(glm::mat4(1.0), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            uboBank1.mvpMat = Prj * View * World;
            uboBank1.mMat = World;
            uboBank1.nMat = glm::inverse(glm::transpose(World));
            DSBank1.map(currentImage, &uboBank1, sizeof(uboBank1), 0);

            World = glm::translate(glm::mat4(1.0), glm::vec3(-11.0f, 0.0f, -4.0f)) * glm::rotate(glm::mat4(1.0), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            uboDwelling13.mvpMat = Prj * View * World;
            uboDwelling13.mMat = World;
            uboDwelling13.nMat = glm::inverse(glm::transpose(World));
            DSDwelling13.map(currentImage, &uboDwelling13, sizeof(uboDwelling13), 0);

            /*Assets in the OPKN section*/
            World = glm::translate(glm::mat4(1.0), glm::vec3(-26.0f, 0.0f, -19.0f)) * glm::rotate(glm::mat4(1.0), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            uboEntertainment6.mvpMat = Prj * View * World;
            uboEntertainment6.mMat = World;
            uboEntertainment6.nMat = glm::inverse(glm::transpose(World));
            DSEntertainment6.map(currentImage, &uboEntertainment6, sizeof(uboEntertainment6), 0);

            World = glm::translate(glm::mat4(1.0), glm::vec3(-26.0f, 0.0f, -7.0f));
            uboApartment4.mvpMat = Prj * View * World;
            uboApartment4.mMat = World;
            uboApartment4.nMat = glm::inverse(glm::transpose(World));
            DSApartment4.map(currentImage, &uboApartment4, sizeof(uboApartment4), 0);

            World = glm::translate(glm::mat4(1.0), glm::vec3(-26.0f, 0.0f, 4.0f)) * glm::rotate(glm::mat4(1.0), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            uboDwelling1.mvpMat = Prj * View * World;
            uboDwelling1.mMat = World;
            uboDwelling1.nMat = glm::inverse(glm::transpose(World));
            DSDwelling1.map(currentImage, &uboDwelling1, sizeof(uboDwelling1), 0);

            /*Road Asset*/
            World = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, -1.2f, 0.0f)) * glm::scale(glm::mat4(1.0), glm::vec3(scalingFactor));
            uboRoad.mvpMat = Prj * View * World;
            uboRoad.mMat = World;
            uboRoad.nMat = glm::inverse(glm::transpose(World));
            DSRoad.map(currentImage, &uboRoad, sizeof(uboRoad), 0);

            /*Coin Asset*/
            CoinYaw -= coinRotSpeed * deltaT;
            World = glm::translate(glm::mat4(1.0), getcurrentCheckpointPos()) * glm::rotate(glm::mat4(1.0), CoinYaw, glm::vec3(0.0f, 1.0f, 0.0f)) *
                    glm::rotate(glm::mat4(1.0), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)) *
                    glm::scale(glm::mat4(1.0), glm::vec3(1.0f, 0.15f, 1.0f));
            uboCoin.mvpMat = Prj * View * World;
            uboCoin.mMat = World;
            uboCoin.nMat = glm::inverse(glm::transpose(World));
            DSCoin.map(currentImage, &uboCoin, sizeof(uboCoin), 0);

            /*Environment Asset*/
            World = glm::rotate(glm::mat4(1.0), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) *
                    glm::scale(glm::mat4(1.0), glm::vec3(scalingFactor));
            uboEnv.mvpMat = Prj * View * World;
            uboEnv.mMat = World;
            uboEnv.nMat = glm::inverse(glm::transpose(World));
            DSEnv.map(currentImage, &uboEnv, sizeof(uboEnv), 0);
        }

        uboSplash.visible = (gameState == SCREEN) ? 1.0f : 0.0f;
        DSSplash.map(currentImage, &uboSplash, sizeof(uboSplash), 0);

        /**/
        //display timer
        //the decimal part of timer is ignored
        codeTime(&x1, timer, 1);
        codeTime(&x2, timer, 2);
        for(int i = 0; i < 8; i++) {
            uboSplash.visible = (x1[i] == '1') ? 1 : 0;
            DS1Timer[i].map(currentImage, &uboSplash, sizeof(uboSplash), 0);

            uboSplash.visible = (x2[i] == '1') ? 1 : 0;
            DS2Timer[i].map(currentImage, &uboSplash, sizeof(uboSplash), 0);
        }
        /**/
    }
};



int main() {
    GoldenRoad app;

    try {
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
