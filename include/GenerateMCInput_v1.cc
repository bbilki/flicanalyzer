void GenerateMCInput()
{
	int PRunNo=RunNo;
	vector <int> RunNos;
	if(RunNo==1000)
	{
		//up to 7095 different PMT voltages
		RunNos.push_back(7074);//closed first
		RunNos.push_back(7076);//closed first
		RunNos.push_back(7078);//closed first
		RunNos.push_back(7082);//closed first
		RunNos.push_back(7084);//closed first
		RunNos.push_back(7092);//closed first
		RunNos.push_back(7094);//closed first
		RunNos.push_back(7096);//closed first
		RunNos.push_back(7104);//bump in slope//closed first
		RunNos.push_back(7106);//closed first
		
		RunNos.push_back(7107);
		RunNos.push_back(7110);
		RunNos.push_back(7121);
	}
	else if(RunNo==2000)
	{
		RunNos.push_back(7252);//closed first
		RunNos.push_back(7254);//closed first
		RunNos.push_back(7256);//closed first
		RunNos.push_back(7259);//closed first
		
		RunNos.push_back(7268);
		RunNos.push_back(7271);
		RunNos.push_back(7273);
	}
	else if(RunNo==3000)
	{
		RunNos.push_back(7308);
		RunNos.push_back(7312);
		RunNos.push_back(7314);
		RunNos.push_back(7316);
		RunNos.push_back(7318);
		RunNos.push_back(7320);
		RunNos.push_back(7322);
		RunNos.push_back(7325);//closed first
		RunNos.push_back(7327);//closed first
		RunNos.push_back(7385);//closed first
		RunNos.push_back(7388);
	}
	
	float SPE[3][5][2]={{{51.74,0.04},{38.53,0.06},{29.66,0.02},{32.04,0.01},{32.38,0.01}},{{0,0},{0,0},{0,0},{0,0},{0,0}},{{36.59,0.11},{28.46,0.07},{23.51,0.01},{22.65,0.01},{21.23,0.02}}};
	float tlims[5][2]={{1100,2040},{1100,2040},{1100,2040},{1150,2100},{1440,2350}};
	float DriftSpeeds[5]={0.149,0.149,0.149,0.147,0.150};//cm/us
	int runper=0;//0:7000-7087; 1:7090-7094; 2-7095-7144; 3:7200-7287; 4:7300-7403
	
	ifstream infile;
	sprintf(hname,"MCTrackParameters_%d_1.txt",PRunNo);
	ofstream outfile(hname);
	int aa=0;float bb[6]={0};
	int i=0;
	
	for(int J=0;J<RunNos.size();J++)
	{
		RunNo=RunNos[J];
		cout<<RunNo<<endl;
		
		if(RunNo>=7000 && RunNo<=7087) runper=0;
		else if(RunNo>=7090 && RunNo<=7094) runper=1;
		else if(RunNo>=7095 && RunNo<=7144) runper=2;
		else if(RunNo>=7200 && RunNo<=7287) runper=3;
		else if(RunNo>=7300 && RunNo<=7403) runper=4;
		
		sprintf(hname,"%s/Files/ValidTrackParameters_%d.txt",AnalysisFilePath,RunNo);
		cout<<hname<<endl;
		
		infile.open(hname);
		while(!infile.eof())
		{
// 			cout<<i<<endl;
			i++;
			infile>>aa>>bb[0]>>bb[1]>>bb[2]>>bb[3]>>bb[4]>>bb[5];
// 			cout<<aa<<" "<<bb[0]<<" "<<bb[1]<<" "<<bb[2]<<" "<<bb[3]<<" "<<bb[4]<<" "<<bb[5]<<endl;
			outfile<<RunNo<<" "<<aa<<" "<<((bb[0]-63.5)*2.54)<<" "<<((bb[1]-63.5)*2.54)<<" "<<(((bb[2]-tlims[runper][0])*DriftSpeeds[runper]*10.)+227.5)<<" "<<((bb[3]-63.5)*2.54)<<" "<<((bb[4]-63.5)*2.54)<<" "<<(((bb[5]-tlims[runper][0])*DriftSpeeds[runper]*10.)+227.5)<<endl;
		}
		infile.close();
	}
	outfile.close();
}