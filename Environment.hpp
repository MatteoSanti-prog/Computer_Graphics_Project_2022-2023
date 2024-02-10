#pragma once

#include "Structs.hpp"


void createEnvironment(std::vector<VertexMesh> &vPos, std::vector<uint32_t> &vIdx) {
    /*Number of created squares*/
    int n = 0;
    /*Number of vertices used at each iteration*/
    int v = 4;
    /*Dimension of a single square*/
	int sizeSquare = 6;
    /*Number of rows*/
    int numberRows = 30 / sizeSquare;
    //Vertices
    for (int i = 0; i < numberRows; i++) {
        for (int j = 0; j < numberRows; j++) {
            // Base
            vPos.push_back({{-15 + j * sizeSquare, 15 - i * sizeSquare, 0}, {0, 0, 1}, {0.0f, 0.0f} }); //A
            vPos.push_back({ {-15 + (1 + j) * sizeSquare, 15 - i * sizeSquare, 0}, {0,0,1}, {1.0f,0.0f} }); //B
            vPos.push_back({ {-15 + j * sizeSquare, 15 - (1 + i) * sizeSquare, 0}, {0,0,1}, {0.0f,1.0f} }); //C
            vPos.push_back({ {-15 + (1 + j) * sizeSquare, 15 - (1 + i) * sizeSquare, 0}, {0,0,1}, {1.0f,1.0f} }); //D
            n++;
        }
    }
    //Indices
    for (int i = 0; i < n; i++) {
        vIdx.push_back(0 + v * i); vIdx.push_back(2 + v * i); vIdx.push_back(1 + v * i);
        vIdx.push_back(1 + v * i); vIdx.push_back(2 + v * i); vIdx.push_back(3 + v * i);
    }
}




