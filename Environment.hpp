void A16::createEnvironment(std::vector<VertexMesh> &vPos, std::vector<uint32_t> &vIdx) {
	// Base
	vPos.push_back({ {-15, 15, 0}, {0,0,1}, {0.58f,0.74f} }); //A
	vPos.push_back({ {15, 15, 0}, {0,0,1}, {0.6f,0.74f} }); //B
	vPos.push_back({ {-15, -15, 0}, {0,0,1}, {0.58f,0.76f} }); //C
	vPos.push_back({ {15, -15, 0}, {0,0,1}, {0.6f,0.76f} }); //D

	// Upper part
	vPos.push_back({ {6,0,1}, {0,0,1}, {0.40f,0.24f} }); //G 
	vPos.push_back({ {8,0,1}, {0,0,1}, {0.42f,0.24f} }); //F
	vPos.push_back({ {6,-8,1}, {0,0,1}, {0.40f,0.26f} }); //H
	vPos.push_back({{8,-8,1}, {0,0,1}, {0.42f,0.26f}}); //E 
	
	vPos.push_back({ {-5.4f,1.8f,1}, {0,0,1}, {0.40f,0.24f} }); //R 
	vPos.push_back({{6,1.8f,1}, {0,0,1},  {0.42f,0.24f} }); //J
	vPos.push_back({{-5.4f,0,1}, {0,0,1}, {0.40f,0.26f} }); //Q
	vPos.push_back({ {6,0,1}, {0,0,1}, {0.42f,0.26f} }); //G
	
	vPos.push_back({{-7.4f,7.4f,1}, {0,0,1}, {0.40f,0.24f} });  //O 
	vPos.push_back({{-5.4f,7.4f,1}, {0,0,1}, {0.42f,0.24f} });  //P
	vPos.push_back({ {-7.4f,-2.3f,1}, {0,0,1}, {0.40f,0.26f} });  //K
	vPos.push_back({ {-5.4f,-2.3f,1}, {0,0,1}, {0.42f,0.26f} });  //N


	//Side part (facing y-axis)
	vPos.push_back({ {8,0,1}, {0,1,0}, {0.40f,0.24f} }); //F
	vPos.push_back({ {6,0,1}, {0,1,0}, {0.42f,0.24f} }); //G  
	vPos.push_back({ {8,0,0}, {0,1,0}, {0.40f,0.26f} });  //F'
	vPos.push_back({{6,0,0}, {0,1,0}, {0.42f,0.26f} });  //G'
	
	vPos.push_back({ {6,-8,1}, {0,-1,0}, {0.40f,0.24f} }); //H  
	vPos.push_back({ {8,-8,1}, {0,-1,0}, {0.42f,0.24f} }); //E
	vPos.push_back({ {6,-8,0}, {0,-1,0}, {0.40f,0.26f} }); //H'
	vPos.push_back({ {8,-8,0}, {0,-1,0}, {0.42f,0.26f} }); //E' 

	vPos.push_back({ {6,1.8f,1}, {0,1,0},  {0.40f,0.24f} }); //J
	vPos.push_back({ {-5.4f,1.8f,1}, {0,1,0}, {0.42f,0.24f} }); //R 
	vPos.push_back({ {6,1.8f,0}, {0,1,0},  {0.40f,0.26f} }); //J'
	vPos.push_back({ {-5.4f,1.8f,0}, {0,1,0}, {0.42f,0.26f} }); //R'

	vPos.push_back({ {-5.4f,0,1}, {0,-1,0}, {0.40f,0.24f} }); //Q 
	vPos.push_back({ {6,0,1}, {0,-1,0}, {0.42f,0.24f} }); //G
	vPos.push_back({ {-5.4f,0,0}, {0,-1,0}, {0.40f,0.26f} }); //Q'
	vPos.push_back({ {6,0,0}, {0,-1,0}, {0.42f,0.26f} }); //G'

	vPos.push_back({ {-5.4f,7.4f,1}, {0,1,0}, {0.40f,0.24f} });  //P
	vPos.push_back({ {-7.4f,7.4f,1}, {0,1,0}, {0.42f,0.24f} });  //O 
	vPos.push_back({ {-5.4f,7.4f,0}, {0,1,0}, {0.40f,0.26f} });  //P'
	vPos.push_back({ {-7.4f,7.4f,0}, {0,1,0}, {0.42f,0.26f} });  //O'

	vPos.push_back({ {-7.4f,-2.3f,1}, {0,-1,0}, {0.40f,0.24f} });  //K 
	vPos.push_back({ {-5.4f,-2.3f,1}, {0,-1,0}, {0.42f,0.24f} });  //N
	vPos.push_back({ {-7.4f,-2.3f,0}, {0,-1,0}, {0.40f,0.26f} });  //K'
	vPos.push_back({ {-5.4f,-2.3f,0}, {0,-1,0}, {0.42f,0.26f} });  //N'


	//Side part (facing x-axis)
	vPos.push_back({ {8,-8,1}, {1,0,0}, {0.40f,0.24f} }); //E 
	vPos.push_back({ {8,0,1}, {1,0,0}, {0.42f,0.24f} }); //F
	vPos.push_back({ {8,-8,0}, {1,0,0}, {0.40f,0.26f} }); //E'
	vPos.push_back({ {8,0,0}, {1,0,0}, {0.42f,0.26f} }); //F'

	vPos.push_back({ {6,0,1}, {-1,0,0}, {0.40f,0.24f} }); //G 
	vPos.push_back({ {6,-8,1}, {-1,0,0}, {0.42f,0.24f} }); //H
	vPos.push_back({ {6,0,0}, {-1,0,0}, {0.40f,0.26f} }); //G'
	vPos.push_back({ {6,-8,0}, {-1,0,0}, {0.42f,0.26f} }); //H'

	vPos.push_back({ {6,0,1}, {1,0,0}, {0.40f,0.24f} }); //G 
	vPos.push_back({ {6,1.8f,1}, {1,0,0},  {0.42f,0.24f} }); //J
	vPos.push_back({ {6,0,0}, {1,0,0}, {0.40f,0.26f} }); //G'
	vPos.push_back({ {6,1.8f,0}, {1,0,0},  {0.42f,0.26f} }); //J'

	vPos.push_back({ {-5.4f,-2.3f,1}, {1,0,0}, {0.40f,0.24f} });  //N 
	vPos.push_back({ {-5.4f,7.4f,1}, {1,0,0}, {0.42f,0.24f} });  //P
	vPos.push_back({ {-5.4f,-2.3f,0}, {1,0,0}, {0.40f,0.26f} });  //N'
	vPos.push_back({ {-5.4f,7.4f,0}, {1,0,0}, {0.42f,0.26f} });  //P'

	vPos.push_back({ {-7.4f,7.4f,1}, {-1,0,0}, {0.40f,0.24f} });  //O 
	vPos.push_back({ {-7.4f,-2.3f,1}, {-1,0,0}, {0.42f,0.24f} });  //K
	vPos.push_back({ {-7.4f,7.4f,0}, {-1,0,0}, {0.40f,0.26f} });  //O'
	vPos.push_back({ {-7.4f,-2.3f,0}, {-1,0,0}, {0.42f,0.26f} });  //K'


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



