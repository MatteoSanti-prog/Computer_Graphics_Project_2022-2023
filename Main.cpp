#include "Starter.hpp"
#include "Constants.hpp"
#include "Controller.hpp"
#include "Environment.hpp"
#include "Structs.hpp"
#include "Checkpoint.hpp"


class GoldenRoad : public BaseProject {
protected:

    DescriptorSetLayout DSLDay, DSLNight, DSLMesh, DSLOverlay;

    VertexDescriptor VMesh, VOverlay;

    Pipeline PMeshDay, PMeshNight, POverlay;

    Model<VertexMesh> MAsset[numberAssets];
    Model<VertexMesh> MCar, MEnv, MRoad, MCoin;
    Model<VertexOverlay> MSplash;
    Model<VertexOverlay> M1timer[rectanglesPerTimer], M2timer[rectanglesPerTimer];

    DescriptorSet DSDay, DSNight;
    DescriptorSet DSAsset[numberAssets];
    DescriptorSet DSCar, DSEnv, DSRoad, DSSplash, DSCoin;
    DescriptorSet DS1Timer[rectanglesPerTimer], DS2Timer[rectanglesPerTimer];

    Texture TCity, TSplash, TGrass, TMetalRoughness, TMarbleRoughness, TTimer;

    MeshUniformBlock uboMesh;
    OverlayUniformBlock uboOverlay;

    GlobalUniformBlockNight globalUniformBlockNight;
    GlobalUniformBlockDay globalUniformBlockDay;

    float Ar;

    /*Timer Variables*/
    float timer = 0;
    float w = 0.004; //segment wide
    float timer_pos_x[rectanglesPerTimer] = {-0.87f,-0.87f,-0.83f,-0.83f,-0.85f,-0.85f,-0.85f,-0.85f}; /**/
    float timer_pos_y[rectanglesPerTimer] = {-0.87f,-0.83f,-0.87f,-0.83f,-0.89f,-0.85f,-0.81f,-0.85f}; /**/
    float delta_x[rectanglesPerTimer] = {w,     w,     w,     w,     0.025f,0.025f,0.025f,w};
    float delta_y[rectanglesPerTimer] = {0.025f,0.025f,0.025f,0.025f,w,     w,     w,     0.045f};
    std::string x1 = "00000000";
    std::string x2 = "00000000";
    float digit_distance = 0.08f;

    int gameState;
    int brakeOnOff;

    void setWindowParameters() {

        windowWidth = 800;
        windowHeight = 600;
        windowTitle = "GoldenRoad";
        windowResizable = GLFW_TRUE;
        initialBackgroundColor = { 0.0f, 0.4f, 1.0f, 1.0f };

        uniformBlocksInPool = 35;
        texturesInPool = 49;
        setsInPool = 35;

        Ar = (float)windowWidth / (float)windowHeight;
    }

