void Environment::createEnvironment() {
	//X Y Z
	vPos.push_back(-15f); vPos.push_back(15f); vPos.push_back(0f); //A	//0
	vPos.push_back(15f); vPos.push_back(15f); vPos.push_back(0f); //B
	vPos.push_back(-15f); vPos.push_back(-15f); vPos.push_back(0f); //C
	vPos.push_back(15f); vPos.push_back(-15f); vPos.push_back(0f); //D

	vPos.push_back(8f); vPos.push_back(-8f); vPos.push_back(1f); //E	//4
	vPos.push_back(6f); vPos.push_back(-8f); vPos.push_back(1f); //H
	vPos.push_back(6f); vPos.push_back(0f); vPos.push_back(1f); //G
	vPos.push_back(8f); vPos.push_back(0f); vPos.push_back(1f); //F

	vPos.push_back(6f); vPos.push_back(1.8f); vPos.push_back(1f); //J	//8
	vPos.push_back(-5.4f); vPos.push_back(1.8f); vPos.push_back(1f); //R
	vPos.push_back(-5.4f); vPos.push_back(-0.2f); vPos.push_back(1f); //Q //AND G

	vPos.push_back(-5.4f); vPos.push_back(-2.3f); vPos.push_back(1f); //N	//11
	vPos.push_back(-7.4f); vPos.push_back(-2.3f); vPos.push_back(1f); //K
	vPos.push_back(-7.4f); vPos.push_back(7.4f); vPos.push_back(1f); //O
	vPos.push_back(-5.4f); vPos.push_back(7.4f); vPos.push_back(1f); //P
	
	//LOWER PART
	vPos.push_back(-15f); vPos.push_back(15f); vPos.push_back(-1f); //A'	//15
	vPos.push_back(15f); vPos.push_back(15f); vPos.push_back(-10f); //B'
	vPos.push_back(-15f); vPos.push_back(-15f); vPos.push_back(-10f); //C'
	vPos.push_back(15f); vPos.push_back(-15f); vPos.push_back(-10f); //D'

	vPos.push_back(8f); vPos.push_back(-8f); vPos.push_back(-1f); //E'	//19
	vPos.push_back(6f); vPos.push_back(-8f); vPos.push_back(-1f); //H'
	vPos.push_back(6f); vPos.push_back(0f); vPos.push_back(-1f); //G'
	vPos.push_back(8f); vPos.push_back(0f); vPos.push_back(-1f); //F'

	vPos.push_back(6f); vPos.push_back(1.8f); vPos.push_back(-1f); //J'	//23
	vPos.push_back(-5.4f); vPos.push_back(1.8f); vPos.push_back(-1f); //R'
	vPos.push_back(-5.4f); vPos.push_back(-0.2f); vPos.push_back(-1f); //Q' //AND G'

	vPos.push_back(-5.4f); vPos.push_back(-2.3f); vPos.push_back(-1f); //N'	//26
	vPos.push_back(-7.4f); vPos.push_back(-2.3f); vPos.push_back(-1f); //K'
	vPos.push_back(-7.4f); vPos.push_back(7.4f); vPos.push_back(-1f); //O'
	vPos.push_back(-5.4f); vPos.push_back(7.4f); vPos.push_back(-1f); //P'	//29
	
	//ABCD 0 1 2 3
	vIdx.push_back(0); vIdx.push_back(1); vIdx.push_back(2); //ABC
	vIdx.push_back(1); vIdx.push_back(2); vIdx.push_back(3); //BCD
	

	//HEFG
	//F7 E4	F'22 E'19
	vIdx.push_back(7); vIdx.push_back(4); vIdx.push_back(22); //FEF'
	vIdx.push_back(4); vIdx.push_back(19); vIdx.push_back(22); //EE'F'
	
	//H5 E4 H'20 E'19
	vIdx.push_back(4); vIdx.push_back(5); vIdx.push_back(20); //EHH'
	vIdx.push_back(4); vIdx.push_back(19); vIdx.push_back(20); //EE'H'
	
	//H5 G6 H'20 G'21
	vIdx.push_back(5); vIdx.push_back(6); vIdx.push_back(21); //HGG'
	vIdx.push_back(5); vIdx.push_back(20); vIdx.push_back(21); //HH'G'

	//G6 F7 G'21 F'22
	vIdx.push_back(6); vIdx.push_back(7); vIdx.push_back(22); //GFF'
	vIdx.push_back(6); vIdx.push_back(21); vIdx.push_back(22); //GG'F'

	//F7 E4 H5 G6
	vIdx.push_back(7); vIdx.push_back(4); vIdx.push_back(5); //FEH
	vIdx.push_back(5); vIdx.push_back(6); vIdx.push_back(7); //HGF
	
	//GJRQ
	//J8 G6 J'23 G'21
	vIdx.push_back(8); vIdx.push_back(6); vIdx.push_back(21); //JGG'
	vIdx.push_back(8); vIdx.push_back(23); vIdx.push_back(21); //JJ'G'

	//G6 Q10 G'21 Q'25
	vIdx.push_back(6); vIdx.push_back(10); vIdx.push_back(25); //GQQ'
	vIdx.push_back(6); vIdx.push_back(21); vIdx.push_back(25); //GG'Q'

	//Q10 R9 Q'25 R'24
	vIdx.push_back(10); vIdx.push_back(9); vIdx.push_back(24); //QRR'
	vIdx.push_back(10); vIdx.push_back(25); vIdx.push_back(24); //QQ'R'

	//R9 J8 R'24 J'23
	vIdx.push_back(9); vIdx.push_back(8); vIdx.push_back(23); //RJJ'
	vIdx.push_back(9); vIdx.push_back(24); vIdx.push_back(23); //RR'J'

	//R9 J8 G6 Q10
	vIdx.push_back(9); vIdx.push_back(8); vIdx.push_back(6); //RJG
	vIdx.push_back(10); vIdx.push_back(9); vIdx.push_back(6); //QRG

	//NKOP
	//P14 N11 P'29 N'26
	vIdx.push_back(14); vIdx.push_back(11); vIdx.push_back(26); //PNN'
	vIdx.push_back(14); vIdx.push_back(29); vIdx.push_back(26); //PP'N'

	//N11 K12 N'26 K'27
	vIdx.push_back(11); vIdx.push_back(12); vIdx.push_back(26); //NKK'
	vIdx.push_back(11); vIdx.push_back(26); vIdx.push_back(26); //NN'K'

	//K12 O13 K'27 O'28
	vIdx.push_back(12); vIdx.push_back(13); vIdx.push_back(28); //KOO'
	vIdx.push_back(12); vIdx.push_back(27); vIdx.push_back(28); //KK'O'

	//O13 P14 O'28 P'29
	vIdx.push_back(13); vIdx.push_back(14); vIdx.push_back(29); //OPP'
	vIdx.push_back(13); vIdx.push_back(28); vIdx.push_back(29); //OO'P'
	//N11 K12 O13 P14 N'26 K'27 O'28 P'29
	vIdx.push_back(13); vIdx.push_back(14); vIdx.push_back(12); //OPK
	vIdx.push_back(11); vIdx.push_back(14); vIdx.push_back(12); //NPK
}



