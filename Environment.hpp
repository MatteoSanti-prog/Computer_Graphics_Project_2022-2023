#pragma once

#include "Structs.hpp"

void createEnvironment(std::vector<VertexMesh> &vPos, std::vector<uint32_t> &vIdx) {
	
<<<<<<< Updated upstream
    
	const float h = 1.5f; //height
=======
>>>>>>> Stashed changes
	// Base
	vPos.push_back({ {-15, 15, 0}, {0,0,1}, {0.58f,0.74f} }); //A
	vPos.push_back({ {15, 15, 0}, {0,0,1}, {0.6f,0.74f} }); //B
	vPos.push_back({ {-15, -15, 0}, {0,0,1}, {0.58f,0.76f} }); //C
	vPos.push_back({ {15, -15, 0}, {0,0,1}, {0.6f,0.76f} }); //D

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

     
	//Indices
	/*Number of created squares*/
	int n = 15;
	/*Number of vertices used at each iteration*/
	int v = 4;
	for (int i = 0; i < n; i++) {
		vIdx.push_back(0 + v * i); vIdx.push_back(2 + v * i); vIdx.push_back(1 + v * i);
		vIdx.push_back(1 + v * i); vIdx.push_back(2 + v * i); vIdx.push_back(3 + v * i);
	}
    
}

void A16::createCircleMesh(std::vector<VertexMesh> &vDef, std::vector<uint32_t> &vIdx) {
    // The procedure fills array vDef with the positions of the vertices and of the normal vectors of the mesh
    // The procedures also fill the array vIdx with the indices of the vertices of the triangles
    // The primitive built here is a circle, with radius 1
    
    //center = 0, 0, 0
    
    float a_increment = 1;
    
    float alpha;
    
    int n = 360/a_increment + 1;
    
    //top face
    vDef.push_back({{0, -2.5f, 0.1f}, {0.0f, 0.0f, 1.0f}, {0.6f,0.6f}});     //top center
    
    for(alpha=0; alpha<=360; alpha+=a_increment){
        vDef.push_back({{0.5*cos(alpha), 0.5*sin(alpha)-2.5f, 0.1f}, {0.0f, 0.0f, 1.0f}, {0.6f,0.6f}});
    }
    
    for(int i=0; i<n-1; i++){
        vIdx.push_back(0); vIdx.push_back(i+1); vIdx.push_back(i+2);
    }
    
    /*
    //bottom face
    vDef.push_back({{0, 5.5f, 3.0f}, {0.0f, 0.0f, -1.0f}, {0.45f,0.6f}});     //bottom center
    
    for(alpha=0; alpha<=360; alpha+=a_increment){
        vDef.push_back({{cos(alpha), sin(alpha)+5.5f, 3.0f}, {0.0f, 0.0f, -1.0f}, {0.45f,0.6f}});
    }
    
    for(int i=n+1; i<2*n; i++){
        vIdx.push_back(n+1); vIdx.push_back(i+1); vIdx.push_back(i+2);
    }
    */
    
}



