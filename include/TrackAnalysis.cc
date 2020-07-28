void TrackAnalysis()
{
	sprintf(hname,"%s/Histos/Tracks_%d.root",AnalysisFilePath,RunNo);
	TFile* inroot1=new TFile(hname);
	TTree* TTrack =  (TTree*) inroot1->Get("T");
	TTrack->SetBranchAddress("E",&td.E);
	TTrack->SetBranchAddress("StartEndColIndT",&td.StartEndColIndT);
	TTrack->SetBranchAddress("FitParams",&td.FitParams);
	TTrack->SetBranchAddress("FitNormChi2",&td.FitNormChi2);
	TTrack->SetBranchAddress("QColTot",&td.QColTot);
	TTrack->SetBranchAddress("NHits",&td.NHits);
	TTrack->SetBranchAddress("Nexcl",&td.Nexcl);
	TTrack->SetBranchAddress("PMTIntegral",&td.PMTIntegral);
	
	sprintf(hname,"%s/Histos/Hits_%d.root",AnalysisFilePath,RunNo);
	TFile* inroot2=new TFile(hname);
	TTree* THit =  (TTree*) inroot2->Get("T");
	THit->SetBranchAddress("E",&hd.E);
	THit->SetBranchAddress("ColIndT",&hd.ColIndT);
	THit->SetBranchAddress("Int",&hd.Int);
	THit->SetBranchAddress("QColTot",&hd.QColTot);
	THit->SetBranchAddress("QColTotZS",&hd.QColTotZS);
	THit->SetBranchAddress("PMTIntegral",&hd.PMTIntegral);
	
	cout<<TTrack->GetEntries()<<" tracks "<<THit->GetEntries()<<" hits "<<endl;
	
	TH1F* hPMT[3];
// 	string PMTNames[3]={"LAr w/TPB","LAr no TPB","LXe"};
	int PMTlc[3]={1,2,4};
	
	sprintf(hname,"TrackAnalysis2_%d.root",RunNo);
	TFile* outroot=new TFile(hname,"recreate");
	
	TH2F* EntryXY=new TH2F("EntryXY","EntryXY",128,-0.5,127.5,128,-0.5,127.5);
	TH1F* EntryT=new TH1F("EntryT","EntryT",256,-0.5,4095.5);
	TH2F* ExitXY=new TH2F("ExitXY","ExitXY",128,-0.5,127.5,128,-0.5,127.5);
	TH1F* ExitT=new TH1F("ExitT","ExitT",256,-0.5,4095.5);
	TH1F* Trackt0=new TH1F("Trackt0","Trackt0",400,0,4000);
	TH1F* DeltaT=new TH1F("DeltaT","DeltaT",256,-0.5,4095.5);
	TH1F* TrackDeltaT=new TH1F("TrackDeltaT","TrackDeltaT",400,0,4000);
	TH1F* AllHitX=new TH1F("AllHitX","AllHitX",128,-0.5,127.5);
	TH1F* AllHitY=new TH1F("AllHitY","AllHitY",128,-0.5,127.5);
	TH1F* AllHitT=new TH1F("AllHitT","AllHitT",4096,-0.5,4095.5);
	TH1F* AllHitQ=new TH1F("AllHitQ","AllHitQ",500,0,1000);
	TH1F* TrackQ=new TH1F("TrackQ","TrackQ",100,0,100000);
	TH1F* TrackDeltaR=new TH1F("TrackDeltaR","TrackDeltaR",1000,0,2500);
	TH1F* TrackDeltaXY=new TH1F("TrackDeltaXY","TrackDeltaXY",200,0,200);
	TH2F* Qvst=new TH2F("Qvst","Qvst",1400,1000,2400,1000,0,1000);
	TH2F* Dt1vsDt2=new TH2F("Dt1vsDt2","Dt1vsDt2",500,-1000,1000,500,-1000,1000);
	TProfile* QvstPr=new TProfile("QvstPr","QvstPr",200,1000,2400,0,1000);
	TProfile* DeltaTvsDeltaR=new TProfile("DeltaTvsDeltaR","DeltaTvsDeltaR",200,0,180,0,1000);
	TProfile* DeltaTvsTrBin=new TProfile("DeltaTvsTrBin","DeltaTvsTrBin",200,0,1000,0,1000);
	TGraph* DeltaTvsTr=new TGraph();
	DeltaTvsTr->SetName("DeltaTvsTr");DeltaTvsTr->SetTitle("DeltaTvsTr");
	DeltaTvsTr->SetMarkerStyle(20);DeltaTvsTr->SetMarkerColor(1);
	TH1F* TrackHitCharge=new TH1F("TrackHitCharge","TrackHitCharge",1000,0,10000);
	TH1F* OutOfTrackHitCharge=new TH1F("OutOfTrackHitCharge","OutOfTrackHitCharge",1000,0,10000);
	TH2F* NHitsinvsoutT=new TH2F("NHitsinvsoutT","NHitsinvsoutT",500,-0.5,499.5,500,-0.5,499.5);
	
	TH2F* EntryXYPMT[3];TH2F* EntryXYPMTSat[3];TH2F* MeanPMTChargeEntryXY[3];
	TH1F* PMTInt[3];TH1F* PMTIntPE[3];TH1F* PMTIntAll[3];
	TProfile* PMTProf[3];TProfile* PMTProfPE[3];
	int PMTcols[3]={1,2,4};
	float PMTPEs[3]={34,1,23};
	for(int i1=0;i1<3;i1++)
	{
		sprintf(hname,"PMTIntAll_%d",i1);
		PMTIntAll[i1]=new TH1F(hname,hname,2000,-1000000,1000000);
		PMTIntAll[i1]->SetLineColor(PMTcols[i1]);
		sprintf(hname,"PMTInt_%d",i1);
		PMTInt[i1]=new TH1F(hname,hname,200,0,100000);
		PMTInt[i1]->SetLineColor(PMTcols[i1]);
		sprintf(hname,"PMTIntPE_%d",i1);
		PMTIntPE[i1]=new TH1F(hname,hname,300,0,3000);
		PMTIntPE[i1]->SetLineColor(PMTcols[i1]);
		sprintf(hname,"EntryXYPMT_%d",i1);
		EntryXYPMT[i1]=new TH2F(hname,hname,128,-0.5,127.5,128,-0.5,127.5);
		sprintf(hname,"EntryXYPMTSat_%d",i1);
		EntryXYPMTSat[i1]=new TH2F(hname,hname,128,-0.5,127.5,128,-0.5,127.5);
		sprintf(hname,"MeanPMTChargeEntryXY_%d",i1);
		MeanPMTChargeEntryXY[i1]=new TH2F(hname,hname,128,-0.5,127.5,128,-0.5,127.5);
		sprintf(hname,"PMTChageProf_%d",i1);
		PMTProf[i1]=new TProfile(hname,hname,50,0,150,0,100000);
		PMTProf[i1]->SetLineColor(PMTcols[i1]);
		sprintf(hname,"PMTPEProf_%d",i1);
		PMTProfPE[i1]=new TProfile(hname,hname,50,0,150,0,3000);
		PMTProfPE[i1]->SetLineColor(PMTcols[i1]);
	}
	
	TH2F* TrackHits[9][2];
	for(int i1=0;i1<9;i1++)
	{
		sprintf(hname,"MeanCharge_%d-%d",(i1*200)+800,((i1+1)*200)+800);
		TrackHits[i1][0]=new TH2F(hname,hname,128,-0.5,127.5,128,-0.5,127.5);
		sprintf(hname,"NHits_%d-%d",(i1*200)+800,((i1+1)*200)+800);
		TrackHits[i1][1]=new TH2F(hname,hname,128,-0.5,127.5,128,-0.5,127.5);
	}
	
	TH1F* QperHit[32];
	for(int i1=0;i1<32;i1++)
	{
		sprintf(hname,"QperHit_%d",i1);
		QperHit[i1]=new TH1F(hname,hname,500,0,1000);
	}
	
	TF1 *func = new TF1("fit",fitf2,0.,1000.,5);
	TF1 *exp1 = new TF1("exp1","[0]*exp(-x*[1])",0.,2400.);
	TF1 *land1 = new TF1("land1","landau",0.,100000.);
	TF1 *gplusl = new TF1("gplusl","gaus(0)+landau(3)",0.,1000.);
	TF1 *gaus1 = new TF1("gaus1","gaus",0.,1000.);
	
	bool foundfirst=true;bool foundlast=true;
	int tmin=5000;int indtmin=0;
	int tmax=0;int indtmax=0;
	double trackQ=0.;
	vector <float> tts;
	
	float x=0;float y=0;float ttmin=0;float ttmax=0;
	bool TrackOK=true;
	int trbin=0;
	float dr=0.;
	int NHitinTrack=0;int NHitoutsideTrack=0;
	vector <int> trhitinds;
	
	float PMTXY[3][2]={{96.5,63.5},{63.5,30.5},{63.5,96.5}};
	int I=0;int J=0;
	
	for(I=0;I<TTrack->GetEntries();I++)
	{
		TTrack->GetEntry(I);
// 		for(J=((I+5)<THit->GetEntries()?(I+5):THit->GetEntries());J>=0;J--)
		for(J=I;J<THit->GetEntries();J++)
		{
			THit->GetEntry(J);
			if(td.E==hd.E) break;
		}
// 		if(J==-1){cout<<I<<" / "<<TTrack->GetEntries()<<" : "<<I<<" "<<J<<endl;continue;}
// 		cout<<I<<" / "<<TTrack->GetEntries()<<" : "<<I<<" "<<J<<endl;
		
		tmin=5000;tmax=0;
		indtmin=0;indtmax=0;
		trackQ=0.;
		NHitinTrack=0;NHitoutsideTrack=0;
		trhitinds.clear();
		for(int i1=0;i1<hd.ColIndT->size();i1++)
		{
			AllHitQ->Fill(hd.Int->at(i1)[2]);
			AllHitX->Fill(hd.ColIndT->at(i1)[0]);
			AllHitY->Fill(hd.ColIndT->at(i1)[1]);
			AllHitT->Fill(hd.ColIndT->at(i1)[2]);
		
			x=td.FitParams->at(0)+td.FitParams->at(1)*(hd.ColIndT->at(i1)[2]-td.FitParams->at(4));
			y=td.FitParams->at(2)+td.FitParams->at(3)*(hd.ColIndT->at(i1)[2]-td.FitParams->at(4));
			if(sqrt(pow(hd.ColIndT->at(i1)[0]-x,2.)+pow(hd.ColIndT->at(i1)[1]-y,2.))<10)
			{
				if(hd.ColIndT->at(i1)[2]>tmax) {tmax=hd.ColIndT->at(i1)[2];indtmax=i1;}
				if(hd.ColIndT->at(i1)[2]<tmin) {tmin=hd.ColIndT->at(i1)[2];indtmin=i1;}
				TrackHitCharge->Fill(hd.Int->at(i1)[2]);
				NHitinTrack++;
				trhitinds.push_back(i1);
				trackQ+=hd.Int->at(i1)[2];
			}
			else
			{
				OutOfTrackHitCharge->Fill(hd.Int->at(i1)[2]);
				NHitoutsideTrack++;
			}
		}
		NHitsinvsoutT->Fill(NHitoutsideTrack,NHitinTrack);
		TrackOK=false;
		
		if(hd.ColIndT->at(indtmin)[0]>10 && hd.ColIndT->at(indtmin)[0]<117 && hd.ColIndT->at(indtmin)[1]>10 && hd.ColIndT->at(indtmin)[1]<117 && hd.ColIndT->at(indtmax)[0]>10 && hd.ColIndT->at(indtmax)[0]<117 && hd.ColIndT->at(indtmax)[1]>10 && hd.ColIndT->at(indtmax)[1]<117) TrackOK=true;
		if(NHitinTrack<50) TrackOK=false;
		
		dr=sqrt(pow(hd.ColIndT->at(indtmin)[0]-hd.ColIndT->at(indtmax)[0],2.)+pow(hd.ColIndT->at(indtmin)[1]-hd.ColIndT->at(indtmax)[1],2.)+pow(hd.ColIndT->at(indtmin)[2]-hd.ColIndT->at(indtmax)[2],2.));
// 		if(dr<700) TrackOK=false;
		
		EntryXY->Fill(hd.ColIndT->at(indtmin)[0],hd.ColIndT->at(indtmin)[1]);
		ExitXY->Fill(hd.ColIndT->at(indtmax)[0],hd.ColIndT->at(indtmax)[1]);
		
		if(TrackOK)
		{
			for(int i1=0;i1<trhitinds.size();i1++)
			{
				Qvst->Fill(hd.ColIndT->at(trhitinds[i1])[2],hd.Int->at(trhitinds[i1])[2]);
				QvstPr->Fill(hd.ColIndT->at(trhitinds[i1])[2],hd.Int->at(trhitinds[i1])[2]);
				if(hd.ColIndT->at(trhitinds[i1])[2]>=800 && hd.ColIndT->at(trhitinds[i1])[2]<2400)
				{
					QperHit[(hd.ColIndT->at(trhitinds[i1])[2]-800)/50]->Fill(hd.Int->at(trhitinds[i1])[2]);
				}
			}
			
			DeltaT->Fill(hd.ColIndT->at(indtmax)[2]-hd.ColIndT->at(indtmin)[2]);
// 			if((FE[i1].H[indtmax].t-FE[i1].H[indtmin].t)<500) cout<<FE[i1].E<<" "<<(FE[i1].H[indtmax].t-FE[i1].H[indtmin].t)<<" : "<<FE[i1].H[indtmin].t<<" "<<FE[i1].H[indtmax].t<<" "<<FE[i1].H.size()<<" "<<FE[i1].fitnormchi2<<endl;
			EntryT->Fill(hd.ColIndT->at(indtmin)[2]);
			ExitT->Fill(hd.ColIndT->at(indtmax)[2]);
			Trackt0->Fill(td.FitParams->at(4));
			TrackQ->Fill(trackQ);
			TrackDeltaR->Fill(dr);
			TrackDeltaXY->Fill(sqrt(pow(hd.ColIndT->at(indtmin)[0]-hd.ColIndT->at(indtmax)[0],2.)+pow(hd.ColIndT->at(indtmin)[1]-hd.ColIndT->at(indtmax)[1],2.)));
			Dt1vsDt2->Fill(td.FitParams->at(4)-hd.ColIndT->at(indtmin)[2],hd.ColIndT->at(indtmax)[2]-td.FitParams->at(4));
		
// 			tts.clear();
// 			tts.push_back(FE[i1].fitParams[4]-(FE[i1].fitParams[0]/FE[i1].fitParams[1]));
// 			tts.push_back(FE[i1].fitParams[4]-(FE[i1].fitParams[2]/FE[i1].fitParams[3]));
// 			tts.push_back(FE[i1].fitParams[4]+((127-FE[i1].fitParams[0])/FE[i1].fitParams[1]));
// 			tts.push_back(FE[i1].fitParams[4]+((127-FE[i1].fitParams[2])/FE[i1].fitParams[3]));
// 			sort(tts.begin(), tts.end());
// 			
// 			DeltaTvsTrBin->Fill(trbin/20,FE[i1].H[indtmax].t-FE[i1].H[indtmin].t);
// 			trbin++;
// 			DeltaTvsTr->SetPoint(DeltaTvsTr->GetN(),DeltaTvsTr->GetN(),FE[i1].H[indtmax].t-FE[i1].H[indtmin].t);
		
// 		ttmin=((FE[i1].fitParams[4]-(FE[i1].fitParams[0]/FE[i1].fitParams[1]))>(FE[i1].fitParams[4]-(FE[i1].fitParams[2]/FE[i1].fitParams[3]))?(FE[i1].fitParams[4]-(FE[i1].fitParams[0]/FE[i1].fitParams[1])):(FE[i1].fitParams[4]-(FE[i1].fitParams[2]/FE[i1].fitParams[3])));
// 		ttmax=((FE[i1].fitParams[4]+((127-FE[i1].fitParams[0])/FE[i1].fitParams[1]))>(FE[i1].fitParams[4]+((127-FE[i1].fitParams[2])/FE[i1].fitParams[3]))?(FE[i1].fitParams[4]+((127-FE[i1].fitParams[2])/FE[i1].fitParams[3])):(FE[i1].fitParams[4]+((127-FE[i1].fitParams[0])/FE[i1].fitParams[1])));
// 		TrackDeltaT->Fill(ttmax-ttmin);
// 			TrackDeltaT->Fill(tts[2]-tts[1]);
			
			DeltaTvsDeltaR->Fill(sqrt(pow(hd.ColIndT->at(indtmin)[0]-hd.ColIndT->at(indtmax)[0],2.)+pow(hd.ColIndT->at(indtmin)[1]-hd.ColIndT->at(indtmax)[1],2.)),hd.ColIndT->at(indtmax)[2]-hd.ColIndT->at(indtmin)[2]);
		
// 		cout<<FE[i1].E<<" "<<FE[i1].PMTint[0]<<" "<<FE[i1].PMTint[1]<<" "<<FE[i1].PMTint[2]<<endl;
			for(int i2=0;i2<3;i2++)
			{
				PMTIntAll[i2]->Fill(td.PMTIntegral->at(i2));
				if(td.PMTIntegral->at(i2)>=0)
				{
					PMTInt[i2]->Fill(td.PMTIntegral->at(i2));
					PMTIntPE[i2]->Fill(td.PMTIntegral->at(i2)/PMTPEs[i2]);
					EntryXYPMT[i2]->Fill(hd.ColIndT->at(indtmin)[0],hd.ColIndT->at(indtmin)[1]);
					MeanPMTChargeEntryXY[i2]->Fill(hd.ColIndT->at(indtmin)[0],hd.ColIndT->at(indtmin)[1],td.PMTIntegral->at(i2));
					PMTProf[i2]->Fill(sqrt(pow(hd.ColIndT->at(indtmin)[0]-PMTXY[i2][0],2.)+pow(hd.ColIndT->at(indtmin)[1]-PMTXY[i2][1],2.)),td.PMTIntegral->at(i2));
					PMTProfPE[i2]->Fill(sqrt(pow(hd.ColIndT->at(indtmin)[0]-PMTXY[i2][0],2.)+pow(hd.ColIndT->at(indtmin)[1]-PMTXY[i2][1],2.)),td.PMTIntegral->at(i2)/PMTPEs[i2]);
				}
				else
				{
					EntryXYPMTSat[i2]->Fill(hd.ColIndT->at(indtmin)[0],hd.ColIndT->at(indtmin)[1]);
				}
			}
		}
// 		for(int i2=0;i2<hd.ColIndT->size();i2++)
		for(int i2=0;i2<trhitinds.size();i2++)
		{
			
			if(hd.ColIndT->at(trhitinds[i2])[2]<800 || hd.ColIndT->at(trhitinds[i2])[2]>=2400) continue;
			TrackHits[(hd.ColIndT->at(trhitinds[i2])[2]-800)/200][0]->Fill(hd.ColIndT->at(trhitinds[i2])[0],hd.ColIndT->at(trhitinds[i2])[1],hd.Int->at(trhitinds[i2])[2]);
			TrackHits[(hd.ColIndT->at(trhitinds[i2])[2]-800)/200][1]->Fill(hd.ColIndT->at(trhitinds[i2])[0],hd.ColIndT->at(trhitinds[i2])[1]);
		}
	}
	outroot->cd();
	
	func->SetParLimits(0,0,1000.);
	func->SetParLimits(1,0,1000.);
	func->SetParLimits(2,0,100.);
	func->SetParLimits(3,-500,500);
	func->SetParLimits(4,-100,100);
	
	func->SetParameter(0,DeltaT->GetBinContent(DeltaT->GetMaximumBin()));
	func->SetParameter(1,DeltaT->GetBinCenter(DeltaT->GetMaximumBin()));
	func->SetParameter(2,10);
	func->SetParameter(3,0.1);
	func->SetParameter(4,0.1);
	DeltaT->Fit(func,"q","q",0.,1000.);
	
	QvstPr->Fit(exp1,"q","q",1000.,2400.);
	
// 	gplusl->SetParameter(0,AllHitQ->GetBinContent(AllHitQ->GetMaximumBin()));
// 	gplusl->SetParameter(1,AllHitQ->GetBinCenter(AllHitQ->GetMaximumBin()));
// 	gplusl->SetParameter(2,10);
// 	gplusl->SetParameter(3,AllHitQ->GetBinContent(AllHitQ->GetMaximumBin()));
// 	gplusl->SetParameter(4,AllHitQ->GetBinCenter(AllHitQ->GetMaximumBin()));
// 	gplusl->SetParameter(5,10);
// 	AllHitQ->Fit(gplusl,"q","q",0.,1000.);
	AllHitQ->Fit(land1,"q","q",0.,1000.);
	
	TrackQ->Fit(land1,"q","q",0.,100000.);
	
	EntryXY->Write();
	EntryT->Write();
	ExitXY->Write();
	ExitT->Write();
	DeltaT->Write();
	Trackt0->Write();
	TrackDeltaT->Write();
	AllHitX->Write();
	AllHitY->Write();
	AllHitT->Write();
	AllHitQ->Write();
	TrackQ->Write();
	TrackDeltaR->Write();
	TrackDeltaXY->Write();
	Dt1vsDt2->Write();
	Qvst->Write();
	QvstPr->Write();
	DeltaTvsDeltaR->Write();
	DeltaTvsTrBin->Write();
	DeltaTvsTr->Write();
	                                                                
	TrackHitCharge->Write();
	OutOfTrackHitCharge->Write();
	NHitsinvsoutT->Write();
	
	for(int i1=0;i1<3;i1++)
	{
		PMTInt[i1]->Scale(1./PMTInt[i1]->Integral());
		PMTInt[i1]->Write();
		PMTIntAll[i1]->Write();
		PMTIntPE[i1]->Write();
		EntryXYPMT[i1]->Write();
		EntryXYPMTSat[i1]->Write();
		MeanPMTChargeEntryXY[i1]->Divide(EntryXYPMT[i1]);
		MeanPMTChargeEntryXY[i1]->Write();
		PMTProf[i1]->Write();
		PMTProfPE[i1]->Write();
	}
	
	for(int i1=0;i1<9;i1++)
	{
		TrackHits[i1][0]->Divide(TrackHits[i1][1]);
		TrackHits[i1][0]->Write();
		TrackHits[i1][1]->Write();
	}
	
	TGraphErrors* tge=new TGraphErrors();
	tge->SetName("MeanHitQvsTime");tge->SetTitle("MeanHitQvsTime");
	tge->SetMarkerStyle(20);tge->SetMarkerColor(1);
	
	for(int i1=0;i1<32;i1++)
	{
// 		gplusl->SetParLimits(0,0,10000);
// 		gplusl->SetParLimits(1,0,100);
// 		gplusl->SetParLimits(2,0,100);
// 		gplusl->SetParLimits(3,0,1000);
// 		gplusl->SetParLimits(4,0,100);
// 		gplusl->SetParLimits(5,0,100);
// 		
// 		gplusl->SetParameter(0,QperHit[i1]->GetBinContent(QperHit[i1]->GetMaximumBin()));
// 		gplusl->SetParameter(1,QperHit[i1]->GetBinCenter(QperHit[i1]->GetMaximumBin()));
// 		gplusl->SetParameter(2,100);
// 		gplusl->SetParameter(3,QperHit[i1]->GetBinContent(QperHit[i1]->GetMaximumBin()));
// 		gplusl->SetParameter(4,QperHit[i1]->GetBinCenter(QperHit[i1]->GetMaximumBin()));
// 		gplusl->SetParameter(5,10);
		if(QperHit[i1]->GetEntries()>0)
		{
			if(QperHit[i1]->GetEntries()<2000) QperHit[i1]->Rebin(4);
			QperHit[i1]->Fit(gaus1,"q","q",QperHit[i1]->GetMean()-QperHit[i1]->GetRMS(),QperHit[i1]->GetMean()+QperHit[i1]->GetRMS());
			QperHit[i1]->Fit(gaus1,"q","q",gaus1->GetParameter(1)-gaus1->GetParameter(2),gaus1->GetParameter(1)+gaus1->GetParameter(2));
			tge->SetPoint(tge->GetN(),i1,gaus1->GetParameter(1));
			tge->SetPointError(tge->GetN()-1,0,gaus1->GetParError(1));
		}
		
// 		QperHit[i1]->Fit(gplusl,"q","q",0.,1000.);
// 		QperHit[i1]->Fit(land1,"q","q",0.,1000.);
		QperHit[i1]->Write();
	}
	
	tge->Fit(exp1,"q","q",1.,8.);
	tge->Write();
	
	outroot->Close();
// 	outfile.close();
	
	inroot1->Close();
	inroot2->Close();
	
// 	sprintf(hname,"rm TrackParameters_%d.txt",RunNo);system(hname);
// 	sprintf(hname,"rm PMTIntegrals_%d.txt",RunNo);system(hname);
// 	sprintf(hname,"rm Hits_%d.txt",RunNo);system(hname);
	sprintf(hname,"cp TrackAnalysis2_%d.root %s/Histos/TrackAnalysis2_%d.root;wait;",RunNo,AnalysisFilePath,RunNo);system(hname);
// 	sprintf(hname,"mv ValidTrackParameters_%d.txt %s/Files/ValidTrackParameters_%d.txt;wait;",RunNo,AnalysisFilePath,RunNo);system(hname);
}