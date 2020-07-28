void MIPsAndTracks()
{
	sprintf(hname,"MIPsAndTracks_%d.root",RunNo);
	TFile* outroot=new TFile(hname,"recreate");
	TTree* T1 =  new TTree("Hits","Hits");
	T1->Branch("E",&hd.E);
	T1->Branch("ColIndT",&hd.ColIndT);
	T1->Branch("Int",&hd.Int);
	T1->Branch("QColTot",&hd.QColTot);
	T1->Branch("QColTotZS",&hd.QColTotZS);
	T1->Branch("QHitTot",&hd.QHitTot);
	T1->Branch("PMTIntegral",&hd.PMTIntegral);
	
	T1->Branch("ColID",&hd.ColID);
	T1->Branch("ColT",&hd.ColT);
	T1->Branch("ColA",&hd.ColA);
	T1->Branch("ColInt",&hd.ColInt);
	T1->Branch("Colw",&hd.Colw);
	T1->Branch("IndID",&hd.IndID);
	T1->Branch("IndT",&hd.IndT);
	T1->Branch("IndA",&hd.IndA);
	T1->Branch("IndInt",&hd.IndInt);
	T1->Branch("Indw",&hd.Indw);
	T1->Branch("EventType",&hd.EventType);
	
	outroot->mkdir("Histos");
	
	sprintf(hname,"ValidTrackParameters_%d.txt",RunNo);
	ofstream outfile(hname);
	vector <int> eventlist;
	
	TTree* Ttr =  new TTree("Tracks","Tracks");
	Ttr->Branch("E",&td.E);
	Ttr->Branch("StartEndColIndT",&td.StartEndColIndT);
	Ttr->Branch("FitParams",&td.FitParams);
	Ttr->Branch("FitNormChi2",&td.FitNormChi2);
	Ttr->Branch("QColTot",&td.QColTot);
	Ttr->Branch("NHits",&td.NHits);
	Ttr->Branch("Nexcl",&td.Nexcl);
	Ttr->Branch("PMTIntegral",&td.PMTIntegral);
	Ttr->Branch("ColTStartEnd",&td.ColTStartEnd);
	Ttr->Branch("ColHitTStartEnd",&td.ColHitTStartEnd);
	
	sprintf(hname,"%s/Run_%d.root",NTupleFilePath,RunNo);
	TFile* inroot=new TFile(hname);
	TTree* T =  (TTree*) inroot->Get("T");
	T->SetBranchAddress("TPCWF",&ed.TPCWF);
	T->SetBranchAddress("PMTWF",&ed.PMTWF);
	
	TH1F* HitCol=new TH1F("HitCol","HitCol",128,-0.5,127.5);
	TH1F* HitInd=new TH1F("HitInd","HitInd",128,-0.5,127.5);
	TH1F* HitTime=new TH1F("HitTime","HitTime",4096,-0.5,4095.5);
	TH1F* HitIntegral1=new TH1F("HitIntegral1","HitIntegral1",500,0,3000);
	TH1F* HitIntegral2=new TH1F("HitIntegral2","HitIntegral2",500,0,3000);
	TH1F* HitIntegral3=new TH1F("HitIntegral3","HitIntegral3",500,0,3000);
	TH1F* NHits=new TH1F("NHits","NHits",2000,-0.5,1999.5);
	TH1F* NHitsPerColPeak=new TH1F("NHitsPerColPeak","NHitsPerColPeak",200,-0.5,199.5);
	TH1F* HQTotCol=new TH1F("QTotCol","QTotCol",4000,0,1000000);
	TH1F* NPoints=new TH1F("NPoints","NPoints",2240,0,224000);//224001,-0.5,224000.5);
	TH1F* ClusterNPointsperArea=new TH1F("ClusterNPointsperArea","ClusterNPointsperArea",1020,-0.01,1.01);
	TH1F* NPointsperArea=new TH1F("NPointsperArea","NPointsperArea",1020,-0.01,1.01);
	TH1F* TotalArea=new TH1F("TotalArea","TotalArea",2240,0,224000);//224001,-0.5,224000.5);
	TH1F* ClmaxPointFraction=new TH1F("ClmaxPointFraction","ClmaxPointFraction",510,-0.01,1.01);
	TH1F* DeltaTTrackColT=new TH1F("DeltaTTrackColT","DeltaTTrackColT",500,0,5000);
	TH1F* DeltaTTrackColHitT=new TH1F("DeltaTTrackColHitT","DeltaTTrackColHitT",500,0,5000);
	TH1F* NClperEvent=new TH1F("NClperEvent","NClperEvent",60,-0.5,59.5);
	TH1F* ClSize=new TH1F("ClSize","ClSize",400,-0.5,399.5);
	TH1F* MaxClSize=new TH1F("MaxClSize","MaxClSize",400,-0.5,399.5);
	
	TH1F* FitNormChi2=new TH1F("FitNormChi2","FitNormChi2",200,0.,100.);
	TH1F* FitClSize=new TH1F("FitClSize","FitClSize",200,-0.5,199.5);
	TH1F* NExclHits=new TH1F("NExclHits","NExclHits",200,-0.5,199.5);
	TH1F* TrackDeltaT=new TH1F("TrackDeltaT","TrackDeltaT",500,0,5000);
	TH1F* ColTrackDeltaT=new TH1F("ColTrackDeltaT","ColTrackDeltaT",500,0,5000);
	
	TH1F* TrackDeltaTSel=new TH1F("TrackDeltaTSel","TrackDeltaTSel",500,0,5000);
	TH1F* AllDeltaT=new TH1F("AllDeltaT","AllDeltaT",500,0,5000);
	TH2F* AllXY=new TH2F("AllXY","AllXY",128,-0.5,127.5,128,-0.5,127.5);
	TH2F* TrackXY=new TH2F("TrackXY","TrackXY",128,-0.5,127.5,128,-0.5,127.5);
	TH2F* TrackEntryXY=new TH2F("TrackEntryXY","TrackEntryXY",128,-0.5,127.5,128,-0.5,127.5);
	TH2F* TrackExitXY=new TH2F("TrackExitXY","TrackExitXY",128,-0.5,127.5,128,-0.5,127.5);
	TH2F* DeltaTvsTrackAngle=new TH2F("DeltaTvsTrackAngle","DeltaTvsTrackAngle",314,0,1.57,500,0,1000);
	TProfile* DeltaTvsTrackAnglePr=new TProfile("DeltaTvsTrackAnglePr","DeltaTvsTrackAnglePr",250,0,1.57,0,1000);
	TH1F* Tfirst=new TH1F("Tfirst","Tfirst",500,0,5000);
	TH1F* Tlast=new TH1F("Tlast","Tlast",500,0,5000);
	TH1F* TColfirst=new TH1F("TColfirst","TColfirst",500,0,5000);
	TH1F* TCollast=new TH1F("TCollast","TCollast",500,0,5000);
	TH1F* Trackt0=new TH1F("Trackt0","Trackt0",500,0,5000);
	TH2F* DeltaTvsTrackt0=new TH2F("DeltaTvsTrackt0","DeltaTvsTrackt0",500,0,5000,500,0,1000);
// 	TH1F* DeltacolperDeltat=new TH1F("DeltacolperDeltat","DeltacolperDeltat",500,0,50);
// 	TH1F* DeltaindperDeltat=new TH1F("DeltaindperDeltat","DeltaindperDeltat",500,0,50);
// 	TH1F* DeltaRperDeltat=new TH1F("DeltaRperDeltat","DeltaRperDeltat",2000,0,100);
// 	TH1F* ColDeltaRR=new TH1F("ColDeltaR","ColDeltaR",2000,0,2000);
// 	TH1F* HitDeltaRR=new TH1F("HitDeltaRR","HitDeltaRR",2000,0,2000);
// 	TH1F* HitDeltaT=new TH1F("HitDeltaT","HitDeltaT",2000,0,1000);
// 	TH1F* HitDeltaR=new TH1F("HitDeltaR","HitDeltaR",200,0,100);
// 	TH2F* HitDeltaR_vs_DeltaT=new TH2F("HitDeltaR_vs_DeltaT","HitDeltaR_vs_DeltaT",200,-0.5,199.5,200,0,20);
	
	TH2F* DeltaTLengthvsAngle=new TH2F("DeltaTLengthvsAngle","DeltaTLengthvsAngle",314,0,1.57,100,0,2000);
	TH2F* DeltaTLengthvsAngle_norm=new TH2F("DeltaTLengthvsAngle_norm","DeltaTLengthvsAngle_norm",314,0,1.57,100,0,2000);
	
	TH1F* FitNormChi2_Col_vs_t=new TH1F("FitNormChi2_Col_vs_t","FitNormChi2_Col_vs_t",1000,0.,4000.);
	TH1F* FitNormChi2_Ind_vs_t=new TH1F("FitNormChi2_Ind_vs_t","FitNormChi2_Ind_vs_t",1000,0.,4000.);
	
	TH2F* NColHits_vs_fitnormchi2=new TH2F("NColHits_vs_fitnormchi2","NColHits_vs_fitnormchi2",200,-0.5,199.5,500,0,2000);
	TH2F* NIndHits_vs_fitnormchi2=new TH2F("NIndHits_vs_fitnormchi2","NIndHits_vs_fitnormchi2",200,-0.5,199.5,500,0,2000);
	
	
	
	TH1F* StartTime=new TH1F("StartTime","StartTime",512,-0.5,4095.5);
	TH1F* EndTime=new TH1F("EndTime","EndTime",512,-0.5,4095.5);
	TH1F* StartCollection=new TH1F("StartCollection","StartCollection",128,-0.5,127.5);
	TH1F* EndCollection=new TH1F("EndCollection","EndCollection",128,-0.5,127.5);
	TH1F* StartInduction=new TH1F("StartInduction","StartInduction",128,-0.5,127.5);
	TH1F* EndInduction=new TH1F("EndInduction","EndInduction",128,-0.5,127.5);
	
	TH1F* ColTWidth=new TH1F("ColTWidth","ColTWidth",128,-0.5,127.5);
	TH1F* IndTWidth=new TH1F("IndTWidth","IndTWidth",128,-0.5,127.5);
	
	TH2F* NHits_T_vs_E=new TH2F("NHits_T_vs_E","NHits_T_vs_E",T->GetEntries(),-0.5,T->GetEntries()-0.5,4096,-0.5,4095.5);
	TH2F* NHits_Col_vs_Ind=new TH2F("NHits_Col_vs_Ind","NHits_Col_vs_Ind",20,-0.5,19.5,20,-0.5,19.5);
	TH2F* QCol_vs_QInd=new TH2F("QCol_vs_QInd","QCol_vs_QInd",50,0,200,50,0,200);
	
	TH1F* ColIndDeltaT=new TH1F("ColIndDeltaT","ColIndDeltaT",2000,-5.,5.);
	TH1F* ColIndMinDeltaT5=new TH1F("ColIndMinDeltaT5","ColIndMinDeltaT5",4000,-10.,10.);
	TH1F* ColWidth=new TH1F("ColWidth","ColWidth",128,-0.5,127.5);
	TH1F* IndWidth=new TH1F("IndWidth","IndWidth",128,-0.5,127.5);
	TH1F* ColAmp=new TH1F("ColAmp","ColAmp",300,0.,300.);
	TH1F* IndAmp=new TH1F("IndAmp","IndAmp",300,0.,300.);
	TH2F* Amp_Col_vs_Ind=new TH2F("Amp_Col_vs_Ind","Amp_Col_vs_Ind",300,0,300,300,0,300);
	TH2F* Width_Col_vs_Ind=new TH2F("Width_Col_vs_Ind","Width_Col_vs_Ind",128,-0.5,127.5,128,-0.5,127.5);
	TH1F* HitIntegralLost=new TH1F("HitIntegralLost","HitIntegralLost",4000,0,1000000);
	TH1F* TrueTotalIntegral=new TH1F("TrueTotalIntegral","TrueTotalIntegral",4000,0,1000000);
	TH2F* Col_Int_vs_Amp=new TH2F("Col_Int_vs_Amp","Col_Int_vs_Amp",300,0,300,500,0,3000);
	
	
	TH1D* QperT[3];
	QperT[0]=new TH1D("QperT","QperT",3500,750,2500);
	QperT[1]=new TH1D("QperT_norm","QperT_norm",3500,750,2500);
	QperT[2]=new TH1D("QperT_div","QperT_div",3500,750,2500);
	
	TH2F* TPC2DAll[10];
	
	TH1D* TPC2DProjX[3];
	
	TSpectrum *TS;
	TSpectrum *TS2;
	Int_t nfoundpeaks=0;
	Double_t *xpeaks;
	TF1* fgaus=new TF1("fgaus","gaus",-1000,6000);
	
	TH1F* BLhist;
	vector <float> BLmeans;
	vector <float> BLsigmas;
	
	bool TPCSaturated;
	float colsigmacut;
	
	peaks2 p2;
	vector <peaks2> colpeaks;
	vector <peaks2> indpeaks;
	vector <peaks2> colpeakssel;
	vector <peaks2> indpeakssel;
	float fitnormchi2;
	float fitpoints[6]={0};
	
	vector <int> bb;
	vector <float> bc;
	
	TPC2DAll[0]=new TH2F("WF_All","WF_All",4096,-0.5,4095.5,256,-0.5,255.5);
	TPC2DAll[1]=new TH2F("WF_BLSub","WF_BLSub",4096,-0.5,4095.5,256,-0.5,255.5);
	TPC2DAll[2]=new TH2F("WF_bkg","WF_bkg",4096,-0.5,4095.5,256,-0.5,255.5);
	TPC2DAll[3]=new TH2F("WF_BLSub_bkgsub_col","WF_BLSub_bkgsub_col",4096,-0.5,4095.5,256,-0.5,255.5);//1-2
	TPC2DAll[4]=new TH2F("WF_BLSub_sum_ind","WF_BLSub_sum_ind",4096,-0.5,4095.5,256,-0.5,255.5);
	TPC2DAll[5]=new TH2F("WF_BLSub_sum_ind_bkg","WF_BLSub_sum_ind_bkg",4096,-0.5,4095.5,256,-0.5,255.5);
	TPC2DAll[6]=new TH2F("WF_BLSub_sum_ind_bkg_sub","WF_BLSub_sum_ind_bkg_sub",4096,-0.5,4095.5,256,-0.5,255.5);//4-5
	TPC2DAll[7]=new TH2F("WF_BLSub_sum_ind_bkg_sub_avg","WF_BLSub_sum_ind_bkg_sub_avg",4096,-0.5,4095.5,256,-0.5,255.5);
	TPC2DAll[8]=new TH2F("WF_filtered_ind","WF_filtered_ind",4096,-0.5,4095.5,256,-0.5,255.5);
	TPC2DAll[9]=new TH2F("WF_col_ind","WF_col_ind",4096,-0.5,4095.5,256,-0.5,255.5);
	
	BLhist=new TH1F("BLhist","BLhist",8000,-4000,4000);
	
	if(RunNo<7200) colsigmacut=5.;
	else colsigmacut=3.;
	
	float peakslope[2]={0};
	float peakintercept[2]={0};
	
	Double_t s[4096];
	Double_t s2[4096];
	Double_t sw[128];
	Double_t indint=0;
	Double_t BLmean=0;
	Double_t BLsigma=0;
	Double_t WFamp=0;
	TS = new TSpectrum();
	TS2 = new TSpectrum(4096);
	
	singlehit sh;
	vector <singlehit> SH;
	TGraph2D* tg2d=new TGraph2D();
	float tracklength=0;
	bool isTrackValid=false;
	
	float QColTot=0;
	float QHitTot=0;
	
	for(int I=0;I<T->GetEntries();I++)
// 	for(int I=0;I<=100;I++)
	{
		T->GetEntry(I);
		if(I%100==0) cout<<I<<" / "<<T->GetEntries()<<endl;
		
		colpeaks.clear();
		indpeaks.clear();
		colpeakssel.clear();
		indpeakssel.clear();
		for(int is1=0;is1<10;is1++){TPC2DAll[is1]->Reset();}
		BLmeans.clear();
		BLsigmas.clear();
		TPCSaturated=false;
		
		for(int i1=0;i1<256;i1++)
		{
			BLhist->Reset();
			for(int i2=0;i2<4096;i2++)
			{
				TPC2DAll[0]->Fill(i2,i1,ed.TPCWF->at(i1)[i2]);
				if(i2<500){BLhist->Fill(ed.TPCWF->at(i1)[i2]);}
			}
			BLhist->Fit(fgaus,"q","q",0.,2000.);
			BLmean=fgaus->GetParameter(1);
			BLsigma=fgaus->GetParameter(2);
			BLmeans.push_back(BLmean);
			BLsigmas.push_back(BLsigma);
			indint=0;
			for(int i2=0;i2<4096;i2++)
			{
				WFamp=(Double_t)ed.TPCWF->at(i1)[i2]-BLmean;
				indint+=WFamp;
				TPC2DAll[1]->Fill(i2,i1,WFamp);
				TPC2DAll[4]->Fill(i2,i1,indint);
				if(i1<128 && WFamp<=-200)
				{
					TPCSaturated=true;
				}
			}
		}
// 		if(TPCSaturated) cout<<"TPC readout saturated."<<endl;
		if(TPCSaturated)
		{
			int nlength=0;
			int nlengthmax=0;
			for(int i1=0;i1<128;i1++)
			{
				for(int i2=2;i2<4096;i2++)
				{
					if((TPC2DAll[0]->GetBinContent(i2-1,i1+1)-TPC2DAll[0]->GetBinContent(i2,i1+1))>500)
					{
						nlength=0;
						while(((TPC2DAll[0]->GetBinContent(i2-1,i1+1)-TPC2DAll[0]->GetBinContent(i2,i1+1))>500))
						{
							TPC2DAll[0]->SetBinContent(i2,i1+1,1024+TPC2DAll[0]->GetBinContent(i2,i1+1));
							TPC2DAll[1]->SetBinContent(i2,i1+1,TPC2DAll[0]->GetBinContent(i2,i1+1)-BLmeans[i1]);
							i2++;
							nlength++;
						}
						if(nlength>nlengthmax) nlengthmax=nlength;
					}
				}
			}
			if(nlengthmax<100)
			{
// 				cout<<"TPC saturation fixed."<<endl;
				TPCSaturated=false;
			}
		}
		vector <int> cutbins;
		int minbin=0;int maxbin=0;int peakbin=0;
		int minbin2=0;int maxbin2=0;
		int leftbin=0;int rightbin=0;
		TGraph* tgr=new TGraph();
		TF1* tflin=new TF1("lin","[0]+[1]*x",0,5000);
		for(int i2=0;i2<=4096;i2++){cutbins.push_back(0);}
// 	// // 	do collection first
		for(int i1=0;i1<128;i1++)
		{
			sprintf(hname,"WF1_%d",i1);TPC2DProjX[0]=TPC2DAll[1]->ProjectionX(hname,i1+1,i1+1,"");
			for(int i2=0;i2<4096;i2++){cutbins[i2]=0;}
			for(int i2=0;i2<4096;i2++)
			{
				if(TPC2DProjX[0]->GetBinContent(i2+1)>colsigmacut*BLsigmas[i1])
				{
					leftbin=-1;rightbin=-1;
					for(int is1=i2;is1<4095;is1++)
					{
						if(TPC2DProjX[0]->GetBinContent(is1+1)>=1.5*BLsigmas[i1] && TPC2DProjX[0]->GetBinContent(is1+2)<1.5*BLsigmas[i1])
						{
							rightbin=is1+1;
							break;
						}
					}
					for(int is1=i2;is1>=2;is1--)
					{
						if(TPC2DProjX[0]->GetBinContent(is1+1)>=1.5*BLsigmas[i1] && TPC2DProjX[0]->GetBinContent(is1)<1.5*BLsigmas[i1])
						{
							leftbin=is1+1;
							break;
						}
					}
					if(rightbin==-1)
					{
						for(int is1=i2;is1<4095;is1++)
						{
							if(TPC2DProjX[0]->GetBinContent(is1+1)>TPC2DProjX[0]->GetBinContent(is1+2))
							{
								rightbin=is1+1;
								break;
							}
						}
						if(rightbin==-1)
						{
							rightbin=4095;
						}
						else
						{
							for(int is1=rightbin;is1<4095;is1++)
							{
								if(TPC2DProjX[0]->GetBinContent(is1+1)<TPC2DProjX[0]->GetBinContent(is1+2))
								{
									rightbin=is1+1;
									break;
								}
							}
						}
					}
					if(leftbin==-1)
					{
						for(int is1=i2;is1>=2;is1--)
						{
							if(TPC2DProjX[0]->GetBinContent(is1+1)<TPC2DProjX[0]->GetBinContent(is1))
							{
								leftbin=is1+1;
								break;
							}
						}
					}
					if(leftbin==-1) leftbin=((i2-20)>=1?(i2-20):1);
					if(rightbin==-1) rightbin=((i2+20)<=4095?(i2+20):4095);
					
					if(leftbin!=-1 && rightbin!=-1)
					{
						for(int is1=leftbin;is1<=rightbin;is1++){cutbins[is1]=1;}
						i2=rightbin;
					}
				}
			}
			for(int i2=0;i2<4096;i2++)
			{
				if(cutbins[i2]==0) s[i2]=TPC2DProjX[0]->GetBinContent(i2+1);
				else s[i2]=0;
				s2[i2]=s[i2];
			}
			TS->Background(s,4096,20,TSpectrum::kBackDecreasingWindow,TSpectrum::kBackOrder2,kFALSE,TSpectrum::kBackSmoothing3,kTRUE);
			TS->Background(s2,4096,2,TSpectrum::kBackDecreasingWindow,TSpectrum::kBackOrder6,kFALSE,TSpectrum::kBackSmoothing3,kTRUE);
			for(int i2=0;i2<4096;i2++)
			{
				if(cutbins[i2]==0) TPC2DAll[2]->SetBinContent(i2+1,i1+1,s2[i2]);
				else TPC2DAll[2]->SetBinContent(i2+1,i1+1,s[i2]);
			}
		}
		TPC2DAll[3]->Add(TPC2DAll[1],1.);
		TPC2DAll[3]->Add(TPC2DAll[2],-1.);
		
		for(int i1=0;i1<128;i1++)
		{
			sprintf(hname,"WF1_%d",i1);TPC2DProjX[0]=TPC2DAll[3]->ProjectionX(hname,i1+1,i1+1,"");
			nfoundpeaks = TS2->Search(TPC2DProjX[0],3,"nobackground",0.05);
			xpeaks = TS2->GetPositionX();
			for(int is1=0;is1<nfoundpeaks;is1++)
			{
				if(TPC2DProjX[0]->GetBinContent(TPC2DProjX[0]->FindBin(xpeaks[is1]))<10) continue;
				TPC2DProjX[0]->Fit(fgaus,"q","q",TPC2DProjX[0]->GetBinCenter(TPC2DProjX[0]->FindBin(xpeaks[is1])-2),TPC2DProjX[0]->GetBinCenter(TPC2DProjX[0]->FindBin(xpeaks[is1])+2));
				
	// 			peaks[i1/128].push_back(fgaus->GetParameter(1));
	// 			peakamplitudes[i1/128].push_back(fgaus->GetParameter(0));
				
	// 			peaks[i1/128].push_back(xpeaks[is1]);
	// 			peakamplitudes[i1/128].push_back(TPC2DProjX[0]->GetBinContent(TPC2DProjX[0]->FindBin(xpeaks[is1])));
				
	// 			peakwires[i1/128].push_back(i1-(128*(i1/128)));
				
				p2.t=fgaus->GetParameter(1);
				p2.w=(i1-(128*(i1/128)));
				p2.integral=0;
				p2.integral2=0;
				p2.amplitude=fgaus->GetParameter(0);
				p2.fwhm=0;
				p2.w10perc=0;
				p2.wfull=0;
				colpeaks.push_back(p2);
			}
		}
// 		cout<<colpeaks.size()<<" collection hits"<<endl;
		
		float sa=0;float ca=0;int npinline=0;int npvaltot=0;
		float integral=0;float integral2=0;
		
		bool isMIP=false;
		if(colpeaks.size()<400)
		{
			for(int is1=0;is1<colpeaks.size();is1++)
			{
				if(colpeaks[is1].t<1000 || colpeaks[is1].t>2600) continue;
				npvaltot++;
			}
			for(int is1=0;is1<colpeaks.size();is1++)
			{
				if(colpeaks[is1].t<1000 || colpeaks[is1].t>2600) continue;
				for(int is2=is1+1;is2<colpeaks.size();is2++)
				{
					if(colpeaks[is2].t<1000 || colpeaks[is2].t>2600) continue;
					sa=(((float)(colpeaks[is2].w-colpeaks[is1].w))/(colpeaks[is2].t-colpeaks[is1].t));
					ca=((colpeaks[is2].t*((float)colpeaks[is1].w))-(colpeaks[is1].t*((float)colpeaks[is2].w)))/(colpeaks[is2].t-colpeaks[is1].t);
					npinline=0;
					for(int is3=0;is3<colpeaks.size();is3++)
					{
						if(colpeaks[is3].t<1000 || colpeaks[is3].t>2600) continue;
						if(is3==is1 || is3==is2) {continue;}
						if(fabs(sa*colpeaks[is3].t+ca-((float)colpeaks[is3].w))<=5) npinline++;
					}
					if((((float)npinline)/((float)npvaltot))>0.7)
					{
						isMIP=true;
						peakslope[0]=sa;
						peakintercept[0]=ca;
						break;
					}
				}
				if(isMIP) break;
			}
		}
// 		if(isMIP) cout<<"MIP"<<endl;
		
		if(!isMIP) continue;
		
		for(int i2=0;i2<4096;i2++)
		{
			for(int i1=1;i1<127;i1++)
			{
				if(TPC2DAll[3]->GetBinContent(i2+1,i1)>0 && TPC2DAll[3]->GetBinContent(i2+1,i1)<10 && TPC2DAll[3]->GetBinContent(i2+1,i1-1)==0 && TPC2DAll[3]->GetBinContent(i2+1,i1+1)==0)
				{
					TPC2DAll[3]->SetBinContent(i2+1,i1,0);
				}
			}
		}
		
		for(int is1=0;is1<colpeaks.size();is1++)
		{
			sprintf(hname,"WF1_%d",is1);TPC2DProjX[0]=TPC2DAll[3]->ProjectionX(hname,colpeaks[is1].w+1,colpeaks[is1].w+1,"");
			peakbin=TPC2DProjX[0]->FindBin(colpeaks[is1].t);
			minbin=-1;maxbin=-1;
			for(int il1=peakbin;il1>=1;il1--)
			{
				if(TPC2DProjX[0]->GetBinContent(il1)==0)
				{
					minbin=il1+1;
					break;
				}
			}
			for(int il1=peakbin;il1<=TPC2DProjX[0]->GetNbinsX();il1++)
			{
				if(TPC2DProjX[0]->GetBinContent(il1)==0)
				{
					maxbin=il1-1;
					break;
				}
			}
			if(minbin!=-1 && maxbin!=-1)
			{
				integral=0.;
				for(int il1=minbin;il1<=maxbin;il1++)
				{
					integral+=TPC2DProjX[0]->GetBinContent(il1);
				}
				colpeaks[is1].integral=integral;
			}
			minbin=-1;maxbin=-1;
			for(int il1=peakbin-2;il1>=2;il1--)
			{
				if(TPC2DProjX[0]->GetBinContent(il1-1)>TPC2DProjX[0]->GetBinContent(il1) || TPC2DProjX[0]->GetBinContent(il1-1)==0)
				{
					minbin=il1;
					break;
				}
			}
			for(int il1=peakbin+2;il1<TPC2DProjX[0]->GetNbinsX();il1++)
			{
				if(TPC2DProjX[0]->GetBinContent(il1+1)>TPC2DProjX[0]->GetBinContent(il1) || TPC2DProjX[0]->GetBinContent(il1+1)==0)
				{
					maxbin=il1;
					break;
				}
			}
			if(minbin!=-1 && maxbin!=-1)
			{
				integral2=0.;
				for(int il1=minbin;il1<=maxbin;il1++)
				{
					integral2+=TPC2DProjX[0]->GetBinContent(il1);
				}
				colpeaks[is1].integral2=integral2;
				colpeaks[is1].wfull=((float)(maxbin-minbin+1));
			}
		}
		
		int sind=0;
		float maxinrange=0;
		float mininrange=0;
		float suminrange=0;
		int Ninrange=0;
		
// 	// // 	now induction
		for(int i1=128;i1<256;i1++)
		{
			sprintf(hname,"WF1_%d",i1);TPC2DProjX[0]=TPC2DAll[4]->ProjectionX(hname,i1+1,i1+1,"");
			for(int i2=0;i2<4096;i2++)
			{
				s[i2]=TPC2DProjX[0]->GetBinContent(i2+1);
				s2[i2]=s[i2];
			}
			TS->Background(s2,4096,13,TSpectrum::kBackDecreasingWindow,TSpectrum::kBackOrder2,kFALSE,TSpectrum::kBackSmoothing5,kFALSE);
			for(int i2=0;i2<4096;i2++)
			{
				TPC2DAll[5]->SetBinContent(i2+1,i1+1,s2[i2]);
				TPC2DAll[4]->SetBinContent(i2+1,i1+1,s[i2]-s2[i2]);
			}
		}
		for(int i2=0;i2<4096;i2++)
		{
			sprintf(hname,"WF2_%d",i2);TPC2DProjX[1]=TPC2DAll[4]->ProjectionY(hname,i2+1,i2+1,"");
			suminrange=0;
			for(int i3=129;i3<255;i3++)
			{
				suminrange+=TPC2DProjX[1]->GetBinContent(i3+1);
			}
			suminrange/=126.;
			for(int i3=128;i3<256;i3++)
			{
				if((TPC2DProjX[1]->GetBinContent(i3+1)-suminrange)>0)
				{
					TPC2DAll[7]->SetBinContent(i2+1,i3+1,TPC2DProjX[1]->GetBinContent(i3+1)-suminrange);
				}
			}
		}
		for(int i1=129;i1<255;i1++)
		{
			sprintf(hname,"WF1_%d",i1);TPC2DProjX[0]=TPC2DAll[7]->ProjectionX(hname,i1+1,i1+1,"");
			sprintf(hname,"WF2_%d",i1);TPC2DProjX[1]=TPC2DAll[4]->ProjectionX(hname,i1+1,i1+1,"");
			for(int i2=0;i2<4096;i2++)
			{
				if(TPC2DProjX[0]->GetBinContent(i2+1)>=30)
				{
					minbin=-1;maxbin=-1;peakbin=-1;
					for(int i3=i2+1;i3<4095;i3++)
					{
						if(TPC2DProjX[1]->GetBinContent(i3)>TPC2DProjX[1]->GetBinContent(i3-1) && TPC2DProjX[1]->GetBinContent(i3)>TPC2DProjX[1]->GetBinContent(i3+1))
						{
							peakbin=i3;
							break;
						}
					}
					for(int i3=peakbin;i3<4095;i3++)
					{
						if((TPC2DProjX[1]->GetBinContent(i3)<TPC2DProjX[1]->GetBinContent(i3+1) && TPC2DProjX[1]->GetBinContent(i3)<0.1*TPC2DProjX[1]->GetBinContent(peakbin))||TPC2DProjX[1]->GetBinContent(i3)<0.1)
						{
							maxbin=i3;
							break;
						}
					}
					for(int i3=peakbin;i3>1;i3--)
					{
						if((TPC2DProjX[1]->GetBinContent(i3)<TPC2DProjX[1]->GetBinContent(i3-1) && TPC2DProjX[1]->GetBinContent(i3)<0.1*TPC2DProjX[1]->GetBinContent(peakbin))||TPC2DProjX[1]->GetBinContent(i3)<0.1)
						{
							minbin=i3;
							break;
						}
					}
					minbin2=-1;maxbin2=-1;
					for(int i3=i2;i3<4096;i3++)
					{
						if(TPC2DProjX[0]->GetBinContent(i3)==0)
						{
							maxbin2=i3;
							break;
						}
					}
					for(int i3=i2;i3>0;i3--)
					{
						if(TPC2DProjX[0]->GetBinContent(i3)==0)
						{
							minbin2=i3;
							break;
						}
					}
					
					if(minbin2>minbin) minbin=minbin2;
					if(maxbin2<maxbin) maxbin=maxbin2;
					
					if(minbin==-1) minbin=0;
					if(maxbin==-1) maxbin=4096;
					if(minbin!=-1 && maxbin!=-1)
					{
						for(int i3=minbin;i3<=maxbin;i3++)
						{
							TPC2DAll[8]->SetBinContent(i3+1,i1+1,TPC2DProjX[1]->GetBinContent(i3+1));
						}
					}
				}
			}
		}
		for(int i1=128;i1<256;i1++)
		{
			sprintf(hname,"WF1_%d",i1);TPC2DProjX[0]=TPC2DAll[8]->ProjectionX(hname,i1+1,i1+1,"");
			nfoundpeaks = TS2->Search(TPC2DProjX[0],5,"nobackground",0.1);
			xpeaks = TS2->GetPositionX();
			for(int is1=0;is1<nfoundpeaks;is1++)
			{
				if(TPC2DProjX[0]->GetBinContent(TPC2DProjX[0]->FindBin(xpeaks[is1]))<10) continue;
				TPC2DProjX[0]->Fit(fgaus,"q","q",TPC2DProjX[0]->GetBinCenter(TPC2DProjX[0]->FindBin(xpeaks[is1])-2),TPC2DProjX[0]->GetBinCenter(TPC2DProjX[0]->FindBin(xpeaks[is1])+2));
				
	// 			peaks[i1/128].push_back(fgaus->GetParameter(1));
	// 			peakamplitudes[i1/128].push_back(fgaus->GetParameter(0));
				
	// 			peaks[i1/128].push_back(xpeaks[is1]);
	// 			peakamplitudes[i1/128].push_back(TPC2DProjX[0]->GetBinContent(TPC2DProjX[0]->FindBin(xpeaks[is1])));
				
	// 			peakwires[i1/128].push_back(i1-(128*(i1/128)));
				
				p2.t=fgaus->GetParameter(1);
				p2.w=(i1-(128*(i1/128)));
				p2.integral=0;
				p2.integral2=0;
				p2.amplitude=fgaus->GetParameter(0);
				p2.fwhm=0;
				p2.w10perc=0;
				p2.wfull=0;
				indpeaks.push_back(p2);
			}
		}
// 		cout<<indpeaks.size()<<" induction hits"<<endl;
		
		sa=0;ca=0;npinline=0;npvaltot=0;bool foundMIP=false;
		for(int is1=0;is1<indpeaks.size();is1++)
		{
			if(indpeaks[is1].t<1000 || indpeaks[is1].t>2600) continue;
			npvaltot++;
		}
		for(int is1=0;is1<indpeaks.size();is1++)
		{
			if(indpeaks[is1].t<1000 || indpeaks[is1].t>2600) continue;
			for(int is2=is1+1;is2<indpeaks.size();is2++)
			{
				if(indpeaks[is2].t<1000 || indpeaks[is2].t>2600) continue;
				sa=(((float)(indpeaks[is2].w-indpeaks[is1].w))/(indpeaks[is2].t-indpeaks[is1].t));
				ca=((indpeaks[is2].t*((float)indpeaks[is1].w))-(indpeaks[is1].t*((float)indpeaks[is2].w)))/(indpeaks[is2].t-indpeaks[is1].t);
				npinline=0;
				for(int is3=0;is3<indpeaks.size();is3++)
				{
					if(indpeaks[is3].t<1000 || indpeaks[is3].t>2600) continue;
					if(is3==is1 || is3==is2) {continue;}
					if(fabs(sa*indpeaks[is3].t+ca-((float)indpeaks[is3].w))<=5) npinline++;
				}
				if((((float)npinline)/((float)npvaltot))>0.7)
				{
					foundMIP=true;
					peakslope[1]=sa;
					peakintercept[1]=ca;
					break;
				}
			}
			if(foundMIP) break;
		}
		
		for(int is1=0;is1<indpeaks.size();is1++)
		{
			sprintf(hname,"WF1_%d",is1);TPC2DProjX[0]=TPC2DAll[8]->ProjectionX(hname,indpeaks[is1].w+128+1,indpeaks[is1].w+128+1,"");
			peakbin=TPC2DProjX[0]->FindBin(indpeaks[is1].t);
			minbin=-1;maxbin=-1;
			for(int il1=peakbin;il1>=1;il1--)
			{
				if(TPC2DProjX[0]->GetBinContent(il1)==0)
				{
					minbin=il1+1;
					break;
				}
			}
			for(int il1=peakbin;il1<=TPC2DProjX[0]->GetNbinsX();il1++)
			{
				if(TPC2DProjX[0]->GetBinContent(il1)==0)
				{
					maxbin=il1-1;
					break;
				}
			}
			if(minbin!=-1 && maxbin!=-1)
			{
				integral=0.;
				for(int il1=minbin;il1<=maxbin;il1++)
				{
					integral+=TPC2DProjX[0]->GetBinContent(il1);
				}
				indpeaks[is1].integral=integral;
			}
			minbin=-1;maxbin=-1;
			for(int il1=peakbin-2;il1>=2;il1--)
			{
				if(TPC2DProjX[0]->GetBinContent(il1-1)>TPC2DProjX[0]->GetBinContent(il1) || TPC2DProjX[0]->GetBinContent(il1-1)==0)
				{
					minbin=il1;
					break;
				}
			}
			for(int il1=peakbin+2;il1<TPC2DProjX[0]->GetNbinsX();il1++)
			{
				if(TPC2DProjX[0]->GetBinContent(il1+1)>TPC2DProjX[0]->GetBinContent(il1) || TPC2DProjX[0]->GetBinContent(il1+1)==0)
				{
					maxbin=il1;
					break;
				}
			}
			if(minbin!=-1 && maxbin!=-1)
			{
				integral2=0.;
				for(int il1=minbin;il1<=maxbin;il1++)
				{
					integral2+=TPC2DProjX[0]->GetBinContent(il1);
				}
				indpeaks[is1].integral2=integral2;
				indpeaks[is1].wfull=((float)(maxbin-minbin+1));
			}
		}
		
// 		cout<<peakslope[0]<<" "<<peakintercept[0]<<" "<<peakslope[1]<<" "<<peakintercept[1]<<endl;
		
		for(int i2=0;i2<4096;i2++)
		{
			for(int i1=0;i1<128;i1++)
			{
				TPC2DAll[9]->SetBinContent(i2+1,i1+1,TPC2DAll[3]->GetBinContent(i2+1,i1+1));
			}
			for(int i1=128;i1<256;i1++)
			{
				TPC2DAll[9]->SetBinContent(i2+1,i1+1,TPC2DAll[8]->GetBinContent(i2+1,i1+1));
			}
		}
		sprintf(hname,"WF_col_ind_%d",I);
		TPC2DAll[9]->SetName(hname);TPC2DAll[9]->SetTitle(hname);
		outroot->cd("Histos");
		TPC2DAll[9]->Write();
		
		SH.clear();
		
		sort(colpeaks.begin(),colpeaks.end(),SortPeaksByT);
		sort(indpeaks.begin(),indpeaks.end(),SortPeaksByT);
		int indsel=-1;
		float dmin=1000.;
		float dmax=0.;
		
		
		if(fabs(peakslope[0])<0.1 || fabs(peakslope[1])<0.1)
		{
			for(int is1=0;is1<colpeaks.size();is1++)
			{
				sprintf(hname,"WF1_%d",is1);TPC2DProjX[0]=TPC2DAll[9]->ProjectionY(hname,((int)colpeaks[is1].t)+1,((int)colpeaks[is1].t)+1,"");
				TPC2DProjX[0]->GetXaxis()->SetRangeUser(127.5,255.5);
				nfoundpeaks = TS2->Search(TPC2DProjX[0],5,"nobackground",0.1);
				xpeaks = TS2->GetPositionX();
				dmax=0.;indsel=-1;
				for(int ik1=0;ik1<nfoundpeaks;ik1++)
				{
					if(TPC2DProjX[0]->GetBinContent(((int)xpeaks[ik1])+1)>dmax)
					{
						dmax=TPC2DProjX[0]->GetBinContent(((int)xpeaks[ik1])+1);
						indsel=((int)xpeaks[ik1])+1;
					}
				}
				if(indsel!=-1 && dmax>30.)
				{
					sh.x=colpeaks[is1].w;
					sh.y=indsel-128;
					sh.t=colpeaks[is1].t;
					sh.Int.push_back(colpeaks[is1].integral);
					sh.Int.push_back(colpeaks[is1].integral2);
					if(sh.x!=0 && sh.y!=0 && sh.x!=127 && sh.y!=127)
					{
						SH.push_back(sh);
					}
				}
			}
		}
		else
		{
			for(int is1=0;is1<colpeaks.size();is1++)
			{
				indsel=-1;
				dmin=1000.;
				for(int is2=0;is2<indpeaks.size();is2++)
				{
					if(fabs(colpeaks[is1].t-indpeaks[is2].t)<dmin)
					{
						dmin=fabs(colpeaks[is1].t-indpeaks[is2].t);
						indsel=is2;
					}
				}
				if(dmin<5)
				{
					sh.x=colpeaks[is1].w;
					sh.y=indpeaks[indsel].w;
					sh.t=colpeaks[is1].t;
					sh.Int.push_back(colpeaks[is1].integral);
					sh.Int.push_back(colpeaks[is1].integral2);
					if(sh.x!=0 && sh.y!=0 && sh.x!=127 && sh.y!=127)
					{
						SH.push_back(sh);
					}
				}
			}
		}
		
		tg2d->Set(0);
		dmin=1000.;dmax=0.;
		for(int i1=0;i1<SH.size();i1++)
		{
			if(fabs(peakslope[0]*SH[i1].t+peakintercept[0]-((float)SH[i1].x))<=5. && fabs(peakslope[1]*SH[i1].t+peakintercept[1]-((float)SH[i1].y))<=5. && SH[i1].t>1000. && SH[i1].t<2600.)
			{
				tg2d->SetPoint(tg2d->GetN(),SH[i1].x,SH[i1].y,SH[i1].t);
				if(SH[i1].t<dmin) dmin=SH[i1].t;
				if(SH[i1].t>dmax) dmax=SH[i1].t;
			}
		}
		
		if(tg2d->GetN()>0)
		{
			pStart[0]=peakslope[0]*dmin+peakintercept[0];
			pStart[1]=peakslope[0];
			pStart[2]=peakslope[1]*dmin+peakintercept[1];
			pStart[3]=peakslope[1];
			pStart[4]=750;
			
			fitnormchi2=line3Dfit(tg2d);
// 			cout<<"Track fit normchi2 "<<fitnormchi2<<endl;
			
			for(int is1=0;is1<colpeaks.size();is1++)
			{
				if(colpeaks[is1].t<900 || colpeaks[is1].t>2600) continue; 
				if(fabs(fitParams[0]+fitParams[1]*(colpeaks[is1].t-fitParams[4])-colpeaks[is1].w)<=10)
				{
					colpeakssel.push_back(colpeaks[is1]);
				}
			}
			sort(colpeakssel.begin(),colpeakssel.end(),SortPeaksByT);
			
			fitpoints[0]=fitParams[0]+fitParams[1]*(colpeakssel[0].t-fitParams[4]);
			fitpoints[1]=fitParams[2]+fitParams[3]*(colpeakssel[0].t-fitParams[4]);
			fitpoints[2]=colpeakssel[0].t;
			fitpoints[3]=fitParams[0]+fitParams[1]*(colpeakssel[colpeakssel.size()-1].t-fitParams[4]);
			fitpoints[4]=fitParams[2]+fitParams[3]*(colpeakssel[colpeakssel.size()-1].t-fitParams[4]);
			fitpoints[5]=colpeakssel[colpeakssel.size()-1].t;
			
			if(fitpoints[0]<0)
			{
				fitpoints[0]=0.;
				fitpoints[2]=fitParams[4]-(fitParams[0]/fitParams[1]);
				fitpoints[1]=fitParams[2]+fitParams[3]*(fitpoints[2]-fitParams[4]);
// 				cout<<"pushed start col to 0"<<endl;
			}
			if(fitpoints[1]<0)
			{
				fitpoints[1]=0.;
				fitpoints[2]=fitParams[4]-(fitParams[2]/fitParams[3]);
				fitpoints[0]=fitParams[0]+fitParams[1]*(fitpoints[2]-fitParams[4]);
// 				cout<<"pushed start ind to 0"<<endl;
			}
			if(fitpoints[3]<0)
			{
				fitpoints[3]=0.;
				fitpoints[5]=fitParams[4]-(fitParams[0]/fitParams[1]);
				fitpoints[4]=fitParams[2]+fitParams[3]*(fitpoints[5]-fitParams[4]);
// 				cout<<"pushed end col to 0"<<endl;
			}
			if(fitpoints[4]<0)
			{
				fitpoints[4]=0.;
				fitpoints[5]=fitParams[4]-(fitParams[2]/fitParams[3]);
				fitpoints[3]=fitParams[0]+fitParams[1]*(fitpoints[5]-fitParams[4]);
// 				cout<<"pushed end ind to 0"<<endl;
			}
			
			if(fitpoints[0]>127)
			{
				fitpoints[0]=127.;
				fitpoints[2]=fitParams[4]+((127.-fitParams[0])/fitParams[1]);
				fitpoints[1]=fitParams[2]+fitParams[3]*(fitpoints[2]-fitParams[4]);
// 				cout<<"pushed start col to 127"<<endl;
			}
			if(fitpoints[1]>127)
			{
				fitpoints[1]=127.;
				fitpoints[2]=fitParams[4]+((127.-fitParams[2])/fitParams[3]);
				fitpoints[0]=fitParams[0]+fitParams[1]*(fitpoints[2]-fitParams[4]);
// 				cout<<"pushed start ind to 127"<<endl;
			}
			if(fitpoints[3]>127)
			{
				fitpoints[3]=127.;
				fitpoints[5]=fitParams[4]+((127.-fitParams[0])/fitParams[1]);
				fitpoints[4]=fitParams[2]+fitParams[3]*(fitpoints[5]-fitParams[4]);
// 				cout<<"pushed end col to 127"<<endl;
			}
			if(fitpoints[4]>127)
			{
				fitpoints[4]=127.;
				fitpoints[5]=fitParams[4]+((127.-fitParams[2])/fitParams[3]);
				fitpoints[3]=fitParams[0]+fitParams[1]*(fitpoints[5]-fitParams[4]);
// 				cout<<"pushed end ind to 127"<<endl;
			}
			
// 			cout<<"Start x,y,t: "<<fitpoints[0]<<" "<<fitpoints[1]<<" "<<fitpoints[2]<<endl;
// 			cout<<"End x,y,t: "<<fitpoints[3]<<" "<<fitpoints[4]<<" "<<fitpoints[5]<<endl;
// 			cout<<"Colpeaktime first: "<<colpeaks[0].t<<" Colpeaktime last: "<<colpeaks[colpeaks.size()-1].t<<endl;
// 			cout<<"Fraction of hits in the track: "<<(((float)tg2d->GetN())/((float)SH.size()))<<endl;
// 			cout<<"t0= "<<fitParams[4]<<endl;
// 			
// 			tracklength=sqrt((pow(fitpoints[0]-fitpoints[3],2)+pow(fitpoints[1]-fitpoints[4],2))*pow(0.254,2)+pow(fitpoints[5]-fitpoints[2],2)*pow(0.15*0.4,2.));
// 			cout<<"Track length= "<<tracklength<<endl;
			
// 			if(tracklength<25) isTrackValid=false;
			
			outfile<<I<<" "<<fitpoints[0]<<" "<<fitpoints[1]<<" "<<fitpoints[2]<<" "<<fitpoints[3]<<" "<<fitpoints[4]<<" "<<fitpoints[5]<<endl;
			
			hd.E=I;
			hd.ColIndT->clear();
			hd.Int->clear();
			QHitTot=0;
			
			for(int ip1=0;ip1<SH.size();ip1++)
			{
				bb.clear();bc.clear();
				bb.push_back(((int)SH[ip1].x));bb.push_back(((int)SH[ip1].y));bb.push_back(((int)SH[ip1].t));
				bc.push_back(SH[ip1].Int[0]);bc.push_back(SH[ip1].Int[1]);
				hd.ColIndT->push_back(bb);
				hd.Int->push_back(bc);
				QHitTot+=SH[ip1].Int[1];
			}
			
			hd.QHitTot=QHitTot;
			hd.QColTotZS=0;
			
			hd.ColID->clear();
			hd.ColT->clear();
			hd.ColA->clear();
			hd.ColInt->clear();
			hd.Colw->clear();
			hd.IndID->clear();
			hd.IndT->clear();
			hd.IndA->clear();
			hd.IndInt->clear();
			hd.Indw->clear();
			
			QColTot=0;
			for(int is1=0;is1<colpeaks.size();is1++)
			{
				hd.ColID->push_back(colpeaks[is1].w);
				hd.ColT->push_back(colpeaks[is1].t);
				hd.ColA->push_back(colpeaks[is1].amplitude);
				hd.ColInt->push_back(colpeaks[is1].integral2);
				hd.Colw->push_back(colpeaks[is1].wfull);
				QColTot+=colpeaks[is1].integral2;
			}
			hd.QColTot=QColTot;
			
			for(int is1=0;is1<indpeaks.size();is1++)
			{
				hd.IndID->push_back(indpeaks[is1].w);
				hd.IndT->push_back(indpeaks[is1].t);
				hd.IndA->push_back(indpeaks[is1].amplitude);
				hd.IndInt->push_back(indpeaks[is1].integral2);
				hd.Indw->push_back(indpeaks[is1].wfull);
			}
			
			T1->Fill();
			
			td.E=I;
			td.StartEndColIndT->clear();
			td.StartEndColIndT->push_back(fitpoints[0]);
			td.StartEndColIndT->push_back(fitpoints[1]);
			td.StartEndColIndT->push_back(fitpoints[2]);
			td.StartEndColIndT->push_back(fitpoints[3]);
			td.StartEndColIndT->push_back(fitpoints[4]);
			td.StartEndColIndT->push_back(fitpoints[5]);
			td.FitParams->clear();
			for(int is2=0;is2<5;is2++)
			{
				td.FitParams->push_back(fitParams[is2]);
			}
			td.FitNormChi2=fitnormchi2;
			td.QColTot=QColTot;
			td.NHits=tg2d->GetN();
			td.Nexcl=SH.size()-tg2d->GetN();
			
			Ttr->Fill();
		}
	}
	outroot->cd();
	T1->Write();
	Ttr->Write();
	outroot->cd("Histos");
	TPC2DAll[9]->Write();
	
	
	
	outfile.close();
	sprintf(hname,"mv ValidTrackParameters_%d.txt %s/Files/ValidTrackParameters_%d.txt;wait;",RunNo,AnalysisFilePath,RunNo);system(hname);
	outroot->Close();
	sprintf(hname,"mv MIPsAndTracks_%d.root %s/Histos/MIPsAndTracks_%d.root;wait;",RunNo,AnalysisFilePath,RunNo);system(hname);
	inroot->Close();
}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
// 	ReadTPCBaselines();
// 	ReadPMTBaselines();
// 	
// 	TSpectrum *s;
// 	Double_t *noxpeaks;
// 	vector <int> xpeaks;
// 	TH1F* hh[2][128];TH1F* hh2[2][128];TH1F* hh3[2][128];
// 	TH1F* ht[4096];TH1F* ht2[4096];TH1F* ht3[4096];
// 	vector <int> inCL;
// 	double x1(0),y1(0),z1(0),t1(0);
// 	double x2(0),y2(0),z2(0),t2(0);
// 	int nid=0;
// 	
// 	for(int i1=0;i1<2;i1++)
// 	{
// 		for(int i2=0;i2<128;i2++)
// 		{
// 			sprintf(hname,"WF_%s_%d",wt[i1].c_str(),i2);
// 			hh[i1][i2]=new TH1F(hname,hname,4096,-0.5,4095.5);
// 			sprintf(hname,"WFZS_%s_%d",wt[i1].c_str(),i2);
// 			hh2[i1][i2]=new TH1F(hname,hname,4096,-0.5,4095.5);
// 			sprintf(hname,"WFAbs_%s_%d",wt[i1].c_str(),i2);
// 			hh3[i1][i2]=new TH1F(hname,hname,4096,-0.5,4095.5);
// 		}
// 	}
// 	for(int i1=0;i1<4096;i1++)
// 	{	
// 		sprintf(hname,"WFt_%d",i1);
// 		ht[i1]=new TH1F(hname,hname,256,-0.5,255.5);
// 		sprintf(hname,"WFtZS_%d",i1);
// 		ht2[i1]=new TH1F(hname,hname,256,-0.5,255.5);
// 		sprintf(hname,"WFtIndInvZS_%d",i1);
// 		ht3[i1]=new TH1F(hname,hname,256,-0.5,255.5);
// 	}
// 	
// 	TGraph* W_vs_t[2];
// 	W_vs_t[0]=new TGraph();
// 	W_vs_t[0]->SetName("Col_vs_t");
// 	W_vs_t[0]->SetTitle("Col_vs_t");
// 	W_vs_t[0]->SetMarkerStyle(20);
// 	W_vs_t[1]=new TGraph();
// 	W_vs_t[1]->SetName("Ind_vs_t");
// 	W_vs_t[1]->SetTitle("Ind_vs_t");
// 	W_vs_t[1]->SetMarkerStyle(20);
// 	TGraph* Col_vs_t_fit=new TGraph();
// 	Col_vs_t_fit->SetName("Col_vs_t_fit");
// 	Col_vs_t_fit->SetTitle("Col_vs_t_fit");
// 	Col_vs_t_fit->SetMarkerStyle(20);
// 	
// 	TGraph2D* tg2d=new TGraph2D();
// 	sprintf(hname,"Ev3d");
// 	tg2d->SetName(hname);tg2d->SetTitle(hname);
// 	tg2d->SetMarkerStyle(20);tg2d->SetMarkerColor(4);
// 	int nh=0;
// 	vector <hits> H;
// 	TH3F *frame3d=new TH3F("frame3d","frame3d",1,-0.5,127.5,1,-0.5,127.5,1,-0.5,4095.5);
// 	TPolyMarker3D *pm3d1;
// 	TCanvas* cc=new TCanvas("cc","cc",600,600);
// 	cc->cd();
// 	gStyle->SetOptStat(0);
// 	
// 	bool isSaturated=false;
// 	float sg=0.;
// 	float qint=0.;
// 	float QtotTrue=0;
// 	float QtotHits=0;
// 	float QtotCol=0;
// 	
// 	vector <int> bb;
// 	vector <float> bc;
// 	int Ev=0;
// 	int integral=0;int integral2=0;int integral3=0;
// 	int peakbin=0;
// 	int peakbin2=0;
// 	int colminbin=0;int colmaxbin=0;
// 	int indminbin=0;int indmaxbin=0;
// 	int xmin=0;int xmax=0;
// 	int xminpre=0;int xmaxpre=0;
// 	int ic=0;int gch=0;
// 	float QColTot=0;
// 	float QColTotZS=0;
// 	float colindmindeltaT=0;
// 	float tmincl=0;float tmaxcl=0;
// 	float colmincl=0;float colmaxcl=0;
// 	float tminallpt=0;float tmaxallpt=0;
// 	float colminallpt=0;float colmaxallpt=0;
// 	float colhittmin=0;float colhittmax=0;
// 	
// 	float tmintrack=5000;float tmaxtrack=0;
// 	int indoftmin=0;int indoftmax=0;
// 	int Nvalid=0;
// 	float QColtot=0;
// 	float fitnormchi2=0;
// 	
// 	TF1* gf=new TF1("gaus","gaus",0.,5000.);
// 	TF1* lf=new TF1("lin","[0]-[1]*x",0.,5000.);
// 	TF1* tflin=new TF1("lin","[0]+[1]*x",750,2500);
// 	
// 	float minQ=0;
// 	int coltsel=0;int coltseldiff=0;
// 	int indtsel=0;int indtseldiff=0;int indisel=0;
// 	float undershootoffset=0;
// 	int nhitspercolpeak=0;
// 	int nhitsprecol=0;
// 	int nhitspostcol=0;
// 	int maxclsize=0;
// 	int indCLHP=0;
// 	double tflinpars[2][2]={{0}};//col, ind - const, slope
// 	float fitnormchi2col=0;
// 	vector <float> trackXY[2];
// 	
// 	for(int I=0;I<T->GetEntries();I++)
// // 	for(int I=8;I<=8;I++)
// 	{
// 		T->GetEntry(I);
// 		int npeaks = 200;
// 		int nfound=0;
// 		float ss=0;
// 		bool found=true;
// 		bool HitOK=true;
// 		
// 		QColTot=0;QColTotZS=0;
// 		for(int i1=0;i1<256;i1++)
// 		{
// 			ic=i1/128;gch=i1-((i1/128)*128);
// // 			for(int i2=0;i2<4096;i2++)
// 			for(int i2=TimeRange[0];i2<TimeRange[1];i2++)
// 			{
// 				ss=((float)ed.TPCWF->at(i1)[i2])-TPCBaselines[ic][gch][0];
// 				if(i1<128)
// 				{
// 					QColTot+=ss;
// 					if(ss>0) QColTotZS+=ss;
// 					hh[ic][gch]->SetBinContent(hh[ic][gch]->FindBin(i2),ss);
// // 					if(ss>ThColInd[ic])
// 					if(ss>(TPCBaselines[ic][gch][1]*ThSigma[ic]))
// 					{
// // 						ht3[i2]->SetBinContent(ht3[i2]->FindBin(ic*128+gch),ss);
// 						hh2[ic][gch]->SetBinContent(hh2[ic][gch]->FindBin(i2),ss);
// 					}
// 				}
// 				else
// 				{
// 					hh[ic][gch]->SetBinContent(hh[ic][gch]->FindBin(i2),ss);
// 					hh3[ic][gch]->SetBinContent(hh3[ic][gch]->FindBin(i2),abs(ss));
// 					ss*=-1;
// // 					if(ss>ThColInd[ic])
// 					if(ss>(TPCBaselines[ic][gch][1]*ThSigma[ic]))
// 					{
// // // 						ht3[i2]->SetBinContent(ht3[i2]->FindBin(ic*128+gch),ss);
// 						hh2[ic][gch]->SetBinContent(hh2[ic][gch]->FindBin(i2),ss);
// 					}
// 				}
// 			}
// 		}
// 		QtotCol=0.;
// 		tminallpt=5000;tmaxallpt=0;
// 		colminallpt=200;colmaxallpt=0;
// 		for(int i1=0;i1<2;i1++)
// 		{
// 			W_vs_t[i1]->Set(0);
// 			for(int i2=0;i2<128;i2++)
// 			{
// 				for(int is1=1;is1<=hh2[i1][i2]->GetNbinsX();is1++)
// 				{
// 					if(hh2[i1][i2]->GetBinContent(is1)>0)
// 					{
// 						if(i1==0)
// 						{
// 							QtotCol+=hh2[i1][i2]->GetBinContent(is1);
// 							if(i2>colmaxallpt) colmaxallpt=i2;
// 							if(i2<colminallpt) colminallpt=i2;
// 							if(hh2[i1][i2]->GetBinCenter(is1)>tmaxallpt) tmaxallpt=hh2[i1][i2]->GetBinCenter(is1);
// 							if(hh2[i1][i2]->GetBinCenter(is1)<tminallpt) tminallpt=hh2[i1][i2]->GetBinCenter(is1);
// 						}
// 						W_vs_t[i1]->SetPoint(W_vs_t[i1]->GetN(),hh2[i1][i2]->GetBinCenter(is1),i2);
// 					}
// 				}
// 			}
// 		}
// 		HQTotCol->Fill(QtotCol);
// 		NPoints->Fill(W_vs_t[0]->GetN());
// 		NPointsperArea->Fill(((float)W_vs_t[0]->GetN())/((tmaxallpt-tminallpt)*(colmaxallpt-colminallpt)));
// 		TotalArea->Fill((tmaxallpt-tminallpt)*(colmaxallpt-colminallpt));
// 		
// 		
// 		
// 		
// // // 		Fill the Hit Candidates, collection then induction
// 		vector <hitcandidateshort> HCS[2][128];
// 		vector <hitcandidateshort> HCSCand;
// 		int startbin=0;int nextstartbin=0;int endbin=0;int minbin=0;int maxbin=0;
// 		int prepeakbin=0;int midpeakbin=0;
// 		float fitminx=0;float fitmaxx=0;
// 		QtotTrue=0;
// 		for(int i2=0;i2<128;i2++)
// 		{
// 			QtotTrue+=hh2[0][i2]->Integral();
// // 			cout<<"0 "<<i2<<endl;
// 			startbin=FindBinAbove(hh2[0][i2],0,1);
// 			while(startbin>=0)
// 			{
// 				peakbin=startbin;
// 				prepeakbin=startbin;
// // 				for(int ik1=startbin;ik1<hh2[0][i2]->GetNbinsX();ik1++)
// 				for(int ik1=startbin;ik1<hh2[0][i2]->FindBin(TimeRange[1]);ik1++)
// 				{
// 					if(hh2[0][i2]->GetBinContent(ik1+1)>=hh2[0][i2]->GetBinContent(ik1))
// 					{
// 						peakbin=ik1+1;
// 					}
// 					else
// 					{
// 						for(int ik2=peakbin;ik2>=startbin;ik2--)
// 						{
// 							if(hh2[0][i2]->GetBinContent(ik2)==hh2[0][i2]->GetBinContent(peakbin)){prepeakbin=ik2;}
// 							else{break;}
// 						}
// 						maxbin=peakbin;
// // 						midpeakbin=(peakbin+prepeakbin)/2;
// // 						for(int ik2=peakbin;ik2<hh2[0][i2]->GetNbinsX();ik2++)
// 						for(int ik2=peakbin;ik2<hh2[0][i2]->FindBin(TimeRange[1]);ik2++)
// 						{
// // 							if(hh2[0][i2]->GetBinContent(ik2+1)>hh2[0][i2]->GetBinContent(ik2))
// 							if(hh[0][i2]->GetBinContent(ik2+1)>hh[0][i2]->GetBinContent(ik2))
// 							{
// // 								endbin=ik2;
// 								maxbin=ik2;
// // 								nextstartbin=ik2+1;
// 								break;
// 							}
// // 							else if(hh2[0][i2]->GetBinContent(ik2)==0)
// // 							{
// // 								endbin=ik2-1;
// // 								nextstartbin=FindBinAbove(hh2[0][i2],0,ik2);
// // 								break;
// // 							}
// 						}
// 						minbin=peakbin;
// // 						for(int ik2=prepeakbin-1;ik2>1;ik2--)
// 						for(int ik2=prepeakbin-1;ik2>hh2[0][i2]->FindBin(TimeRange[0]);ik2--)
// 						{
// // 							if(hh[0][i2]->GetBinContent(ik2-1)>=hh[0][i2]->GetBinContent(ik2))
// 							if(hh[0][i2]->GetBinContent(ik2-1)>hh[0][i2]->GetBinContent(ik2))
// 							{
// 								minbin=ik2;
// 								break;
// 							}
// 						}
// 						for(int ik2=minbin;ik2<peakbin;ik2++)
// 						{
// 							if(hh[0][i2]->GetBinContent(ik2+1)>hh[0][i2]->GetBinContent(ik2))
// 							{
// 								minbin=ik2+1;
// 								break;
// 							}
// 						}
// // 						if((endbin-startbin)>3)
// 						if((maxbin-minbin)>3)
// 						{
// // 							fitminx=(((prepeakbin-2)>=1)?(hh2[0][i2]->GetBinCenter(prepeakbin-2)-0.1):0.);
// 							fitminx=(((prepeakbin-2)>=minbin)?(hh2[0][i2]->GetBinCenter(prepeakbin-2)-0.1):(hh2[0][i2]->GetBinCenter(minbin)-0.1));
// // 							fitmaxx=(((peakbin+2)<=endbin)?(hh2[0][i2]->GetBinCenter(peakbin+2)+0.1):(hh2[0][i2]->GetBinCenter(endbin)+0.1));
// 							fitmaxx=(((peakbin+2)<=maxbin)?(hh2[0][i2]->GetBinCenter(peakbin+2)+0.1):(hh2[0][i2]->GetBinCenter(maxbin)+0.1));
// 							hh2[0][i2]->Fit(gf,"q","q",fitminx,fitmaxx);
// 							hcs.t=gf->GetParameter(1);
// 							hcs.Int=0;
// 							hcs.ci=0;
// 							hcs.wid=i2;
// // 							hcs.w=endbin-startbin+1;
// 							hcs.w=maxbin-minbin+1;
// 							
// 							undershootoffset=0.;
// // 							if(hh[0][i2]->GetBinContent(minbin)<0 || hh[0][i2]->GetBinContent(maxbin)<0)
// // 							{
// // 								undershootoffset=hh[0][i2]->GetBinContent(minbin);
// // 								if(hh[0][i2]->GetBinContent(maxbin)<undershootoffset)
// // 								{
// // 									undershootoffset=hh[0][i2]->GetBinContent(maxbin);
// // 								}
// // 							}
// 							
// // 							hcs.a=gf->GetParameter(0)+fabs(undershootoffset);
// 							hcs.a=gf->GetParameter(0);
// // 							for(int ik2=startbin;ik2<=endbin;ik2++)
// 							for(int ik2=minbin;ik2<=maxbin;ik2++)
// 							{
// // 								hcs.Int+=hh2[0][i2]->GetBinContent(ik2);
// // 								hcs.Int+=(hh[0][i2]->GetBinContent(ik2)+fabs(undershootoffset));
// 								if(hh[0][i2]->GetBinContent(ik2)>0) hcs.Int+=(hh[0][i2]->GetBinContent(ik2));
// 							}
// 							HCS[0][i2].push_back(hcs);
// 						}
// 						nextstartbin=maxbin+1;
// 						break;
// 					}
// 				}
// // 				cout<<I<<" 0 "<<i2<<" "<<startbin<<" "<<peakbin<<" "<<minbin<<" "<<maxbin<<" "<<nextstartbin<<" "<<FindBinAbove(hh2[0][i2],0,nextstartbin)<<endl;
// // 				startbin=nextstartbin;
// 				startbin=FindBinAbove(hh2[0][i2],0,nextstartbin);
// // 				if((hh2[0][i2]->FindBin(TimeRange[1])-startbin)<10) break;
// // 				if(startbin==hh2[0][i2]->FindBin(2500)) startbin=-1;
// 			}
// 		}
// 		for(int i2=0;i2<128;i2++)
// 		{
// // 			cout<<"1 "<<i2<<endl;
// 			startbin=FindBinAbove(hh3[1][i2],ThColInd[1],1);
// 			while(startbin>=0)
// 			{
// // 				cout<<I<<" 1 "<<i2<<" "<<startbin<<endl;
// 				
// // 				minbin=((startbin-1)>=1)?(startbin-1):1;
// // 				maxbin=((startbin+1)<=hh[1][i2]->GetNbinsX())?(startbin+1):hh[1][i2]->GetNbinsX();
// 				while(!((hh[1][i2]->GetBinContent(startbin)<hh[1][i2]->GetBinContent(startbin-1))&&(hh[1][i2]->GetBinContent(startbin)>hh[1][i2]->GetBinContent(startbin+1))))
// 				{
// 					startbin=FindBinAbove(hh3[1][i2],ThColInd[1],startbin+1);
// 					if(startbin==-1) break;
// 				}
// 				if(startbin==-1) break;
// 				for(int ik1=startbin;ik1<hh[1][i2]->GetNbinsX();ik1++)
// 				{
// 					if(hh[1][i2]->GetBinContent(ik1+1)>=hh[1][i2]->GetBinContent(ik1))
// 					{
// 						maxbin=ik1;
// 						break;
// 					}
// 				}
// 				for(int ik1=startbin;ik1>1;ik1--)
// 				{
// 					if(hh[1][i2]->GetBinContent(ik1-1)<=hh[1][i2]->GetBinContent(ik1))
// 					{
// 						minbin=ik1;
// 						break;
// 					}
// 				}
// 				if((maxbin-minbin)>3 && ((hh[1][i2]->GetBinContent(minbin)-hh[1][i2]->GetBinContent(maxbin))>(ThColInd[1]*2.)))
// 				{
// 					midpeakbin=(minbin+maxbin)/2;
// 					fitminx=(((midpeakbin-2)>=minbin)?(hh[1][i2]->GetBinCenter(midpeakbin-2)-0.1):(hh[1][i2]->GetBinCenter(minbin)-0.1));
// 					fitmaxx=(((midpeakbin+2)<=maxbin)?(hh[1][i2]->GetBinCenter(midpeakbin+2)+0.1):(hh[1][i2]->GetBinCenter(maxbin)+0.1));
// 					hh[1][i2]->Fit(lf,"q","q",fitminx,fitmaxx);
// 					hcs.t=(lf->GetParameter(0)/lf->GetParameter(1));
// 					hcs.Int=0;
// 					hcs.ci=0;
// 					hcs.wid=i2;
// 					hcs.w=maxbin-minbin+1;
// 					hcs.a=hh[1][i2]->GetBinContent(minbin)-hh[1][i2]->GetBinContent(maxbin);
// 					HCS[1][i2].push_back(hcs);
// 				}
// 				startbin=FindBinAbove(hh3[1][i2],ThColInd[1],maxbin);
// 			}
// 		}
// 		vector <hits> Hits;
// // 		for(int i1=0;i1<128;i1++)
// // 		{
// // 			for(int ik1=0;ik1<HCS[0][i1].size();ik1++)
// // 			{
// // 				colindmindeltaT=10000.;
// // 				nhitspercolpeak=0;
// // 				for(int i2=0;i2<128;i2++)
// // 				{
// // 					for(int ik2=0;ik2<HCS[1][i2].size();ik2++)
// // 					{
// // 						if(fabs(HCS[0][i1][ik1].t-(HCS[1][i2][ik2].t+5.))<fabs(colindmindeltaT))
// // 						{
// // 							colindmindeltaT=HCS[0][i1][ik1].t-(HCS[1][i2][ik2].t+5.);
// // 						}
// // 						if(fabs(HCS[0][i1][ik1].t-(HCS[1][i2][ik2].t+5.))<=2.)
// // 						{
// // 							hit.col=i1;
// // 							hit.ind=i2;
// // 							hit.t=((int)HCS[0][i1][ik1].t);
// // 							hit.Int=HCS[0][i1][ik1].Int+HCS[1][i2][ik2].Int;
// // 							hit.Int2=HCS[1][i2][ik2].Int;
// // 							hit.Int3=HCS[0][i1][ik1].Int;
// // 							HitCol->Fill(hit.col);
// // 							HitInd->Fill(hit.ind);
// // 							HitTime->Fill(hit.t);
// // 							HitIntegral1->Fill(hit.Int);
// // 							HitIntegral2->Fill(hit.Int2);
// // 							HitIntegral3->Fill(hit.Int3);
// // 							HitOK=true;
// // // 							if(hit.col==0 || hit.col==127 || hit.ind==0 || hit.ind==127) HitOK=false;
// // 	// 						if(RunNo==7074 && hit.col==93) HitOK=false;
// // 	// 						if(RunNo==7076 && hit.col==93) HitOK=false;
// // 	// 						if(RunNo==7084 && hit.col==88) HitOK=false;
// // 							
// // 							hit.colT=HCS[0][i1][ik1].t;
// // 							hit.indT=HCS[1][i2][ik2].t;
// // 							hit.colWidth=HCS[0][i1][ik1].w;
// // 							hit.indWidth=HCS[1][i2][ik2].w;
// // 							hit.colAmp=HCS[0][i1][ik1].a;
// // 							hit.indAmp=HCS[1][i2][ik2].a;
// // 							
// // 							if(HitOK)
// // 							{
// // 								Hits.push_back(hit);
// // 							}
// // 							nhitspercolpeak++;
// // 						}
// // 					}
// // 				}
// // 				ColIndMinDeltaT5->Fill(colindmindeltaT);
// // 				NHitsPerColPeak->Fill(nhitspercolpeak);
// // 			}
// // 		}
// 		for(int i1=0;i1<128;i1++)
// 		{
// 			for(int ik1=0;ik1<HCS[0][i1].size();ik1++)
// 			{
// 				colindmindeltaT=10000.;
// 				nhitspercolpeak=0;
// 				
// 				if(HCS[0][i1][ik1].a<(ThColInd[0]*1.5)) continue;
// 				nhitsprecol=((int)(HCS[0][i1][ik1].Int/400.))+1;
// 				HCSCand.clear();
// 				
// 				for(int i2=0;i2<128;i2++)
// 				{
// 					for(int ik2=0;ik2<HCS[1][i2].size();ik2++)
// 					{
// 						if(fabs(HCS[0][i1][ik1].t-(HCS[1][i2][ik2].t+HitColIndDist))<fabs(colindmindeltaT))
// 						{
// 							colindmindeltaT=HCS[0][i1][ik1].t-(HCS[1][i2][ik2].t+HitColIndDist);
// 						}
// 						if(fabs(HCS[0][i1][ik1].t-(HCS[1][i2][ik2].t+HitColIndDist))>2.) continue;
// 						if(HCSCand.size()==0)
// 						{
// 							HCSCand.push_back(HCS[1][i2][ik2]);
// 							continue;
// 						}
// 						for(int ip1=0;ip1<HCSCand.size();ip1++)
// 						{
// 							if(fabs(HCS[0][i1][ik1].t-(HCS[1][i2][ik2].t+HitColIndDist))<fabs(HCS[0][i1][ik1].t-(HCSCand[ip1].t+HitColIndDist)))
// 							{
// 								HCSCand.push_back(HCS[1][i2][ik2]);
// 								for(int ip2=ip1+1;ip2<HCSCand.size();ip2++)
// 								{
// 									HCSCand[ip2]=HCSCand[ip2-1];
// 								}
// 								HCSCand[ip1]=HCS[1][i2][ik2];
// 								break;
// 							}
// 						}
// 					}
// 				}
// // 				if(nhitsprecol<HCSCand.size()) nhitspostcol=nhitsprecol;
// // 				else nhitspostcol=HCSCand.size();
// 				nhitspostcol=HCSCand.size();
// 				NHitsPerColPeak->Fill(nhitspostcol);
// 				if(nhitspostcol==0) continue;
// 				for(int ip1=0;ip1<HCSCand.size();ip1++)
// 				{
// 					hit.col=i1;
// 					hit.ind=HCSCand[ip1].wid;
// 					hit.t=((int)HCS[0][i1][ik1].t);
// // 					hit.Int=(HCS[0][i1][ik1].Int/((float)nhitspostcol))+HCSCand[ip1].Int;
// // 					hit.Int2=HCSCand[ip1].Int;
// // 					hit.Int3=(HCS[0][i1][ik1].Int/((float)nhitspostcol));
// 					hit.Int=(HCS[0][i1][ik1].Int/((float)nhitspostcol));
// 					hit.Int2=HCS[0][i1][ik1].Int;
// 					hit.Int3=(HCS[0][i1][ik1].Int/((float)nhitspostcol));
// 					HitCol->Fill(hit.col);
// 					HitInd->Fill(hit.ind);
// 					HitTime->Fill(hit.t);
// 					HitIntegral1->Fill(hit.Int);
// 					HitIntegral2->Fill(hit.Int2);
// 					HitIntegral3->Fill(hit.Int3);
// 					HitOK=true;
// // 					if(hit.col==0 || hit.col==127 || hit.ind==0 || hit.ind==127) HitOK=false;
// // 					if(RunNo==7074 && hit.col==93) HitOK=false;
// // 					if(RunNo==7076 && hit.col==93) HitOK=false;
// // 					if(RunNo==7084 && hit.col==88) HitOK=false;
// 					
// 					hit.colT=HCS[0][i1][ik1].t;
// 					hit.indT=HCSCand[ip1].t;
// 					hit.colWidth=HCS[0][i1][ik1].w;
// 					hit.indWidth=HCSCand[ip1].w;
// 					hit.colAmp=HCS[0][i1][ik1].a;
// 					hit.indAmp=HCSCand[ip1].a;
// 					
// 					if(HitOK)
// 					{
// 						Hits.push_back(hit);
// 					}
// 					nhitspercolpeak++;
// 					if(nhitspercolpeak==nhitspostcol) break;
// 				}
// 				ColIndMinDeltaT5->Fill(colindmindeltaT);
// 			}
// 		}
// 		
// 		NHits->Fill(Hits.size());
// 		
// 		hd.E=I;
// 		hd.QColTot=QColTot;
// 		hd.QColTotZS=QColTotZS;
// 		hd.ColIndT->clear();
// 		hd.Int->clear();
// 		int mins[3]={5000,5000,5000};int maxs[3]={0,0,0};//Col, ind, t
// 		
// // 		cc->cd();
// // 		frame3d->Draw();
// // // 		pm3d1 = new TPolyMarker3D(H.size());
// // 		pm3d1 = new TPolyMarker3D();
// 		
// 		QtotHits=0;
// 		
// 		for(int i1=0;i1<Hits.size();i1++)
// 		{
// 			bb.clear();bc.clear();
// 			bb.push_back(Hits[i1].col);bb.push_back(Hits[i1].ind);bb.push_back(Hits[i1].t);
// 			bc.push_back(Hits[i1].Int);bc.push_back(Hits[i1].Int2);bc.push_back(Hits[i1].Int3);
// 			hd.ColIndT->push_back(bb);
// 			hd.Int->push_back(bc);
// 			if(Hits[i1].col<mins[0]) mins[0]=Hits[i1].col;if(Hits[i1].col>maxs[0]) maxs[0]=Hits[i1].col;
// 			if(Hits[i1].ind<mins[1]) mins[1]=Hits[i1].ind;if(Hits[i1].ind>maxs[1]) maxs[1]=Hits[i1].ind;
// 			if(Hits[i1].t<mins[2]) mins[2]=Hits[i1].t;if(Hits[i1].t>maxs[2]) maxs[2]=Hits[i1].t;
// 			
// 			ColIndDeltaT->Fill(Hits[i1].colT-(Hits[i1].indT+5.));
// 			ColWidth->Fill(Hits[i1].colWidth);
// 			IndWidth->Fill(Hits[i1].indWidth);
// 			ColAmp->Fill(Hits[i1].colAmp);
// 			IndAmp->Fill(Hits[i1].indAmp);
// 			Amp_Col_vs_Ind->Fill(Hits[i1].indAmp,Hits[i1].colAmp);
// 			Width_Col_vs_Ind->Fill(Hits[i1].indWidth,Hits[i1].colWidth);
// 			QtotHits+=Hits[i1].Int3;
// 			Col_Int_vs_Amp->Fill(Hits[i1].colAmp,Hits[i1].Int3);
// 			
// 			AllXY->Fill(Hits[i1].col,Hits[i1].ind);
// 			
// // 			if(Hits[i1].Int3<30 || Hits[i1].Int3>2000)
// // 			{
// // 				cout<<I<<" "<<Hits[i1].col<<" "<<Hits[i1].ind<<" "<<Hits[i1].t<<" "<<Hits[i1].Int3<<endl;
// // 			}
// 			
// // 			cout<<i1<<" "<<Hits[i1].col<<" "<<Hits[i1].ind<<" "<<Hits[i1].t<<endl;
// // 			pm3d1->SetPoint(i1,Hits[i1].col,Hits[i1].ind,Hits[i1].t);
// 		}
// 		
// 		hd.QHitTot=QtotHits;
// 		
// 		HitIntegralLost->Fill(QtotTrue-QtotHits);
// 		TrueTotalIntegral->Fill(QtotTrue);
// 		AllDeltaT->Fill(maxs[2]-mins[2]);
// 		
// // 		pm3d1->SetMarkerColor(kRed);
// // 		pm3d1->SetMarkerStyle(24);   
// // 		pm3d1->Draw();
// // 		sprintf(hname,"Ev_%d",I);
// // 		cc->SetName(hname);cc->SetTitle(hname);
// // 		outroot->cd();
// // 		cc->Write();
// 		
// 		StartTime->Fill(mins[2]);EndTime->Fill(maxs[2]);
// 		StartCollection->Fill(mins[0]);EndCollection->Fill(maxs[0]);
// 		StartInduction->Fill(mins[1]);EndInduction->Fill(maxs[1]);
// 		
// 		hd.PMTIntegral->clear();
// 		for(int i1=0;i1<3;i1++)
// 		{
// 			isSaturated=false;
// 			qint=0.;
// // 			for(int i2=0;i2<ed.PMTWF->at(i1).size();i2++)
// 			for(int i2=1500;i2<7500;i2++)
// 			{
// 				sg=(-1.)*(((float)ed.PMTWF->at(i1)[i2])-PMTBaselines[i1][0]);
// 				if(sg>ThPMT[i1]) qint+=sg;
// 				if(ed.PMTWF->at(i1)[i2]==0){isSaturated=true;}
// 			}
// 			if(isSaturated) qint*=-1.;
// 			hd.PMTIntegral->push_back(qint);
// 		}
// 		
// 		hd.ColID->clear();
// 		hd.ColT->clear();
// 		hd.ColA->clear();
// 		hd.ColInt->clear();
// 		hd.Colw->clear();
// 		hd.IndID->clear();
// 		hd.IndT->clear();
// 		hd.IndA->clear();
// 		hd.Indw->clear();
// 		for(int i1=0;i1<128;i1++)
// 		{
// 			for(int ik1=0;ik1<HCS[0][i1].size();ik1++)
// 			{
// 				hd.ColID->push_back(HCS[0][i1][ik1].wid);
// 				hd.ColT->push_back(HCS[0][i1][ik1].t);
// 				hd.ColA->push_back(HCS[0][i1][ik1].a);
// 				hd.ColInt->push_back(HCS[0][i1][ik1].Int);
// 				hd.Colw->push_back(HCS[0][i1][ik1].w);
// 			}
// 			for(int ik1=0;ik1<HCS[1][i1].size();ik1++)
// 			{
// 				hd.IndID->push_back(HCS[1][i1][ik1].wid);
// 				hd.IndT->push_back(HCS[1][i1][ik1].t);
// 				hd.IndA->push_back(HCS[1][i1][ik1].a);
// 				hd.Indw->push_back(HCS[1][i1][ik1].w);
// 			}
// 		}
// 		
// 		if(W_vs_t[0]->GetN()<1000) hd.EventType=3;
// 		else if(W_vs_t[0]->GetN()>4000) hd.EventType=2;
// 		else	//going into track fitting
// 		{
// 			hd.EventType=1;//assume partial traversing, then identify
// 			vector <clusters> CL;
// 			cl.hi.clear();
// 			inCL.clear();
// 			for(int i1=0;i1<W_vs_t[0]->GetN();i1++)
// 			{
// 				inCL.push_back(0);
// 			}
// 			for(int i1=0;i1<W_vs_t[0]->GetN();i1++)
// 			{
// 				if(inCL[i1]!=0) continue;
// 				cl.hi.clear();
// 				cl.hi.push_back(i1);
// 				inCL[i1]=1;
// 				nid=1;
// 				while(nid>0)
// 				{
// 					nid=0;
// 					for(int i2=0;i2<W_vs_t[0]->GetN();i2++)
// 					{
// 						if(inCL[i2]!=0) continue;
// 						W_vs_t[0]->GetPoint(i2,x2,y2);
// 						for(int i3=0;i3<cl.hi.size();i3++)
// 						{
// 							W_vs_t[0]->GetPoint(cl.hi[i3],x1,y1);
// 							if(fabs(x1-x2)<=1 && fabs(y1-y2)<=1)
// 							{
// 								cl.hi.push_back(i2);
// 								nid++;
// 								inCL[i2]=1;
// 								break;
// 							}
// 						}
// 					}
// 				}
// 				CL.push_back(cl);
// 				cl.hi.clear();
// 			}
// 			maxclsize=0;
// 			for(int i1=0;i1<CL.size();i1++)
// 			{
// 				tmincl=5000;tmaxcl=0;
// 				colmincl=200;colmaxcl=0;
// 				for(int i2=0;i2<CL[i1].hi.size();i2++)
// 				{
// 					W_vs_t[0]->GetPoint(CL[i1].hi[i2],x1,y1);
// 					if(x1<tmincl) tmincl=x1;if(x1>tmaxcl) tmaxcl=x1;
// 					if(y1<colmincl) colmincl=y1;if(y1>colmaxcl) colmaxcl=y1;
// 	// 				if(x1<tminallpt) tminallpt=x1;if(x1>tmaxallpt) tmaxallpt=x1;
// 	// 				if(y1<colminallpt) colminallpt=y1;if(y1>colmaxallpt) colmaxallpt=y1;
// 				}
// 				ClusterNPointsperArea->Fill(((float)CL[i1].hi.size())/((tmaxcl-tmincl)*(colmaxcl-colmincl)));
// 				ClSize->Fill(CL[i1].hi.size());
// 				if(CL[i1].hi.size()>maxclsize) {maxclsize=CL[i1].hi.size();indCLHP=i1;}
// 			}
// 			MaxClSize->Fill(maxclsize);
// 			NClperEvent->Fill(CL.size());
// 			Col_vs_t_fit->Set(0);
// 			tmincl=5000;tmaxcl=0;
// 			colmincl=200;colmaxcl=0;
// 			for(int i1=0;i1<CL[indCLHP].hi.size();i1++)
// 			{
// 				W_vs_t[0]->GetPoint(CL[indCLHP].hi[i1],x1,y1);
// 				Col_vs_t_fit->SetPoint(Col_vs_t_fit->GetN(),x1,y1);
// 				if(x1>tmaxcl) tmaxcl=x1;if(x1<tmincl) tmincl=x1;
// 				if(y1>colmaxcl) colmaxcl=y1;if(y1<colmincl) colmincl=y1;
// 			}
// 			tflin->SetParameter(0,colmincl);
// 			tflin->SetParameter(1,((float)(colmaxcl-colmincl))/(tmaxcl-tmincl));
// 			Col_vs_t_fit->Fit(tflin,"q","q",tmincl-1,tmaxcl+1);
// 			fitnormchi2col=((float)tflin->GetChisquare())/((float)tflin->GetNDF());
// 			tflinpars[0][0]=tflin->GetParameter(0);tflinpars[0][1]=tflin->GetParameter(1);
// 			
// 			FitNormChi2_Col_vs_t->Fill(fitnormchi2col);
// 			ColTrackDeltaT->Fill(tmaxcl-tmincl);
// 			ClmaxPointFraction->Fill(((float)CL[indCLHP].hi.size())/((float)W_vs_t[0]->GetN()));
// 			
// // 			cout<<I<<" "<<(((float)CL[indCLHP].hi.size())/((float)W_vs_t[0]->GetN()))<<endl;
// 			if((((float)CL[indCLHP].hi.size())/((float)W_vs_t[0]->GetN()))>0.8)
// // 			if(fitnormchi2col<20)
// 			{
// // 				cout<<I<<" passed point fraction"<<endl;
// 				//for the track
// 				tmintrack=5000;tmaxtrack=0;
// 				indoftmin=0;indoftmax=0;Nvalid=0;
// 				
// 				tg2d->Set(0);
// 				trackXY[0].clear();trackXY[1].clear();
// 				for(int i1=0;i1<hd.ColIndT->size();i1++)
// 				{
// 					if(float(hd.ColIndT->at(i1)[2])<tmincl || float(hd.ColIndT->at(i1)[2])>tmaxcl) continue;
// 					x1=tflinpars[0][0]+tflinpars[0][1]*((float)hd.ColIndT->at(i1)[2]);
// 					if(fabs(x1-((float)hd.ColIndT->at(i1)[0]))<=5.)
// 					{
// 						tg2d->SetPoint(tg2d->GetN(),hd.ColIndT->at(i1)[0],hd.ColIndT->at(i1)[1],hd.ColIndT->at(i1)[2]);
// 						Nvalid++;
// 						if(((float)hd.ColIndT->at(i1)[2])>tmaxtrack) {tmaxtrack=hd.ColIndT->at(i1)[2];indoftmax=i1;}
// 						if(((float)hd.ColIndT->at(i1)[2])<tmintrack) {tmintrack=hd.ColIndT->at(i1)[2];indoftmin=i1;}
// 						trackXY[0].push_back(hd.ColIndT->at(i1)[0]);
// 						trackXY[1].push_back(hd.ColIndT->at(i1)[1]);
// 					}
// 				}
// 				FitClSize->Fill(Nvalid);
// 				if(Nvalid>=5)
// 				{
// // 					cout<<I<<" passed nvalid"<<endl;
// 					pStart[0]=hd.ColIndT->at(indoftmin)[0];
// 					pStart[1]=((float)(hd.ColIndT->at(indoftmax)[0]-hd.ColIndT->at(indoftmin)[0]))/((float)(hd.ColIndT->at(indoftmax)[2]-hd.ColIndT->at(indoftmin)[2]));
// 					pStart[2]=hd.ColIndT->at(indoftmin)[1];
// 					pStart[3]=((float)(hd.ColIndT->at(indoftmax)[1]-hd.ColIndT->at(indoftmin)[1]))/((float)(hd.ColIndT->at(indoftmax)[2]-hd.ColIndT->at(indoftmin)[2]));
// 					pStart[4]=750;
// 					
// 					fitnormchi2=line3Dfit(tg2d);
// 					
// 					if(fitnormchi2<10)
// 					{
// // 						cout<<I<<" passed normchi2"<<endl;
// 						TrackDeltaT->Fill(tmaxtrack-tmintrack);
// 						FitNormChi2->Fill(fitnormchi2);
// 						
// 						
// 						td.E=hd.E;
// 						td.StartEndColIndT->clear();
// 						td.StartEndColIndT->push_back(hd.ColIndT->at(indoftmin)[0]);
// 						td.StartEndColIndT->push_back(hd.ColIndT->at(indoftmin)[1]);
// 						td.StartEndColIndT->push_back(hd.ColIndT->at(indoftmin)[2]);
// 						td.StartEndColIndT->push_back(hd.ColIndT->at(indoftmax)[0]);
// 						td.StartEndColIndT->push_back(hd.ColIndT->at(indoftmax)[1]);
// 						td.StartEndColIndT->push_back(hd.ColIndT->at(indoftmax)[2]);
// 						td.FitParams->clear();
// 						for(int is2=0;is2<5;is2++)
// 						{
// 							td.FitParams->push_back(fitParams[is2]);
// 						}
// 						td.FitNormChi2=fitnormchi2;
// 						td.QColTot=QtotCol;
// 						td.NHits=Nvalid;
// 						td.Nexcl=hd.ColIndT->size()-Nvalid;
// 						td.PMTIntegral->clear();
// 						for(int is2=0;is2<3;is2++)
// 						{
// 							td.PMTIntegral->push_back(hd.PMTIntegral->at(is2));
// 						}
// 						
// 						td.ColTStartEnd->clear();
// 						td.ColHitTStartEnd->clear();
// 						
// 						td.ColTStartEnd->push_back(tmincl);
// 						td.ColTStartEnd->push_back(tmaxcl);
// 						
// 						colhittmin=5000;colhittmax=0;
// 						for(int i1=0;i1<128;i1++)
// 						{
// 							for(int ik1=0;ik1<HCS[0][i1].size();ik1++)
// 							{
// 								x1=tflinpars[0][0]+tflinpars[0][1]*HCS[0][i1][ik1].t;
// 								if(fabs(x1-((float)i1))<=5)
// 								{
// 									if(HCS[0][i1][ik1].t<colhittmin) colhittmin=HCS[0][i1][ik1].t;
// 									if(HCS[0][i1][ik1].t>colhittmax) colhittmax=HCS[0][i1][ik1].t;
// 								}
// 							}
// 						}
// 						td.ColHitTStartEnd->push_back(colhittmin);
// 						td.ColHitTStartEnd->push_back(colhittmax);
// 						
// 						Ttr->Fill();
// 						
// 						for(int ip1=0;ip1<trackXY[0].size();ip1++)
// 						{
// 							TrackXY->Fill(trackXY[0][ip1],trackXY[1][ip1]);
// 						}
// 						
// // 						//track timing
// // 						x1=fitParams[0]+fitParams[1]*(tmintrack-fitParams[4]);
// // 						y1=fitParams[2]+fitParams[3]*(tmintrack-fitParams[4]);
// // 						z1=tmintrack;
// // 						
// // 						x2=fitParams[0]+fitParams[1]*(tmaxtrack-fitParams[4]);
// // 						y2=fitParams[2]+fitParams[3]*(tmaxtrack-fitParams[4]);
// // 						z2=tmaxtrack;
// 						
// 						//collection hit timing
// 						x1=fitParams[0]+fitParams[1]*(colhittmin-fitParams[4]);
// 						y1=fitParams[2]+fitParams[3]*(colhittmin-fitParams[4]);
// 						z1=colhittmin;
// 						
// 						x2=fitParams[0]+fitParams[1]*(colhittmax-fitParams[4]);
// 						y2=fitParams[2]+fitParams[3]*(colhittmax-fitParams[4]);
// 						z2=colhittmax;
// 						
// 						
// 			// 			DeltaTvsTrackAngle->Fill(atan(sqrt(pow(x2-x1,2)+pow(y2-y1,2))/(tmax-tmin)),(tmax-tmin));
// 			// 			DeltaTvsTrackAnglePr->Fill(atan(sqrt(pow(x2-x1,2)+pow(y2-y1,2))/(tmax-tmin)),(tmax-tmin));
// 						Tfirst->Fill(tmintrack);
// 						Tlast->Fill(tmaxtrack);
// 						TColfirst->Fill(colhittmin);
// 						TCollast->Fill(colhittmax);
// 						Trackt0->Fill(fitParams[4]);
// 						TrackEntryXY->Fill(x1,y1);
// 						TrackExitXY->Fill(x2,y2);
// 			// 			if(tmin<=1160 && tmax>=1920) TrackDeltaTSel->Fill(tmax-tmin);
// 						DeltaTvsTrackt0->Fill(fitParams[4],tmaxtrack-tmintrack);
// 			// 			DeltaTLengthvsAngle->Fill(atan(sqrt(pow(x2-x1,2)+pow(y2-y1,2))/(tmax-tmin)),sqrt(pow(x2-x1,2)+pow(y2-y1,2)+pow(tmax-tmin,2)),tmax-tmin);
// 			// 			DeltaTLengthvsAngle_norm->Fill(atan(sqrt(pow(x2-x1,2)+pow(y2-y1,2))/(tmax-tmin)),sqrt(pow(x2-x1,2)+pow(y2-y1,2)+pow(tmax-tmin,2)));
// 						NExclHits->Fill(td.Nexcl);
// 						
// 						if((x1>1 && x1<127) && (x2>1 && x2<127) && (y1>1 && y1<127) && (y2>1 && y2<127))
// 						{
// 							hd.EventType=0;
// 							DeltaTTrackColT->Fill(tmaxcl-tmincl);
// 							DeltaTTrackColHitT->Fill(colhittmax-colhittmin);
// 							
// 							for(int i1=0;i1<128;i1++)
// 							{
// 								for(int ik1=0;ik1<HCS[0][i1].size();ik1++)
// 								{
// 									x1=tflinpars[0][0]+tflinpars[0][1]*HCS[0][i1][ik1].t;
// 									if(fabs(x1-((float)i1))<=5)
// 									{
// 										QperT[0]->Fill(HCS[0][i1][ik1].t,HCS[0][i1][ik1].Int);
// 										QperT[2]->Fill(HCS[0][i1][ik1].t,HCS[0][i1][ik1].Int);
// 										QperT[1]->Fill(HCS[0][i1][ik1].t);
// 									}
// 								}
// 							}
// 						}
// // 						if(td.NHits>10 && td.Nexcl<10)
// 						if(td.Nexcl<10)
// 						{
// // 							outfile<<td.E<<" "<<hd.ColIndT->at(CL[indCLHP].hi[indoftmin])[0]<<" "<<hd.ColIndT->at(CL[indCLHP].hi[indoftmin])[1]<<" "<<hd.ColIndT->at(CL[indCLHP].hi[indoftmin])[2]<<" "<<hd.ColIndT->at(CL[indCLHP].hi[indoftmax])[0]<<" "<<hd.ColIndT->at(CL[indCLHP].hi[indoftmax])[1]<<" "<<hd.ColIndT->at(CL[indCLHP].hi[indoftmax])[2]<<endl;
// 							
// 							outfile<<td.E<<" "<<x1<<" "<<y1<<" "<<z1<<" "<<x2<<" "<<y2<<" "<<z2<<endl;
// 							
// 						}
// 					}
// 					else hd.EventType=3;
// 				}
// 				else hd.EventType=3;
// 			}
// 			else hd.EventType=3;
// 			
// // 			if(I<50)
// // 			{
// // // 				sprintf(hname,"Col_vs_t_%d",I);W_vs_t[0]->SetName(hname);W_vs_t[0]->SetTitle(hname);
// // // 				sprintf(hname,"Ind_vs_t_%d",I);W_vs_t[1]->SetName(hname);W_vs_t[1]->SetTitle(hname);
// // // 				outroot->cd();
// // // 				W_vs_t[0]->Write();
// // // 				W_vs_t[1]->Write();
// // 				
// // 				sprintf(hname,"Col_vs_t_%d",I);Col_vs_t_fit->SetName(hname);Col_vs_t_fit->SetTitle(hname);
// // 				outroot->cd();
// // 				Col_vs_t_fit->Write();
// // 			}
// 		}
// 		
// 		
// 		
// 		
// 		
// 		
// 		
// 		
// 		
// 		
// 		
// 		
// 		
// 		
// 		
// 		
// 		
// 		
// 		
// 		
// 		
// 		
// 		
// 		
// 		
// 		
// 		
// 		
// 		
// 		
// 		
// 		T1->Fill();
// 		
// // 		if((RunNo==7268 && I==77) || (RunNo==7285 && I==2))
// // 		{
// // 			outroot->cd("Histos");
// // 			for(int i1=0;i1<2;i1++)
// // 			{
// // 				for(int i2=0;i2<128;i2++)
// // 				{
// // 					hh2[i1][i2]->Write();
// // 				}
// // 			}
// // 			for(int i1=750;i1<2500;i1++)
// // 			{
// // 				ht3[i1]->Write();
// // 			}
// // 		}
// 		
// // 		for(int i1=0;i1<4096;i1++)
// 		for(int i1=750;i1<2500;i1++)
// 		{
// 			ht[i1]->Reset();
// 			ht2[i1]->Reset();
// 			ht3[i1]->Reset();
// 		}
// 		for(int i1=0;i1<2;i1++)
// 		{
// 			for(int i2=0;i2<128;i2++)
// 			{
// 				hh[i1][i2]->Reset();
// 				hh2[i1][i2]->Reset();
// 				hh3[i1][i2]->Reset();
// 			}
// 		}
// 		Hits.clear();
// // 		for(int i1=0;i1<256;i1++)
// // 		{
// // 			hcw[i1].clear();
// // 		}
// 		if(I%100==0) cout<<"Event: "<<I<<endl;
// // 		if(I==50) break;
// 	}
// 	outroot->cd();
// 	T1->Write();
// 	Ttr->Write();
// 	gStyle->SetOptStat(0);
// 	outroot->cd("Histos");
// 	HitCol->Write();
// 	HitInd->Write();
// 	HitTime->Write();
// 	HitIntegral1->Write();
// 	HitIntegral2->Write();
// 	HitIntegral3->Write();
// 	NHits->Write();
// 	StartTime->Write();
// 	EndTime->Write();
// 	StartCollection->Write();
// 	EndCollection->Write();
// 	StartInduction->Write();
// 	EndInduction->Write();
// 	NHits_T_vs_E->Write();
// 	NHits_Col_vs_Ind->Write();
// 	ColTWidth->Write();
// 	IndTWidth->Write();
// 	FitNormChi2_Col_vs_t->Write();
// 	ClmaxPointFraction->Write();
// 	TrackDeltaT->Write();
// 	FitNormChi2->Write();
// 	FitClSize->Write();
// 	NExclHits->Write();
// 	
// 	Tfirst->Write();
// 	Tlast->Write();
// 	TColfirst->Write();
// 	TCollast->Write();
// 	Trackt0->Write();
// 	TrackEntryXY->Write();
// 	TrackExitXY->Write();
// 	TrackXY->Write();
// 	DeltaTvsTrackt0->Write();
// 	DeltaTTrackColT->Write();
// 	DeltaTTrackColHitT->Write();
// 	
// 	AllDeltaT->Write();
// 	AllXY->Write();
// 	
// 	
// 	HQTotCol->Write();
// 	NPoints->Write();
// 	ClusterNPointsperArea->Write();
// 	NPointsperArea->Write();
// 	TotalArea->Write();
// 	
// 	NClperEvent->Write();
// 	ClSize->Write();
// 	MaxClSize->Write();
// 	
// // 	TrackDeltaT->Write();
// 	ColTrackDeltaT->Write();
// 	QperT[0]->Write();
// 	QperT[1]->Write();
// 	QperT[2]->Divide(QperT[1]);
// 	QperT[2]->Write();
// 	
// // 	TH1F* TrackDeltaTSel=new TH1F("TrackDeltaTSel","TrackDeltaTSel",500,0,5000);
// // 	TH1F* AllDeltaT=new TH1F("AllDeltaT","AllDeltaT",500,0,5000);
// // 	TH2F* AllXY=new TH2F("AllXY","AllXY",128,-0.5,127.5,128,-0.5,127.5);
// // 	TH2F* TrackXY=new TH2F("TrackXY","TrackXY",128,-0.5,127.5,128,-0.5,127.5);
// // 	TH2F* DeltaTvsTrackt0=new TH2F("DeltaTvsTrackt0","DeltaTvsTrackt0",500,0,5000,500,0,1000);
// // 	TH1F* DeltacolperDeltat=new TH1F("DeltacolperDeltat","DeltacolperDeltat",500,0,50);
// // 	TH1F* DeltaindperDeltat=new TH1F("DeltaindperDeltat","DeltaindperDeltat",500,0,50);
// // 	TH1F* DeltaRperDeltat=new TH1F("DeltaRperDeltat","DeltaRperDeltat",2000,0,100);
// // 	TH1F* ColDeltaRR=new TH1F("ColDeltaR","ColDeltaR",2000,0,2000);
// // 	TH1F* HitDeltaRR=new TH1F("HitDeltaRR","HitDeltaRR",2000,0,2000);
// // 	TH1F* HitDeltaT=new TH1F("HitDeltaT","HitDeltaT",2000,0,1000);
// // 	TH1F* HitDeltaR=new TH1F("HitDeltaR","HitDeltaR",200,0,100);
// // 	TH2F* HitDeltaR_vs_DeltaT=new TH2F("HitDeltaR_vs_DeltaT","HitDeltaR_vs_DeltaT",200,-0.5,199.5,200,0,20);
// 	
// // 	TH2F* DeltaTLengthvsAngle=new TH2F("DeltaTLengthvsAngle","DeltaTLengthvsAngle",314,0,1.57,100,0,2000);
// // 	TH2F* DeltaTLengthvsAngle_norm=new TH2F("DeltaTLengthvsAngle_norm","DeltaTLengthvsAngle_norm",314,0,1.57,100,0,2000);
// 	
// // 	TH1F* FitNormChi2_Ind_vs_t=new TH1F("FitNormChi2_Ind_vs_t","FitNormChi2_Ind_vs_t",1000,0.,4000.);
// 	
// // 	TH2F* NColHits_vs_fitnormchi2=new TH2F("NColHits_vs_fitnormchi2","NColHits_vs_fitnormchi2",200,-0.5,199.5,500,0,2000);
// // 	TH2F* NIndHits_vs_fitnormchi2=new TH2F("NIndHits_vs_fitnormchi2","NIndHits_vs_fitnormchi2",200,-0.5,199.5,500,0,2000);
// 	
// 	
// // 	TH2F* NHits_T_vs_E=new TH2F("NHits_T_vs_E","NHits_T_vs_E",T->GetEntries(),-0.5,T->GetEntries()-0.5,4096,-0.5,4095.5);
// // 	TH2F* NHits_Col_vs_Ind=new TH2F("NHits_Col_vs_Ind","NHits_Col_vs_Ind",20,-0.5,19.5,20,-0.5,19.5);
// // 	TH2F* QCol_vs_QInd=new TH2F("QCol_vs_QInd","QCol_vs_QInd",50,0,200,50,0,200);
// 	
// 	
// 	ColIndDeltaT->Write();
// 	ColIndMinDeltaT5->Write();
// 	ColWidth->Write();
// 	IndWidth->Write();
// 	ColAmp->Write();
// 	IndAmp->Write();
// 	Amp_Col_vs_Ind->Write();
// 	Width_Col_vs_Ind->Write();
// 	HitIntegralLost->Write();
// 	TrueTotalIntegral->Write();
// 	Col_Int_vs_Amp->Write();
// 	NHitsPerColPeak->Write();
// 	DeltaTTrackColT->Write();
// 	DeltaTTrackColHitT->Write();
// 	
// 	outroot->Close();
// // 	outroottr->cd();
// // 	Ttr->Write();
// // 	outroottr->Close();
// 	inroot->Close();
// 	outfile.close();
// 	
// // 	sprintf(hname,"cp Hits_%d.root %s/Histos/Hits_%d.root;wait;",RunNo,AnalysisFilePath,RunNo);system(hname);
// // 	sprintf(hname,"cp Tracks_%d.root %s/Histos/Tracks_%d.root",RunNo,AnalysisFilePath,RunNo);system(hname);
// 	sprintf(hname,"cp HitsAndTracks_%d.root %s/Histos/HitsAndTracks_%d.root;wait;",RunNo,AnalysisFilePath,RunNo);system(hname);
// 	sprintf(hname,"cp ValidTrackParameters_%d.txt %s/Files/ValidTrackParameters_%d.txt;wait;",RunNo,AnalysisFilePath,RunNo);system(hname);
// }