void createCoin(std::vector<VertexMesh> &vPos, std::vector<uint32_t> &vIdx) {
    /*Temp variables*/
    int i, total0, total1, total2, count = 0;
    /*Radius and half height of the circles*/
    float radiusExternal = 1.0f, radiusInternal = 0.7f, halfHOuter = 1.0f, halfHInner = 0.5f;
    /*Normal, UV and center points for each section*/
    glm::vec2 UVCoordinates = glm::vec2({0.9f, 0.4f});
    glm::vec3 normalExternalUp = glm::vec3({0.0f, 1.0f, 0.0f });
    glm::vec3 normalExternalBottom = glm::vec3({0.0f, -1.0f, 0.0f });
    glm::vec3 normalInternalUp = glm::vec3({0.0f, 1.0f, 0.0f });
    glm::vec3 normalInternalBottom = glm::vec3({0.0f, -1.0f, 0.0f });
    glm::vec3 centerExternalUp = glm::vec3(0.0f, halfHOuter, 0.0f);
    glm::vec3 centerExternalBottom = glm::vec3(0.0f, -halfHOuter, 0.0f);
    glm::vec3 centerInternalUp = glm::vec3(0.0f, halfHInner, 0.0f);
    glm::vec3 centerInternalBottom = glm::vec3(0.0f, -halfHInner, 0.0f);
    vPos.push_back({centerInternalUp, normalInternalUp, UVCoordinates});
    count += 1;
    vPos.push_back({centerInternalBottom, normalInternalBottom, UVCoordinates});
    count += 1;
    /*The loop will create circles using iterating over points which are 12 degree of distance*/
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0), glm::radians(12.0f), glm::vec3(0, 1, 0));
    /*Variables to store points*/
    glm::vec3 pointExternalUp = glm::vec3(radiusExternal, halfHOuter, 0.0f);
    glm::vec3 pointExternalBottom = glm::vec3(radiusExternal, -halfHOuter, 0.0f);
    glm::vec3 pointInternalUp = glm::vec3(radiusInternal, halfHOuter, 0.0f);
    glm::vec3 pointInternalBottom = glm::vec3(radiusInternal, -halfHOuter, 0.0f);
    vPos.push_back({pointExternalUp, normalExternalUp, UVCoordinates });
    count += 1;
    vPos.push_back({pointExternalBottom, normalExternalBottom, UVCoordinates });
    count += 1;
    vPos.push_back({pointInternalUp, normalInternalUp, UVCoordinates });
    count += 1;
    vPos.push_back({pointInternalBottom, normalInternalBottom, UVCoordinates });
    count += 1;
    /*Here we create the vertices of the up and bottom annulus*/
    for (i = 0; i < 30; i++) {
        pointExternalUp = glm::vec3(rotation * glm::vec4(pointExternalUp, 1.0f));
        vPos.push_back({pointExternalUp, normalExternalUp, UVCoordinates });
        count += 1;
        pointExternalBottom = glm::vec3(rotation * glm::vec4(pointExternalBottom, 1.0f));
        vPos.push_back({pointExternalBottom, normalExternalBottom, UVCoordinates });
        count += 1;
        pointInternalUp = glm::vec3(rotation * glm::vec4(pointInternalUp, 1.0f));
        vPos.push_back({pointInternalUp, normalInternalUp, UVCoordinates });
        count += 1;
        pointInternalBottom = glm::vec3(rotation * glm::vec4(pointInternalBottom, 1.0f));
        vPos.push_back({pointInternalBottom, normalInternalBottom, UVCoordinates });
        count += 1;
    }
    total0 = count;
    pointInternalUp = glm::vec3(radiusInternal, halfHInner, 0.0f);
    pointInternalBottom = glm::vec3(radiusInternal, -halfHInner, 0.0f);
    vPos.push_back({pointInternalUp, normalInternalUp, UVCoordinates });
    count += 1;
    vPos.push_back({pointInternalBottom, normalInternalBottom, UVCoordinates });
    count += 1;
    /*Here we create the vertices for the inner concave circle*/
    for (i = 0; i < 30; i++) {
        pointInternalUp = glm::vec3(rotation * glm::vec4(pointInternalUp, 1.0f));
        vPos.push_back({pointInternalUp, normalInternalUp, UVCoordinates });
        count += 1;
        pointInternalBottom = glm::vec3(rotation * glm::vec4(pointInternalBottom, 1.0f));
        vPos.push_back({pointInternalBottom, normalInternalBottom, UVCoordinates });
        count += 1;
    }
    total1 = count;


    /*From here we create the lateral surfaces*/
    pointExternalUp = glm::vec3(radiusExternal, halfHOuter, 0.0f);
    pointExternalBottom = glm::vec3(radiusExternal, -halfHOuter, 0.0f);
    /*The normal of the lateral surface will be computed as glm::normalize(P - C)*/
    normalExternalUp = glm::normalize(pointExternalUp - centerExternalUp);
    normalExternalBottom = glm::normalize(pointExternalBottom - centerExternalBottom);
    vPos.push_back({pointExternalUp, normalExternalUp, UVCoordinates });
    count += 1;
    vPos.push_back({pointExternalBottom, normalExternalBottom, UVCoordinates });
    count += 1;
    pointInternalUp = glm::vec3(radiusInternal, halfHOuter, 0.0f);
    pointInternalBottom = glm::vec3(radiusInternal, -halfHOuter, 0.0f);
    normalInternalUp = glm::normalize(pointInternalUp - centerInternalUp);
    normalInternalBottom = glm::normalize(pointInternalBottom - centerInternalBottom);
    vPos.push_back({pointInternalUp, normalInternalUp, UVCoordinates });
    count += 1;
    vPos.push_back({pointInternalBottom, normalInternalBottom, UVCoordinates });
    count += 1;
    /*Here we create the vertices of the lateral surface*/
    for (i = 0; i < 30; i++) {
        pointExternalUp = glm::vec3(rotation * glm::vec4(pointExternalUp, 1.0f));
        normalExternalUp = glm::normalize(pointExternalUp - centerExternalUp);
        vPos.push_back({pointExternalUp, normalExternalUp, UVCoordinates });
        count += 1;
        pointExternalBottom = glm::vec3(rotation * glm::vec4(pointExternalBottom, 1.0f));
        normalExternalBottom = glm::normalize(pointExternalBottom - centerExternalBottom);
        vPos.push_back({pointExternalBottom, normalExternalBottom, UVCoordinates });
        count += 1;
        pointInternalUp = glm::vec3(rotation * glm::vec4(pointInternalUp, 1.0f));
        normalInternalUp = glm::normalize(pointInternalUp - centerInternalUp);
        vPos.push_back({pointInternalUp, normalInternalUp, UVCoordinates });
        count += 1;
        pointInternalBottom = glm::vec3(rotation * glm::vec4(pointInternalBottom, 1.0f));
        normalInternalBottom = glm::normalize(pointInternalBottom - centerInternalBottom);
        vPos.push_back({pointInternalBottom, normalInternalBottom, UVCoordinates });
        count += 1;
    }
    total2 = count;
    pointInternalUp = glm::vec3(radiusInternal, halfHInner, 0.0f);
    pointInternalBottom = glm::vec3(radiusInternal, -halfHInner, 0.0f);
    normalInternalUp = glm::normalize(pointInternalUp - centerInternalUp);
    normalInternalBottom = glm::normalize(pointInternalBottom - centerInternalBottom);
    vPos.push_back({pointInternalUp, normalInternalUp, UVCoordinates });
    vPos.push_back({pointInternalBottom, normalInternalBottom, UVCoordinates });
    /*Here we create the surface between the inner circle and the outer circle*/
    for (i = 0; i < 30; i++) {
        pointInternalUp = glm::vec3(rotation * glm::vec4(pointInternalUp, 1.0f));
        normalInternalUp = glm::normalize(pointInternalUp - centerInternalUp);
        vPos.push_back({pointInternalUp, normalInternalUp, UVCoordinates });
        pointInternalBottom = glm::vec3(rotation * glm::vec4(pointInternalBottom, 1.0f));
        normalInternalBottom = glm::normalize(pointInternalBottom - centerInternalBottom);
        vPos.push_back({pointInternalBottom, normalInternalBottom, UVCoordinates });
    }


    /*Here we create the triangles which implement the cylinder*/
    for (i = 0; i < 30; i++) {
        vIdx.push_back(2 + 4 * i); vIdx.push_back(2 + 4 * (1 + i)); vIdx.push_back(4 * (1 + i));
        vIdx.push_back(3 + 4 * i); vIdx.push_back(1 + 4 * (1 + i)); vIdx.push_back(3 + 4 * (1 + i));
        vIdx.push_back(4 * (1 + i)); vIdx.push_back(2 + 4 * (1 + i)); vIdx.push_back(4 * (2 + i));
        vIdx.push_back(3 + 4 * (1 + i)); vIdx.push_back(1 + 4 * (1 + i)); vIdx.push_back(1 + 4 * (2 + i));
        vIdx.push_back(total0 + 2 * i); vIdx.push_back(total0 + 2 * (1 + i)); vIdx.push_back(0);
        vIdx.push_back(total0 + 1 + 2 * (1 + i)); vIdx.push_back(total0 + 1 + 2 * i); vIdx.push_back(1);
        vIdx.push_back(total1 + 4 * i); vIdx.push_back(total1 + 1 + 4 * i); vIdx.push_back(total1 + 4 * (1 + i));
        vIdx.push_back(total1 + 4 * (1 + i)); vIdx.push_back(total1 + 1 + 4 * i); vIdx.push_back(total1 + 1 + 4 * (1 + i));
        vIdx.push_back(total1 + 2 + 4 * i); vIdx.push_back(total1 + 2 + 4 * (1 + i)); vIdx.push_back(total2 + 2 * i);
        vIdx.push_back(total1 + 2 + 4 * (1 + i)); vIdx.push_back(total2 + 2 * (1 + i)); vIdx.push_back(total2 + 2 * i);
        vIdx.push_back(total1 + 3 + 4 * i); vIdx.push_back(total2 + 1 + 2 * i); vIdx.push_back(total1 + 3 + 4 * (1 + i));
        vIdx.push_back(total1 + 3 + 4 * (1 + i)); vIdx.push_back(total2 + 1 + 2 * i); vIdx.push_back(total2 + 1 + 2 * (1 + i));
    }
}



