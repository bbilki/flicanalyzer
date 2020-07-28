void MIPAnalysis()
{
	vector <int> RunNos;
	if(RunNo==1000)
	{
		RunNos.push_back(7074);
		RunNos.push_back(7076);
		RunNos.push_back(7078);
		RunNos.push_back(7082);
		RunNos.push_back(7084);
		RunNos.push_back(7092);
		RunNos.push_back(7094);
		RunNos.push_back(7096);
		RunNos.push_back(7104);
		RunNos.push_back(7106);
		RunNos.push_back(7107);
		RunNos.push_back(7110);
		RunNos.push_back(7121);
	}
	else if(RunNo==2000)
	{
		RunNos.push_back(7264);
		RunNos.push_back(7266);
		RunNos.push_back(7268);
		RunNos.push_back(7271);
		RunNos.push_back(7273);
		RunNos.push_back(7275);
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
		RunNos.push_back(7323);
		RunNos.push_back(7325);
		RunNos.push_back(7327);
		RunNos.push_back(7379);
		RunNos.push_back(7385);
		RunNos.push_back(7388);
	}
	
	float driftvelocities[3]={0.1479,0.1464,0.1502};
	
	sprintf(hname,"%s/Histos/Flic_MC_%d.root",AnalysisFilePath,RunNo);
	cout<<hname<<endl;
	TFile* inMC=new TFile(hname);
	TTree* MCTree=(TTree*) inMC->Get("FlicMCShort");
	MCTree->SetBranchAddress("RunNo",&fc.RunNo);
	MCTree->SetBranchAddress("EvNo",&fc.EvNo);
	MCTree->SetBranchAddress("PMTPE",&fc.PMTPE);
	MCTree->SetBranchAddress("PMTPE2",&fc.PMTPE2);
	MCTree->SetBranchAddress("dETot",&fc.dETot);
// 	cout<<TMC->GetEntries()<<endl;
// 	TMC->GetEntry(1);
// 	cout<<fc.RunNo<<" "<<fc.EvNo<<endl;
	
	sprintf(hname,"%s/Histos/PMTIntegrals_%d_v2.root",AnalysisFilePath,RunNo);
	TFile* inroot=new TFile(hname);
	TTree* T=(TTree*) inroot->Get("PMTIntegrals");
	T->SetBranchAddress("RunNo",&pint.RunNo);
	T->SetBranchAddress("EventNo",&pint.EventNo);
	
	T->SetBranchAddress("integral_1500_2500",&pint.integral_1500_2500);
	T->SetBranchAddress("integral_1500_6000",&pint.integral_1500_6000);
	T->SetBranchAddress("integral_1500_8000",&pint.integral_1500_8000);
	T->SetBranchAddress("integral_1500_10000",&pint.integral_1500_10000);
	T->SetBranchAddress("integral_1500_15000",&pint.integral_1500_15000);
	
	T->SetBranchAddress("integralZS_1500_2500",&pint.integralZS_1500_2500);
	T->SetBranchAddress("integralZS_1500_6000",&pint.integralZS_1500_6000);
	T->SetBranchAddress("integralZS_1500_8000",&pint.integralZS_1500_8000);
	T->SetBranchAddress("integralZS_1500_10000",&pint.integralZS_1500_10000);
	T->SetBranchAddress("integralZS_1500_15000",&pint.integralZS_1500_15000);
	
	T->SetBranchAddress("integral4sigma_1500_2500",&pint.integral4sigma_1500_2500);
	T->SetBranchAddress("integral4sigma_1500_6000",&pint.integral4sigma_1500_6000);
	T->SetBranchAddress("integral4sigma_1500_8000",&pint.integral4sigma_1500_8000);
	T->SetBranchAddress("integral4sigma_1500_10000",&pint.integral4sigma_1500_10000);
	T->SetBranchAddress("integral4sigma_1500_15000",&pint.integral4sigma_1500_15000);
	
	T->SetBranchAddress("isSaturated",&pint.isSaturated);
	T->SetBranchAddress("isPedestalOK",&pint.isPedestalOK);
	T->SetBranchAddress("undershoot",&pint.undershoot);
	T->SetBranchAddress("spe",&pint.spe);
	T->SetBranchAddress("speerr",&pint.speerr);
	
	sprintf(hname,"MIPAnalysis_%d.root",RunNo);
	TFile* outroot=new TFile(hname,"recreate");
	
	int linecol=0;
	if(RunNo==1000) linecol=1;
	else if(RunNo==2000) linecol=4;
	else if(RunNo==3000) linecol=2;
	
	TH2F* EntryXYPMTMap[NPMTs][2];
	TH2F* EntryXYPMTSelMap[NPMTs][2];
	TProfile* q_vs_t[4];vector <TProfile*> Q_vs_t[4];
	TProfile* a_vs_t;vector <TProfile*> A_vs_t;
	TH1F* trackDeltaT;vector <TH1F*> TrackDeltaT;
	TH1F* startendT;vector <TH1F*> StartEndT;
	TH1F* startT;vector <TH1F*> StartT;
	TH1F* endT;vector <TH1F*> EndT;
	TH1F* qT;vector <TH1F*> QT[20];
	TH2F* w_vs_q;vector <TH2F*> W_vs_Q[20];
	TH1F* PEPerMeV[NPMTs][5];
	TH1F* PEs[NPMTs];
	TH1F* PMTCharge[NPMTs];
	TGraphErrors* tgeqt;
	
	TH1F* MCPEs[NPMTs];
	TH1F* MCPEPerMeV[NPMTs][5];
	
	for(int i1=0;i1<NPMTs;i1++)
	{
		sprintf(hname,"EntryXYPMTMap_%d",i1);
		EntryXYPMTMap[i1][0]=new TH2F(hname,hname,128,-0.5,127.5,128,-0.5,127.5);
		sprintf(hname,"EntryXYPMTMap_%d_norm",i1);
		EntryXYPMTMap[i1][1]=new TH2F(hname,hname,128,-0.5,127.5,128,-0.5,127.5);
		
		sprintf(hname,"EntryXYPMTSelMap_%d",i1);
		EntryXYPMTSelMap[i1][0]=new TH2F(hname,hname,128,-0.5,127.5,128,-0.5,127.5);
		sprintf(hname,"EntryXYPMTSelMap_%d_norm",i1);
		EntryXYPMTSelMap[i1][1]=new TH2F(hname,hname,128,-0.5,127.5,128,-0.5,127.5);
		
		sprintf(hname,"PEs_%s_vertical",PMTNames[i1].c_str());
		PEs[i1]=new TH1F(hname,hname,300,0.,3000.);
		PEs[i1]->SetLineColor(linecol);
		PEs[i1]->GetXaxis()->SetTitle("Number of Photoelectrons");
		PEs[i1]->GetXaxis()->CenterTitle();
		
		sprintf(hname,"PMTCharge_%s_vertical",PMTNames[i1].c_str());
		PMTCharge[i1]=new TH1F(hname,hname,1000,-1000,80000.);
		PMTCharge[i1]->SetLineColor(linecol);
		PMTCharge[i1]->GetXaxis()->SetTitle("Charge (au)");
		PMTCharge[i1]->GetXaxis()->CenterTitle();
		
		sprintf(hname,"PEPerMeV_%s_vertical",PMTNames[i1].c_str());
		PEPerMeV[i1][0]=new TH1F(hname,hname,240,0.,15.);
		PEPerMeV[i1][0]->SetLineColor(linecol);
		PEPerMeV[i1][0]->GetXaxis()->SetTitle("Number of Photoelectrons / MeV");
		PEPerMeV[i1][0]->GetXaxis()->CenterTitle();
		
		sprintf(hname,"PEPerMeV_%s_vertical_2",PMTNames[i1].c_str());
		PEPerMeV[i1][1]=new TH1F(hname,hname,240,0.,15.);
		PEPerMeV[i1][1]->SetLineColor(linecol);
		PEPerMeV[i1][1]->GetXaxis()->SetTitle("Number of Photoelectrons / MeV");
		PEPerMeV[i1][1]->GetXaxis()->CenterTitle();
		
		sprintf(hname,"PEPerMeV_%s_horizontal_1",PMTNames[i1].c_str());
		PEPerMeV[i1][2]=new TH1F(hname,hname,240,0.,15.);
		PEPerMeV[i1][2]->SetLineColor(linecol);
		PEPerMeV[i1][2]->GetXaxis()->SetTitle("Number of Photoelectrons / MeV");
		PEPerMeV[i1][2]->GetXaxis()->CenterTitle();
		
		sprintf(hname,"PEPerMeV_%s_all",PMTNames[i1].c_str());
		PEPerMeV[i1][3]=new TH1F(hname,hname,240,0.,15.);
		PEPerMeV[i1][3]->SetLineColor(linecol);
		PEPerMeV[i1][3]->GetXaxis()->SetTitle("Number of Photoelectrons / MeV");
		PEPerMeV[i1][3]->GetXaxis()->CenterTitle();
		
		sprintf(hname,"MCPEs_%s_vertical2",PMTNames[i1].c_str());
		MCPEs[i1]=new TH1F(hname,hname,300,0.,3000.);
		MCPEs[i1]->SetLineColor(linecol);
		MCPEs[i1]->GetXaxis()->SetTitle("Number of Photoelectrons");
		MCPEs[i1]->GetXaxis()->CenterTitle();
		
		sprintf(hname,"MCPEPerMeV_%s_vertical2",PMTNames[i1].c_str());
		MCPEPerMeV[i1][1]=new TH1F(hname,hname,240,0.,15.);
		MCPEPerMeV[i1][1]->SetLineColor(linecol);
		MCPEPerMeV[i1][1]->GetXaxis()->SetTitle("Number of Photoelectrons / MeV");
		MCPEPerMeV[i1][1]->GetXaxis()->CenterTitle();
	}
	for(int J=0;J<RunNos.size();J++)
	{
		sprintf(hname,"StartEndTime_%d",RunNos[J]);
		startendT=new TH1F(hname,hname,1500,0,1500);
		startendT->GetXaxis()->SetTitle("Track Start-End Time (#mus)");
		startendT->GetXaxis()->CenterTitle();
		StartEndT.push_back(startendT);
		
		sprintf(hname,"StartTime_%d",RunNos[J]);
		startT=new TH1F(hname,hname,1500,0,1500);
		startT->GetXaxis()->SetTitle("Track Start Time (#mus)");
		startT->GetXaxis()->CenterTitle();
		StartT.push_back(startT);
		
		sprintf(hname,"EndTime_%d",RunNos[J]);
		endT=new TH1F(hname,hname,1500,0,1500);
		endT->GetXaxis()->SetTitle("Track End Time (#mus)");
		endT->GetXaxis()->CenterTitle();
		EndT.push_back(endT);
		
		for(int i1=0;i1<20;i1++)
		{
			sprintf(hname,"QT_%d_%d",RunNos[J],i1);
// 			qT=new TH1F(hname,hname,1000,0,80000);
			qT=new TH1F(hname,hname,200,0,1000);
			qT->GetXaxis()->SetTitle("Charge / Hit (au)");
			qT->GetXaxis()->CenterTitle();
			QT[i1].push_back(qT);
		}
		
// 		for(int i1=0;i1<20;i1++)
// 		{
// 			sprintf(hname,"W_vs_Q_%d_%d",RunNos[J],i1);
// 			w_vs_q=new TH2F(hname,hname,200,0,2000,20,0,20);
// 			w_vs_q->GetXaxis()->SetTitle("Charge / Hit (au)");
// 			w_vs_q->GetXaxis()->CenterTitle();
// 			w_vs_q->GetYaxis()->SetTitle("Pulse Width (x 0.4 #mus)");
// 			w_vs_q->GetYaxis()->CenterTitle();
// 			W_vs_Q[i1].push_back(w_vs_q);
// 		}
		
		sprintf(hname,"Q_vs_t_0_5_%d",RunNos[J]);
		q_vs_t[0]=new TProfile(hname,hname,200,0,400,0,2000000);
		q_vs_t[0]->GetXaxis()->SetTitle("Hit Time (#mus)");
		q_vs_t[0]->GetXaxis()->CenterTitle();
		q_vs_t[0]->GetYaxis()->SetTitle("Hit Charge (au)");
		q_vs_t[0]->GetYaxis()->CenterTitle();
		Q_vs_t[0].push_back(q_vs_t[0]);
		
		sprintf(hname,"Q_vs_t_5_10_%d",RunNos[J]);
		q_vs_t[1]=new TProfile(hname,hname,200,0,400,0,2000000);
		q_vs_t[1]->GetXaxis()->SetTitle("Hit Time (#mus)");
		q_vs_t[1]->GetXaxis()->CenterTitle();
		q_vs_t[1]->GetYaxis()->SetTitle("Hit Charge (au)");
		q_vs_t[1]->GetYaxis()->CenterTitle();
		Q_vs_t[1].push_back(q_vs_t[1]);
		
		sprintf(hname,"Q_vs_t_10_50_%d",RunNos[J]);
		q_vs_t[2]=new TProfile(hname,hname,200,0,400,0,2000000);
		q_vs_t[2]->GetXaxis()->SetTitle("Hit Time (#mus)");
		q_vs_t[2]->GetXaxis()->CenterTitle();
		q_vs_t[2]->GetYaxis()->SetTitle("Hit Charge (au)");
		q_vs_t[2]->GetYaxis()->CenterTitle();
		Q_vs_t[2].push_back(q_vs_t[2]);
		
		sprintf(hname,"Q_vs_t_all_%d",RunNos[J]);
		q_vs_t[3]=new TProfile(hname,hname,200,0,400,0,2000000);
		q_vs_t[3]->GetXaxis()->SetTitle("Hit Time (#mus)");
		q_vs_t[3]->GetXaxis()->CenterTitle();
		q_vs_t[3]->GetYaxis()->SetTitle("Hit Charge (au)");
		q_vs_t[3]->GetYaxis()->CenterTitle();
		Q_vs_t[3].push_back(q_vs_t[3]);
		
		sprintf(hname,"A_vs_t_%d",RunNos[J]);
// 		q_vs_t=new TProfile(hname,hname,100,500,2500,0,450);
// 		q_vs_t=new TProfile(hname,hname,100,200,1000,0,450);
		a_vs_t=new TProfile(hname,hname,200,0,400,0,2000);
		a_vs_t->GetXaxis()->SetTitle("Hit Time (#mus)");
		a_vs_t->GetXaxis()->CenterTitle();
		a_vs_t->GetYaxis()->SetTitle("Hit Amplitude (au)");
		a_vs_t->GetYaxis()->CenterTitle();
		A_vs_t.push_back(a_vs_t);
		
		sprintf(hname,"TrackDeltaT_%d",RunNos[J]);
// 		trackDeltaT=new TH1F(hname,hname,200,0,1200.);
// 		trackDeltaT->GetXaxis()->SetTitle("Track #Deltat (x 0.4 #us)");
		trackDeltaT=new TH1F(hname,hname,200,0,500.);
		trackDeltaT->GetXaxis()->SetTitle("Track #Deltat (#mus)");
		trackDeltaT->GetXaxis()->CenterTitle();
		TrackDeltaT.push_back(trackDeltaT);
	}
	TH1F* TrackDeltaR=new TH1F("TrackDeltaR","TrackDeltaR",250,0.,50.);
	
	TF1* tfgaus=new TF1("tfgaus","gaus",0.,1000.);
	TF1* tfexp=new TF1("tfexp","[0]*exp(-x/[1])",0.,1000.);
	TF1* tfexp2=new TF1("tfexp","[0]*exp(-(x-[2])/[1])",0.,1000.);
	TF1 *func = new TF1("fit",fitf2,0.,1000.,5);
	TF1* tflandau=new TF1("tflandau","landau",0.,1000.);
	
	tfgaus->SetNpx(1000);
	tflandau->SetNpx(1000);
	
	TFile* inroot2;
	int PMTind=0;
	int Hind=0;
	float tracklength=0.;
	float x[2]={0.};float y[2]={0.};float t[2]={0.};
	float peperMeV[NPMTs]={0.};
	float PMTpe[NPMTs]={0.};
	float PMTQ[NPMTs]={0.};
	float driftv=0.;
	float deltaR=0.;
	
	TGraphErrors* tgQTAll=new TGraphErrors();
	tgQTAll->SetName("ElectronLifetimes");
	tgQTAll->SetTitle("ElectronLifetimes");
	tgQTAll->GetXaxis()->SetTitle("Run No");
	tgQTAll->GetXaxis()->CenterTitle();
	tgQTAll->GetYaxis()->SetTitle("Electron Lifetime (#mus)");
	tgQTAll->GetYaxis()->CenterTitle();
	tgQTAll->SetMarkerStyle(20);
	tgQTAll->SetMarkerColor(1);
	
	TGraphErrors* tgDTAll=new TGraphErrors();
	tgDTAll->SetName("DriftVelocity");
	tgDTAll->SetTitle("DriftVelocity");
	tgDTAll->GetXaxis()->SetTitle("Run No");
	tgDTAll->GetXaxis()->CenterTitle();
	tgDTAll->GetYaxis()->SetTitle("Drift Velocity (cm/#mus)");
	tgDTAll->GetYaxis()->CenterTitle();
	tgDTAll->SetMarkerStyle(20);
	tgDTAll->SetMarkerColor(1);
	
// 	ofstream outMCMatch;
// 	int NMCMatch=0;
// 	if(RunNo==3000)
// 	{
// 		outMCMatch.open("forMCMatching.txt");
// 	}
	
	bool saturated=false;
	bool baselinebad=false;
	bool windowhit=false;
	
	int MCind=0;
	int NEvts[5]={0};
	
	float PMTXY[3][2]={{95.,63.5},{63.5,32.},{63.5,95.}};
	float qefact[3]={0.2903,2.848,3.787};
	float qefact2[3]={0.2328,3.341,2.88};
	float qefact3[3]={0.0892,0.515,1.618};
	
	float tstart=0.;
	float tend=0.;
	int Qbin=0;
	int pointid=0;
	
	for(int J=0;J<RunNos.size();J++)
	{
		sprintf(hname,"%s/Histos/MIPsAndTracks_%d.root",AnalysisFilePath,RunNos[J]);
		cout<<hname<<endl;
		inroot2=new TFile(hname);
		TTree* T1 = (TTree*) inroot2->Get("Hits");
		T1->SetBranchAddress("E",&hd.E);
		T1->SetBranchAddress("ColIndT",&hd.ColIndT);
		T1->SetBranchAddress("Int",&hd.Int);
		T1->SetBranchAddress("QColTot",&hd.QColTot);
		T1->SetBranchAddress("QColTotZS",&hd.QColTotZS);
		T1->SetBranchAddress("QHitTot",&hd.QHitTot);
		T1->SetBranchAddress("PMTIntegral",&hd.PMTIntegral);
		
		T1->SetBranchAddress("ColID",&hd.ColID);
		T1->SetBranchAddress("ColT",&hd.ColT);
		T1->SetBranchAddress("ColA",&hd.ColA);
		T1->SetBranchAddress("ColInt",&hd.ColInt);
		T1->SetBranchAddress("Colw",&hd.Colw);
		T1->SetBranchAddress("IndID",&hd.IndID);
		T1->SetBranchAddress("IndT",&hd.IndT);
		T1->SetBranchAddress("IndA",&hd.IndA);
		T1->SetBranchAddress("IndInt",&hd.IndInt);
		T1->SetBranchAddress("Indw",&hd.Indw);
		T1->SetBranchAddress("EventType",&hd.EventType);
		
		TTree* Ttr = (TTree*) inroot2->Get("Tracks");
		Ttr->SetBranchAddress("E",&td.E);
		Ttr->SetBranchAddress("StartEndColIndT",&td.StartEndColIndT);
		Ttr->SetBranchAddress("FitParams",&td.FitParams);
		Ttr->SetBranchAddress("FitNormChi2",&td.FitNormChi2);
		Ttr->SetBranchAddress("QColTot",&td.QColTot);
		Ttr->SetBranchAddress("NHits",&td.NHits);
		Ttr->SetBranchAddress("Nexcl",&td.Nexcl);
		Ttr->SetBranchAddress("PMTIntegral",&td.PMTIntegral);
		Ttr->SetBranchAddress("ColTStartEnd",&td.ColTStartEnd);
		Ttr->SetBranchAddress("ColHitTStartEnd",&td.ColHitTStartEnd);
		
		for(int I=0;I<Ttr->GetEntries();I++)
		{
			Ttr->GetEntry(I);
			Hind=I;
			for(int i1=PMTind;i1<T->GetEntries();i1++)
			{
				T->GetEntry(i1);
				if(pint.RunNo==RunNos[J] && pint.EventNo==td.E)
				{
					PMTind=i1;
					break;
				}
			}
			T1->GetEntry(Hind);
			T->GetEntry(PMTind);
			x[0]=td.StartEndColIndT->at(0);
			y[0]=td.StartEndColIndT->at(1);
			t[0]=td.StartEndColIndT->at(2);
			x[1]=td.StartEndColIndT->at(3);
			y[1]=td.StartEndColIndT->at(4);
			t[1]=td.StartEndColIndT->at(5);
			
// // // // // 			
			for(int i1=0;i1<NPMTs;i1++)
			{
// 				PMTQ[i1]=pint.integral_1500_10000->at(i1);
				PMTQ[i1]=pint.integral4sigma_1500_10000->at(i1);
// 				PMTQ[i1]=pint.integralZS_1500_10000->at(i1);
				PMTpe[i1]=(PMTQ[i1]/pint.spe->at(i1));
			}
// // // // // 			
			
			for(int i1=0;i1<NPMTs;i1++)
			{
				EntryXYPMTMap[i1][0]->Fill(x[0],y[0],PMTpe[i1]);
				EntryXYPMTMap[i1][1]->Fill(x[0],y[0]);
			}
			
			NEvts[0]++;
			saturated=false;
			for(int i1=0;i1<NPMTs;i1++)
			{
				if(pint.isSaturated->at(i1)==1){saturated=true;break;}
			}
			if(saturated) continue;
			NEvts[1]++;
			baselinebad=false;
			for(int i1=0;i1<NPMTs;i1++)
			{
				if(pint.isPedestalOK->at(i1)==0){baselinebad=true;break;}
			}
			if(baselinebad) continue;
			NEvts[2]++;
			windowhit=false;
			for(int i1=0;i1<NPMTs;i1++)
			{
				if((sqrt(pow(x[0]-PMTXY[i1][0],2.)+pow(y[0]-PMTXY[i1][1],2.))*0.254)<4.){windowhit=true;break;}
			}
			if(windowhit) continue;
			NEvts[3]++;
			
			if(x[0]>=10 && x[0]<118 && x[1]>=10 && x[1]<118 && y[0]>=10 && y[0]<118 && y[1]>=10 && y[1]<118)
			{
				NEvts[4]++;
			}
			
			for(int i1=0;i1<NPMTs;i1++)
			{
				EntryXYPMTSelMap[i1][0]->Fill(x[0],y[0],PMTpe[i1]);
				EntryXYPMTSelMap[i1][1]->Fill(x[0],y[0]);
			}
			
			tracklength=sqrt((pow(x[1]-x[0],2.)+pow(y[1]-y[0],2.))*pow(0.254,2.)+pow(t[2]-t[1],2.)*pow(0.4*driftvelocities[(RunNo/1000)-1],2.));//cm
			
// 			if(RunNos[J]==7318 && NMCMatch<100)
// 			{
// 				outMCMatch<<RunNos[J]<<" "<<td.E<<" "<<PMTpe[0]<<" "<<PMTpe[1]<<" "<<PMTpe[2]<<endl;
// 				NMCMatch++;
// 			}
			
			for(int i1=0;i1<NPMTs;i1++)
			{
				peperMeV[i1]=PMTpe[i1]/(2.1*tracklength);
				
				if(x[0]>=32 && x[0]<96 && x[1]>=32 && x[1]<96 && y[0]>=32 && y[0]<96 && y[1]>=32 && y[1]<96)
				{
					PEPerMeV[i1][0]->Fill(peperMeV[i1]);
				}
				if(x[0]>=10 && x[0]<118 && x[1]>=10 && x[1]<118 && y[0]>=10 && y[0]<118 && y[1]>=10 && y[1]<118)
				{
					PEPerMeV[i1][1]->Fill(peperMeV[i1]);
					
					PEs[i1]->Fill(PMTpe[i1]);
					PMTCharge[i1]->Fill(PMTQ[i1]);
					
// 					cout<<TMC->GetEntries()<<endl;
					for(int ip1=MCind;ip1<MCTree->GetEntries();ip1++)
					{
						MCTree->GetEntry(ip1);
						if(fc.RunNo==RunNos[J] && fc.EvNo==td.E)
						{
							MCind=ip1;
							break;
						}
					}
					
// 					MCPEs[i1]->Fill(fc.PMTPE->at(i1));
// // 					MCPEPerMeV[i1][1]->Fill(fc.PMTPE->at(i1)/(2.1*tracklength));
// 					MCPEPerMeV[i1][1]->Fill(fc.PMTPE->at(i1)/fc.dETot);
// // 					MCPEs[i1]->Fill(fc.PMTPE->at(i1)*qefact[i1]);
// // 					MCPEPerMeV[i1][1]->Fill(fc.PMTPE->at(i1)*qefact[i1]/(2.1*tracklength));
					
					MCPEs[i1]->Fill(fc.PMTPE->at(i1)*qefact3[i1]);
					MCPEPerMeV[i1][1]->Fill(fc.PMTPE->at(i1)*qefact3[i1]/fc.dETot);
				}
// 				if((x[0]<=10 || x[0]>118) && (x[1]<=10 || x[1]>118) && (y[0]<=10 || y[0]>118) && (y[1]<=10 || y[1]>118))
// 				{
// 					PEPerMeV[i1][2]->Fill(peperMeV[i1]);
// 				}
// 				PEPerMeV[i1][3]->Fill(peperMeV[i1]);
			}
			
// 			if(I%500==0) cout<<I<<endl;
// 			if(I==2) break;
		}
		inroot2->Close();
	}
// 	if(RunNo==3000)
// 	{
// 		outMCMatch.close();
// 	}
	inroot->Close();
	outroot->cd();
	
	int minbin=0;
	for(int i1=0;i1<NPMTs;i1++)
	{
		EntryXYPMTMap[i1][0]->Divide(EntryXYPMTMap[i1][1]);
		EntryXYPMTMap[i1][0]->Write();
		EntryXYPMTSelMap[i1][0]->Divide(EntryXYPMTSelMap[i1][1]);
		EntryXYPMTSelMap[i1][0]->Write();
		for(int i2=0;i2<2;i2++)
		{
			PEPerMeV[i1][i2]->Scale(1./PEPerMeV[i1][i2]->Integral());
			tflandau->SetParameter(0,PEPerMeV[i1][i2]->GetBinContent(PEPerMeV[i1][i2]->GetMaximumBin()));
			tflandau->SetParameter(1,PEPerMeV[i1][i2]->GetBinCenter(PEPerMeV[i1][i2]->GetMaximumBin()));
			tflandau->SetParameter(1,10.);
			
// 			if((PEPerMeV[i1][i2]->GetMaximumBin()-3)>0) minbin=PEPerMeV[i1][i2]->GetMaximumBin()-3;
// 			else minbin=1;
// 			PEPerMeV[i1][i2]->Fit(tflandau,"q","q",PEPerMeV[i1][i2]->GetBinCenter(PEPerMeV[i1][i2]->GetMaximumBin()-3),PEPerMeV[i1][i2]->GetBinCenter(PEPerMeV[i1][i2]->GetMaximumBin()+3));
// 			PEPerMeV[i1][i2]->Fit(tflandau,"q","q",tflandau->GetParameter(1)-2.*tflandau->GetParameter(2),tflandau->GetParameter(1)+2.*tflandau->GetParameter(2));
			
			PEPerMeV[i1][i2]->Fit(tflandau,"q","q",0.,15.);
			
			PEPerMeV[i1][i2]->Write();
		}
		PEs[i1]->Write();
		MCPEs[i1]->Write();
		
		MCPEPerMeV[i1][1]->Scale(1./MCPEPerMeV[i1][1]->Integral());
		tflandau->SetParameter(0,MCPEPerMeV[i1][1]->GetBinContent(MCPEPerMeV[i1][1]->GetMaximumBin()));
		tflandau->SetParameter(1,MCPEPerMeV[i1][1]->GetBinCenter(MCPEPerMeV[i1][1]->GetMaximumBin()));
		tflandau->SetParameter(1,10.);
		MCPEPerMeV[i1][1]->Fit(tflandau,"q","q",0.,15.);
		MCPEPerMeV[i1][1]->Write();
		
		PMTCharge[i1]->Write();
	}
	
	
	
	
	
	for(int J=0;J<RunNos.size();J++)
	{
		sprintf(hname,"%s/Histos/MIPsAndTracks_%d.root",AnalysisFilePath,RunNos[J]);
		cout<<hname<<endl;
		inroot2=new TFile(hname);
		TTree* T1 = (TTree*) inroot2->Get("Hits");
		T1->SetBranchAddress("E",&hd.E);
		T1->SetBranchAddress("ColIndT",&hd.ColIndT);
		T1->SetBranchAddress("Int",&hd.Int);
		T1->SetBranchAddress("QColTot",&hd.QColTot);
		T1->SetBranchAddress("QColTotZS",&hd.QColTotZS);
		T1->SetBranchAddress("QHitTot",&hd.QHitTot);
		T1->SetBranchAddress("PMTIntegral",&hd.PMTIntegral);
		
		T1->SetBranchAddress("ColID",&hd.ColID);
		T1->SetBranchAddress("ColT",&hd.ColT);
		T1->SetBranchAddress("ColA",&hd.ColA);
		T1->SetBranchAddress("ColInt",&hd.ColInt);
		T1->SetBranchAddress("Colw",&hd.Colw);
		T1->SetBranchAddress("IndID",&hd.IndID);
		T1->SetBranchAddress("IndT",&hd.IndT);
		T1->SetBranchAddress("IndA",&hd.IndA);
		T1->SetBranchAddress("IndInt",&hd.IndInt);
		T1->SetBranchAddress("Indw",&hd.Indw);
		T1->SetBranchAddress("EventType",&hd.EventType);
		
		TTree* Ttr = (TTree*) inroot2->Get("Tracks");
		Ttr->SetBranchAddress("E",&td.E);
		Ttr->SetBranchAddress("StartEndColIndT",&td.StartEndColIndT);
		Ttr->SetBranchAddress("FitParams",&td.FitParams);
		Ttr->SetBranchAddress("FitNormChi2",&td.FitNormChi2);
		Ttr->SetBranchAddress("QColTot",&td.QColTot);
		Ttr->SetBranchAddress("NHits",&td.NHits);
		Ttr->SetBranchAddress("Nexcl",&td.Nexcl);
		Ttr->SetBranchAddress("PMTIntegral",&td.PMTIntegral);
		Ttr->SetBranchAddress("ColTStartEnd",&td.ColTStartEnd);
		Ttr->SetBranchAddress("ColHitTStartEnd",&td.ColHitTStartEnd);
		
		for(int I=0;I<Ttr->GetEntries();I++)
		{
			Ttr->GetEntry(I);
			Hind=I;
			T1->GetEntry(Hind);
			x[0]=td.StartEndColIndT->at(0);
			y[0]=td.StartEndColIndT->at(1);
			t[0]=td.StartEndColIndT->at(2);
			x[1]=td.StartEndColIndT->at(3);
			y[1]=td.StartEndColIndT->at(4);
			t[1]=td.StartEndColIndT->at(5);
			
			if(x[0]>=10 && x[0]<118 && x[1]>=10 && x[1]<118 && y[0]>=10 && y[0]<118 && y[1]>=10 && y[1]<118)
			{
				deltaR=sqrt(pow(x[0]-x[1],2)+pow(y[0]-y[1],2))*0.254;
				TrackDeltaR->Fill(deltaR);
			}
			if(x[0]>=32 && x[0]<96 && x[1]>=32 && x[1]<96 && y[0]>=32 && y[0]<96 && y[1]>=32 && y[1]<96)
			{
				TrackDeltaT[J]->Fill((t[1]-t[0])*0.4);
				StartEndT[J]->Fill(t[0]*0.4);
				StartEndT[J]->Fill(t[1]*0.4);
				StartT[J]->Fill(t[0]*0.4);
				EndT[J]->Fill(t[1]*0.4);
			}
		}
		if(TrackDeltaT[J]->GetBinContent(TrackDeltaT[J]->GetMaximumBin())>50)
		{
			tfgaus->SetParameter(0,TrackDeltaT[J]->GetBinContent(TrackDeltaT[J]->GetMaximumBin()));
			tfgaus->SetParameter(1,TrackDeltaT[J]->GetBinCenter(TrackDeltaT[J]->GetMaximumBin()));
			tfgaus->SetParameter(2,10.);
			TrackDeltaT[J]->Fit(tfgaus,"q","q",TrackDeltaT[J]->GetBinCenter(TrackDeltaT[J]->GetMaximumBin()-3),TrackDeltaT[J]->GetBinCenter(TrackDeltaT[J]->GetMaximumBin()+3));
			TrackDeltaT[J]->Fit(tfgaus,"q","q",tfgaus->GetParameter(1)-2.*tfgaus->GetParameter(2),tfgaus->GetParameter(1)+2.*tfgaus->GetParameter(2));
			
			pointid=tgDTAll->GetN();
			tgDTAll->SetPoint(pointid,RunNos[J],52./tfgaus->GetParameter(1));
// 			tgDTAll->SetPointError(pointid,0,(52./tfgaus->GetParameter(1))*tfgaus->GetParError(1)/tfgaus->GetParameter(1));
			tgDTAll->SetPointError(pointid,0,(52./tfgaus->GetParameter(1))*tfgaus->GetParameter(2)/tfgaus->GetParameter(1));
			
			tfgaus->SetParameter(0,StartT[J]->GetBinContent(StartT[J]->GetMaximumBin()));
			tfgaus->SetParameter(1,StartT[J]->GetBinCenter(StartT[J]->GetMaximumBin()));
			tfgaus->SetParameter(2,10.);
			StartT[J]->Fit(tfgaus,"q","q",StartT[J]->GetBinCenter(StartT[J]->GetMaximumBin()-3),StartT[J]->GetBinCenter(StartT[J]->GetMaximumBin()+3));
			StartT[J]->Fit(tfgaus,"q","q",tfgaus->GetParameter(1)-2.*tfgaus->GetParameter(2),tfgaus->GetParameter(1)+2.*tfgaus->GetParameter(2));
			tstart=tfgaus->GetParameter(1)-1.5*tfgaus->GetParameter(2);
			
			tfgaus->SetParameter(0,EndT[J]->GetBinContent(EndT[J]->GetMaximumBin()));
			tfgaus->SetParameter(1,EndT[J]->GetBinCenter(EndT[J]->GetMaximumBin()));
			tfgaus->SetParameter(2,10.);
			EndT[J]->Fit(tfgaus,"q","q",EndT[J]->GetBinCenter(EndT[J]->GetMaximumBin()-3),EndT[J]->GetBinCenter(EndT[J]->GetMaximumBin()+3));
			EndT[J]->Fit(tfgaus,"q","q",tfgaus->GetParameter(1)-2.*tfgaus->GetParameter(2),tfgaus->GetParameter(1)+2.*tfgaus->GetParameter(2));
			tend=tfgaus->GetParameter(1)+1.5*tfgaus->GetParameter(2);
			
			for(int I=0;I<Ttr->GetEntries();I++)
			{
				Ttr->GetEntry(I);
				Hind=I;
				T1->GetEntry(Hind);
				x[0]=td.StartEndColIndT->at(0);
				y[0]=td.StartEndColIndT->at(1);
				t[0]=td.StartEndColIndT->at(2);
				x[1]=td.StartEndColIndT->at(3);
				y[1]=td.StartEndColIndT->at(4);
				t[1]=td.StartEndColIndT->at(5);
				
// 				if(x[0]>=32 && x[0]<96 && x[1]>=32 && x[1]<96 && y[0]>=32 && y[0]<96 && y[1]>=32 && y[1]<96)
				{
					for(int i1=0;i1<hd.ColT->size();i1++)
					{
						if((hd.ColT->at(i1)*0.4)>=tstart && (hd.ColT->at(i1)*0.4)<=tend)
						{
							Qbin=((int)((hd.ColT->at(i1)*0.4-tstart)/20.));
// 							QT[Qbin][J]->Fill(hd.ColInt->at(i1));
							QT[Qbin][J]->Fill(hd.ColA->at(i1));
// 							W_vs_Q[Qbin][J]->Fill(hd.ColInt->at(i1),hd.Colw->at(i1));
							
// 							if(hd.ColA->at(i1)>15.)
							{
								
								deltaR=sqrt(pow(x[0]-x[1],2)+pow(y[0]-y[1],2))*0.254;
								
								if(deltaR<5.) Q_vs_t[0][J]->Fill((hd.ColT->at(i1)*0.4-tstart),hd.ColInt->at(i1));
								else if(deltaR<10.) Q_vs_t[1][J]->Fill((hd.ColT->at(i1)*0.4-tstart),hd.ColInt->at(i1));
								else Q_vs_t[2][J]->Fill((hd.ColT->at(i1)*0.4-tstart),hd.ColInt->at(i1));
								Q_vs_t[3][J]->Fill((hd.ColT->at(i1)*0.4-tstart),hd.ColInt->at(i1));
								
	// 							if(RunNos[J]==7107 && Qbin==0 && hd.ColInt->at(i1)<40.)
	// 							{
	// 								cout<<I<<" "<<hd.ColID->at(i1)<<" "<<hd.ColT->at(i1)<<" "<<hd.ColA->at(i1)<<" "<<hd.ColInt->at(i1)<<endl;
	// 							}
								
	// 							Q_vs_t[J]->Fill(hd.ColT->at(i1)*0.4,hd.ColInt->at(i1));
	// 							Q_vs_t[J]->Fill((hd.ColT->at(i1)*0.4-tstart),hd.ColInt->at(i1));
								A_vs_t[J]->Fill((hd.ColT->at(i1)*0.4-tstart),hd.ColA->at(i1));
							}
						}
					}
				}
// 				if(I%500==0) cout<<I<<endl;
	// 			if(I==2) break;
			}
		}
		inroot2->Close();
		
		tgeqt=new TGraphErrors();
		sprintf(hname,"MeanQ_vs_T_%d",RunNos[J]);
		tgeqt->SetName(hname);tgeqt->SetTitle(hname);
		tgeqt->GetXaxis()->SetTitle("Time (#mus)");
		tgeqt->GetXaxis()->CenterTitle();
		tgeqt->GetYaxis()->SetTitle("Mean Charge (au)");
		tgeqt->GetYaxis()->CenterTitle();
			
		for(int i1=0;i1<20;i1++)
		{
// 				tfgaus->SetParameter(0,QT[i1][J]->GetBinContent(QT[i1][J]->GetMaximumBin()));
// 				tfgaus->SetParameter(1,QT[i1][J]->GetBinCenter(QT[i1][J]->GetMaximumBin()));
// 				tfgaus->SetParameter(2,10.);
// 				QT[i1][J]->Fit(tfgaus,"q","q",QT[i1][J]->GetBinCenter(QT[i1][J]->GetMaximumBin()-3),QT[i1][J]->GetBinCenter(QT[i1][J]->GetMaximumBin()+3));
// 				QT[i1][J]->Fit(tfgaus,"q","q",tfgaus->GetParameter(1)-2.*tfgaus->GetParameter(2),tfgaus->GetParameter(1)+2.*tfgaus->GetParameter(2));
			
// 				pointid=tgeqt->GetN();
// 				tgeqt->SetPoint(pointid,((float)i1)*20.,tfgaus->GetParameter(1));
// 				tgeqt->SetPointError(pointid,0.,tfgaus->GetParError(1));
			
			pointid=tgeqt->GetN();
			tgeqt->SetPoint(pointid,((float)i1)*20.,QT[i1][J]->GetMean());
			tgeqt->SetPointError(pointid,0.,QT[i1][J]->GetRMS()/sqrt(QT[i1][J]->GetEntries()));
		}
		tfexp->SetParameter(0,100);
		tfexp->SetParameter(1,500.);
		tgeqt->Fit(tfexp,"q","q",100.,300.);
		
		pointid=tgQTAll->GetN();
		tgQTAll->SetPoint(pointid,RunNos[J],tfexp->GetParameter(1));
		tgQTAll->SetPointError(pointid,0,tfexp->GetParError(1));
		
		outroot->cd();
		for(int i1=0;i1<20;i1++)
		{
			QT[i1][J]->Write();
// 				W_vs_Q[i1][J]->Write();
		}
		tgeqt->SetMarkerStyle(20);
		tgeqt->SetMarkerColor(1);
		tgeqt->Write();
		
		for(int ik1=0;ik1<4;ik1++)
		{
			Q_vs_t[ik1][J]->Fit(tfexp,"q","q",100.,300.);
			
			tfexp2->SetParameter(0,tfexp->GetParameter(0));
			tfexp2->SetParameter(1,tfexp->GetParameter(1));
			tfexp2->FixParameter(2,100.);
			Q_vs_t[ik1][J]->Fit(tfexp2,"q","q",100.,300.);
			
			Q_vs_t[ik1][J]->Write();
		}
		
		A_vs_t[J]->Fit(tfexp,"q","q",100.,300.);
		A_vs_t[J]->Write();
		
		TrackDeltaT[J]->Write();
		StartEndT[J]->Write();
		StartT[J]->Write();
		EndT[J]->Write();
	}
	TrackDeltaR->Write();
	
	tgQTAll->Write();
	tgDTAll->Write();
	
	TGraph* tga1=new TGraph();//driftvelocity
	tga1->SetName("axis1");
	tga1->SetPoint(0,7000,0);
	tga1->SetPoint(1,7400,2);
	tga1->Write();
	
	cout<<RunNo<<" NEvts : "<<NEvts[0]<<" "<<NEvts[1]<<" "<<NEvts[2]<<" "<<NEvts[3]<<" "<<NEvts[4]<<endl;
	
	outroot->Close();
	inMC->Close();
	
	sprintf(hname,"mv MIPAnalysis_%d.root %s/Histos/MIPAnalysis_%d.root",RunNo,AnalysisFilePath,RunNo);system(hname);
}