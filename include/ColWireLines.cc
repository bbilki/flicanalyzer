void ColWireLines()
{
	sprintf(hname,"ColWireLines_%d.root",RunNo);
	TFile* outroot=new TFile(hname,"recreate");
	outroot->mkdir("Trends");
	
	sprintf(hname,"%s/Run_%d.root",NTupleFilePath,RunNo);
	TFile* inroot=new TFile(hname);
	TTree* T =  (TTree*) inroot->Get("T");
	T->SetBranchAddress("TPCWF",&ed.TPCWF);
	T->SetBranchAddress("PMTWF",&ed.PMTWF);
	
	TH1F* HitCol=new TH1F("HitCol","HitCol",128,-0.5,127.5);
	TH1F* HitTime=new TH1F("HitTime","HitTime",4096,-0.5,4095.5);
	TH1F* HitIntegral1=new TH1F("HitIntegral1","HitIntegral1",1024,0,1023.5);
	TH1F* HitIntegral2=new TH1F("HitIntegral2","HitIntegral2",1024,0,1023.5);
	TH1F* HitIntegral3=new TH1F("HitIntegral3","HitIntegral3",500,0,3000);
	TH1F* NHits=new TH1F("NHits","NHits",2000,-0.5,1999.5);
	
	TH1F* StartTime=new TH1F("StartTime","StartTime",512,-0.5,4095.5);
	TH1F* EndTime=new TH1F("EndTime","EndTime",512,-0.5,4095.5);
	TH1F* StartCollection=new TH1F("StartCollection","StartCollection",128,-0.5,127.5);
	TH1F* EndCollection=new TH1F("EndCollection","EndCollection",128,-0.5,127.5);
	TH1F* DeltaT=new TH1F("DeltaT","DeltaT",1024,-0.5,4095.5);
	TH1F* DeltaCol=new TH1F("DeltaCol","DeltaCol",128,-0.5,127.5);
	TH1F* FitNormChi2=new TH1F("FitNormChi2","FitNormChi2",500,0.,50.);
	TH1F* LinePar0=new TH1F("LinePar0","LinePar0",800,-200.,200.);
	TH1F* LinePar1=new TH1F("LinePar1","LinePar1",1000,-1000.,1000.);
	
	TH2F* Col_vs_t[2];
	Col_vs_t[0]=new TH2F("Col_vs_t","Col_vs_t",4096,-0.5,4095.5,128,-0.5,127.5);
	Col_vs_t[1]=new TH2F("Col_vs_t_norm","Col_vs_t_norm",4096,-0.5,4095.5,128,-0.5,127.5);
	
	TProfile* Qvst=new TProfile("Qvst","Qvst",500,0,4000,0,2000);
	TGraph* tg1;
	
	TF1* lin1=new TF1("lin1","[0]+[1]*x",0.,200.);
	
	ReadTPCBaselines();
	
	TSpectrum *s;
	Double_t *xpeaks;
	TH1F* hh[2][128];TH1F* hh2[2][128];
	TH1F* ht[4096];TH1F* ht2[4096];
	
	for(int i1=0;i1<2;i1++)
	{
		for(int i2=0;i2<128;i2++)
		{
			sprintf(hname,"WF_%s_%d",wt[i1].c_str(),i2);
			hh[i1][i2]=new TH1F(hname,hname,4096,-0.5,4095.5);
			sprintf(hname,"WFZS_%s_%d",wt[i1].c_str(),i2);
			hh2[i1][i2]=new TH1F(hname,hname,4096,-0.5,4095.5);
		}
	}
	for(int i1=0;i1<4096;i1++)
	{	
		sprintf(hname,"WFt_%d",i1);
		ht[i1]=new TH1F(hname,hname,256,-0.5,255.5);
		sprintf(hname,"WFtZS_%d",i1);
		ht2[i1]=new TH1F(hname,hname,256,-0.5,255.5);
	}
	
	vector <hits> Hits;
	int nh=0;
	vector <hits> H;
	
	vector <int> bb;
	vector <float> bc;
	int Ev=0;
	int integral=0;int integral2=0;int integral3=0;
	int peakbin=0;
	int ic=0;int gch=0;
	float QColTot=0;
	float QColTotZS=0;
	float nchi2=0.;
	for(int I=0;I<T->GetEntries();I++)
	{
		T->GetEntry(I);
		int npeaks = 200;
		int nfound=0;
		float ss=0;
		bool found=true;
		bool HitOK=true;
		
		QColTot=0;QColTotZS=0;
		for(int i1=0;i1<128;i1++)
		{
			for(int i2=0;i2<4096;i2++)
			{
				ss=((float)ed.TPCWF->at(i1)[i2])-TPCBaselines[0][i1][0];
				ht[i2]->SetBinContent(ht[i2]->FindBin(i1),ss);
				if(ss>ThColInd[0])
				{
					ht2[i2]->SetBinContent(ht2[i2]->FindBin(i1),ss);
					hh2[0][i1]->SetBinContent(hh2[0][i1]->FindBin(i2),ss);
				}
				QColTot+=ss;
				if(ss>0) QColTotZS+=ss;
			}
		}
		int nh=0;
		for(int i1=0;i1<128;i1++)
		{
			s = new TSpectrum(2*npeaks);
			nfound = s->Search(hh2[0][i1],3.,"",0.1);
			xpeaks = s->GetPositionX();
			for(int is1=0;is1<nfound;is1++)
			{
				peakbin=hh2[0][i1]->FindBin(xpeaks[is1]);
				integral3=0;
				for(int ip1=peakbin;ip1<=hh2[0][i1]->GetNbinsX();ip1++)
				{
					if(hh2[0][i1]->GetBinContent(ip1)>ThColInd[0])
					{
						integral3+=hh2[0][i1]->GetBinContent(ip1);
					}
					else break;
				}
				for(int ip1=peakbin-1;ip1>=1;ip1--)
				{
					if(hh2[0][i1]->GetBinContent(ip1)>ThColInd[0])
					{
						integral3+=hh2[0][i1]->GetBinContent(ip1);
					}
					else break;
				}
				Col_vs_t[0]->Fill(xpeaks[is1],i1,integral3);
				Col_vs_t[1]->Fill(xpeaks[is1],i1);
				nh++;
			}
			delete s;
		}
		
// 		for(int i1=0;i1<4096;i1++)
// 		{
// 			s = new TSpectrum(2*npeaks);
// 			nfound = s->Search(ht2[i1],3.,"",0.1);
// 			xpeaks = s->GetPositionX();
// 			
// 			for(int is1=0;is1<nfound;is1++)
// 			{
// 				peakbin=ht[i1]->FindBin(xpeaks[is1]);
// 				integral=0;integral2=0;integral3=0;
// 				for(int ip1=peakbin;ip1<=ht[i1]->GetNbinsX();ip1++)
// 				{
// 					if(ht[i1]->GetBinContent(ip1)>ThColInd[0])
// 					{
// 						integral3+=ht[i1]->GetBinContent(ip1);
// 					}
// 					else break;
// 				}
// 				for(int ip1=peakbin-1;ip1>=1;ip1--)
// 				{
// 					if(ht[i1]->GetBinContent(ip1)>ThColInd[0])
// 					{
// 						integral3+=ht[i1]->GetBinContent(ip1);
// 					}
// 					else break;
// 				}
// 				
// 				hit.col=((int)xpeaks[is1]);
// 				hit.ind=0;
// 				hit.t=i1;
// 				hit.Int=0;//induction amplitude
// 				hit.Int2=ht2[i1]->GetBinContent(ht2[i1]->FindBin(xpeaks[is1]));//collection amplitude
// 				hit.Int3=integral3;
// 				
// 				Hits.push_back(hit);
// 				nh++;
// 			}
// 			delete s;
// 		}
// 		tg1=new TGraph();
// 		
// 		int mins[3]={5000,5000,5000};int maxs[3]={0,0,0};//Col, ind, t
// 		for(int i1=0;i1<Hits.size();i1++)
// 		{
// 			tg1->SetPoint(tg1->GetN(),Hits[i1].col,Hits[i1].t);
// 			if(Hits[i1].col<mins[0]) mins[0]=Hits[i1].col;if(Hits[i1].col>maxs[0]) maxs[0]=Hits[i1].col;
// 			if(Hits[i1].t<mins[2]) mins[2]=Hits[i1].t;if(Hits[i1].t>maxs[2]) maxs[2]=Hits[i1].t;
// 		}
// 		tg1->Fit(lin1,"q","q",0.,200.);
// 		nchi2=lin1->GetChisquare()/lin1->GetNDF();
// 		LinePar0->Fill(lin1->GetParameter(0));
// 		LinePar1->Fill(lin1->GetParameter(1));
// 		FitNormChi2->Fill(nchi2);
// 		outroot->cd("Trends");
// 		tg1->SetMarkerStyle(20);
// 		tg1->SetMarkerColor(1);
// 		sprintf(hname,"Ev_%d",I);
// 		tg1->SetName(hname);
// 		tg1->SetTitle(hname);
// 		tg1->Write();
// // 		if(nchi2<1.)
// 		{
// 			NHits->Fill(nh);
// 			StartTime->Fill(mins[2]);
// 			EndTime->Fill(maxs[2]);
// 			StartCollection->Fill(mins[0]);
// 			EndCollection->Fill(maxs[0]);
// 			DeltaT->Fill(maxs[2]-mins[2]);
// 			DeltaCol->Fill(maxs[0]-mins[0]);
// 			for(int i1=0;i1<Hits.size();i1++)
// 			{
// 				Qvst->Fill(Hits[i1].t,Hits[i1].Int3);
// 				HitCol->Fill(Hits[i1].col);
// 				HitTime->Fill(Hits[i1].t);
// 				HitIntegral3->Fill(Hits[i1].Int3);
// 			}
// 		}
		for(int i1=0;i1<4096;i1++)
		{
			ht[i1]->Reset();
			ht2[i1]->Reset();
		}
		for(int i1=0;i1<128;i1++)
		{
			hh2[0][i1]->Reset();
		}
		Hits.clear();
		if(I%10==0) cout<<"Event: "<<I<<endl;
// 		if(I==1000) break;
	}
	outroot->cd();
	HitCol->Write();
	HitTime->Write();
	HitIntegral1->Write();
	HitIntegral2->Write();
	HitIntegral3->Write();
	NHits->Write();
	StartTime->Write();
	EndTime->Write();
	StartCollection->Write();
	EndCollection->Write();
	DeltaT->Write();
	DeltaCol->Write();
	FitNormChi2->Write();
	LinePar0->Write();
	LinePar1->Write();
	Col_vs_t[0]->Divide(Col_vs_t[1]);
	Col_vs_t[0]->Write();
	Col_vs_t[1]->Write();
	
	outroot->Close();
	inroot->Close();
	
	sprintf(hname,"cp ColWireLines_%d.root %s/Histos/ColWireLines_%d.root;wait;",RunNo,AnalysisFilePath,RunNo);system(hname);
}
