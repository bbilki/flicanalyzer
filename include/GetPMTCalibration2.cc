void GetPMTCalibration2()
{
	sprintf(hname,"PMTCalibration_%d.root",RunNo);
	TFile* outroot=new TFile(hname,"recreate");
	
	sprintf(hname,"%s/Run_%d.root",NTupleFilePath,RunNo);
	TFile* inroot=new TFile(hname);
	TTree* T =  (TTree*) inroot->Get("T");
	T->SetBranchAddress("TPCWF",&ed.TPCWF);
	T->SetBranchAddress("PMTWF",&ed.PMTWF);
	
	TH1D* hh[11][3];TH1F* htr[3];TH1F* hs[3];TH1F* hs2[3];
	TH1F* hfitmean[3];TH1F* hfitrms[3];
	TH1D* Ampvst[3];
	TH1F* BLs[3];
	
	if(RunNo>7000)
	{
		ReadPMTBaselines();
	}
	
	for(int i1=0;i1<3;i1++)
	{
		for(int i2=0;i2<10;i2++)
		{
			sprintf(hname,"Int_%d_%d",(i2+1)*5,i1);
			hh[i2][i1]=new TH1D(hname,hname,3000,-200.,2800.);
// 			hh[i2][i1]=new TH1D(hname,hname,6000,-200.,2800.);
		}
		
		sprintf(hname,"Amp_vs_t_%d",i1);
		Ampvst[i1]=new TH1D(hname,hname,15000,-0.5,14999.5);
		sprintf(hname,"BLs_%d",i1);
		BLs[i1]=new TH1F(hname,hname,2000,0.,1000.);
		
		sprintf(hname,"TransientsWindow_%d",i1);
// 		htr[i1]=new TH1F(hname,hname,2000,-100,900);
		htr[i1]=new TH1F(hname,hname,3000,-200.,2800.);
		sprintf(hname,"SignalWindow_%d",i1);
// 		hs[i1]=new TH1F(hname,hname,2000,-100,900);
		hs[i1]=new TH1F(hname,hname,3000,-200.,2800.);
		sprintf(hname,"SignalWindow2_%d",i1);
		hs2[i1]=new TH1F(hname,hname,3000,-200.,2800.);
		sprintf(hname,"BLfitmean_%d",i1);
		hfitmean[i1]=new TH1F(hname,hname,2000,-100,900);
		sprintf(hname,"BLfitrms_%d",i1);
		hfitrms[i1]=new TH1F(hname,hname,2000,-100,900);
	}
// 	TH2F* Evvst[3];
// 	for(int i1=0;i1<3;i1++)
// 	{
// 		sprintf(hname,"Ev_vs_t_%d",i1);
// 		Evvst[i1]=new TH2F(hname,hname,15000,-0.5,14999.5,1000,-0.5,999.5);
// 	}
	
	TGraph* BLvsE[3];
	for(int i1=0;i1<3;i1++)
	{
		sprintf(hname,"BL_vs_E_%d",i1);
		BLvsE[i1]=new TGraph();
		BLvsE[i1]->SetName(hname);BLvsE[i1]->SetTitle(hname);
		BLvsE[i1]->SetMarkerStyle(24+i1);
		BLvsE[i1]->SetMarkerColor(1+i1);
		BLvsE[i1]->GetXaxis()->SetTitle("Event ID");BLvsE[i1]->GetXaxis()->CenterTitle();
		BLvsE[i1]->GetYaxis()->SetTitle("Mean Amplitude of First 500 Time Bins");BLvsE[i1]->GetYaxis()->CenterTitle();
	}
	
	TF1* g1=new TF1("g1","gaus",-1000.,2000.);
	
	TF1* lin=new TF1("lin","[0]",0.,4095.);
	TH1F* hb=new TH1F("hb","hb",15000,-0.5,14999.5);
// 	TF1* tf=new TF1("GL","gaus(0)+landau(3)",-100.,200.);
// 	TF1* tf=new TF1("GLG","gaus(0)+landau(3)+gaus(6)",-100.,200.);
	TF1* tf=new TF1("GGG","gaus(0)+gaus(3)+gaus(6)",-50.,100.);
	
// 	TF1* tf=new TF1("GG","gaus(0)+gaus(3)",-50.,100.);
// 	TF1* tf=new TF1("GL","gaus(0)+landau(3)",-50.,100.);
// 	TF1* tf=new TF1("GLG","gaus(0)+landau(3)+gaus(6)",-50.,100.);
	TF1* tfg=new TF1("G","gaus",-50.,100.);
	TF1* tfgg=new TF1("GG","gaus(0)+gaus(3)",-50.,100.);
	TF1* tfggg=new TF1("GGG","gaus(0)+gaus(3)+gaus(6)",-50.,100.);
	
	vector <float> Amp;
	float hsum[20]={0.};
	float hped=0;
	float hped2=0;float hsig2=0;
	float s=0;float str=0;float ss=0.;
	
	for(int I=0;I<T->GetEntries();I++)
	{
		T->GetEntry(I);
		for(int i1=0;i1<3;i1++)
		{
			hped=0;
			for(int i2=0;i2<500;i2++){hped+=ed.PMTWF->at(i1)[i2];}
			hped/=500.;
			BLvsE[i1]->SetPoint(BLvsE[i1]->GetN(),I,hped);
			if(I<200) continue;
			
			for(int is1=0;is1<20;is1++){hsum[is1]=0;}
			for(int i2=0;i2<ed.PMTWF->at(i1).size();i2++)
// 			for(int i2=2500;i2<10000;i2++)
			{
				Ampvst[i1]->Fill(i2,-1.*(ed.PMTWF->at(i1)[i2]-hped));
// 				if(I<1000) Evvst[i1]->Fill(i2,I,ed.PMTWF->at(i1)[i2]);
				
				for(int i3=0;i3<10;i3++)
				{
					if(i2>0 && i2%((i3+1)*5)==0){hh[i3][i1]->Fill(hsum[i3]);hsum[i3]=0;}
				}
				if(RunNo>7000)
				{
					for(int is1=0;is1<10;is1++){hsum[is1]+=(((-1.*(ed.PMTWF->at(i1)[i2]-PMTBaselines[i1][0]))>ThPMT[i1])?(-1.*(ed.PMTWF->at(i1)[i2]-PMTBaselines[i1][0])):0);}
				}
				else
				{
					for(int is1=0;is1<10;is1++){hsum[is1]+=(-1.*(ed.PMTWF->at(i1)[i2]-hped));}
				}
			}
			for(int i2=0;i2<ed.PMTWF->at(i1).size();i2+=60)
			{
				str=0;ss=0;
				for(int i3=i2;i3<(i2+15);i3++)
				{
					s=-1.*(ed.PMTWF->at(i1)[i3]-hped);
					str+=s;
				}
				for(int i3=i2+45;i3<i2+60;i3++)
				{
					s=-1.*(ed.PMTWF->at(i1)[i3]-hped);
					str+=s;
				}
				for(int i3=i2+15;i3<i2+45;i3++)
				{
					s=-1.*(ed.PMTWF->at(i1)[i3]-hped);
					ss+=s;
				}
				htr[i1]->Fill(str);
			}
		}
		if(I%1000==0) cout<<I<<" / "<<T->GetEntries()<<endl;
	}
// 	sprintf(hname,"Files/Calibrations_%d.txt",RunNo);
// 	ofstream outfile(hname);
	
	outroot->cd();
	
	float transientcuts[3]={0};
	for(int i1=0;i1<3;i1++)
	{
		htr[i1]->Fit(tfg,"q","q",-50,100);
		htr[i1]->Write();
		transientcuts[i1]=tfg->GetParameter(1)+3.5*tfg->GetParameter(2);
	}
	for(int I=0;I<T->GetEntries();I++)
	{
		T->GetEntry(I);
		for(int i1=0;i1<3;i1++)
		{
			hped=0;
			for(int i2=0;i2<500;i2++){hped+=ed.PMTWF->at(i1)[i2];}
			hped/=500.;
			if(I<200) continue;
			
			for(int i2=0;i2<ed.PMTWF->at(i1).size();i2+=60)
			{
				str=0;ss=0;
				for(int i3=i2;i3<(i2+15);i3++)
				{
					s=-1.*(ed.PMTWF->at(i1)[i3]-hped);
					str+=s;
				}
				for(int i3=i2+45;i3<i2+60;i3++)
				{
					s=-1.*(ed.PMTWF->at(i1)[i3]-hped);
					str+=s;
				}
				for(int i3=i2+15;i3<i2+45;i3++)
				{
					s=-1.*(ed.PMTWF->at(i1)[i3]-hped);
					ss+=s;
				}
				if(str<transientcuts[i1])
				{
					hs[i1]->Fill(ss);
				}
			}
		}
		if(I%1000==0) cout<<I<<" / "<<T->GetEntries()<<endl;
	}
	for(int i1=0;i1<3;i1++)
	{
		hs[i1]->Fit(tfg,"q","q",-50.,100.);
		tfgg->SetParameter(0,tfg->GetParameter(0));
		tfgg->SetParameter(1,tfg->GetParameter(1));
		tfgg->SetParameter(2,tfg->GetParameter(2));
		tfgg->SetParameter(3,tfg->GetParameter(0)/100.);
		tfgg->SetParameter(4,tfg->GetParameter(1)+30.);
		tfgg->SetParameter(5,tfg->GetParameter(2)*2);
		hs[i1]->Fit(tfgg,"q","q",-50.,60.);
		float chi2b=1000;float xchi2b=0.;float xchi2bsel=0.;
		xchi2b=40.;
		while(xchi2b<60.)
		{
			hs[i1]->Fit(tfgg,"q","q",-50.,xchi2b);
			if((tfgg->GetChisquare()/tfgg->GetNDF())<chi2b)
			{
				chi2b=(tfgg->GetChisquare()/tfgg->GetNDF());
				xchi2bsel=xchi2b;
			}
			xchi2b+=0.1;
		}
		hs[i1]->Fit(tfgg,"q","q",-50.,xchi2bsel);
		hs[i1]->Write();
		
		hs2[i1]=(TH1F*) hs[i1]->Clone();
		sprintf(hname,"SignalWindow2_%d",i1);
		hs2[i1]->SetName(hname);hs2[i1]->SetTitle(hname);
		hs2[i1]->Fit(tfgg,"q","q",-50.,xchi2bsel);
		tfggg->SetParameter(0,tfgg->GetParameter(0));
		tfggg->SetParameter(1,tfgg->GetParameter(1));
		tfggg->SetParameter(2,tfgg->GetParameter(2));
		tfggg->SetParameter(3,tfgg->GetParameter(3));
		tfggg->SetParameter(4,tfgg->GetParameter(4));
		tfggg->SetParameter(5,tfgg->GetParameter(5));
		tfggg->SetParameter(6,tfgg->GetParameter(3)/10);
		tfggg->SetParameter(7,tfgg->GetParameter(4)*2);
		tfggg->SetParameter(8,tfgg->GetParameter(5)*4);
		hs2[i1]->Fit(tfggg,"q","q",-50.,tfgg->GetParameter(4)+1.5*tfgg->GetParameter(5));
		hs2[i1]->Write();
	}
	
	for(int i1=0;i1<3;i1++)
	{
// 		htr[i1]->Fit(tfg,"q","q",-50,100);
// 		htr[i1]->Write();
// 		hs[i1]->Write();
		
		for(int i2=0;i2<10;i2++)
		{
			if(hh[i2][i1]->GetEntries()>0)
			{
				hh[i2][i1]->Fit(tfg,"q","q",-50.,100.);
				tfgg->SetParameter(0,tfg->GetParameter(0));
				tfgg->SetParameter(1,tfg->GetParameter(1));
				tfgg->SetParameter(2,tfg->GetParameter(2));
				tfgg->SetParameter(3,tfg->GetParameter(0)/100.);
				tfgg->SetParameter(4,tfg->GetParameter(1)+30.);
				tfgg->SetParameter(5,tfg->GetParameter(2)*2);
				
				tfgg->SetParLimits(3,0,1e6);
				tfgg->SetParLimits(4,20,60);
				tfgg->SetParLimits(5,0,100);
				
				hh[i2][i1]->Fit(tfgg,"q","q",-50.,60.);
				
				tfggg->SetParameter(0,tfgg->GetParameter(0));
				tfggg->SetParameter(1,tfgg->GetParameter(1));
				tfggg->SetParameter(2,tfgg->GetParameter(2));
				tfggg->SetParameter(3,tfgg->GetParameter(3));
				tfggg->SetParameter(4,tfgg->GetParameter(4));
				tfggg->SetParameter(5,tfgg->GetParameter(5));
				tfggg->SetParameter(6,tfgg->GetParameter(3)/10);
				tfggg->SetParameter(7,tfgg->GetParameter(4)*2);
				tfggg->SetParameter(8,tfgg->GetParameter(5)*4);
				
				tfggg->SetParLimits(6,0,1e6);
				tfggg->SetParLimits(7,30,100);
				tfggg->SetParLimits(8,0,200);
				
				hh[i2][i1]->Fit(tfggg,"q","q",-50.,100.);
			}
		}
	}
// 	
	for(int i1=0;i1<3;i1++)
	{
		for(int i2=0;i2<10;i2++)
		{
			if(hh[i2][i1]->GetEntries()>0) hh[i2][i1]->Write();
		}
		Ampvst[i1]->Write();
// 		hh_5[i1]->Write();
// 		hh_10[i1]->Write();
// 		hh_20[i1]->Write();
// 		Evvst[i1]->Write();
		BLvsE[i1]->Write();
		BLs[i1]->Write();
	}
	
// 	outfile.close();
	outroot->Close();
	
	sprintf(hname,"cp PMTCalibration_%d.root %s/Histos/PMTCalibration_%d.root",RunNo,AnalysisFilePath,RunNo);system(hname);
// 	for(int i1=0;i1<3;i1++)
// 	{
// 		sprintf(hname,"rm waveforms_ch%d_run%d.txt",i1,RunNo);system(hname);
// 	}
}