void FitTracks()
{
	sprintf(hname,"ValidTrackParameters_%d.txt",RunNo);
	ofstream outfile(hname);
	vector <int> eventlist;
	
	sprintf(hname,"Tracks_%d.root",RunNo);
	TFile* outroot=new TFile(hname,"recreate");
	TTree* T1 =  new TTree("T","T");
	T1->Branch("E",&td.E);
	T1->Branch("StartEndColIndT",&td.StartEndColIndT);
	T1->Branch("FitParams",&td.FitParams);
	T1->Branch("FitNormChi2",&td.FitNormChi2);
	T1->Branch("QColTot",&td.QColTot);
	T1->Branch("NHits",&td.NHits);
	T1->Branch("Nexcl",&td.Nexcl);
	T1->Branch("PMTIntegral",&td.PMTIntegral);
	
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
	TH1F* Trackt0=new TH1F("Trackt0","Trackt0",500,0,5000);
	TH2F* DeltaTvsTrackt0=new TH2F("DeltaTvsTrackt0","DeltaTvsTrackt0",500,0,5000,500,0,1000);
	TH1F* DeltacolperDeltat=new TH1F("DeltacolperDeltat","DeltacolperDeltat",500,0,50);
	TH1F* DeltaindperDeltat=new TH1F("DeltaindperDeltat","DeltaindperDeltat",500,0,50);
	TH1F* DeltaRperDeltat=new TH1F("DeltaRperDeltat","DeltaRperDeltat",2000,0,100);
	TH1F* ColDeltaRR=new TH1F("ColDeltaR","ColDeltaR",2000,0,2000);
	TH1F* HitDeltaRR=new TH1F("HitDeltaRR","HitDeltaRR",2000,0,2000);
	TH1F* HitDeltaT=new TH1F("HitDeltaT","HitDeltaT",2000,0,1000);
	TH1F* HitDeltaR=new TH1F("HitDeltaR","HitDeltaR",200,0,100);
	TH2F* HitDeltaR_vs_DeltaT=new TH2F("HitDeltaR_vs_DeltaT","HitDeltaR_vs_DeltaT",200,-0.5,199.5,200,0,20);
	
	TH2F* DeltaTLengthvsAngle=new TH2F("DeltaTLengthvsAngle","DeltaTLengthvsAngle",314,0,1.57,100,0,2000);
	TH2F* DeltaTLengthvsAngle_norm=new TH2F("DeltaTLengthvsAngle_norm","DeltaTLengthvsAngle_norm",314,0,1.57,100,0,2000);
	
	TH1F* FitNormChi2_Col_vs_t=new TH1F("FitNormChi2_Col_vs_t","FitNormChi2_Col_vs_t",1000,0.,4000.);
	TH1F* FitNormChi2_Ind_vs_t=new TH1F("FitNormChi2_Ind_vs_t","FitNormChi2_Ind_vs_t",1000,0.,4000.);
	
	TH2F* NColHits_vs_fitnormchi2=new TH2F("NColHits_vs_fitnormchi2","NColHits_vs_fitnormchi2",200,-0.5,199.5,500,0,2000);
	TH2F* NIndHits_vs_fitnormchi2=new TH2F("NIndHits_vs_fitnormchi2","NIndHits_vs_fitnormchi2",200,-0.5,199.5,500,0,2000);
	
	TGraph2D* tg2d=new TGraph2D();
	sprintf(hname,"Ev3d");
	tg2d->SetName(hname);tg2d->SetTitle(hname);
	tg2d->SetMarkerStyle(20);tg2d->SetMarkerColor(2);
	
	TGraph2D* tg2dA=new TGraph2D();
	sprintf(hname,"Ev3dout");
	tg2dA->SetName(hname);tg2dA->SetTitle(hname);
	tg2dA->SetMarkerStyle(20);tg2dA->SetMarkerColor(4);
	
	TGraph* tg1dcol=new TGraph();
	sprintf(hname,"Col_vs_t");
	tg1dcol->SetName(hname);tg1dcol->SetTitle(hname);
	
	TGraph* tg1dind=new TGraph();
	sprintf(hname,"Ind_vs_t");
	tg1dind->SetName(hname);tg1dind->SetTitle(hname);
	
	TF1* tflin=new TF1("lin","[0]+[1]*x",750,2500);
	TF1* tflin2=new TF1("lin2","[0]+[1]*x",750,2500);
	
	TCanvas* cc=new TCanvas("cc","cc",600,600);
	TGraph2D* tg2daxis=new TGraph2D();
	tg2daxis->SetName("Axis");tg2daxis->SetTitle("Axis");
	tg2daxis->SetPoint(0,0,0,0);tg2daxis->SetPoint(0,127,127,4095);
	
	sprintf(hname,"%s/Histos/Hits_%d.root",AnalysisFilePath,RunNo);
	TFile* inroot=new TFile(hname);
	TTree* T =  (TTree*) inroot->Get("T");
	T->SetBranchAddress("E",&hd.E);
	T->SetBranchAddress("ColIndT",&hd.ColIndT);
	T->SetBranchAddress("Int",&hd.Int);
	T->SetBranchAddress("QColTot",&hd.QColTot);
	T->SetBranchAddress("QColTotZS",&hd.QColTotZS);
	T->SetBranchAddress("PMTIntegral",&hd.PMTIntegral);
	T->SetBranchAddress("ColID",&hd.ColID);
	T->SetBranchAddress("ColT",&hd.ColT);
	T->SetBranchAddress("ColA",&hd.ColA);
	T->SetBranchAddress("ColInt",&hd.ColInt);
	T->SetBranchAddress("Colw",&hd.Colw);
	T->SetBranchAddress("IndID",&hd.IndID);
	T->SetBranchAddress("IndT",&hd.IndT);
	T->SetBranchAddress("IndA",&hd.IndA);
	T->SetBranchAddress("Indw",&hd.Indw);
	
// 	sprintf(hname,"%s/Run_%d.root",NTupleFilePath,RunNo);
// 	TFile* inroot=new TFile(hname);
// 	TTree* TN =  (TTree*) inroot->Get("T");
// 	TN->SetBranchAddress("TPCWF",&ed.TPCWF);
// 	TN->SetBranchAddress("PMTWF",&ed.PMTWF);
	
// 	ReadTPCBaselines();
	
	bool found=false;
	bool skipEvent=false;
	vector <hits> H;
	vector <int> inCL;
	int b[7]={0};
	int Ev=0;
	bool foundcl=false;
	int nid=0;
	int indCLHP=0;int maxCLsize=0;
	float fitnormchi2=0;float fitnormchi2col=0;float fitnormchi2ind=0;
	TPolyLine3D *l;
	int tmin=5000;int tmax=0;
	int mincol=0;int maxcol=0;
	float tmincol=5000;float tmaxcol=0;
	float tmintrack=5000;float tmaxtrack=0;
	float tminall=5000;float tmaxall=0;
	int indoftmin=0;int indoftmax=0;
	TH3F *frame3d=new TH3F("frame3d","frame3d",1,-0.5,127.5,1,-0.5,127.5,1,-0.5,4095.5);
	TPolyMarker3D *pm3d1;
	TPolyMarker3D *pm3d2;
	double x1(0),y1(0),z1(0),t1(0);
	double x2(0),y2(0),z2(0),t2(0);
	double tflinpars[2][2]={{0}};//col, ind - const, slope
	int Nvalid=0;
	float QColtot=0;
	
// 	for(int I=0;I<T->GetEntries();I++)
// 	{
// 		T->GetEntry(I);
// 		
// 		if(hd.ColIndT->size()>300) continue;
// 		for(int i1=0;i1<hd.ColIndT->size();i1++)
// 		{
// 			for(int i2=i1+1;i2<hd.ColIndT->size();i2++)
// 			{
// 				DeltaRperDeltat->Fill(fabs(sqrt(pow(hd.ColIndT->at(i1)[0]-hd.ColIndT->at(i2)[0],2)+pow(hd.ColIndT->at(i1)[1]-hd.ColIndT->at(i2)[1],2))/((float)(hd.ColIndT->at(i1)[2]-hd.ColIndT->at(i2)[2]))));
// 				HitDeltaT->Fill(((float)(hd.ColIndT->at(i1)[2]-hd.ColIndT->at(i2)[2])));
// 				HitDeltaR_vs_DeltaT->Fill(((float)(hd.ColIndT->at(i1)[2]-hd.ColIndT->at(i2)[2])),fabs(sqrt(pow(hd.ColIndT->at(i1)[0]-hd.ColIndT->at(i2)[0],2)+pow(hd.ColIndT->at(i1)[1]-hd.ColIndT->at(i2)[1],2))));
// 				HitDeltaR->Fill(fabs(sqrt(pow(hd.ColIndT->at(i1)[0]-hd.ColIndT->at(i2)[0],2)+pow(hd.ColIndT->at(i1)[1]-hd.ColIndT->at(i2)[1],2))));
// 				HitDeltaRR->Fill(fabs(sqrt(pow(hd.ColIndT->at(i1)[0]-hd.ColIndT->at(i2)[0],2)+pow(hd.ColIndT->at(i1)[1]-hd.ColIndT->at(i2)[1],2)+pow(hd.ColIndT->at(i1)[2]-hd.ColIndT->at(i2)[2],2))));
// 			}
// 		}
// 		for(int i1=0;i1<hd.ColID->size();i1++)
// 		{
// 			for(int i2=i1+1;i2<hd.ColID->size();i2++)
// 			{
// 				ColDeltaRR->Fill(fabs(sqrt(pow(hd.ColID->at(i1)-hd.ColID->at(i2),2)+pow(hd.ColT->at(i1)-hd.ColT->at(i2),2))));
// 			}
// 		}
// 	}
	
	
// 	for(int I=0;I<T->GetEntries();I++)
// 	{
// 		T->GetEntry(I);
// 		
// 		if(hd.ColIndT->size()<5) continue;
// 		if(hd.ColIndT->size()>300) continue;
// 		
// 		vector <clusters> CL;
// 		cl.hi.clear();
// 		inCL.clear();
// 		for(int i1=0;i1<hd.ColIndT->size();i1++)
// 		{
// 			inCL.push_back(0);
// 		}
// 		tmin=5000;tmax=0;
// 		for(int i1=0;i1<hd.ColIndT->size();i1++)
// 		{
// 			if(hd.ColIndT->at(i1)[2]>tmax) tmax=hd.ColIndT->at(i1)[2];
// 			if(hd.ColIndT->at(i1)[2]<tmin) tmin=hd.ColIndT->at(i1)[2];
// 			if(inCL[i1]!=0) continue;
// 			cl.hi.clear();
// 			cl.hi.push_back(i1);
// 			inCL[i1]=1;
// 			nid=1;
// 			while(nid>0)
// 			{
// 				nid=0;
// 				for(int i2=0;i2<hd.ColIndT->size();i2++)
// 				{
// 					if(inCL[i2]!=0) continue;
// 					for(int i3=0;i3<cl.hi.size();i3++)
// 					{
// // 						if(sqrt(pow(H[cl.hi[i3]].col-H[i2].col,2)+pow(H[cl.hi[i3]].ind-H[i2].ind,2))<10 && (H[cl.hi[i3]].t-H[i2].t)<20)
// // 						if(sqrt(pow(hd.ColIndT->at(i1)[0]-hd.ColIndT->at(i2)[0],2)+pow(hd.ColIndT->at(i1)[1]-hd.ColIndT->at(i2)[1],2))<10 && (hd.ColIndT->at(i1)[2]-hd.ColIndT->at(i2)[2])<20)
// // 						if(fabs(sqrt(pow(hd.ColIndT->at(i1)[0]-hd.ColIndT->at(i2)[0],2)+pow(hd.ColIndT->at(i1)[1]-hd.ColIndT->at(i2)[1],2)))<5 && abs(hd.ColIndT->at(i1)[2]-hd.ColIndT->at(i2)[2])<10)
// 						if(fabs(sqrt(pow(hd.ColIndT->at(i1)[0]-hd.ColIndT->at(i2)[0],2)+pow(hd.ColIndT->at(i1)[1]-hd.ColIndT->at(i2)[1],2)))<3)
// // 						if((sqrt(pow(hd.ColIndT->at(i1)[0]-hd.ColIndT->at(i2)[0],2)+pow(hd.ColIndT->at(i1)[1]-hd.ColIndT->at(i2)[1],2))/(hd.ColIndT->at(i1)[2]-hd.ColIndT->at(i2)[2]))<=3)
// // 						if(fabs(sqrt(pow(hd.ColIndT->at(i1)[0]-hd.ColIndT->at(i2)[0],2)+pow(hd.ColIndT->at(i1)[1]-hd.ColIndT->at(i2)[1],2))/((float)(hd.ColIndT->at(i1)[2]-hd.ColIndT->at(i2)[2])))<1.5)
// 						{
// 							cl.hi.push_back(i2);
// 							nid++;
// 							inCL[i2]=1;
// 							break;
// 						}
// 					}
// 				}
// 			}
// 			CL.push_back(cl);
// 			cl.hi.clear();
// 		}
// 		AllDeltaT->Fill(tmax-tmin);
// 		
// 		int indbestcl=-1;int nhitsintr=0;
// 		for(int i2=0;i2<CL.size();i2++)
// 		{
// 			if(CL[i2].hi.size()<5) continue;
// 			tg2d->Set(0);
// 			tmin=5000;tmax=0;
// 			for(int i1=0;i1<CL[i2].hi.size();i1++)
// 			{
// 				tg2d->SetPoint(tg2d->GetN(),hd.ColIndT->at(CL[i2].hi[i1])[0],hd.ColIndT->at(CL[i2].hi[i1])[1],hd.ColIndT->at(CL[i2].hi[i1])[2]);
// 				if(hd.ColIndT->at(CL[i2].hi[i1])[2]<tmin){tmin=hd.ColIndT->at(CL[i2].hi[i1])[2];indoftmin=i1;}
// 				if(hd.ColIndT->at(CL[i2].hi[i1])[2]>tmax){tmax=hd.ColIndT->at(CL[i2].hi[i1])[2];indoftmax=i1;}
// 			}
// 		
// 			pStart[0]=hd.ColIndT->at(CL[i2].hi[indoftmin])[0];
// 			pStart[1]=((float)(hd.ColIndT->at(CL[i2].hi[indoftmax])[0]-hd.ColIndT->at(CL[i2].hi[indoftmin])[0]))/((float)(hd.ColIndT->at(CL[i2].hi[indoftmax])[2]-hd.ColIndT->at(CL[i2].hi[indoftmin])[2]));
// 			pStart[2]=hd.ColIndT->at(CL[i2].hi[indoftmin])[1];
// 			pStart[3]=((float)(hd.ColIndT->at(CL[i2].hi[indoftmax])[1]-hd.ColIndT->at(CL[i2].hi[indoftmin])[1]))/((float)(hd.ColIndT->at(CL[i2].hi[indoftmax])[2]-hd.ColIndT->at(CL[i2].hi[indoftmin])[2]));
// 			pStart[4]=1500;
// 		
// 			fitnormchi2=line3Dfit(tg2d);
// 		
// 			for(int i1=0;i1<CL.size();i1++)
// 			{
// 				if(i1==i2) continue;
// 				for(int i3=0;i3<CL[i1].hi.size();i3++)
// 				{
// 					x1=fitParams[0]+fitParams[1]*(hd.ColIndT->at(CL[i1].hi[i3])[2]-fitParams[4]);
// 					y1=fitParams[2]+fitParams[3]*(hd.ColIndT->at(CL[i1].hi[i3])[2]-fitParams[4]);
// 					if(fabs(sqrt(pow(x1-hd.ColIndT->at(CL[i1].hi[i3])[0],2.)+pow(y1-hd.ColIndT->at(CL[i1].hi[i3])[1],2.)))<10.)
// 					{
// 						tg2d->SetPoint(tg2d->GetN(),hd.ColIndT->at(CL[i1].hi[i3])[0],hd.ColIndT->at(CL[i1].hi[i3])[1],hd.ColIndT->at(CL[i1].hi[i3])[2]);
// 					}
// 				}
// 			}
// 			if(tg2d->GetN()>nhitsintr)
// 			{
// 				nhitsintr=tg2d->GetN();
// 				indbestcl=i2;
// 			}
// 		}
// 		if(indbestcl<0) continue;
// 		tg2d->Set(0);
// 		tmin=5000;tmax=0;
// 		for(int i1=0;i1<CL[indbestcl].hi.size();i1++)
// 		{
// 			tg2d->SetPoint(tg2d->GetN(),hd.ColIndT->at(CL[indbestcl].hi[i1])[0],hd.ColIndT->at(CL[indbestcl].hi[i1])[1],hd.ColIndT->at(CL[indbestcl].hi[i1])[2]);
// 			if(hd.ColIndT->at(CL[indbestcl].hi[i1])[2]<tmin){tmin=hd.ColIndT->at(CL[indbestcl].hi[i1])[2];indoftmin=i1;}
// 			if(hd.ColIndT->at(CL[indbestcl].hi[i1])[2]>tmax){tmax=hd.ColIndT->at(CL[indbestcl].hi[i1])[2];indoftmax=i1;}
// 		}
// 	
// 		pStart[0]=hd.ColIndT->at(CL[indbestcl].hi[indoftmin])[0];
// 		pStart[1]=((float)(hd.ColIndT->at(CL[indbestcl].hi[indoftmax])[0]-hd.ColIndT->at(CL[indbestcl].hi[indoftmin])[0]))/((float)(hd.ColIndT->at(CL[indbestcl].hi[indoftmax])[2]-hd.ColIndT->at(CL[indbestcl].hi[indoftmin])[2]));
// 		pStart[2]=hd.ColIndT->at(CL[indbestcl].hi[indoftmin])[1];
// 		pStart[3]=((float)(hd.ColIndT->at(CL[indbestcl].hi[indoftmax])[1]-hd.ColIndT->at(CL[indbestcl].hi[indoftmin])[1]))/((float)(hd.ColIndT->at(CL[indbestcl].hi[indoftmax])[2]-hd.ColIndT->at(CL[indbestcl].hi[indoftmin])[2]));
// 		pStart[4]=1500;
// 	
// 		fitnormchi2=line3Dfit(tg2d);
// 	
// 		for(int i1=0;i1<CL.size();i1++)
// 		{
// 			if(i1==indbestcl) continue;
// 			for(int i2=0;i2<CL[i1].hi.size();i2++)
// 			{
// 				x1=fitParams[0]+fitParams[1]*(hd.ColIndT->at(CL[i1].hi[i2])[2]-fitParams[4]);
// 				y1=fitParams[2]+fitParams[3]*(hd.ColIndT->at(CL[i1].hi[i2])[2]-fitParams[4]);
// 				if(fabs(sqrt(pow(x1-hd.ColIndT->at(CL[i1].hi[i2])[0],2.)+pow(y1-hd.ColIndT->at(CL[i1].hi[i2])[1],2.)))<10.)
// 				{
// 					CL[indbestcl].hi.push_back(CL[i1].hi[i2]);
// 					
// 					tg2d->SetPoint(tg2d->GetN(),hd.ColIndT->at(CL[i1].hi[i2])[0],hd.ColIndT->at(CL[i1].hi[i2])[1],hd.ColIndT->at(CL[i1].hi[i2])[2]);
// 					if(hd.ColIndT->at(CL[i1].hi[i2])[2]<tmin){tmin=hd.ColIndT->at(CL[i1].hi[i2])[2];indoftmin=CL[indbestcl].hi.size()-1;}
// 					if(hd.ColIndT->at(CL[i1].hi[i2])[2]>tmax){tmax=hd.ColIndT->at(CL[i1].hi[i2])[2];indoftmax=CL[indbestcl].hi.size()-1;}
// 					TrackXY->Fill(hd.ColIndT->at(CL[i1].hi[i2])[0],hd.ColIndT->at(CL[i1].hi[i2])[1]);
// 					QColtot+=hd.QColTot;
// 					
// 					CL[i1].hi[i2]=CL[i1].hi[CL[i1].hi.size()-1];
// 					CL[i1].hi.pop_back();
// 					i2--;
// 				}
// 			}
// 		}
// 		
// // 		tmin=5000;tmax=0;
// // 		QColtot=0;
// // 		tg2d->Set(0);
// // 		
// // 		for(int i1=0;i1<CL[indCLHP].hi.size();i1++)
// // 		{
// // 			tg2d->SetPoint(tg2d->GetN(),hd.ColIndT->at(CL[indCLHP].hi[i1])[0],hd.ColIndT->at(CL[indCLHP].hi[i1])[1],hd.ColIndT->at(CL[indCLHP].hi[i1])[2]);
// // 			if(hd.ColIndT->at(CL[indCLHP].hi[i1])[2]<tmin){tmin=hd.ColIndT->at(CL[indCLHP].hi[i1])[2];indoftmin=i1;}
// // 			if(hd.ColIndT->at(CL[indCLHP].hi[i1])[2]>tmax){tmax=hd.ColIndT->at(CL[indCLHP].hi[i1])[2];indoftmax=i1;}
// // 			TrackXY->Fill(hd.ColIndT->at(CL[indCLHP].hi[i1])[0],hd.ColIndT->at(CL[indCLHP].hi[i1])[1]);
// // 			QColtot+=hd.QColTot;
// // 		}
// // 		
// // 		pStart[0]=hd.ColIndT->at(CL[indCLHP].hi[indoftmin])[0];
// // 		pStart[1]=((float)(hd.ColIndT->at(CL[indCLHP].hi[indoftmax])[0]-hd.ColIndT->at(CL[indCLHP].hi[indoftmin])[0]))/((float)(hd.ColIndT->at(CL[indCLHP].hi[indoftmax])[2]-hd.ColIndT->at(CL[indCLHP].hi[indoftmin])[2]));
// // 		pStart[2]=hd.ColIndT->at(CL[indCLHP].hi[indoftmin])[1];
// // 		pStart[3]=((float)(hd.ColIndT->at(CL[indCLHP].hi[indoftmax])[1]-hd.ColIndT->at(CL[indCLHP].hi[indoftmin])[1]))/((float)(hd.ColIndT->at(CL[indCLHP].hi[indoftmax])[2]-hd.ColIndT->at(CL[indCLHP].hi[indoftmin])[2]));
// // 		pStart[4]=1500;
// // 		
// // 		fitnormchi2=line3Dfit(tg2d);
// 		
// // 		cout<<I<<endl;
// // 		cout<<"before"<<endl;
// // 		for(int i1=0;i1<CL.size();i1++)
// // 		{
// // 			cout<<i1<<" "<<CL[i1].hi.size()<<endl;
// // 		}
// 		
// // 		if(I<10)
// // 		{
// // 			sprintf(hname,"TrackHits_%d",I);
// // 			tg2d->SetName(hname);tg2d->SetTitle(hname);
// // 			outroot->cd();
// // 			tg2d->Write();
// // 		}
// 		
// // 		for(int i1=0;i1<CL.size();i1++)
// // 		{
// // 			if(i1==indCLHP) continue;
// // 			for(int i2=0;i2<CL[i1].hi.size();i2++)
// // 			{
// // 				x1=fitParams[0]+fitParams[1]*(hd.ColIndT->at(CL[i1].hi[i2])[2]-fitParams[4]);
// // 				y1=fitParams[2]+fitParams[3]*(hd.ColIndT->at(CL[i1].hi[i2])[2]-fitParams[4]);
// // 				if(fabs(sqrt(pow(x1-hd.ColIndT->at(CL[i1].hi[i2])[0],2.)+pow(y1-hd.ColIndT->at(CL[i1].hi[i2])[1],2.)))<10.)
// // 				{
// // 					CL[indCLHP].hi.push_back(CL[i1].hi[i2]);
// // 					
// // 					tg2d->SetPoint(tg2d->GetN(),hd.ColIndT->at(CL[i1].hi[i2])[0],hd.ColIndT->at(CL[i1].hi[i2])[1],hd.ColIndT->at(CL[i1].hi[i2])[2]);
// // 					if(hd.ColIndT->at(CL[i1].hi[i2])[2]<tmin){tmin=hd.ColIndT->at(CL[i1].hi[i2])[2];indoftmin=CL[indCLHP].hi.size()-1;}
// // 					if(hd.ColIndT->at(CL[i1].hi[i2])[2]>tmax){tmax=hd.ColIndT->at(CL[i1].hi[i2])[2];indoftmax=CL[indCLHP].hi.size()-1;}
// // 					TrackXY->Fill(hd.ColIndT->at(CL[i1].hi[i2])[0],hd.ColIndT->at(CL[i1].hi[i2])[1]);
// // 					QColtot+=hd.QColTot;
// // 					
// // 					CL[i1].hi[i2]=CL[i1].hi[CL[i1].hi.size()-1];
// // 					CL[i1].hi.pop_back();
// // 					i2--;
// // 				}
// // 			}
// // 		}
// 		
// 		
// // 		cout<<"after"<<endl;
// // 		for(int i1=0;i1<CL.size();i1++)
// // 		{
// // 			cout<<i1<<" "<<CL[i1].hi.size()<<endl;
// // 		}
// 		for(int i1=0;i1<CL.size();i1++)
// 		{
// 			if(CL[i1].hi.size()==0)
// 			{
// 				CL[i1].hi=CL[CL.size()-1].hi;
// 				CL.pop_back();
// 				i1--;
// 			}
// 		}
// 		
// 		int HitsinCLs1=0;int HitsinCLs2=0;
// 		indCLHP=-1;maxCLsize=0;
// 		for(int i1=0;i1<CL.size();i1++)
// 		{
// 			if(CL[i1].hi.size()>maxCLsize){maxCLsize=CL[i1].hi.size();indCLHP=i1;}
// 			HitsinCLs1+=CL[i1].hi.size();
// 		}
// 		
// 		indCLHP=-1;maxCLsize=0;
// 		for(int i1=0;i1<CL.size();i1++)
// 		{
// 			ClSize->Fill(CL[i1].hi.size());
// 			if(CL[i1].hi.size()>maxCLsize){maxCLsize=CL[i1].hi.size();indCLHP=i1;}
// 			HitsinCLs2+=CL[i1].hi.size();
// 		}
// // 		cout<<I<<" "<<hd.ColIndT->size()<<" "<<HitsinCLs1<<" "<<HitsinCLs2<<endl;
// 		
// // 		cout<<"after 2"<<endl;
// // 		for(int i1=0;i1<CL.size();i1++)
// // 		{
// // 			cout<<i1<<" "<<CL[i1].hi.size()<<endl;
// // 		}
// // 		cout<<endl;
// 		
// 		maxCLsize=CL[indCLHP].hi.size();
// 		MaxClSize->Fill(maxCLsize);
// 		NClperEvent->Fill(CL.size());
// 		
// 		pStart[0]=hd.ColIndT->at(CL[indCLHP].hi[indoftmin])[0];
// 		pStart[1]=((float)(hd.ColIndT->at(CL[indCLHP].hi[indoftmax])[0]-hd.ColIndT->at(CL[indCLHP].hi[indoftmin])[0]))/((float)(hd.ColIndT->at(CL[indCLHP].hi[indoftmax])[2]-hd.ColIndT->at(CL[indCLHP].hi[indoftmin])[2]));
// 		pStart[2]=hd.ColIndT->at(CL[indCLHP].hi[indoftmin])[1];
// 		pStart[3]=((float)(hd.ColIndT->at(CL[indCLHP].hi[indoftmax])[1]-hd.ColIndT->at(CL[indCLHP].hi[indoftmin])[1]))/((float)(hd.ColIndT->at(CL[indCLHP].hi[indoftmax])[2]-hd.ColIndT->at(CL[indCLHP].hi[indoftmin])[2]));
// 		pStart[4]=1500;
// 		
// 		fitnormchi2=line3Dfit(tg2d);
// 		
// 		TrackDeltaT->Fill(tmax-tmin);
// 		FitNormChi2->Fill(fitnormchi2);
// 		
// // 		td.E=I;
// 		td.E=hd.E;
// 		td.StartEndColIndT->clear();
// 		td.StartEndColIndT->push_back(hd.ColIndT->at(CL[indCLHP].hi[indoftmin])[0]);
// 		td.StartEndColIndT->push_back(hd.ColIndT->at(CL[indCLHP].hi[indoftmin])[1]);
// 		td.StartEndColIndT->push_back(hd.ColIndT->at(CL[indCLHP].hi[indoftmin])[2]);
// 		td.StartEndColIndT->push_back(hd.ColIndT->at(CL[indCLHP].hi[indoftmax])[0]);
// 		td.StartEndColIndT->push_back(hd.ColIndT->at(CL[indCLHP].hi[indoftmax])[1]);
// 		td.StartEndColIndT->push_back(hd.ColIndT->at(CL[indCLHP].hi[indoftmax])[2]);
// 		td.FitParams->clear();
// 		for(int is2=0;is2<5;is2++)
// 		{
// 			td.FitParams->push_back(fitParams[is2]);
// 		}
// 		td.FitNormChi2=fitnormchi2;
// 		td.QColTot=QColtot;
// 		td.NHits=CL[indCLHP].hi.size();
// 		td.Nexcl=hd.ColIndT->size()-CL[indCLHP].hi.size();
// 		td.PMTIntegral->clear();
// 		for(int is2=0;is2<3;is2++)
// 		{
// 			td.PMTIntegral->push_back(hd.PMTIntegral->at(is2));
// 		}
// 		
// 		T1->Fill();
// 		
// 		if(fitnormchi2<5.)
// 		{
// // // 			cc->cd(); 
// // // 			frame3d->Draw();
// // // 			
// // // 			pm3d1 = new TPolyMarker3D();
// // // 			pm3d2 = new TPolyMarker3D();
// // // 			
// // // // 			pm3d1 = new TPolyMarker3D(CL[indCLHP].hi.size());
// // // // // 			pm3d2 = new TPolyMarker3D(H.size()-CL[indCLHP].hi.size());
// // // // 			pm3d2 = new TPolyMarker3D(hd.ColIndT->size()-CL[indCLHP].hi.size());
// // // 			
// // // 			for(int i1=0;i1<CL[indCLHP].hi.size();i1++)
// // // 			{
// // // // 				pm3d1->SetPoint(i1,H[CL[indCLHP].hi[i1]].col,H[CL[indCLHP].hi[i1]].ind,H[CL[indCLHP].hi[i1]].t);
// // // 				pm3d1->SetPoint(i1,hd.ColIndT->at(CL[indCLHP].hi[i1])[0],hd.ColIndT->at(CL[indCLHP].hi[i1])[1],hd.ColIndT->at(CL[indCLHP].hi[i1])[2]);
// // // 			}
// // // 			
// // // 			int na=0;
// // // 			for(int i1=0;i1<CL.size();i1++)
// // // 			{
// // // 				if(i1==indCLHP) continue;
// // // 				for(int i2=0;i2<CL[i1].hi.size();i2++)
// // // 				{
// // // // 					pm3d2->SetPoint(na,H[CL[i1].hi[i2]].col,H[CL[i1].hi[i2]].ind,H[CL[i1].hi[i2]].t);
// // // 					pm3d2->SetPoint(na,hd.ColIndT->at(CL[i1].hi[i2])[0],hd.ColIndT->at(CL[i1].hi[i2])[1],hd.ColIndT->at(CL[i1].hi[i2])[2]);
// // // 					na++;
// // // 				}
// // // 			}
// // // // 					pm3d1->SetMarkerSize(2);
// // // 			pm3d1->SetMarkerColor(kRed);
// // // 			pm3d1->SetMarkerStyle(24);   
// // // 			pm3d1->Draw();
// // // // 					pm3d2->SetMarkerSize(2);
// // // 			pm3d2->SetMarkerColor(kBlue);
// // // 			pm3d2->SetMarkerStyle(24);   
// // // 			pm3d2->Draw();
// // // 			
// // // 			l = new TPolyLine3D(2);
// 			
// 			x1=fitParams[0]+fitParams[1]*(tmin-fitParams[4]);
// 			y1=fitParams[2]+fitParams[3]*(tmin-fitParams[4]);
// 			z1=tmin;
// // // 			l->SetPoint(0,x1,y1,z1);
// 			
// 			x2=fitParams[0]+fitParams[1]*(tmax-fitParams[4]);
// 			y2=fitParams[2]+fitParams[3]*(tmax-fitParams[4]);
// 			z2=tmax;
// // // 			l->SetPoint(1,x2,y2,z2);
// // // 			l->SetLineColor(kBlack);
// // // 			l->SetLineWidth(2);
// // // 			l->Draw("same");
// 			
// 			DeltaTvsTrackAngle->Fill(atan(sqrt(pow(x2-x1,2)+pow(y2-y1,2))/(tmax-tmin)),(tmax-tmin));
// 			DeltaTvsTrackAnglePr->Fill(atan(sqrt(pow(x2-x1,2)+pow(y2-y1,2))/(tmax-tmin)),(tmax-tmin));
// 			Tfirst->Fill(tmin);
// 			Tlast->Fill(tmax);
// //					if(tmin<=1160) Tlast->Fill(tmax);
// 			Trackt0->Fill(fitParams[4]);
// 			TrackEntryXY->Fill(x1,y1);
// 			TrackExitXY->Fill(x2,y2);
// 			if(tmin<=1160 && tmax>=1920) TrackDeltaTSel->Fill(tmax-tmin);
// 			DeltaTvsTrackt0->Fill(fitParams[4],tmax-tmin);
// 			DeltaTLengthvsAngle->Fill(atan(sqrt(pow(x2-x1,2)+pow(y2-y1,2))/(tmax-tmin)),sqrt(pow(x2-x1,2)+pow(y2-y1,2)+pow(tmax-tmin,2)),tmax-tmin);
// 			DeltaTLengthvsAngle_norm->Fill(atan(sqrt(pow(x2-x1,2)+pow(y2-y1,2))/(tmax-tmin)),sqrt(pow(x2-x1,2)+pow(y2-y1,2)+pow(tmax-tmin,2)));
// 			
// //					cout<<Ev<<" "<<tmin<<" "<<tmax<<" "<<(tmax-tmin)<<endl;
// 			
// // 			sprintf(hname,"Ev_%d_%5.3f",td.E,fitnormchi2);
// // 			cc->SetName(hname);cc->SetTitle(hname);
// // 			outroot->cd();
// // 			cc->Write();
// // 			tg2dA->Set(0);
// // 			delete l;
// 			
// 			FitClSize->Fill(CL[indCLHP].hi.size());
// 			NExclHits->Fill(hd.ColIndT->size()-CL[indCLHP].hi.size());
// // 			if(CL[indCLHP].hi.size()>15 && (H.size()-CL[indCLHP].hi.size())<10)
// // 			{
// // 				Nvalid++;
// // 				outfile<<Ev<<" "<<fitParams[0]<<" "<<fitParams[1]<<" "<<fitParams[2]<<" "<<fitParams[3]<<" "<<fitParams[4]<<" "<<fitnormchi2<<" "<<CL[indCLHP].hi.size()<<endl;
// // 			}
// // 			if(td.NHits>15 && td.Nexcl<10)
// 			if(td.NHits>10 && td.Nexcl<10)
// 			{
// 				outfile<<td.E<<" "<<hd.ColIndT->at(CL[indCLHP].hi[indoftmin])[0]<<" "<<hd.ColIndT->at(CL[indCLHP].hi[indoftmin])[1]<<" "<<hd.ColIndT->at(CL[indCLHP].hi[indoftmin])[2]<<" "<<hd.ColIndT->at(CL[indCLHP].hi[indoftmax])[0]<<" "<<hd.ColIndT->at(CL[indCLHP].hi[indoftmax])[1]<<" "<<hd.ColIndT->at(CL[indCLHP].hi[indoftmax])[2]<<endl;
// 			}
// 		}
// 		tg2d->Set(0);
// 		inCL.clear();
// 		cl.hi.clear();
// 		CL.clear();
// // 		if(I==10) break;
// 		if(I%100==0) cout<<I<<" / "<<T->GetEntries()<<endl;
// 	}
	
// 	for(int I=0;I<T->GetEntries();I++)
// 	{
// 		T->GetEntry(I);
// 		
// 		if(hd.ColIndT->size()>300) continue;
// 		for(int i1=0;i1<hd.ColIndT->size();i1++)
// 		{
// 			for(int i2=i1+1;i2<hd.ColIndT->size();i2++)
// 			{
// 				DeltaRperDeltat->Fill(fabs(sqrt(pow(hd.ColIndT->at(i1)[0]-hd.ColIndT->at(i2)[0],2)+pow(hd.ColIndT->at(i1)[1]-hd.ColIndT->at(i2)[1],2))/((float)(hd.ColIndT->at(i1)[2]-hd.ColIndT->at(i2)[2]))));
// 			}
// 		}
// 	}
	
// 	for(int I=0;I<T->GetEntries();I++)
// 	{
// 		T->GetEntry(I);
// 		if(hd.ColIndT->size()<5) continue;
// 		
// 		for(int i1=0;i1<hd.ColIndT->size();i1++)
// 		{
// 			for(int i2=i1+1;i2<hd.ColIndT->size();i2++)
// 			{
// 				DeltaRperDeltat->Fill(fabs((sqrt(pow(hd.ColIndT->at(i1)[0]-hd.ColIndT->at(i2)[0],2.)+pow(hd.ColIndT->at(i1)[1]-hd.ColIndT->at(i2)[1],2.))/((float)(hd.ColIndT->at(i1)[2]-hd.ColIndT->at(i2)[2])))));
// 			}
// 		}
// 		if(I%100==0) cout<<I<<" / "<<T->GetEntries()<<endl;
// 	}
	
	
	for(int I=0;I<T->GetEntries();I++)
	{
		T->GetEntry(I);
		if(hd.ColIndT->size()<5) continue;
		if(hd.ColIndT->size()>300) continue;
		
		vector <clusters> CL;
		cl.hi.clear();
		inCL.clear();
		for(int i1=0;i1<hd.ColID->size();i1++)
		{
			inCL.push_back(0);
		}
		tminall=5000;tmaxall=0;
		for(int i1=0;i1<hd.ColID->size();i1++)
		{
			if(hd.ColT->at(i1)>tmaxall) tmaxall=hd.ColT->at(i1);
			if(hd.ColT->at(i1)<tminall) tminall=hd.ColT->at(i1);
			if(inCL[i1]!=0) continue;
			cl.hi.clear();
			cl.hi.push_back(i1);
			inCL[i1]=1;
			nid=1;
			while(nid>0)
			{
				nid=0;
				for(int i2=0;i2<hd.ColID->size();i2++)
				{
					if(inCL[i2]!=0) continue;
					for(int i3=0;i3<cl.hi.size();i3++)
					{
// 						if(abs(hd.ColID->at(i2)-hd.ColID->at(cl.hi[i3]))<3 && abs(hd.ColT->at(i2)-hd.ColT->at(cl.hi[i3]))<10)
// 						if(abs(hd.ColID->at(i2)-hd.ColID->at(cl.hi[i3]))<=1 && abs(hd.ColT->at(i2)-hd.ColT->at(cl.hi[i3]))<=100)
						if(abs(hd.ColID->at(i2)-hd.ColID->at(cl.hi[i3]))<=1)
// 						if(fabs(sqrt(pow(hd.ColID->at(i2)-hd.ColID->at(cl.hi[i3]),2)+pow(hd.ColT->at(i2)-hd.ColT->at(cl.hi[i3]),2)))<140)
// 						if(fabs(((float)hd.ColIndT->at(i1)[0]-hd.ColIndT->at(i2)[0])/((float)hd.ColIndT->at(i1)[2]-hd.ColIndT->at(i2)[2]))<1.5)
						{
							cl.hi.push_back(i2);
							nid++;
							inCL[i2]=1;
							break;
						}
					}
				}
			}
			CL.push_back(cl);
			cl.hi.clear();
		}
		AllDeltaT->Fill(tmaxall-tminall);
		int HitsinCLs1=0;int HitsinCLs2=0;
		indCLHP=-1;maxCLsize=0;
		for(int i1=0;i1<CL.size();i1++)
		{
			if(CL[i1].hi.size()>maxCLsize){maxCLsize=CL[i1].hi.size();indCLHP=i1;}
			HitsinCLs1+=CL[i1].hi.size();
		}
		tg1dcol->Set(0);tg1dind->Set(0);
		
		tmincol=5000;tmaxcol=0;
		mincol=200;maxcol=0;
		for(int i1=0;i1<CL[indCLHP].hi.size();i1++)
		{
			tg1dcol->SetPoint(tg1dcol->GetN(),hd.ColT->at(CL[indCLHP].hi[i1]),hd.ColID->at(CL[indCLHP].hi[i1]));
// 			tg1dind->SetPoint(tg1dind->GetN(),hd.ColIndT->at(CL[indCLHP].hi[i1])[2],hd.ColIndT->at(CL[indCLHP].hi[i1])[1]);
			if(hd.ColT->at(CL[indCLHP].hi[i1])>tmaxcol) {tmaxcol=hd.ColT->at(CL[indCLHP].hi[i1]);indoftmax=i1;}
			if(hd.ColT->at(CL[indCLHP].hi[i1])<tmincol) {tmincol=hd.ColT->at(CL[indCLHP].hi[i1]);indoftmin=i1;}
			if(hd.ColID->at(CL[indCLHP].hi[i1])>maxcol) {maxcol=hd.ColID->at(CL[indCLHP].hi[i1]);}
			if(hd.ColID->at(CL[indCLHP].hi[i1])<mincol) {mincol=hd.ColID->at(CL[indCLHP].hi[i1]);}
		}
		ColTrackDeltaT->Fill(tmaxcol-tmincol);
		
		tflin->SetParameter(0,mincol);
		tflin->SetParameter(1,((float)(maxcol-mincol))/(tmaxcol-tmincol));
		tg1dcol->Fit(tflin,"q","q",tmincol-1,tmaxcol+1);
		fitnormchi2col=tflin->GetChisquare()/tflin->GetNDF();
		tflinpars[0][0]=tflin->GetParameter(0);tflinpars[0][1]=tflin->GetParameter(1);
// 		tg1dind->Fit(tflin,"q","q",tmin-1,tmax+1);
// 		fitnormchi2ind=tflin->GetChisquare()/tflin->GetNDF();
// 		tflinpars[1][0]=tflin->GetParameter(0);tflinpars[1][1]=tflin->GetParameter(1);
		
		if(I<10)
		{
			sprintf(hname,"Col_vs_t_%d",I);
			tg1dcol->SetName(hname);tg1dcol->SetTitle(hname);
			tg1dcol->SetMarkerStyle(20);
			outroot->cd();
			tg1dcol->Write();
		}
		
// 		for(int i1=0;i1<tg1dcol->GetN();i1++)
// 		{
// 			tg1dcol->GetPoint(i1,x1,y1);
// 			if(fabs(tflinpars[0][0]+tflinpars[0][1]*x1-y1)>10)
// 			{
// 				tg1dcol->RemovePoint(i1);
// 				i1--;
// 			}
// 		}
// 		for(int i1=0;i1<tg1dind->GetN();i1++)
// 		{
// 			tg1dind->GetPoint(i1,x1,y1);
// 			if(fabs(tflinpars[1][0]+tflinpars[1][1]*x1-y1)>10)
// 			{
// 				tg1dind->RemovePoint(i1);
// 				i1--;
// 			}
// 		}
// 		
// 		tg1dcol->Fit(tflin,"q","q",tmin-1,tmax+1);
// 		fitnormchi2col=tflin->GetChisquare()/tflin->GetNDF();
// 		tflinpars[0][0]=tflin->GetParameter(0);tflinpars[0][1]=tflin->GetParameter(1);
// 		tg1dind->Fit(tflin,"q","q",tmin-1,tmax+1);
// 		fitnormchi2ind=tflin->GetChisquare()/tflin->GetNDF();
// 		tflinpars[1][0]=tflin->GetParameter(0);tflinpars[1][1]=tflin->GetParameter(1);
		
		
// 		bool nd=true;
// 		while(nd)
// 		{
// 			nd=false;
// 			tg1dcol->Fit(tflin,"q","q",tmin-1,tmax+1);
// 			tflinpars[0][0]=tflin->GetParameter(0);tflinpars[0][1]=tflin->GetParameter(1);
// 			for(int i1=0;i1<tg1dcol->GetN();i1++)
// 			{
// 				tg1dcol->GetPoint(i1,x1,y1);
// 				if(fabs(tflinpars[0][0]+tflinpars[0][1]*x1-y1)>10){tg1dcol->RemovePoint(i1);nd=true;break;}
// 			}
// 		}
// 		fitnormchi2col=tflin->GetChisquare()/tflin->GetNDF();
		
		FitNormChi2_Col_vs_t->Fill(fitnormchi2col);
		NColHits_vs_fitnormchi2->Fill(fitnormchi2col,tg1dcol->GetN());
		
		FitNormChi2_Ind_vs_t->Fill(fitnormchi2ind);
		NIndHits_vs_fitnormchi2->Fill(fitnormchi2ind,tg1dind->GetN());
		
// 		cout<<I<<" "<<hd.ColIndT->at(nmaxind)[0]<<" "<<hd.ColIndT->at(nmaxind)[1]<<" "<<hd.ColIndT->at(nmaxind)[2]<<" "<<nmax<<" "<<fitnormchi2col<<" "<<fitnormchi2ind<<endl;
		
// 		if(fitnormchi2col>100 || fitnormchi2ind>100) continue;
		
		
		
// 		sprintf(hname,"[0]+%f*x",ameancol);
// 		tflin2=new TF1("lin2",hname,750,2500);
// 		tg1dcol->Fit(tflin2,"q","q",tmin-1,tmax+1);
		
		
		
// 		tmin=5000;tmax=0;
// 		indoftmin=0;indoftmax=0;
// 		for(int i1=0;i1<hd.IndID->size();i1++)
// 		{
// 			tg1dind->SetPoint(tg1dind->GetN(),hd.IndT->at(i1),hd.IndID->at(i1));
// 			if(hd.IndT->at(i1)<tmin) {tmin=hd.IndT->at(i1);indoftmin=i1;}
// 			if(hd.IndT->at(i1)>tmax) {tmax=hd.IndT->at(i1);indoftmax=i1;}
// 		}
// 		tg1dind->Fit(tflin,"q","q",tmin-1,tmax+1);
// // 		tg1dind->Fit(tflin,"q","q",tmin-1,tmin+500);
// 		fitnormchi2ind=tflin->GetChisquare()/tflin->GetNDF();
// 		tflinpars[1][0]=tflin->GetParameter(0);tflinpars[1][1]=tflin->GetParameter(1);
// 		
// 		nd=true;
// 		while(nd)
// 		{
// 			nd=false;
// 			tg1dind->Fit(tflin,"q","q",tmin-1,tmax+1);
// 			tflinpars[1][0]=tflin->GetParameter(0);tflinpars[1][1]=tflin->GetParameter(1);
// 			for(int i1=0;i1<tg1dind->GetN();i1++)
// 			{
// 				tg1dind->GetPoint(i1,x1,y1);
// 				if(fabs(tflinpars[1][0]+tflinpars[1][1]*x1-y1)>10){tg1dind->RemovePoint(i1);nd=true;break;}
// 			}
// 		}
// 		fitnormchi2col=tflin->GetChisquare()/tflin->GetNDF();
		
		
		
// 		if(fitnormchi2col>100 || fitnormchi2ind>100) continue;
		
		
		//for the track
		tmintrack=5000;tmaxtrack=0;
		indoftmin=0;indoftmax=0;Nvalid=0;
		
		QColtot=0;
		tg2d->Set(0);
		for(int i1=0;i1<hd.ColIndT->size();i1++)
		{
			if(float(hd.ColIndT->at(i1)[2])<tmincol || float(hd.ColIndT->at(i1)[2])>tmaxcol) continue;
			x1=tflinpars[0][0]+tflinpars[0][1]*((float)hd.ColIndT->at(i1)[2]);
// 			y1=tflinpars[1][0]+tflinpars[1][1]*hd.ColIndT->at(i1)[2];
// 			if(sqrt(pow(x1-hd.ColIndT->at(i1)[0],2)+pow(y1-hd.ColIndT->at(i1)[1],2))<=10.)
			if(fabs(x1-hd.ColIndT->at(i1)[0])<=10.)
			{
				tg2d->SetPoint(tg2d->GetN(),hd.ColIndT->at(i1)[0],hd.ColIndT->at(i1)[1],hd.ColIndT->at(i1)[2]);
				Nvalid++;
				if(((float)hd.ColIndT->at(i1)[2])>tmaxtrack) {tmaxtrack=hd.ColIndT->at(i1)[2];indoftmax=i1;}
				if(((float)hd.ColIndT->at(i1)[2])<tmintrack) {tmintrack=hd.ColIndT->at(i1)[2];indoftmin=i1;}
			}
		}
		pStart[0]=hd.ColIndT->at(indoftmin)[0];
		pStart[1]=((float)(hd.ColIndT->at(indoftmax)[0]-hd.ColIndT->at(indoftmin)[0]))/((float)(hd.ColIndT->at(indoftmax)[2]-hd.ColIndT->at(indoftmin)[2]));
		pStart[2]=hd.ColIndT->at(indoftmin)[1];
		pStart[3]=((float)(hd.ColIndT->at(indoftmax)[1]-hd.ColIndT->at(indoftmin)[1]))/((float)(hd.ColIndT->at(indoftmax)[2]-hd.ColIndT->at(indoftmin)[2]));
		pStart[4]=750;
		
		fitnormchi2=line3Dfit(tg2d);
		
		TrackDeltaT->Fill(tmaxtrack-tmintrack);
		FitNormChi2->Fill(fitnormchi2);
		
// 		if(fitnormchi2<5.)
		{
			td.E=hd.E;
			td.StartEndColIndT->clear();
			td.StartEndColIndT->push_back(hd.ColIndT->at(indoftmin)[0]);
			td.StartEndColIndT->push_back(hd.ColIndT->at(indoftmin)[1]);
			td.StartEndColIndT->push_back(hd.ColIndT->at(indoftmin)[2]);
			td.StartEndColIndT->push_back(hd.ColIndT->at(indoftmax)[0]);
			td.StartEndColIndT->push_back(hd.ColIndT->at(indoftmax)[1]);
			td.StartEndColIndT->push_back(hd.ColIndT->at(indoftmax)[2]);
			td.FitParams->clear();
			for(int is2=0;is2<5;is2++)
			{
				td.FitParams->push_back(fitParams[is2]);
			}
			td.FitNormChi2=fitnormchi2;
			td.QColTot=QColtot;
			td.NHits=Nvalid;
			td.Nexcl=hd.ColIndT->size()-Nvalid;
			td.PMTIntegral->clear();
			for(int is2=0;is2<3;is2++)
			{
				td.PMTIntegral->push_back(hd.PMTIntegral->at(is2));
			}
			
			T1->Fill();
			
			x1=fitParams[0]+fitParams[1]*(tmintrack-fitParams[4]);
			y1=fitParams[2]+fitParams[3]*(tmintrack-fitParams[4]);
			z1=tmintrack;
			
			x2=fitParams[0]+fitParams[1]*(tmaxtrack-fitParams[4]);
			y2=fitParams[2]+fitParams[3]*(tmaxtrack-fitParams[4]);
			z2=tmaxtrack;
			
// 			DeltaTvsTrackAngle->Fill(atan(sqrt(pow(x2-x1,2)+pow(y2-y1,2))/(tmax-tmin)),(tmax-tmin));
// 			DeltaTvsTrackAnglePr->Fill(atan(sqrt(pow(x2-x1,2)+pow(y2-y1,2))/(tmax-tmin)),(tmax-tmin));
			Tfirst->Fill(tmincol);
			Tlast->Fill(tmaxcol);
			Trackt0->Fill(fitParams[4]);
			TrackEntryXY->Fill(x1,y1);
			TrackExitXY->Fill(x2,y2);
// 			if(tmin<=1160 && tmax>=1920) TrackDeltaTSel->Fill(tmax-tmin);
			DeltaTvsTrackt0->Fill(fitParams[4],tmaxcol-tmincol);
// 			DeltaTLengthvsAngle->Fill(atan(sqrt(pow(x2-x1,2)+pow(y2-y1,2))/(tmax-tmin)),sqrt(pow(x2-x1,2)+pow(y2-y1,2)+pow(tmax-tmin,2)),tmax-tmin);
// 			DeltaTLengthvsAngle_norm->Fill(atan(sqrt(pow(x2-x1,2)+pow(y2-y1,2))/(tmax-tmin)),sqrt(pow(x2-x1,2)+pow(y2-y1,2)+pow(tmax-tmin,2)));
			NExclHits->Fill(td.Nexcl);
// // 			if(td.NHits>15 && td.Nexcl<10)
// 			if(td.NHits>10 && td.Nexcl<10)
// 			{
// 				outfile<<td.E<<" "<<hd.ColIndT->at(CL[indCLHP].hi[indoftmin])[0]<<" "<<hd.ColIndT->at(CL[indCLHP].hi[indoftmin])[1]<<" "<<hd.ColIndT->at(CL[indCLHP].hi[indoftmin])[2]<<" "<<hd.ColIndT->at(CL[indCLHP].hi[indoftmax])[0]<<" "<<hd.ColIndT->at(CL[indCLHP].hi[indoftmax])[1]<<" "<<hd.ColIndT->at(CL[indCLHP].hi[indoftmax])[2]<<endl;
// 			}
		}
		if(I%100==0) cout<<I<<" / "<<T->GetEntries()<<endl;
		if(I>50) break;
	}
	
// 	for(int I=0;I<T->GetEntries();I++)
// 	{
// 		T->GetEntry(I);
// 		
// 		if(hd.ColIndT->size()<5) continue;
// 		
// // 		if(hd.ColIndT->size()<20) continue;
// 		vector <clusters> CL;
// 		cl.hi.clear();
// 		inCL.clear();
// 		for(int i1=0;i1<hd.ColIndT->size();i1++)
// 		{
// 			inCL.push_back(0);
// 		}
// 		tmin=5000;tmax=0;
// 		for(int i1=0;i1<hd.ColIndT->size();i1++)
// 		{
// 			if(hd.ColIndT->at(i1)[2]>tmax) tmax=hd.ColIndT->at(i1)[2];
// 			if(hd.ColIndT->at(i1)[2]<tmin) tmin=hd.ColIndT->at(i1)[2];
// 			if(inCL[i1]!=0) continue;
// 			cl.hi.clear();
// 			cl.hi.push_back(i1);
// 			inCL[i1]=1;
// 			nid=1;
// 			while(nid>0)
// 			{
// 				nid=0;
// 				for(int i2=0;i2<hd.ColIndT->size();i2++)
// 				{
// 					if(inCL[i2]!=0) continue;
// 					for(int i3=0;i3<cl.hi.size();i3++)
// 					{
// // 						if(sqrt(pow(H[cl.hi[i3]].col-H[i2].col,2)+pow(H[cl.hi[i3]].ind-H[i2].ind,2))<10 && (H[cl.hi[i3]].t-H[i2].t)<20)
// // 						if(sqrt(pow(hd.ColIndT->at(i1)[0]-hd.ColIndT->at(i2)[0],2)+pow(hd.ColIndT->at(i1)[1]-hd.ColIndT->at(i2)[1],2))<10 && (hd.ColIndT->at(i1)[2]-hd.ColIndT->at(i2)[2])<20)
// 						if(sqrt(pow(hd.ColIndT->at(i1)[0]-hd.ColIndT->at(i2)[0],2)+pow(hd.ColIndT->at(i1)[1]-hd.ColIndT->at(i2)[1],2))<5 && (hd.ColIndT->at(i1)[2]-hd.ColIndT->at(i2)[2])<30)
// 						{
// 							cl.hi.push_back(i2);
// 							nid++;
// 							inCL[i2]=1;
// 							break;
// 						}
// 					}
// 				}
// 			}
// 			CL.push_back(cl);
// 			cl.hi.clear();
// 		}
// 		AllDeltaT->Fill(tmax-tmin);
// 		
// 		int HitsinCLs1=0;int HitsinCLs2=0;
// 		indCLHP=-1;maxCLsize=0;
// 		for(int i1=0;i1<CL.size();i1++)
// 		{
// 			if(CL[i1].hi.size()>maxCLsize){maxCLsize=CL[i1].hi.size();indCLHP=i1;}
// 			HitsinCLs1+=CL[i1].hi.size();
// 		}
// 		
// 		tmin=5000;tmax=0;
// 		QColtot=0;
// 		tg2d->Set(0);
// 		
// 		for(int i1=0;i1<CL[indCLHP].hi.size();i1++)
// 		{
// 			tg2d->SetPoint(tg2d->GetN(),hd.ColIndT->at(CL[indCLHP].hi[i1])[0],hd.ColIndT->at(CL[indCLHP].hi[i1])[1],hd.ColIndT->at(CL[indCLHP].hi[i1])[2]);
// 			if(hd.ColIndT->at(CL[indCLHP].hi[i1])[2]<tmin){tmin=hd.ColIndT->at(CL[indCLHP].hi[i1])[2];indoftmin=i1;}
// 			if(hd.ColIndT->at(CL[indCLHP].hi[i1])[2]>tmax){tmax=hd.ColIndT->at(CL[indCLHP].hi[i1])[2];indoftmax=i1;}
// 			TrackXY->Fill(hd.ColIndT->at(CL[indCLHP].hi[i1])[0],hd.ColIndT->at(CL[indCLHP].hi[i1])[1]);
// 			QColtot+=hd.QColTot;
// 		}
// 		
// 		pStart[0]=hd.ColIndT->at(CL[indCLHP].hi[indoftmin])[0];
// 		pStart[1]=((float)(hd.ColIndT->at(CL[indCLHP].hi[indoftmax])[0]-hd.ColIndT->at(CL[indCLHP].hi[indoftmin])[0]))/((float)(hd.ColIndT->at(CL[indCLHP].hi[indoftmax])[2]-hd.ColIndT->at(CL[indCLHP].hi[indoftmin])[2]));
// 		pStart[2]=hd.ColIndT->at(CL[indCLHP].hi[indoftmin])[1];
// 		pStart[3]=((float)(hd.ColIndT->at(CL[indCLHP].hi[indoftmax])[1]-hd.ColIndT->at(CL[indCLHP].hi[indoftmin])[1]))/((float)(hd.ColIndT->at(CL[indCLHP].hi[indoftmax])[2]-hd.ColIndT->at(CL[indCLHP].hi[indoftmin])[2]));
// 		pStart[4]=1500;
// 		
// 		fitnormchi2=line3Dfit(tg2d);
// 		
// // 		cout<<I<<endl;
// // 		cout<<"before"<<endl;
// // 		for(int i1=0;i1<CL.size();i1++)
// // 		{
// // 			cout<<i1<<" "<<CL[i1].hi.size()<<endl;
// // 		}
// 		
// 		for(int i1=0;i1<CL.size();i1++)
// 		{
// 			if(i1==indCLHP) continue;
// 			for(int i2=0;i2<CL[i1].hi.size();i2++)
// 			{
// 				x1=fitParams[0]+fitParams[1]*(hd.ColIndT->at(CL[i1].hi[i2])[2]-fitParams[4]);
// 				y1=fitParams[2]+fitParams[3]*(hd.ColIndT->at(CL[i1].hi[i2])[2]-fitParams[4]);
// 				if(sqrt(pow(x1-hd.ColIndT->at(CL[i1].hi[i2])[0],2.)+pow(y1-hd.ColIndT->at(CL[i1].hi[i2])[1],2.))<10.)
// 				{
// 					CL[indCLHP].hi.push_back(CL[i1].hi[i2]);
// 					
// 					tg2d->SetPoint(tg2d->GetN(),hd.ColIndT->at(CL[i1].hi[i2])[0],hd.ColIndT->at(CL[i1].hi[i2])[1],hd.ColIndT->at(CL[i1].hi[i2])[2]);
// 					if(hd.ColIndT->at(CL[i1].hi[i2])[2]<tmin){tmin=hd.ColIndT->at(CL[i1].hi[i2])[2];indoftmin=CL[indCLHP].hi.size()-1;}
// 					if(hd.ColIndT->at(CL[i1].hi[i2])[2]>tmax){tmax=hd.ColIndT->at(CL[i1].hi[i2])[2];indoftmax=CL[indCLHP].hi.size()-1;}
// 					TrackXY->Fill(hd.ColIndT->at(CL[i1].hi[i2])[0],hd.ColIndT->at(CL[i1].hi[i2])[1]);
// 					QColtot+=hd.QColTot;
// 					
// 					CL[i1].hi[i2]=CL[i1].hi[CL[i1].hi.size()-1];
// 					CL[i1].hi.pop_back();
// 					i2--;
// 				}
// 			}
// 		}
// // 		cout<<"after"<<endl;
// // 		for(int i1=0;i1<CL.size();i1++)
// // 		{
// // 			cout<<i1<<" "<<CL[i1].hi.size()<<endl;
// // 		}
// 		for(int i1=0;i1<CL.size();i1++)
// 		{
// 			if(CL[i1].hi.size()==0)
// 			{
// 				CL[i1].hi=CL[CL.size()-1].hi;
// 				CL.pop_back();
// 				i1--;
// 			}
// 		}
// 		indCLHP=-1;maxCLsize=0;
// 		for(int i1=0;i1<CL.size();i1++)
// 		{
// 			ClSize->Fill(CL[i1].hi.size());
// 			if(CL[i1].hi.size()>maxCLsize){maxCLsize=CL[i1].hi.size();indCLHP=i1;}
// 			HitsinCLs2+=CL[i1].hi.size();
// 		}
// // 		cout<<I<<" "<<hd.ColIndT->size()<<" "<<HitsinCLs1<<" "<<HitsinCLs2<<endl;
// 		
// // 		cout<<"after 2"<<endl;
// // 		for(int i1=0;i1<CL.size();i1++)
// // 		{
// // 			cout<<i1<<" "<<CL[i1].hi.size()<<endl;
// // 		}
// // 		cout<<endl;
// 		
// 		maxCLsize=CL[indCLHP].hi.size();
// 		MaxClSize->Fill(maxCLsize);
// 		NClperEvent->Fill(CL.size());
// 		
// 		pStart[0]=hd.ColIndT->at(CL[indCLHP].hi[indoftmin])[0];
// 		pStart[1]=((float)(hd.ColIndT->at(CL[indCLHP].hi[indoftmax])[0]-hd.ColIndT->at(CL[indCLHP].hi[indoftmin])[0]))/((float)(hd.ColIndT->at(CL[indCLHP].hi[indoftmax])[2]-hd.ColIndT->at(CL[indCLHP].hi[indoftmin])[2]));
// 		pStart[2]=hd.ColIndT->at(CL[indCLHP].hi[indoftmin])[1];
// 		pStart[3]=((float)(hd.ColIndT->at(CL[indCLHP].hi[indoftmax])[1]-hd.ColIndT->at(CL[indCLHP].hi[indoftmin])[1]))/((float)(hd.ColIndT->at(CL[indCLHP].hi[indoftmax])[2]-hd.ColIndT->at(CL[indCLHP].hi[indoftmin])[2]));
// 		pStart[4]=1500;
// 		
// 		fitnormchi2=line3Dfit(tg2d);
// 		
// 		TrackDeltaT->Fill(tmax-tmin);
// 		FitNormChi2->Fill(fitnormchi2);
// 		
// // 		td.E=I;
// 		td.E=hd.E;
// 		td.StartEndColIndT->clear();
// 		td.StartEndColIndT->push_back(hd.ColIndT->at(CL[indCLHP].hi[indoftmin])[0]);
// 		td.StartEndColIndT->push_back(hd.ColIndT->at(CL[indCLHP].hi[indoftmin])[1]);
// 		td.StartEndColIndT->push_back(hd.ColIndT->at(CL[indCLHP].hi[indoftmin])[2]);
// 		td.StartEndColIndT->push_back(hd.ColIndT->at(CL[indCLHP].hi[indoftmax])[0]);
// 		td.StartEndColIndT->push_back(hd.ColIndT->at(CL[indCLHP].hi[indoftmax])[1]);
// 		td.StartEndColIndT->push_back(hd.ColIndT->at(CL[indCLHP].hi[indoftmax])[2]);
// 		td.FitParams->clear();
// 		for(int is2=0;is2<5;is2++)
// 		{
// 			td.FitParams->push_back(fitParams[is2]);
// 		}
// 		td.FitNormChi2=fitnormchi2;
// 		td.QColTot=QColtot;
// 		td.NHits=CL[indCLHP].hi.size();
// 		td.Nexcl=hd.ColIndT->size()-CL[indCLHP].hi.size();
// 		td.PMTIntegral->clear();
// 		for(int is2=0;is2<3;is2++)
// 		{
// 			td.PMTIntegral->push_back(hd.PMTIntegral->at(is2));
// 		}
// 		
// 		T1->Fill();
// 		
// 		if(fitnormchi2<5.)
// 		{
// // // 			cc->cd(); 
// // // 			frame3d->Draw();
// // // 			
// // // 			pm3d1 = new TPolyMarker3D();
// // // 			pm3d2 = new TPolyMarker3D();
// // // 			
// // // // 			pm3d1 = new TPolyMarker3D(CL[indCLHP].hi.size());
// // // // // 			pm3d2 = new TPolyMarker3D(H.size()-CL[indCLHP].hi.size());
// // // // 			pm3d2 = new TPolyMarker3D(hd.ColIndT->size()-CL[indCLHP].hi.size());
// // // 			
// // // 			for(int i1=0;i1<CL[indCLHP].hi.size();i1++)
// // // 			{
// // // // 				pm3d1->SetPoint(i1,H[CL[indCLHP].hi[i1]].col,H[CL[indCLHP].hi[i1]].ind,H[CL[indCLHP].hi[i1]].t);
// // // 				pm3d1->SetPoint(i1,hd.ColIndT->at(CL[indCLHP].hi[i1])[0],hd.ColIndT->at(CL[indCLHP].hi[i1])[1],hd.ColIndT->at(CL[indCLHP].hi[i1])[2]);
// // // 			}
// // // 			
// // // 			int na=0;
// // // 			for(int i1=0;i1<CL.size();i1++)
// // // 			{
// // // 				if(i1==indCLHP) continue;
// // // 				for(int i2=0;i2<CL[i1].hi.size();i2++)
// // // 				{
// // // // 					pm3d2->SetPoint(na,H[CL[i1].hi[i2]].col,H[CL[i1].hi[i2]].ind,H[CL[i1].hi[i2]].t);
// // // 					pm3d2->SetPoint(na,hd.ColIndT->at(CL[i1].hi[i2])[0],hd.ColIndT->at(CL[i1].hi[i2])[1],hd.ColIndT->at(CL[i1].hi[i2])[2]);
// // // 					na++;
// // // 				}
// // // 			}
// // // // 					pm3d1->SetMarkerSize(2);
// // // 			pm3d1->SetMarkerColor(kRed);
// // // 			pm3d1->SetMarkerStyle(24);   
// // // 			pm3d1->Draw();
// // // // 					pm3d2->SetMarkerSize(2);
// // // 			pm3d2->SetMarkerColor(kBlue);
// // // 			pm3d2->SetMarkerStyle(24);   
// // // 			pm3d2->Draw();
// // // 			
// // // 			l = new TPolyLine3D(2);
// 			
// 			x1=fitParams[0]+fitParams[1]*(tmin-fitParams[4]);
// 			y1=fitParams[2]+fitParams[3]*(tmin-fitParams[4]);
// 			z1=tmin;
// // // 			l->SetPoint(0,x1,y1,z1);
// 			
// 			x2=fitParams[0]+fitParams[1]*(tmax-fitParams[4]);
// 			y2=fitParams[2]+fitParams[3]*(tmax-fitParams[4]);
// 			z2=tmax;
// // // 			l->SetPoint(1,x2,y2,z2);
// // // 			l->SetLineColor(kBlack);
// // // 			l->SetLineWidth(2);
// // // 			l->Draw("same");
// 			
// 			DeltaTvsTrackAngle->Fill(atan(sqrt(pow(x2-x1,2)+pow(y2-y1,2))/(tmax-tmin)),(tmax-tmin));
// 			DeltaTvsTrackAnglePr->Fill(atan(sqrt(pow(x2-x1,2)+pow(y2-y1,2))/(tmax-tmin)),(tmax-tmin));
// 			Tfirst->Fill(tmin);
// 			Tlast->Fill(tmax);
// //					if(tmin<=1160) Tlast->Fill(tmax);
// 			Trackt0->Fill(fitParams[4]);
// 			TrackEntryXY->Fill(x1,y1);
// 			TrackExitXY->Fill(x2,y2);
// 			if(tmin<=1160 && tmax>=1920) TrackDeltaTSel->Fill(tmax-tmin);
// 			DeltaTvsTrackt0->Fill(fitParams[4],tmax-tmin);
// 			DeltaTLengthvsAngle->Fill(atan(sqrt(pow(x2-x1,2)+pow(y2-y1,2))/(tmax-tmin)),sqrt(pow(x2-x1,2)+pow(y2-y1,2)+pow(tmax-tmin,2)),tmax-tmin);
// 			DeltaTLengthvsAngle_norm->Fill(atan(sqrt(pow(x2-x1,2)+pow(y2-y1,2))/(tmax-tmin)),sqrt(pow(x2-x1,2)+pow(y2-y1,2)+pow(tmax-tmin,2)));
// 			
// //					cout<<Ev<<" "<<tmin<<" "<<tmax<<" "<<(tmax-tmin)<<endl;
// 			
// // 			sprintf(hname,"Ev_%d_%5.3f",td.E,fitnormchi2);
// // 			cc->SetName(hname);cc->SetTitle(hname);
// // 			outroot->cd();
// // 			cc->Write();
// // 			tg2dA->Set(0);
// // 			delete l;
// 			
// 			FitClSize->Fill(CL[indCLHP].hi.size());
// 			NExclHits->Fill(hd.ColIndT->size()-CL[indCLHP].hi.size());
// // 			if(CL[indCLHP].hi.size()>15 && (H.size()-CL[indCLHP].hi.size())<10)
// // 			{
// // 				Nvalid++;
// // 				outfile<<Ev<<" "<<fitParams[0]<<" "<<fitParams[1]<<" "<<fitParams[2]<<" "<<fitParams[3]<<" "<<fitParams[4]<<" "<<fitnormchi2<<" "<<CL[indCLHP].hi.size()<<endl;
// // 			}
// // 			if(td.NHits>15 && td.Nexcl<10)
// 			if(td.NHits>10 && td.Nexcl<10)
// 			{
// 				outfile<<td.E<<" "<<hd.ColIndT->at(CL[indCLHP].hi[indoftmin])[0]<<" "<<hd.ColIndT->at(CL[indCLHP].hi[indoftmin])[1]<<" "<<hd.ColIndT->at(CL[indCLHP].hi[indoftmin])[2]<<" "<<hd.ColIndT->at(CL[indCLHP].hi[indoftmax])[0]<<" "<<hd.ColIndT->at(CL[indCLHP].hi[indoftmax])[1]<<" "<<hd.ColIndT->at(CL[indCLHP].hi[indoftmax])[2]<<endl;
// 			}
// 		}
// 		tg2d->Set(0);
// 		inCL.clear();
// 		cl.hi.clear();
// 		CL.clear();
// // 		if(I==10) break;
// 		if(I%100==0) cout<<I<<" / "<<T->GetEntries()<<endl;
// 	}
// 	cout<<Nvalid<<" / "<<Ev<<" "<<(((float)Nvalid)/((float)Ev))<<endl;
	DeltaTLengthvsAngle->Divide(DeltaTLengthvsAngle_norm);
	outroot->Write();
	outroot->Close();
	outfile.close();
	inroot->Close();
	
// 	sprintf(hname,"cp TrackParameters_%d.txt %s/Files/TrackParameters_%d.txt;wait;",RunNo,AnalysisFilePath,RunNo);
// 	system(hname);
	sprintf(hname,"cp Tracks_%d.root %s/Histos/Tracks_%d.root",RunNo,AnalysisFilePath,RunNo);system(hname);
	sprintf(hname,"cp ValidTrackParameters_%d.txt %s/Files/ValidTrackParameters_%d.txt;wait;",RunNo,AnalysisFilePath,RunNo);system(hname);
}