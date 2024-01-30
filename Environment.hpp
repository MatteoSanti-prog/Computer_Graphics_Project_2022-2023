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



    /*
	// Upper part
	vPos.push_back({ {5,1,h}, {0,0,1}, {0.40f,0.24f} }); //G 
	vPos.push_back({ {7,1,h}, {0,0,1}, {0.42f,0.24f} }); //F
	vPos.push_back({ {5,-7,h}, {0,0,1}, {0.40f,0.26f} }); //H
	vPos.push_back({ {7,-7,h}, {0,0,1}, {0.42f,0.26f}}); //E 
	
	vPos.push_back({ {-5,2,h}, {0,0,1}, {0.40f,0.24f} }); //R 
	vPos.push_back({ {4,2,h}, {0,0,1},  {0.42f,0.24f} }); //J
	vPos.push_back({ {-5,0,h}, {0,0,1}, {0.40f,0.26f} }); //Q
	vPos.push_back({ {4,0,h}, {0,0,1}, {0.42f,0.26f} }); //I(G)
	
	vPos.push_back({ {-8,7,h}, {0,0,1}, {0.40f,0.24f} });  //O 
	vPos.push_back({ {-5,7,h}, {0,0,1}, {0.42f,0.24f} });  //P
	vPos.push_back({ {-8,-3,h}, {0,0,1}, {0.40f,0.26f} });  //K
	vPos.push_back({ {-5,-3,h}, {0,0,1}, {0.42f,0.26f} });  //N


	//Side part (facing y-axis)
	vPos.push_back({ {7,1,h}, {0,1,0}, {0.40f,0.24f} }); //F
	vPos.push_back({ {5,1,h}, {0,1,0}, {0.42f,0.24f} }); //G  
	vPos.push_back({ {7,1,0}, {0,1,0}, {0.40f,0.26f} });  //F'
	vPos.push_back({{5,1,0}, {0,1,0}, {0.42f,0.26f} });  //G'
	
	vPos.push_back({ {5,-7,h}, {0,-1,0}, {0.40f,0.24f} }); //H  
	vPos.push_back({ {7,-7,h}, {0,-1,0}, {0.42f,0.24f} }); //E
	vPos.push_back({ {5,-7,0}, {0,-1,0}, {0.40f,0.26f} }); //H'
	vPos.push_back({ {7,-7,0}, {0,-1,0}, {0.42f,0.26f} }); //E' 

	vPos.push_back({ {4,2,h}, {0,1,0},  {0.40f,0.24f} }); //J
	vPos.push_back({ {-5,2,h}, {0,1,0}, {0.42f,0.24f} }); //R 
	vPos.push_back({ {4,2,0}, {0,1,0},  {0.40f,0.26f} }); //J'
	vPos.push_back({ {-5,2,0}, {0,1,0}, {0.42f,0.26f} }); //R'

	vPos.push_back({ {-5,0,h}, {0,-1,0}, {0.40f,0.24f} }); //Q 
	vPos.push_back({ {4,0,h}, {0,-1,0}, {0.42f,0.24f} }); //I(G)
	vPos.push_back({ {-5,0,0}, {0,-1,0}, {0.40f,0.26f} }); //Q'
	vPos.push_back({ {4,0,0}, {0,-1,0}, {0.42f,0.26f} }); //I'(G)

	vPos.push_back({ {-5,7,h}, {0,1,0}, {0.40f,0.24f} });  //P
	vPos.push_back({ {-8,7,h}, {0,1,0}, {0.42f,0.24f} });  //O 
	vPos.push_back({ {-5,7,0}, {0,1,0}, {0.40f,0.26f} });  //P'
	vPos.push_back({ {-8,7,0}, {0,1,0}, {0.42f,0.26f} });  //O'

	vPos.push_back({ {-8,-3,h}, {0,-1,0}, {0.40f,0.24f} });  //K 
	vPos.push_back({ {-5,-3,h}, {0,-1,0}, {0.42f,0.24f} });  //N
	vPos.push_back({ {-8,-3,0}, {0,-1,0}, {0.40f,0.26f} });  //K'
	vPos.push_back({ {-5,-3,0}, {0,-1,0}, {0.42f,0.26f} });  //N'


	//Side part (facing x-axis)
	vPos.push_back({ {7,-7,h}, {1,0,0}, {0.40f,0.24f} }); //E 
	vPos.push_back({ {7,1,h}, {1,0,0}, {0.42f,0.24f} }); //F
	vPos.push_back({ {7,-7,0}, {1,0,0}, {0.40f,0.26f} }); //E'
	vPos.push_back({ {7,1,0}, {1,0,0}, {0.42f,0.26f} }); //F'

	vPos.push_back({ {5,1,h}, {-1,0,0}, {0.40f,0.24f} }); //G 
	vPos.push_back({ {5,-7,h}, {-1,0,0}, {0.42f,0.24f} }); //H
	vPos.push_back({ {5,1,0}, {-1,0,0}, {0.40f,0.26f} }); //G'
	vPos.push_back({ {5,-7,0}, {-1,0,0}, {0.42f,0.26f} }); //H'

	vPos.push_back({ {4,0,h}, {1,0,0}, {0.40f,0.24f} }); //G(I)
	vPos.push_back({ {4,2,h}, {1,0,0},  {0.42f,0.24f} }); //J
	vPos.push_back({ {4,0,0}, {1,0,0}, {0.40f,0.26f} }); //G'(I)
	vPos.push_back({ {4,2,0}, {1,0,0},  {0.42f,0.26f} }); //J'

	vPos.push_back({ {-5,-3,h}, {1,0,0}, {0.40f,0.24f} });  //N 
	vPos.push_back({ {-5,7,h}, {1,0,0}, {0.42f,0.24f} });  //P
	vPos.push_back({ {-5,-3,0}, {1,0,0}, {0.40f,0.26f} });  //N'
	vPos.push_back({ {-5,7,0}, {1,0,0}, {0.42f,0.26f} });  //P'

	vPos.push_back({ {-8,7,h}, {-1,0,0}, {0.40f,0.24f} });  //O 
	vPos.push_back({ {-8,-3,h}, {-1,0,0}, {0.42f,0.24f} });  //K
	vPos.push_back({ {-8,7,0}, {-1,0,0}, {0.40f,0.26f} });  //O'
	vPos.push_back({ {-8,-3,0}, {-1,0,0}, {0.42f,0.26f} });  //K'
    */
}



