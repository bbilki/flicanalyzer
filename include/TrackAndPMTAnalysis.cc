void TrackAndPMTAnalysis()//for light yield
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
	
	float PMTTOTFit[3][5]={{-3667.1,1036.12,-77.2925,2.88782,-0.0330686},{0,0,0,0,0},{4952.66,-1114.38,99.3245,-3.55144,0.0481073}};
	float PMTTOTTh[3]={110,0,40};
	float PMTintegral=0;
	float PMTintegrals[3]={0};
	float PMTTOTintegral=0;
	float PMTTOT=0;
	
	TH1F* hPMT[3];
	float PMTXY[3][2]={{95.,63.5},{63.5,32.},{63.5,95.}};
// 	string PMTNames[3]={"LAr w/TPB","LAr no TPB","LXe"};
	int PMTlc[3]={1,2,4};
	float TOTThresholds[20]={10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200};
	float ThresholdOffset[3]={70,0,10};
	float threshold=0;
	
	TF1* tf1=new TF1("gaus","gaus",0.,3000.);
	TF1* tf2 = new TF1("fit",fitf3f,0.,1000,7);
	
	sprintf(hname,"TrackAndPMTAnalysis_%d.root",RunNo);
	TFile* outroot=new TFile(hname,"recreate");
	
	TH2F* NColHitsvsChi2Vertical=new TH2F("NColHitsvsChi2Vertical","NColHitsvsChi2Vertical",200,0,10,200,0,200);
	TH1F* AllHitT=new TH1F("AllHitT","AllHitT",500,0,5000);
	TH1F* VerticalChi2=new TH1F("VerticalChi2","VerticalChi2",100,0,10);
	TH1F* VerticalChi2Sel=new TH1F("VerticalChi2Sel","VerticalChi2Sel",100,0,10);
	TH1F* NColHits=new TH1F("NColHits","NColHits",1500,-0.5,1499.5);
	TH1F* NColHitsPerDeltaT=new TH1F("NColHitsPerDeltaT","NColHitsPerDeltaT",500,0,0.5);
	TH1F* NColHitsVertical=new TH1F("NColHitsVertical","NColHitsVertical",1500,-0.5,1499.5);
	
	TH1F* DistancetoPMT[2][2];
	DistancetoPMT[0][0]=new TH1F("DistancetoPMT_0_vertical","DistancetoPMT_0_vertical",150,0,60);
	DistancetoPMT[1][0]=new TH1F("DistancetoPMT_1_vertical","DistancetoPMT_1_vertical",150,0,60);
	DistancetoPMT[0][1]=new TH1F("DistancetoPMT_0_nonvertical","DistancetoPMT_0_nonvertical",150,0,60);
	DistancetoPMT[1][1]=new TH1F("DistancetoPMT_1_nonvertical","DistancetoPMT_1_nonvertical",150,0,60);
	
	TH1F* PMTIntegral[3][3][3];
	string s1[3]={"vertical","nonvertical","all"};
	string s2[3]={"nonsaturated","saturated","all"};
	for(int i1=0;i1<3;i1++)
	{
		if(i1==1) continue;
		for(int i2=0;i2<3;i2++)
		{
			for(int i3=0;i3<3;i3++)
			{
				sprintf(hname,"PMTIntegral_%d_%s_%s",i1,s1[i2].c_str(),s2[i3].c_str());
				PMTIntegral[i1][i2][i3]=new TH1F(hname,hname,300,0,60);
			}
		}
	}
	TProfile* PMTIntvsDist[3][3];
	for(int i1=0;i1<3;i1++)
	{
		if(i1==1) continue;
		for(int i2=0;i2<3;i2++)
		{
			sprintf(hname,"PMTIntvsDist_%d_%s",i1,s2[i2].c_str());
			PMTIntvsDist[i1][i2]=new TProfile(hname,hname,300,0,60,0,60);
		}
	}
	
	TH1F* TotalCollectionCharge=new TH1F("TotalCollectionCharge","TotalCollectionCharge",1200,-200000,1000000);
	TH1F* TotalCollectionChargeZS=new TH1F("TotalCollectionChargeZS","TotalCollectionChargeZS",1000,0,1000000);
	TProfile* DeltaTvsEvent=new TProfile("DeltaTvsEvent","DeltaTvsEvent",500,0,500,0,1000);
	TH1F* TrackColHitInt=new TH1F("TrackColHitInt","TrackColHitInt",2000,0,20000);
	TH1F* TrackColHitAmp=new TH1F("TrackColHitAmp","TrackColHitAmp",600,0,1500);
	TH1F* VerticalTrackDeltaR=new TH1F("VerticalTrackDeltaR","VerticalTrackDeltaR",100,0,200);
	TH1F* VerticalTrackLength=new TH1F("VerticalTrackLength","VerticalTrackLength",300,0,60);
	TH1F* NonVerticalTrackLength=new TH1F("NonVerticalTrackLength","NonVerticalTrackLength",300,0,60);
	TH1F* AllTrackLength=new TH1F("AllTrackLength","AllTrackLength",300,0,60);
	TH2F* VerticalTrackEntryXY=new TH2F("VerticalTrackEntryXY","VerticalTrackEntryXY",128,-0.5,127.5,128,-0.5,127.5);
	TH2F* VerticalTrackExitXY=new TH2F("VerticalTrackExitXY","VerticalTrackExitXY",128,-0.5,127.5,128,-0.5,127.5);
	TH1F* VerticalTrackColHitsDeltaT=new TH1F("VerticalTrackColHitsDeltaT","VerticalTrackColHitsDeltaT",1024,-0.5,4095.5);
	TH1F* VerticalTrackColHitsDeltaTSel=new TH1F("VerticalTrackColHitsDeltaTSel","VerticalTrackColHitsDeltaTSel",1024,-0.5,4095.5);
	TH1F* VerticalTrackColHitsDeltaTSel2=new TH1F("VerticalTrackColHitsDeltaTSel2","VerticalTrackColHitsDeltaTSel2",1024,-0.5,4095.5);
	TH1F* VerticalTrackTimeofFirstColHit=new TH1F("VerticalTrackTimeofFirstColHit","VerticalTrackTimeofFirstColHit",2000,0,4000);
	TH1F* VerticalTrackTimeofFirstColHits[15];
	TH1F* VerticalTrackColHitsDeltaTs[15];
	TProfile* DeltaTvsDeltaR=new TProfile("DeltaTvsDeltaR","DeltaTvsDeltaR",200,0,200,0,1000);
	for(int i1=0;i1<RunNos.size();i1++)
	{
		sprintf(hname,"VerticalTrackTimeofFirstColHits_R%d",RunNos[i1]);
		VerticalTrackTimeofFirstColHits[i1]=new TH1F(hname,hname,2000,0,4000);
		sprintf(hname,"VerticalTrackColHitsDeltaT_R%d",RunNos[i1]);
		VerticalTrackColHitsDeltaTs[i1]=new TH1F(hname,hname,2000,0,4000);
	}
