#pragma once

struct MeshUniformBlock {
    alignas(4) float amb;
    alignas(4) float gamma;
    alignas(16) glm::vec3 sColor;
    alignas(16) glm::mat4 mvpMat;
    alignas(16) glm::mat4 mMat;
    alignas(16) glm::mat4 nMat;
};

struct GlobalUniformBlockFree {
    alignas(16) glm::vec3 FreeDlightDir;
    alignas(16) glm::vec3 FreeDlightColor;
    alignas(16) glm::vec3 FreeAmbLightColor;
    alignas(16) glm::vec3 FreeEyePos;
};

struct GlobalUniformBlockGL {
    alignas(16) glm::vec3 GLDlightPos;
    alignas(16) glm::vec3 GLDlightDir;
    alignas(16) glm::vec3 GLDlightColor;
    alignas(16) glm::vec3 GLAmbLightColor;
    alignas(16) glm::vec3 GLeyePos;
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