    void onWindowResize(int width, int height) {
        Ar = (float)width / (float)height;
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
        MAsset[0].init(this, &VMesh, "Models/apartment_001.mgcg", MGCG);
        MAsset[1].init(this, &VMesh, "Models/dwelling&store_001.mgcg", MGCG);
        MAsset[2].init(this, &VMesh, "Models/dwelling&store_008.mgcg", MGCG);
        MAsset[3].init(this, &VMesh, "Models/dwelling&store_002.mgcg", MGCG);
        MAsset[4].init(this, &VMesh, "Models/dwelling_012.mgcg", MGCG);
        MAsset[5].init(this, &VMesh, "Models/apartment_003.mgcg", MGCG);
        MAsset[6].init(this, &VMesh, "Models/apartment_002.mgcg", MGCG);
        MAsset[7].init(this, &VMesh, "Models/bank_001.mgcg", MGCG);
        MAsset[8].init(this, &VMesh, "Models/dwelling_012.mgcg", MGCG);
        MAsset[9].init(this, &VMesh, "Models/landscape_entertainments_006.mgcg", MGCG);
        MAsset[10].init(this, &VMesh, "Models/apartment_002.mgcg", MGCG);
        MAsset[11].init(this, &VMesh, "Models/dwelling_001.mgcg", MGCG);

        MRoad.init(this, &VMesh, "Models/road.obj", OBJ);

        MSplash.vertices = { {{-1.0f, -1.0f}, {0.001f, 0.001f}}, {{-1.0f, 1.0f}, {0.001f,0.999f}},
                             {{ 1.0f,-1.0f}, {0.999f,0.001f}}, {{ 1.0f, 1.0f}, {0.999f,0.999f}} };
        MSplash.indices = { 0, 1, 2,    1, 2, 3 };
        MSplash.initMesh(this, &VOverlay);

        for(int i = 0; i < rectanglesPerTimer; i++){
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
        }

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

        for (int i = 0; i < numberAssets; i ++) {
            DSAsset[i].init(this, &DSLMesh, {
                    {0, UNIFORM, sizeof(MeshUniformBlock), nullptr},
                    {1, TEXTURE, 0, &TCity},
                    {2, TEXTURE, 0, &TMarbleRoughness}
            });
        }

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

        for(int i = 0; i < rectanglesPerTimer; i++){
            DS1Timer[i].init(this, &DSLOverlay, {
                    {0, UNIFORM, sizeof(OverlayUniformBlock), nullptr},
                    {1, TEXTURE, 0, &TTimer}
            });
            DS2Timer[i].init(this, &DSLOverlay, {
                    {0, UNIFORM, sizeof(OverlayUniformBlock), nullptr},
                    {1, TEXTURE, 0, &TTimer}
            });
        }

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
        for (int i = 0; i < numberAssets; i++) {
            DSAsset[i].cleanup();
        }
        DSRoad.cleanup();
        DSCoin.cleanup();
        DSEnv.cleanup();
        DSSplash.cleanup();
        DSDay.cleanup();
        DSNight.cleanup();
        for(int i = 0; i < rectanglesPerTimer; i++) {
            DS1Timer[i].cleanup();
            DS2Timer[i].cleanup();
        }
    }

    void localCleanup() {

        TCity.cleanup();
        TTimer.cleanup();
        TSplash.cleanup();
        TGrass.cleanup();
        TMetalRoughness.cleanup();
        TMarbleRoughness.cleanup();

        MCar.cleanup();
        for (int i = 0; i < numberAssets; i++) {
            MAsset[i].cleanup();
        }
        MRoad.cleanup();
        MCoin.cleanup();
        MEnv.cleanup();
        MSplash.cleanup();
        for(int i = 0; i < rectanglesPerTimer; i++) {
            M1timer[i].cleanup();
            M2timer[i].cleanup();
        }

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
        for (int i = 0; i < numberAssets; i++) {
            MAsset[i].bind(commandBuffer);
            DSAsset[i].bind(commandBuffer, currPipeline, 1, currentImage);
            vkCmdDrawIndexed(commandBuffer,
                             static_cast<uint32_t>(MAsset[i].indices.size()), 1, 0, 0, 0);
        }

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

        for(int i = 0; i < rectanglesPerTimer; i++){
            M1timer[i].bind(commandBuffer);
            DS1Timer[i].bind(commandBuffer, POverlay, 0, currentImage);
            vkCmdDrawIndexed(commandBuffer,
                             static_cast<uint32_t>(M2timer[i].indices.size()), 1, 0, 0, 0);
            M2timer[i].bind(commandBuffer);
            DS2Timer[i].bind(commandBuffer, POverlay, 0, currentImage);
            vkCmdDrawIndexed(commandBuffer,
                             static_cast<uint32_t>(M2timer[i].indices.size()), 1, 0, 0, 0);

        }

        /*Depending on the state of the game we choose which pipeline to use*/
        if (gameState == FREE_CAMERA) {
            switchPipeline(commandBuffer, currentImage, PMeshDay, DSDay);
        }
        if (gameState == GAME) {
            switchPipeline(commandBuffer, currentImage, PMeshNight, DSNight);
        }
    }

    /*
    Associate a sequence of bits for each digit from 0 to 9.
    The value of each bit switch on or off the segments that compose the timer on screen.
    t is the value of the timer, v indicate which digit of the timer need to be translated
    */
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