// 	TH1F* InclinedTrackSlope;
	
	float x1(0),y1(0),t1(0),x2(0),y2(0),t2(0);
	float x1f(0),y1f(0),t1f(0),x2f(0),y2f(0),t2f(0);
	
	float tracklength=0;float pmtdist[3]={0};
	
	TFile* inroot;
	TFile* inroot2;
	TTree* TH;
	TTree* TT;
	TTree* T;
	bool PMTsat=false;
	float tmin=0;float tmax=0;
	int indtmin=0;int indtmax=0;
	float x=0;float y=0;
	float deltaR=0;
	float deltaT=0;
	bool vertical=true;
	int NEvtvertical=0;
	float sg=0;
	int TOTbinmin=0;int TOTbinmax=0;
	for(int J=0;J<RunNos.size();J++)
	{
		RunNo=RunNos[J];
		
		if(RunNo>=7000 && RunNo<=7087) runper=0;
		else if(RunNo>=7090 && RunNo<=7094) runper=1;
		else if(RunNo>=7095 && RunNo<=7144) runper=2;
		else if(RunNo>=7200 && RunNo<=7287) runper=3;
		else if(RunNo>=7300 && RunNo<=7403) runper=4;
		
		PMTBaselineRun=FindBaselineRun(RunNo);
		cout<<"Run: "<<RunNo<<" PMTBaselineRun: "<<PMTBaselineRun<<endl;
		ReadPMTBaselines();
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
		
		sprintf(hname,"%s/Run_%d.root",NTupleFilePath,RunNo);
// 		cout<<hname<<endl;
		inroot2=new TFile(hname);
		T =  (TTree*) inroot2->Get("T");
		T->SetBranchAddress("TPCWF",&ed.TPCWF);
		T->SetBranchAddress("PMTWF",&ed.PMTWF);
		
		for(int I=0;I<TT->GetEntries();I++)
		{
			TT->GetEntry(I);
			
			for(int II=td.E;II<TH->GetEntries();II++)
			{
				TH->GetEntry(II);
				if(hd.E==td.E) break;
			}
			if(!(hd.EventType==0 || hd.EventType==1)) continue;
			T->GetEntry(hd.E);
			
			tmin=10000.;tmax=0.;vertical=true;
			for(int ik1=0;ik1<hd.ColID->size();ik1++)
			{
				x=td.FitParams->at(0)+td.FitParams->at(1)*(hd.ColT->at(ik1)-td.FitParams->at(4));
				if(fabs(x-((float)hd.ColID->at(ik1)))<=5)
				{
					TrackColHitInt->Fill(hd.ColInt->at(ik1));
					TrackColHitAmp->Fill(hd.ColA->at(ik1));
					if(hd.ColInt->at(ik1)>10)
					{
						if(hd.ColT->at(ik1)<tmin){tmin=hd.ColT->at(ik1);indtmin=ik1;}
						if(hd.ColT->at(ik1)>tmax){tmax=hd.ColT->at(ik1);indtmax=ik1;}
					}
				}
			}
			deltaT=tmax-tmin;
			NColHits->Fill(hd.ColID->size());
			if(tmin<tlims[runper][0] || tmax>tlims[runper][1]) continue;
			
			for(int ik1=0;ik1<hd.ColIndT->size();ik1++)
			{
				AllHitT->Fill(hd.ColIndT->at(ik1)[2]);
			}
			TotalCollectionCharge->Fill(hd.QColTot);
			TotalCollectionChargeZS->Fill(hd.QColTotZS);
			
			x1=((float)td.StartEndColIndT->at(0));
			y1=((float)td.StartEndColIndT->at(1));
			t1=((float)td.StartEndColIndT->at(2));
			x2=((float)td.StartEndColIndT->at(3));
			y2=((float)td.StartEndColIndT->at(4));
			t2=((float)td.StartEndColIndT->at(5));
			
			x1f=td.FitParams->at(0)+td.FitParams->at(1)*(tmin-td.FitParams->at(4));
			y1f=td.FitParams->at(2)+td.FitParams->at(3)*(tmin-td.FitParams->at(4));
			t1f=tmin;
			x2f=td.FitParams->at(0)+td.FitParams->at(1)*(tmax-td.FitParams->at(4));
			y2f=td.FitParams->at(2)+td.FitParams->at(3)*(tmax-td.FitParams->at(4));
			t2f=tmax;
			
			tracklength=sqrt(pow((x1f-x2f)*0.254,2)+pow((y1f-y2f)*0.254,2)+pow((tmax-tmin)*0.4*DriftSpeeds[runper],2));
// 			tracklength=sqrt(pow((x1-x2)*0.254,2)+pow((y1-y2)*0.254,2)+pow((tmax-tmin)*0.4*DriftSpeeds[runper],2));
			AllTrackLength->Fill(tracklength);
			
			for(int is1=0;is1<3;is1++)
			{
				if(is1==1) continue;
				pmtdist[is1]=sqrt(pow((x1f-PMTXY[is1][0])*0.254,2)+pow((y1f-PMTXY[is1][1])*0.254,2)+pow(((tmin-tlims[runper][0])*0.4*DriftSpeeds[runper])+4,2));
			}
			
			for(int i1=0;i1<3;i1++)
			{
				if(i1==1) continue;
				
				if(hd.PMTIntegral->at(i1)>0)//unsaturated
				{
					PMTsat=false;
				}
				else
				{
					PMTsat=true;
				}
				PMTintegral=0;
				if(!PMTsat)
				{
					for(int i2=1900;i2<10000;i2++)
					{
						PMTintegral+=(-1.)*(((float)ed.PMTWF->at(i1)[i2])-PMTBaselines[i1][0]);
					}
				}
				else//saturated
				{
					for(int i2=1900;i2<10000;i2++)
					{
						sg=(-1.)*(((float)ed.PMTWF->at(i1)[i2])-PMTBaselines[i1][0]);
						if(sg>PMTTOTTh[i1])
						{
							TOTbinmin=i2;
							break;
						}
						else
						{
							PMTintegral+=sg;
						}
					}
					for(int i2=TOTbinmin+1;i2<10000;i2++)
					{
						sg=(-1.)*(((float)ed.PMTWF->at(i1)[i2])-PMTBaselines[i1][0]);
						if(sg<PMTTOTTh[i1])
						{
							TOTbinmax=i2-1;
							break;
						}
					}
					PMTTOT=((float)TOTbinmax)-((float)TOTbinmin);
					PMTTOTintegral=PMTTOTFit[i1][0]+(PMTTOTFit[i1][1]*PMTTOT)+(PMTTOTFit[i1][1]*pow(PMTTOT,2))+(PMTTOTFit[i1][3]*pow(PMTTOT,3))+(PMTTOTFit[i1][4]*pow(PMTTOT,4));
					for(int i2=TOTbinmax+1;i2<10000;i2++)
					{
						sg=(-1.)*(((float)ed.PMTWF->at(i1)[i2])-PMTBaselines[i1][0]);
						PMTintegral+=sg;
					}
					PMTintegral+=PMTTOTintegral;
				}
				PMTintegrals[i1]=((PMTintegral/SPE[i1][runper][0])/tracklength)/2.1;//npe/MeV
			}
			
			for(int ip1=0;ip1<3;ip1++)
			{
				if(ip1==1) continue;
				PMTIntegral[ip1][2][2]->Fill(PMTintegrals[ip1]);
				PMTIntvsDist[ip1][2]->Fill(pmtdist[ip1],PMTintegrals[ip1]);
				if(hd.PMTIntegral->at(ip1)>0)//unsaturated
				{
					PMTIntegral[ip1][2][0]->Fill(PMTintegrals[ip1]);
					PMTIntvsDist[ip1][0]->Fill(pmtdist[ip1],PMTintegrals[ip1]);
				}
				else
				{
					PMTIntegral[ip1][2][1]->Fill(PMTintegrals[ip1]);
					PMTIntvsDist[ip1][1]->Fill(pmtdist[ip1],PMTintegrals[ip1]);
				}
			}
			
			deltaR=sqrt(pow(x1-x2,2)+pow(y1-y2,2));
			
// 			if(x1>2 && x1<125 && y1>2 && y1<125 && x2>2 && x2<125 && y2>2 && y2<125 && td.FitNormChi2<3)//vertical tracks
// 			if(x1>2 && x1<125 && y1>2 && y1<125 && x2>2 && x2<125 && y2>2 && y2<125)//vertical tracks
// 			if(x1>10 && x1<117 && y1>10 && y1<117 && x2>10 && x2<117 && y2>10 && y2<117)//vertical tracks//this was the original
			if(x1>10 && x1<117 && y1>10 && y1<117 && x2>10 && x2<117 && y2>10 && y2<117 && deltaR>15 && deltaR<35)//vertical tracks
			{
				NColHitsVertical->Fill(hd.ColID->size());
				VerticalChi2->Fill(td.FitNormChi2);
				NColHitsvsChi2Vertical->Fill(td.FitNormChi2,hd.ColID->size());
				NColHitsPerDeltaT->Fill(((float)hd.ColID->size())/(tmax-tmin));
				
				VerticalTrackEntryXY->Fill(x1,y1);
				VerticalTrackExitXY->Fill(x2,y2);
				VerticalTrackDeltaR->Fill(deltaR);
				
// 				tmin=td.ColHitTStartEnd->at(0);tmax=td.ColHitTStartEnd->at(1);
				
				VerticalTrackColHitsDeltaT->Fill(tmax-tmin);
// 				if(deltaR>10 && deltaR<30)
				if((((float)hd.ColID->size())/(tmax-tmin))>0. && (((float)hd.ColID->size())/(tmax-tmin))<0.1)
				{
					VerticalTrackColHitsDeltaTSel->Fill(tmax-tmin);
					if(runper==4 && RunNo>=7316) VerticalTrackColHitsDeltaTSel2->Fill(tmax-tmin);
				}
				VerticalTrackColHitsDeltaTs[J]->Fill(tmax-tmin);
				VerticalTrackTimeofFirstColHit->Fill(tmin);
				VerticalTrackTimeofFirstColHits[J]->Fill(tmin);
				if((tmax-tmin)<500 || (tmax-tmin)>1000)
				{
					VerticalChi2Sel->Fill(td.FitNormChi2);
// 					cout<<RunNos[J]<<" "<<td.E<<" "<<(tmax-tmin)<<" "<<tmin<<" "<<tmax<<endl;
				}
				
				DeltaTvsDeltaR->Fill(deltaR,tmax-tmin);
				DeltaTvsEvent->Fill(NEvtvertical/500,tmax-tmin);
				VerticalTrackLength->Fill(tracklength);
				
				DistancetoPMT[0][0]->Fill(pmtdist[0]);
				DistancetoPMT[1][0]->Fill(pmtdist[2]);
				
				for(int ip1=0;ip1<3;ip1++)
				{
					if(ip1==1) continue;
					PMTIntegral[ip1][0][2]->Fill(PMTintegrals[ip1]);
					if(hd.PMTIntegral->at(ip1)>0)//unsaturated
					{
						PMTIntegral[ip1][0][0]->Fill(PMTintegrals[ip1]);
					}
					else
					{
						PMTIntegral[ip1][0][1]->Fill(PMTintegrals[ip1]);
					}
				}
				
				NEvtvertical++;
			}
			else
			{
				NonVerticalTrackLength->Fill(tracklength);
				DistancetoPMT[0][1]->Fill(pmtdist[0]);
				DistancetoPMT[1][1]->Fill(pmtdist[2]);
				for(int ip1=0;ip1<3;ip1++)
				{
					if(ip1==1) continue;
					PMTIntegral[ip1][1][2]->Fill(PMTintegrals[ip1]);
					if(hd.PMTIntegral->at(ip1)>0)//unsaturated
					{
						PMTIntegral[ip1][1][0]->Fill(PMTintegrals[ip1]);
					}
					else
					{
						PMTIntegral[ip1][1][1]->Fill(PMTintegrals[ip1]);
					}
				}
			}
		}
		inroot->Close();
		inroot2->Close();
	}
	
	outroot->cd();
	
	VerticalTrackDeltaR->Scale(1./VerticalTrackDeltaR->Integral());
	VerticalTrackDeltaR->GetXaxis()->SetTitle("#Delta R (x 2.54 mm)");VerticalTrackDeltaR->GetXaxis()->CenterTitle();
