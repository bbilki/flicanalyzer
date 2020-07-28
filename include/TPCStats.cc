void TPCStats()
{
	sprintf(hname,"TPCStats.root");
	TFile* outroot=new TFile(hname,"recreate");
	
// 	int RunNos[32]={7074,7076,7078,7082,7084,7092,7094,7096,7104,7106,7107,7110,7121,7252,7254,7256,7259,7268,7271,7273,7275,7308,7312,7314,7316,7318,7320,7322,7325,7327,7385,7388};
	int RunNos[31]={7074,7076,7078,7082,7084,7092,7094,7096,7104,7106,7107,7110,7121,7252,7254,7256,7259,7268,7271,7273,7308,7312,7314,7316,7318,7320,7322,7325,7327,7385,7388};
	
	int NEvt=0;
	int NPMTSat[3]={0};
	int NSat=0;
	int NEvtType[4]={0};
	
	TFile* inroot;
	TTree* TH;
	TTree* TT;
	bool PMTsat=false;
	for(int J=0;J<31;J++)
	{
		RunNo=RunNos[J];
		sprintf(hname,"%s/Histos/HitsAndTracks_%d.root",AnalysisFilePath,RunNo);
		inroot=new TFile(hname);
		TH =  (TTree*) inroot->Get("Hits");
		TH->SetBranchAddress("E",&hd.E);
		TH->SetBranchAddress("ColIndT",&hd.ColIndT);
		TH->SetBranchAddress("Int",&hd.Int);
		TH->SetBranchAddress("QColTot",&hd.QColTot);
		TH->SetBranchAddress("QColTotZS",&hd.QColTotZS);
		TH->SetBranchAddress("QHitTot",&hd.QHitTot);
		TH->SetBranchAddress("PMTIntegral",&hd.PMTIntegral);
		TH->SetBranchAddress("ColID",&hd.ColID);
		TH->SetBranchAddress("ColT",&hd.ColT);
		TH->SetBranchAddress("ColA",&hd.ColA);
		TH->SetBranchAddress("ColInt",&hd.ColInt);
		TH->SetBranchAddress("Colw",&hd.Colw);
		TH->SetBranchAddress("IndID",&hd.IndID);
		TH->SetBranchAddress("IndT",&hd.IndT);
		TH->SetBranchAddress("IndA",&hd.IndA);
		TH->SetBranchAddress("Indw",&hd.Indw);
		TH->SetBranchAddress("EventType",&hd.EventType);
		TT =  (TTree*) inroot->Get("Tracks");
		TT->SetBranchAddress("E",&td.E);
		TT->SetBranchAddress("StartEndColIndT",&td.StartEndColIndT);
		TT->SetBranchAddress("FitParams",&td.FitParams);
		TT->SetBranchAddress("FitNormChi2",&td.FitNormChi2);
		TT->SetBranchAddress("NHits",&td.NHits);
		TT->SetBranchAddress("Nexcl",&td.Nexcl);
		TT->SetBranchAddress("PMTIntegral",&td.PMTIntegral);
		TT->SetBranchAddress("ColTStartEnd",&td.ColTStartEnd);
		TT->SetBranchAddress("ColHitTStartEnd",&td.ColHitTStartEnd);
		
		NEvt=TH->GetEntries();
		NSat=0;
		NPMTSat[0]=0;NPMTSat[1]=0;NPMTSat[2]=0;
		NEvtType[0]=0;NEvtType[1]=0;NEvtType[2]=0;NEvtType[3]=0;
		for(int I=0;I<TH->GetEntries();I++)
		{
			TH->GetEntry(I);
			
			PMTsat=false;
			for(int i1=0;i1<3;i1++)
			{
				if(hd.PMTIntegral->at(i1)<0){NPMTSat[i1]++;PMTsat=true;}
			}
			if(!PMTsat) NEvtType[hd.EventType]++;
			else{NSat++;}
		}
		cout<<"Run: "<<RunNo<<" "<<NEvt<<" Events "<<NSat<<" with PMT saturation. Event Types: "<<NEvtType[0]<<" "<<NEvtType[1]<<" "<<NEvtType[2]<<" "<<NEvtType[3]<<endl;
		inroot->Close();
	}
}