        /*timer: delta time single frame*/
        auto delta = std::chrono::system_clock::now();
        //auto start = std::chrono::high_resolution_clock::now();


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
                timer = 0;
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
                /*Timer is incremented by a small delta equivalent to the duration of a frame*/
                timer += (std::chrono::duration_cast<std::chrono::duration<float>>(delta.time_since_epoch()).count() * std::pow(10, -11)) * 2;
                //auto end = std::chrono::high_resolution_clock::now();
                //timer += std::chrono::duration_cast<std::chrono::duration<float>>(end - start).count() * 1000;
                std::cout << "timer: " << timer << std::endl;

                gameState = gameLogic(deltaT, m, r, View, World, CarPos, CarYaw, CamPos, brakeOnOff);
                globalUniformBlockNight.lightPos = CarPos + glm::vec3(- sin(CarYaw) * 0.4f, 0.6f, - cos(CarYaw) * 0.4f);
                globalUniformBlockNight.lightDir = glm::vec3(sin(CarYaw), 0.3f, cos(CarYaw));
                globalUniformBlockNight.lightColorBrakes = brakeOnOff == 0 ? glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) :
                                                           (brakeOnOff == 1) ? glm::vec4(1.0f) : glm::vec4(0.0f);
                globalUniformBlockNight.eyePos = CamPos;
                DSNight.map(currentImage, &globalUniformBlockNight, sizeof(globalUniformBlockNight), 0);
                if (timer >= 100) {
                    resetGameLogic();
                    gameState = SCREEN;
                }
                break;
        }

        Prj = glm::perspective(fovy, Ar, nearPlane, farPlane);
        Prj[1][1] *= -1;

        if(gameState != SCREEN){

            /*Car Asset*/
            uboMesh.mvpMat = Prj * View * World;
            uboMesh.mMat = World;
            uboMesh.nMat = glm::inverse(glm::transpose(World));
            DSCar.map(currentImage, &uboMesh, sizeof(uboMesh), 0);

            /*Coin Asset*/
            CoinYaw -= coinRotSpeed * deltaT;
            World = glm::translate(glm::mat4(1.0), getcurrentCheckpointPos()) * glm::rotate(glm::mat4(1.0), CoinYaw, glm::vec3(0.0f, 1.0f, 0.0f)) *
                    glm::rotate(glm::mat4(1.0), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)) *
                    glm::scale(glm::mat4(1.0), glm::vec3(1.0f, 0.15f, 1.0f));
            uboMesh.mvpMat = Prj * View * World;
            uboMesh.mMat = World;
            uboMesh.nMat = glm::inverse(glm::transpose(World));
            DSCoin.map(currentImage, &uboMesh, sizeof(uboMesh), 0);

            /*Road Asset*/
            uboMesh.mvpMat = Prj * View * WorldRoad;
            uboMesh.mMat = WorldRoad;
            uboMesh.nMat = glm::inverse(glm::transpose(WorldRoad));
            DSRoad.map(currentImage, &uboMesh, sizeof(uboMesh), 0);

            /*Environment Asset*/
            uboMesh.mvpMat = Prj * View * WorldEnv;
            uboMesh.mMat = WorldEnv;
            uboMesh.nMat = glm::inverse(glm::transpose(WorldEnv));
            DSEnv.map(currentImage, &uboMesh, sizeof(uboMesh), 0);

            /*Buildings Assets*/
            for (int i = 0; i < numberAssets; i ++) {
                uboMesh.mvpMat = Prj * View * WorldAsset[i];
                uboMesh.mMat = WorldAsset[i];
                uboMesh.nMat = glm::inverse(glm::transpose(WorldAsset[i]));
                DSAsset[i].map(currentImage, &uboMesh, sizeof(uboMesh), 0);
            }
        }

        uboOverlay.visible = (gameState == SCREEN) ? 1.0f : 0.0f;
        DSSplash.map(currentImage, &uboOverlay, sizeof(uboOverlay), 0);

        /*
        Display timer
        The decimal part of timer is ignored
         */
        codeTime(&x1, timer, 1);
        codeTime(&x2, timer, 2);
        for(int i = 0; i < rectanglesPerTimer; i++) {
            uboOverlay.visible = (x1[i] == '1') ? 1 : 0;
            DS1Timer[i].map(currentImage, &uboOverlay, sizeof(uboOverlay), 0);

            uboOverlay.visible = (x2[i] == '1') ? 1 : 0;
            DS2Timer[i].map(currentImage, &uboOverlay, sizeof(uboOverlay), 0);
        }
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
