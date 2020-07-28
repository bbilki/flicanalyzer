void HitAnalysis3()
{
	sprintf(hname,"HitAnalysis3_%d.root",RunNo);
	TFile* outroot=new TFile(hname,"recreate");
	
	sprintf(hname,"%s/Run_%d.root",NTupleFilePath,RunNo);
	TFile* inroot=new TFile(hname);
	TTree* T =  (TTree*) inroot->Get("T");
	T->SetBranchAddress("TPCWF",&ed.TPCWF);
	T->SetBranchAddress("PMTWF",&ed.PMTWF);
	
	ReadTPCBaselines();
	ReadPMTBaselines();
	
	TH1F* HitIntegral=new TH1F("HitIntegral","HitIntegral",500,0,3000);
	TH1F* HitAmplitude=new TH1F("HitAmplitude","HitAmplitude",300,0,300);
	TH1F* HitWidth=new TH1F("HitWidth","HitWidth",300,-0.5,299.5);
	TH1F* HitWidth_10percent=new TH1F("HitWidth_10percent","HitWidth_10percent",300,-0.5,299.5);
	
	TH2F* Amp_vs_Int_2D=new TH2F("Amp_vs_Int_2D","Amp_vs_Int_2D",500,0,3000,200,0,200);
	TH2F* Int_vs_Width_2D=new TH2F("Int_vs_Width_2D","Int_vs_Width_2D",300,-0.5,299.5,500,0,3000);
	TH2F* Int_vs_Width_10percent_2D=new TH2F("Int_vs_Width_10percent_2D","Int_vs_Width_10percent_2D",300,-0.5,299.5,500,0,3000);
	TH2F* Int_vs_undershoot_2D=new TH2F("Int_vs_undershoot_2D","Int_vs_undershoot_2D",500,-1000,1000,500,0,3000);
	
	TH2F* Amp_vs_Int_2D_Ind[128];
	for(int i1=0;i1<128;i1++)
	{
		sprintf(hname,"Amp_vs_Int_2D_%d",i1);
		Amp_vs_Int_2D_Ind[i1]=new TH2F(hname,hname,500,0,3000,200,0,200);
	}
	
	TProfile* Amp_vs_Int_Profile=new TProfile("Amp_vs_Int_Profile","Amp_vs_Int_Profile",300,0,3000,0,300);
	
	TSpectrum *s;
	Double_t *xpeaks;
// 	vector <int> xpeaks;
	TH1F* hh[2][128];
	vector <int> inCL;
	double x1(0),y1(0),z1(0),t1(0);
	double x2(0),y2(0),z2(0),t2(0);
	int nid=0;
	
	for(int i1=0;i1<2;i1++)
	{
		for(int i2=0;i2<128;i2++)
		{
			sprintf(hname,"WF_%s_%d",wt[i1].c_str(),i2);
			hh[i1][i2]=new TH1F(hname,hname,4096,-0.5,4095.5);
		}
	}
	
	TGraph* Amp_vs_Int;
	Amp_vs_Int=new TGraph();
	Amp_vs_Int->SetName("Amp_vs_Int");
	Amp_vs_Int->SetTitle("Amp_vs_Int");
	Amp_vs_Int->SetMarkerStyle(20);
	Amp_vs_Int->SetMarkerColor(1);
	
	TGraph* Int_vs_Width;
	Int_vs_Width=new TGraph();
	Int_vs_Width->SetName("Int_vs_Width");
	Int_vs_Width->SetTitle("Int_vs_Width");
	Int_vs_Width->SetMarkerStyle(20);
	Int_vs_Width->SetMarkerColor(2);
	
	TGraph* Int_vs_Width_10percent;
	Int_vs_Width_10percent=new TGraph();
	Int_vs_Width_10percent->SetName("Int_vs_Width_10percent");
	Int_vs_Width_10percent->SetTitle("Int_vs_Width_10percent");
	Int_vs_Width_10percent->SetMarkerStyle(20);
	Int_vs_Width_10percent->SetMarkerColor(4);
	
	int maxbin=0;float maxamp=0;
	int leftbin=0;int rightbin=0;
	int leftbin_10percent=0;int rightbin_10percent=0;
	float integral=0;
	int width=0;int width_10percent=0;
	float undershoot=0;
	
	int ic=0;int gch=0;
	
	cout<<T->GetEntries()<<" events"<<endl;
	for(int I=0;I<T->GetEntries();I++)
	{
		T->GetEntry(I);
		int npeaks = 200;
		int nfound=0;
		float ss=0;
		
		for(int i1=0;i1<128;i1++)
		{
			ic=i1/128;gch=i1-((i1/128)*128);
			for(int i2=TimeRange[0];i2<TimeRange[1];i2++)
			{
				ss=((float)ed.TPCWF->at(i1)[i2])-TPCBaselines[ic][gch][0];
				hh[ic][gch]->SetBinContent(hh[ic][gch]->FindBin(i2),ss);
			}
			s = new TSpectrum(2*npeaks);
			nfound = s->Search(hh[ic][gch],3.,"",0.1);
// 			xpeaks = s->GetPositionX();
			if(nfound==1)
			{
				maxbin=hh[ic][gch]->GetMaximumBin();
				maxamp=hh[ic][gch]->GetBinContent(maxbin);
				for(int ik1=maxbin;ik1>=1;ik1--)
				{
					if(hh[ic][gch]->GetBinContent(ik1)<0){leftbin=ik1+1;break;}
				}
				for(int ik1=maxbin;ik1<=hh[ic][gch]->GetNbinsX();ik1++)
				{
					if(hh[ic][gch]->GetBinContent(ik1)<0){rightbin=ik1-1;break;}
				}
				integral=0;
				for(int ik1=leftbin;ik1<=rightbin;ik1++)
				{
					integral+=hh[ic][gch]->GetBinContent(ik1);
				}
				undershoot=0;
				for(int ik1=rightbin;ik1<=(((rightbin+100)<=hh[ic][gch]->GetNbinsX())?(rightbin+100):hh[ic][gch]->GetNbinsX());ik1++)
				{
					undershoot+=hh[ic][gch]->GetBinContent(ik1);
				}
				for(int ik1=maxbin;ik1>=1;ik1--)
				{
					if((hh[ic][gch]->GetBinContent(ik1)/maxamp)<0.1){leftbin_10percent=ik1+1;break;}
				}
				for(int ik1=maxbin;ik1<=hh[ic][gch]->GetNbinsX();ik1++)
				{
					if((hh[ic][gch]->GetBinContent(ik1)/maxamp)<0.1){rightbin_10percent=ik1-1;break;}
				}
				width=rightbin-leftbin+1;
				width_10percent=rightbin_10percent-leftbin_10percent+1;
				HitIntegral->Fill(integral);
				HitAmplitude->Fill(maxamp);
				HitWidth->Fill(width);
				HitWidth_10percent->Fill(width_10percent);
				Amp_vs_Int->SetPoint(Amp_vs_Int->GetN(),integral,maxamp);
				Int_vs_Width->SetPoint(Int_vs_Width->GetN(),width,integral);
				Int_vs_Width_10percent->SetPoint(Int_vs_Width_10percent->GetN(),width_10percent,integral);
				Amp_vs_Int_2D->Fill(integral,maxamp);
				Int_vs_Width_2D->Fill(width,integral);
				Int_vs_Width_10percent_2D->Fill(width_10percent,integral);
				Amp_vs_Int_2D_Ind[i1]->Fill(integral,maxamp);
				Amp_vs_Int_Profile->Fill(integral,maxamp);
				Int_vs_undershoot_2D->Fill(undershoot,integral);
			}
		}
		if(I%100==0) cout<<"Event: "<<I<<endl;
// 		if(I==50) break;
	}
	
	outroot->cd();
	HitIntegral->Write();
	HitAmplitude->Write();
	HitWidth->Write();
	HitWidth_10percent->Write();
	Amp_vs_Int->Write();
	Int_vs_Width->Write();
	Int_vs_Width_10percent->Write();
	Amp_vs_Int_2D->Write();
	Int_vs_Width_2D->Write();
	Int_vs_Width_10percent_2D->Write();
	Int_vs_undershoot_2D->Write();
	Amp_vs_Int_Profile->Write();
	for(int i1=0;i1<128;i1++)
	{
		Amp_vs_Int_2D_Ind[i1]->Write();
	}
	
	inroot->Close();
	
	sprintf(hname,"mv HitAnalysis3_%d.root %s/Histos/;wait;",RunNo,AnalysisFilePath);system(hname);
}