// 	VerticalTrackDeltaR->GetYaxis()->SetTitle("Events / 1.27 mm");VerticalTrackDeltaR->GetYaxis()->CenterTitle();
	VerticalTrackDeltaR->Fit(tf1,"q","q",0.,100);
	VerticalTrackDeltaR->Write();
	
	
	tf2->SetParameter(0,VerticalTrackColHitsDeltaT->GetBinContent(VerticalTrackColHitsDeltaT->GetMaximumBin()));
	tf2->SetParameter(1,VerticalTrackColHitsDeltaT->GetBinCenter(VerticalTrackColHitsDeltaT->GetMaximumBin()));
	tf2->SetParameter(2,10);
	tf2->SetParameter(3,0.1);
	tf2->SetParameter(4,0.1);
	tf2->SetParameter(5,0.1);
// 	VerticalTrackColHitsDeltaT->Fit(tf2,"q","q",800,900);
// 	VerticalTrackColHitsDeltaT->Fit(tf2,"q","q",tf2->GetParameter(1)-2.5*tf2->GetParameter(2),tf2->GetParameter(1)+2*tf2->GetParameter(2));
	VerticalTrackColHitsDeltaT->Fit(tf1,"q","q",800,900);
	VerticalTrackColHitsDeltaT->Fit(tf2,"q","q",tf1->GetParameter(1)-2.*tf1->GetParameter(2),tf1->GetParameter(1)+2*tf1->GetParameter(2));
	
	
