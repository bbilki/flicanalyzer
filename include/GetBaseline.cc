void GetBaseline()
{
	sprintf(hname,"%s/Run_%d.root",NTupleFilePath,RunNo);
	TFile* inroot=new TFile(hname);
	TTree* T =  (TTree*) inroot->Get("T");
	T->SetBranchAddress("TPCWF",&ed.TPCWF);
	T->SetBranchAddress("PMTWF",&ed.PMTWF);
	
	sprintf(hname,"Baselines_%d.root",RunNo);
	TFile* outroot=new TFile(hname,"recreate");
	
	TH1F* AllNormChi2=new TH1F("AllNormChi2","AllNormChi2",400,0.,2.);
	TH1D* AllAmp[3];
	AllAmp[0]=new TH1D("AllCollAmp","AllCollAmp",400,-100,100);
	AllAmp[1]=new TH1D("AllIndAmp","AllIndAmp",400,-100,100);
	AllAmp[2]=new TH1D("AllAmp","AllAmp",400,-100,100);
	
	TH1D* AllPMTAmp[3];
	for(int i1=0;i1<3;i1++)
	{
		sprintf(hname,"AllPMTAmp_%d",i1);
		AllPMTAmp[i1]=new TH1D(hname,hname,200,-100,100);
	}
	
	TGraphErrors* tg[2];
	for(int i1=0;i1<2;i1++)
	{
		tg[i1]=new TGraphErrors();
		sprintf(hname,"Baseline_%s",wt[i1].c_str());
		tg[i1]->SetName(hname);
		tg[i1]->SetTitle(hname);
		tg[i1]->SetMarkerStyle(24+i1);
		tg[i1]->SetMarkerColor(1+i1);
	}
	TGraphErrors* tgPMT;
	tgPMT=new TGraphErrors();
	sprintf(hname,"PMTBaseline");
	tgPMT->SetName(hname);
	tgPMT->SetTitle(hname);
	tgPMT->SetMarkerStyle(20);
	tgPMT->SetMarkerColor(1);
	
	TH1F* HB[2][128];
	for(int i1=0;i1<2;i1++)
	{
		for(int i2=0;i2<128;i2++)
		{
			sprintf(hname,"Amp_%s_%d",wt[i1].c_str(),i2);
			HB[i1][i2]=new TH1F(hname,hname,1000,-0.5,999.5);
		}
	}
	TH1F* PMTHB[3];
	for(int i1=0;i1<3;i1++)
	{
		sprintf(hname,"PMT_Amp_%d",i1);
		PMTHB[i1]=new TH1F(hname,hname,1500,-0.5,1499.5);
	}
	
	int ic=0;int gch=0;
	for(int I=0;I<T->GetEntries();I++)
	{
		T->GetEntry(I);
		for(int i1=0;i1<256;i1++)
		{
			ic=i1/128;gch=i1-((i1/128)*128);
			for(int i2=0;i2<4096;i2++)
			{
				HB[ic][gch]->Fill(ed.TPCWF->at(i1)[i2]);
			}
		}
		
		for(int i1=0;i1<3;i1++)
		{
			for(int i2=0;i2<ed.PMTWF->at(i1).size();i2++)
			{
				PMTHB[i1]->Fill(ed.PMTWF->at(i1)[i2]);
			}
		}
// 		if(I%10==0) cout<<"Event: "<<I<<endl;
	}
	
	float res[2][128][2]={{{0.}}};//mean sigma
	float resPMT[3][2]={{0.}};//mean sigma
	TF1* g=new TF1("gaus","gaus",-100.,3000.);
	for(int i1=0;i1<2;i1++)
	{
		for(int i2=0;i2<128;i2++)
		{
			g->SetParameter(0,HB[i1][i2]->GetMaximum());
			g->SetParameter(1,HB[i1][i2]->GetMean());
			g->SetParameter(2,HB[i1][i2]->GetRMS());
			HB[i1][i2]->Fit(g,"q","q",10.,1000.);
			res[i1][i2][0]=g->GetParameter(1);
			res[i1][i2][1]=g->GetParameter(2);
		}
	}
	for(int i1=0;i1<3;i1++)
	{
		g->SetParameter(0,PMTHB[i1]->GetMaximum());
		g->SetParameter(1,PMTHB[i1]->GetMean());
		g->SetParameter(2,PMTHB[i1]->GetRMS());
		PMTHB[i1]->Fit(g,"q","q",10.,2000.);
		resPMT[i1][0]=g->GetParameter(1);
		resPMT[i1][1]=g->GetParameter(2);
	}
	
	for(int I=0;I<T->GetEntries();I++)
	{
		T->GetEntry(I);
		for(int i1=0;i1<256;i1++)
		{
			ic=i1/128;gch=i1-((i1/128)*128);
			for(int i2=0;i2<4096;i2++)
			{
				AllAmp[ic]->Fill(ed.TPCWF->at(i1)[i2]-res[ic][gch][0]);
				AllAmp[2]->Fill(ed.TPCWF->at(i1)[i2]-res[ic][gch][0]);
			}
		}
		for(int i1=0;i1<3;i1++)
		{
			for(int i2=0;i2<ed.PMTWF->at(i1).size();i2++)
			{
				AllPMTAmp[i1]->Fill(ed.PMTWF->at(i1)[i2]-resPMT[i1][0]);
			}
		}
	}
	
	sprintf(hname,"TPCBaselines_%d.txt",RunNo);
	ofstream outfile(hname);
	
	for(int i1=0;i1<2;i1++)
	{
		for(int i2=0;i2<128;i2++)
		{
			outfile<<i1<<" "<<i2<<" "<<res[i1][i2][0]<<" "<<res[i1][i2][1]<<endl;//mean sigma
			tg[i1]->SetPoint(i2,i2,res[i1][i2][0]);
			tg[i1]->SetPointError(i2,0,res[i1][i2][1]);
		}
	}
	outfile.close();
	
	sprintf(hname,"PMTBaselines_%d.txt",RunNo);
	ofstream PMToutfile(hname);
	
	for(int i1=0;i1<3;i1++)
	{
		PMToutfile<<i1<<" "<<resPMT[i1][0]<<" "<<resPMT[i1][1]<<endl;
		tgPMT->SetPoint(i1,i1,resPMT[i1][0]);
		tgPMT->SetPointError(i1,0,resPMT[i1][1]);
	}
	
	outroot->cd();
	tgPMT->Write();
	tg[0]->Write();
	tg[1]->Write();
	for(int i1=0;i1<3;i1++)
	{
		g->SetParameter(0,AllAmp[i1]->GetMaximum());
		g->SetParameter(1,AllAmp[i1]->GetMean());
		g->SetParameter(2,AllAmp[i1]->GetRMS());
		AllAmp[i1]->Fit(g,"q","q",-50.,50.);
		AllAmp[i1]->Write();
		
		g->SetParameter(0,AllPMTAmp[i1]->GetMaximum());
		g->SetParameter(1,AllPMTAmp[i1]->GetMean());
		g->SetParameter(2,AllPMTAmp[i1]->GetRMS());
		AllPMTAmp[i1]->Fit(g,"q","q",-50.,50.);
		AllPMTAmp[i1]->Write();
	}
	for(int i1=0;i1<3;i1++)
	{
		PMTHB[i1]->Write();
	}
	
	for(int i1=0;i1<2;i1++)
	{
		for(int i2=0;i2<128;i2++)
		{
			HB[i1][i2]->Write();
		}
	}
	
	outroot->Close();
	inroot->Close();
	outfile.close();
	PMToutfile.close();
	
	sprintf(hname,"cp Baselines_%d.root %s/Histos/Baselines_%d.root;wait;",RunNo,AnalysisFilePath,RunNo);system(hname);
	sprintf(hname,"cp TPCBaselines_%d.txt %s/Files/TPCBaselines_%d.txt;wait;",RunNo,AnalysisFilePath,RunNo);system(hname);
	sprintf(hname,"cp PMTBaselines_%d.txt %s/Files/PMTBaselines_%d.txt",RunNo,AnalysisFilePath,RunNo);system(hname);
}