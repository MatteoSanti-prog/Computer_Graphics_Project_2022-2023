#pragma once

struct MeshUniformBlock {
    alignas(16) glm::mat4 mvpMat;
    alignas(16) glm::mat4 mMat;
    alignas(16) glm::mat4 nMat;
};


struct GlobalUniformBlockNight {
    alignas(16) glm::vec3 lightPos;
    alignas(16) glm::vec3 lightDir;
    alignas(16) glm::vec3 lightColorBrakes;
    alignas(16) glm::vec3 eyePos;
};

struct GlobalUniformBlockDay {
    alignas(16) glm::vec3 lightDir;
    alignas(16) glm::vec3 lightColor;
    alignas(16) glm::vec3 eyePos;
};

struct OverlayUniformBlock {
    alignas(4) float visible;
};

struct VertexMesh {
    glm::vec3 pos;
    glm::vec3 norm;
    glm::vec2 UV;
};

struct VertexOverlay {
    glm::vec2 pos;
    glm::vec2 UV;
};