// 	VerticalTrackColHitsDeltaT->Fit(tf1,"q","q",800,900);
// 	VerticalTrackColHitsDeltaT->Fit(tf1,"q","q",tf1->GetParameter(1)-1.5*tf1->GetParameter(2),tf1->GetParameter(1)+1.5*tf1->GetParameter(2));
	VerticalTrackColHitsDeltaT->GetXaxis()->SetTitle("#Deltat (x 400 ns)");VerticalTrackColHitsDeltaT->GetXaxis()->CenterTitle();
	VerticalTrackColHitsDeltaT->Write();
	
// 	VerticalTrackColHitsDeltaTSel->Fit(tf2,"q","q",800,900);
// 	VerticalTrackColHitsDeltaTSel->Fit(tf2,"q","q",tf2->GetParameter(1)-2.5*tf2->GetParameter(2),tf2->GetParameter(1)+2*tf2->GetParameter(2));
	VerticalTrackColHitsDeltaTSel->Fit(tf1,"q","q",800,900);
	VerticalTrackColHitsDeltaTSel->Fit(tf2,"q","q",tf1->GetParameter(1)-2.*tf1->GetParameter(2),tf1->GetParameter(1)+2*tf1->GetParameter(2));
	
	
// 	VerticalTrackColHitsDeltaTSel->Fit(tf1,"q","q",800,900);
// 	VerticalTrackColHitsDeltaTSel->Fit(tf1,"q","q",tf1->GetParameter(1)-1.5*tf1->GetParameter(2),tf1->GetParameter(1)+1.5*tf1->GetParameter(2));
	VerticalTrackColHitsDeltaTSel->GetXaxis()->SetTitle("#Deltat (x 400 ns)");VerticalTrackColHitsDeltaTSel->GetXaxis()->CenterTitle();
	VerticalTrackColHitsDeltaTSel->Write();
	
	if(PRunNo==3000)
	{
// 		VerticalTrackColHitsDeltaTSel2->Fit(tf2,"q","q",800,900);
// 		VerticalTrackColHitsDeltaTSel2->Fit(tf2,"q","q",tf2->GetParameter(1)-2.5*tf2->GetParameter(2),tf2->GetParameter(1)+2*tf2->GetParameter(2));
		VerticalTrackColHitsDeltaTSel2->Fit(tf1,"q","q",800,900);
		VerticalTrackColHitsDeltaTSel2->Fit(tf2,"q","q",tf1->GetParameter(1)-2.*tf1->GetParameter(2),tf1->GetParameter(1)+2*tf1->GetParameter(2));
		
		
// 		VerticalTrackColHitsDeltaTSel2->Fit(tf1,"q","q",800,900);
// 		VerticalTrackColHitsDeltaTSel2->Fit(tf1,"q","q",tf1->GetParameter(1)-1.5*tf1->GetParameter(2),tf1->GetParameter(1)+1.5*tf1->GetParameter(2));
		VerticalTrackColHitsDeltaTSel2->GetXaxis()->SetTitle("#Deltat (x 400 ns)");VerticalTrackColHitsDeltaTSel2->GetXaxis()->CenterTitle();
		VerticalTrackColHitsDeltaTSel2->Write();
	}
	
	NColHitsvsChi2Vertical->Write();
	VerticalChi2->Write();
	VerticalChi2Sel->Write();
	NColHits->Write();
	NColHitsVertical->Write();
	NColHitsPerDeltaT->Write();
	TotalCollectionCharge->Write();
	TotalCollectionChargeZS->Write();
	DeltaTvsEvent->Write();
	AllHitT->Write();
	TrackColHitInt->Write();
	TrackColHitAmp->Write();
	
	for(int i1=0;i1<2;i1++)
	{
		for(int i2=0;i2<2;i2++)
		{
			DistancetoPMT[i1][i2]->Write();
		}
	}
	
	VerticalTrackLength->Write();
	NonVerticalTrackLength->Write();
	AllTrackLength->Write();
	VerticalTrackEntryXY->Write();
	VerticalTrackExitXY->Write();
	VerticalTrackTimeofFirstColHit->Write();
	for(int i1=0;i1<RunNos.size();i1++)
	{
		VerticalTrackTimeofFirstColHits[i1]->Write();
		VerticalTrackColHitsDeltaTs[i1]->Write();
	}
	DeltaTvsDeltaR->Write();
	
	for(int i1=0;i1<3;i1++)
	{
		if(i1==1) continue;
		for(int i2=0;i2<3;i2++)
		{
			for(int i3=0;i3<3;i3++)
			{
				PMTIntegral[i1][i2][i3]->Write();
			}
		}
	}
	for(int i1=0;i1<3;i1++)
	{
		if(i1==1) continue;
		for(int i2=0;i2<3;i2++)
		{
			PMTIntvsDist[i1][i2]->Write();
		}
	}
	
	outroot->Close();
// 	sprintf(hname,"cp PMTAnalysis_%d.root %s/Histos/PMTAnalysis_%d.root;wait;",PRunNo,AnalysisFilePath,RunNo);system(hname);
}