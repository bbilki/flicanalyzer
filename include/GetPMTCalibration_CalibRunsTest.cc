void GetPMTCalibration_CalibRunsTest()
{
	sprintf(hname,"PMTCalibration_CalibRunTest_%d.root",RunNo);
	TFile* outroot=new TFile(hname,"recreate");
	
	sprintf(hname,"%s/Run_%d.root",NTupleFilePath,RunNo);
	TFile* inroot=new TFile(hname);
	TTree* T =  (TTree*) inroot->Get("T");
	T->SetBranchAddress("TPCWF",&ed.TPCWF);
	T->SetBranchAddress("PMTWF",&ed.PMTWF);
	
	TH1F* hs2[NPMTs];
	TH1F* MeanPedestalsAll[NPMTs];
	TH1F* MeanPedestals[NPMTs];TH1F* LinFitPedestals[NPMTs];TH1F* GausFitPedestals[NPMTs];TH1F* GausFitPedestalRMS[NPMTs];
	TH1F* BLFitOverMean[NPMTs];
	TH1D* Ampvst[NPMTs];
	TH1F* BLs[NPMTs];
	TH1F* MaxAmpoversigma[NPMTs];
	TH1F* MaxAmpover3sigma[NPMTs];
	TH2F* MaxAmpover3sigmavsWidth[NPMTs];
	TH1F* Integral[NPMTs];
	TH1F* IntegralAll[NPMTs];
	TH1F* Amplitudes[NPMTs];
	TH1F* AllAmplitudes[NPMTs];
	TH1F* AllAmplitudesPre[NPMTs];
	TH1F* MaxAmplitudes[NPMTs];
	TH1F* PulseWidth[NPMTs];
	TH1F* IntegralSel[NPMTs][6];
	TH1F* IntegralSel2[NPMTs];
	TH1F* IntegralSel3[NPMTs];
	TH1F* AmplitudeoverRMS[NPMTs];
	
	for(int i1=0;i1<NPMTs;i1++)
	{
		sprintf(hname,"Amp_vs_t_%d",i1);
		Ampvst[i1]=new TH1D(hname,hname,15000,-0.5,14999.5);
		sprintf(hname,"BLs_%d",i1);
		BLs[i1]=new TH1F(hname,hname,2000,0.,1000.);
		
		sprintf(hname,"MeanPedestalsAll_%d",i1);
		MeanPedestalsAll[i1]=new TH1F(hname,hname,4000,700.,1100.);
		sprintf(hname,"MeanPedestals_%d",i1);
		MeanPedestals[i1]=new TH1F(hname,hname,4000,700.,1100.);
		sprintf(hname,"LinFitPedestals_%d",i1);
		LinFitPedestals[i1]=new TH1F(hname,hname,4000,700.,1100.);
		sprintf(hname,"GausFitPedestals_%d",i1);
		GausFitPedestals[i1]=new TH1F(hname,hname,4000,700.,1100.);
		sprintf(hname,"GausFitPedestalRMS_%d",i1);
		GausFitPedestalRMS[i1]=new TH1F(hname,hname,400,0.,2.);
		sprintf(hname,"MaxAmpoversigma_%d",i1);
		MaxAmpoversigma[i1]=new TH1F(hname,hname,10000,0.,10.);
		sprintf(hname,"MaxAmpover3sigma_%d",i1);
		MaxAmpover3sigma[i1]=new TH1F(hname,hname,10000,0.,10.);
		sprintf(hname,"MaxAmpover3sigmavsWidth_%d",i1);
		MaxAmpover3sigmavsWidth[i1]=new TH2F(hname,hname,20,-0.5,19.5,10000,0.,10.);
		
		sprintf(hname,"BLFitOverMean_%d",i1);
		BLFitOverMean[i1]=new TH1F(hname,hname,5000,0.9995,1.0005);
		
		sprintf(hname,"Integral_%d",i1);
		Integral[i1]=new TH1F(hname,hname,1000,0,200);
		sprintf(hname,"IntegralAll_%d",i1);
		IntegralAll[i1]=new TH1F(hname,hname,1000,0,200);
		
		sprintf(hname,"PulseWidth_%d",i1);
		PulseWidth[i1]=new TH1F(hname,hname,30,-0.5,29.5);
		sprintf(hname,"Amplitudes_%d",i1);
		Amplitudes[i1]=new TH1F(hname,hname,200,0,20);
		sprintf(hname,"AllAmplitudes_%d",i1);
		AllAmplitudes[i1]=new TH1F(hname,hname,1000,-50,50);
		sprintf(hname,"AllAmplitudesPre_%d",i1);
		AllAmplitudesPre[i1]=new TH1F(hname,hname,125,-50,50);
		sprintf(hname,"MaxAmplitudes_%d",i1);
		MaxAmplitudes[i1]=new TH1F(hname,hname,200,0,20);
		
		for(int i2=0;i2<6;i2++)
		{
			sprintf(hname,"IntegralSel_%d_%d",i1,i2);
			IntegralSel[i1][i2]=new TH1F(hname,hname,1000,0,200);
		}
		
		sprintf(hname,"IntegralSel2_%d",i1);
		IntegralSel2[i1]=new TH1F(hname,hname,1000,0,200);
		sprintf(hname,"IntegralSel3_%d",i1);
		IntegralSel3[i1]=new TH1F(hname,hname,1000,0,200);
		
		sprintf(hname,"AmplitudeoverRMS_%d",i1);
// 		AmplitudeoverRMS[i1]=new TH1F(hname,hname,120,-10,10);
		AmplitudeoverRMS[i1]=new TH1F(hname,hname,200,-10,10);
	}
	TGraph* BLvsE[NPMTs];
	for(int i1=0;i1<NPMTs;i1++)
	{
		sprintf(hname,"BL_vs_E_%d",i1);
		BLvsE[i1]=new TGraph();
		BLvsE[i1]->SetName(hname);BLvsE[i1]->SetTitle(hname);
		BLvsE[i1]->SetMarkerStyle(24+i1);
		BLvsE[i1]->SetMarkerColor(1+i1);
		BLvsE[i1]->GetXaxis()->SetTitle("Event ID");BLvsE[i1]->GetXaxis()->CenterTitle();
		BLvsE[i1]->GetYaxis()->SetTitle("Mean Amplitude of First 1000 Time Bins");BLvsE[i1]->GetYaxis()->CenterTitle();
	}
	
	TF1* g1=new TF1("g1","gaus",-1000.,2000.);
	TF1* lin=new TF1("lin","[0]",0.,15000);
	TF1* linplussine=new TF1("linplussine","[0]+sin([1]*x+[2])",0.,15000);
	TF1* gint=new TF1("gint","expo(0)+gaus(2)",0.,200.);
	TF1* gexpo=new TF1("gexpo","expo",0.,200.);
	
	TH1F* hb=new TH1F("hb","hb",15000,-0.5,14999.5);
	TH1F* hp=new TH1F("hp","hp",2000,-0.5,1999.5);
	
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
	float hped2=0;
	float hped3=0;float hsig3=0;
	float hped4=0;float hped4sina=0;float hped4sinb=0;
	float s=0;float str=0;float ss=0.;
	int minbin=0;int maxbin=0;
	float maxamp=0;
	float maxampratio=0;
	int isel=0;
	
	vector <float> peds[NPMTs][7];//mean, lin fit, gaus fit mean, gaus fit sigma, lin+sin constant, lin+sin sin ax, lin+sin sin b
	float AmpoverRMS[NPMTs]={0};
	float AmpoverRMSPrePeak[NPMTs]={0};
	float PedRMSCut[NPMTs]={0};
// 	AmpoverRMS[0]=1.2;AmpoverRMS[1]=2;AmpoverRMS[2]=1.8;
// 	AmpoverRMS[0]=2.9;AmpoverRMS[1]=3.7;AmpoverRMS[2]=3.6;
	
// 	cout<<T->GetEntries()<<endl;s
	for(int I=0;I<T->GetEntries();I++)
	{
		T->GetEntry(I);
		for(int i1=0;i1<NPMTs;i1++)
		{
			hped=0;
			hb->Reset();hp->Reset();
			for(int i2=0;i2<1000;i2++)
			{
				hped+=ed.PMTWF->at(i1)[i2];
				hb->SetBinContent(i2+1,ed.PMTWF->at(i1)[i2]);
				hp->Fill(ed.PMTWF->at(i1)[i2]);
			}
			hped/=1000.;
			hb->Fit(lin,"q","q",0.,1000.);
			hped2=lin->GetParameter(0);
// 			hped2=round(lin->GetParameter(0));
// 			hb->Fit(linplussine,"q","q",0.,1000.);
// 			hped4=linplussine->GetParameter(0);
// 			hped4sina=linplussine->GetParameter(1);
// 			hped4sinb=linplussine->GetParameter(2);
			hp->Fit(g1,"q","q",0.,2000.);
			hped3=g1->GetParameter(1);
			hsig3=g1->GetParameter(2);
			
			MeanPedestalsAll[i1]->Fill(hped);
			BLvsE[i1]->SetPoint(BLvsE[i1]->GetN(),I,hped);
			
			peds[i1][0].push_back(hped);
			peds[i1][1].push_back(hped2);
			peds[i1][2].push_back(hped3);
			peds[i1][3].push_back(hsig3);
// 			peds[i1][4].push_back(hped4);
// 			peds[i1][5].push_back(hped4sina);
// 			peds[i1][6].push_back(hped4sinb);
			
			if(I<200) continue;
			if(hped>0) BLFitOverMean[i1]->Fill(hped2/hped);
			MeanPedestals[i1]->Fill(hped);
			LinFitPedestals[i1]->Fill(hped2);
			GausFitPedestals[i1]->Fill(hped3);
			GausFitPedestalRMS[i1]->Fill(hsig3);
			
			for(int i2=0;i2<1000;i2++)
			{
				s=1.*(ed.PMTWF->at(i1)[i2]-hped3);
				AllAmplitudesPre[i1]->Fill(s);
			}
		}
	}
	
	for(int i1=0;i1<NPMTs;i1++)
	{
		MeanPedestals[i1]->Fit(g1,"q","q",700.,1100.);
		LinFitPedestals[i1]->Fit(g1,"q","q",700.,1100.);
		GausFitPedestals[i1]->Fit(g1,"q","q",700.,1100.);
		GausFitPedestalRMS[i1]->Fit(g1,"q","q",0.5,1.5);
		PedRMSCut[i1]=g1->GetX(g1->GetParameter(0)/1e3,g1->GetParameter(1),2.);
	}
	
	for(int I=0;I<T->GetEntries();I++)
	{
// 		cout<<I<<endl;
		if(I<200) continue;
		T->GetEntry(I);
		for(int i1=0;i1<NPMTs;i1++)
		{
			hped3=peds[i1][2][I];
			hsig3=peds[i1][3][I];
			if(hsig3>PedRMSCut[i1]) continue;
			for(int i2=0;i2<1500;i2++)
			{
				s=-1*(ed.PMTWF->at(i1)[i2]-hped3);
				if(i2<2000) AmplitudeoverRMS[i1]->Fill(s/hsig3);
			}
		}
	}
	for(int i1=0;i1<NPMTs;i1++)
	{
		int a1binmax=AmplitudeoverRMS[i1]->GetMaximumBin();
		int a1binlast=0;
// 		int a2binmax=0;
// 		int a2binlast=0;
		AmplitudeoverRMS[i1]->Fit(g1,"q","q",AmplitudeoverRMS[i1]->GetBinCenter(a1binmax-4),AmplitudeoverRMS[i1]->GetBinCenter(a1binmax+4));
		float a1last=g1->GetX(g1->GetParameter(0)/1e3,g1->GetParameter(1),10.);
		AmplitudeoverRMS[i1]->GetXaxis()->SetRangeUser(a1last,10.);
		int a2binmax=AmplitudeoverRMS[i1]->GetMaximumBin();
		AmplitudeoverRMS[i1]->Fit(g1,"q+","q same",AmplitudeoverRMS[i1]->GetBinCenter(a2binmax-3),AmplitudeoverRMS[i1]->GetBinCenter(a2binmax+3));
		float a2last=g1->GetX(g1->GetParameter(0)/1e3,g1->GetParameter(1),10.);
		AmpoverRMS[i1]=a2last;
		AmpoverRMSPrePeak[i1]=a1last;
		cout<<"PMT "<<i1<<" amplitude/rms "<<AmpoverRMS[i1]<<" second peak start "<<AmpoverRMSPrePeak[i1]<<endl;
		AmplitudeoverRMS[i1]->GetXaxis()->SetRangeUser(-10.,10.);
	}
	
	for(int I=0;I<T->GetEntries();I++)
	{
// 		cout<<I<<endl;
		if(I<200) continue;
		T->GetEntry(I);
		for(int i1=0;i1<NPMTs;i1++)
		{
			hped3=peds[i1][2][I];
			hsig3=peds[i1][3][I];
			if(hsig3>PedRMSCut[i1]) continue;
			for(int i2=50;i2<ed.PMTWF->at(i1).size()-50;i2++)
			{
				s=-1*(ed.PMTWF->at(i1)[i2]-hped3);
				if(i2<1000) AllAmplitudes[i1]->Fill(s);
// 				if(i2<2000) AmplitudeoverRMS[i1]->Fill(s/hsig3);
				Ampvst[i1]->Fill(i2,s);
// 				if(s>3.*hsig3)
				if((s/hsig3)>AmpoverRMS[i1])
				{
					maxamp=0;
					maxbin=i2-1;
					for(int i3=i2;i3<ed.PMTWF->at(i1).size();i3++)
					{
						s=-1*(ed.PMTWF->at(i1)[i3]-hped3);
						if(s>maxamp) maxamp=s;
						if(s<=0)
						{
							maxbin=i3-1;
							break;
						}
					}
					minbin=i2+1;
					for(int i3=i2-1;i3>=1;i3--)
					{
						s=-1*(ed.PMTWF->at(i1)[i3]-hped3);
						if(s>maxamp) maxamp=s;
						if(s<=0)
						{
							minbin=i3+1;
							break;
						}
					}
					PulseWidth[i1]->Fill((maxbin-minbin+1));
// 					if(i1==0 && (maxbin-minbin+1)<1) cout<<I<<" "<<maxamp<<" "<<minbin<<" "<<maxbin<<endl; 
					MaxAmpoversigma[i1]->Fill(maxamp/hsig3);
// 					if((maxbin-minbin+1)>=8)
					if((maxbin-minbin+1)>2)
					{
						ss=0;maxamp=0;
						for(int i3=minbin;i3<=maxbin;i3++)
						{
							s=-1*(ed.PMTWF->at(i1)[i3]-hped3);
							ss+=s;
							if(s>maxamp) maxamp=s;
							Amplitudes[i1]->Fill(s);
						}
						MaxAmplitudes[i1]->Fill(maxamp);
// 						maxampratio=maxamp/(3.*hsig3);
						maxampratio=maxamp/(hsig3);
						
						if((maxbin-minbin+1)>3) IntegralSel[i1][0]->Fill(ss);
						if((maxbin-minbin+1)>4) IntegralSel[i1][1]->Fill(ss);
						if((maxbin-minbin+1)>5) IntegralSel[i1][2]->Fill(ss);
						if((maxbin-minbin+1)>6) IntegralSel[i1][3]->Fill(ss);
						if((maxbin-minbin+1)>7) IntegralSel[i1][4]->Fill(ss);
						if((maxbin-minbin+1)>4 && (maxbin-minbin+1)<20) IntegralSel[i1][5]->Fill(ss);
						
// 						isel=0;
// 						if(i1==0)
// 						{
// 							if(maxampratio<1.15) isel=0;
// 							else if(maxampratio<1.61) isel=1;
// 							else if(maxampratio<2.08) isel=2;
// 							else if(maxampratio<2.6) isel=3;
// 							else if(maxampratio<3.05) isel=4;
// 							else isel=5;
// 						}
// 						else if(i1==1)
// 						{
// 							if(maxampratio<1.2) isel=0;
// 							else if(maxampratio<1.7) isel=1;
// 							else isel=2;
// 						}
// 						else if(i1==2)
// 						{
// 							if(maxampratio<1.3) isel=0;
// 							else if(maxampratio<1.8) isel=1;
// 							else if(maxampratio<2.35) isel=2;
// 							else if(maxampratio<2.95) isel=3;
// 							else if(maxampratio<3.5) isel=4;
// 							else isel=5;
// 						}
						
						
// 						if((i1==0 && (maxamp/(3.*hsig3))>1.15 && (maxamp/(3.*hsig3))<1.55)||(i1==1 && (maxamp/(3.*hsig3))>1.2 && (maxamp/(3.*hsig3))<1.6)||(i1==2 && (maxamp/(3.*hsig3))>1.3 && (maxamp/(3.*hsig3))<1.8))
// 						{
							Integral[i1]->Fill(ss);
// 							IntegralSel[i1][isel]->Fill(ss);
// 							if(maxampratio>2.) IntegralSel2[i1]->Fill(ss);
// 							if(maxampratio>2. && maxampratio<3.) IntegralSel3[i1]->Fill(ss);
// 						}
						MaxAmpover3sigma[i1]->Fill(maxamp/(3.*hsig3));
						MaxAmpover3sigmavsWidth[i1]->Fill(maxbin-minbin+1,maxamp/(3.*hsig3));
					}
					i2=maxbin+1;
					if(i2>(ed.PMTWF->at(i1).size()-50)) break;
				}
			}
			for(int i2=50;i2<ed.PMTWF->at(i1).size()-50;i2++)
			{
				s=-1*(ed.PMTWF->at(i1)[i2]-hped3);
				if((s/hsig3)>AmpoverRMSPrePeak[i1] && (s/hsig3)<AmpoverRMS[i1])
				{
					maxamp=0;
					maxbin=i2-1;
					for(int i3=i2;i3<ed.PMTWF->at(i1).size();i3++)
					{
						s=-1*(ed.PMTWF->at(i1)[i3]-hped3);
						if(s>maxamp) maxamp=s;
						if(s<=0)
						{
							maxbin=i3-1;
							break;
						}
					}
					minbin=i2+1;
					for(int i3=i2-1;i3>=1;i3--)
					{
						s=-1*(ed.PMTWF->at(i1)[i3]-hped3);
						if(s>maxamp) maxamp=s;
						if(s<=0)
						{
							minbin=i3+1;
							break;
						}
					}
					if((maxbin-minbin+1)>2)
					{
						ss=0;maxamp=0;
						for(int i3=minbin;i3<=maxbin;i3++)
						{
							s=-1*(ed.PMTWF->at(i1)[i3]-hped3);
							ss+=s;
							if(s>maxamp) maxamp=s;
						}
						IntegralSel2[i1]->Fill(ss);
						if((maxbin-minbin+1)>3) IntegralSel3[i1]->Fill(ss);
					}
					i2=maxbin+1;
					if(i2>(ed.PMTWF->at(i1).size()-50)) break;
				}
			}
		}
	}
	outroot->cd();
	
	for(int i1=0;i1<NPMTs;i1++)
	{
		Ampvst[i1]->Write();
		BLvsE[i1]->Write();
		MeanPedestalsAll[i1]->Write();
// 		MeanPedestals[i1]->Fit(g1,"q","q",700.,1100.);
		MeanPedestals[i1]->Write();
// 		LinFitPedestals[i1]->Fit(g1,"q","q",700.,1100.);
		LinFitPedestals[i1]->Write();
// 		GausFitPedestals[i1]->Fit(g1,"q","q",700.,1100.);
		GausFitPedestals[i1]->Write();
// 		GausFitPedestalRMS[i1]->Fit(g1,"q","q",0.5,1.5);
		GausFitPedestalRMS[i1]->Write();
		BLFitOverMean[i1]->Write();
		MaxAmpoversigma[i1]->Write();
		MaxAmpover3sigma[i1]->Write();
		MaxAmpover3sigmavsWidth[i1]->Write();
		PulseWidth[i1]->Write();
		Amplitudes[i1]->Write();
		AllAmplitudes[i1]->Write();
		AllAmplitudesPre[i1]->Fit(g1,"q","q",-50,50.);
		AllAmplitudesPre[i1]->Write();
		MaxAmplitudes[i1]->Write();
		
		gexpo->SetParameter(1,-0.2);
		IntegralSel[i1][1]->Fit(gexpo,"q","q",IntegralSel[i1][1]->GetBinCenter(IntegralSel[i1][1]->GetMaximumBin()),IntegralSel[i1][1]->GetBinCenter(IntegralSel[i1][1]->GetMaximumBin()+5.));
		float leftlim=gexpo->GetX(IntegralSel[i1][1]->GetBinContent(IntegralSel[i1][1]->GetMaximumBin())/10,IntegralSel[i1][1]->GetBinCenter(IntegralSel[i1][1]->GetMaximumBin()),30);
		IntegralSel[i1][1]->GetXaxis()->SetRangeUser(leftlim,200);
		float speest=IntegralSel[i1][1]->GetMean();
		IntegralSel[i1][1]->GetXaxis()->SetRangeUser(0,200);
		
// 		maxbin=IntegralSel[i1][1]->GetMaximumBin();
// 		maxamp=IntegralSel[i1][1]->GetBinContent(maxbin);
// 		for(int is1=maxbin;is1<=IntegralSel[i1][1]->GetNbinsX();is1++)
// 		{
// 			
// 		}
		
// 		gint->SetParameter(0,IntegralSel[i1][1]->GetBinContent(IntegralSel[i1][1]->GetMaximumBin()));
		gint->SetParameter(1,-0.2);
		gint->SetParameter(2,IntegralSel[i1][1]->GetBinContent(IntegralSel[i1][1]->GetMaximumBin())/10);
		gint->SetParLimits(2,0,1e4);
		gint->SetParameter(3,speest);
		gint->SetParLimits(3,10,150);
		gint->SetParameter(4,10.);
		gint->SetParLimits(4,5,100);
		IntegralSel[i1][1]->Fit(gint,"q","q",IntegralSel[i1][1]->GetBinCenter(IntegralSel[i1][1]->GetMaximumBin()),200);
		IntegralSel[i1][1]->Fit(gint,"q","q",IntegralSel[i1][1]->GetBinCenter(IntegralSel[i1][1]->GetMaximumBin()),gint->GetParameter(3)+1.5*gint->GetParameter(4));
		
		cout<<"PMT "<<i1<<" spe "<<gint->GetParameter(3)<<endl;
		
// 		tfgg->SetParameter(0,IntegralSel[i1][1]->GetBinContent(IntegralSel[i1][1]->GetMaximumBin()));
// 		tfgg->SetParameter(3,IntegralSel[i1][1]->GetBinContent(IntegralSel[i1][1]->GetMaximumBin())/10);
// 		tfgg->SetParameter(4,40.);
// 		tfgg->SetParameter(5,10.);
// 		IntegralSel[i1][1]->Fit(tfgg,"q","q",0,200);
// 		IntegralSel[i1][1]->Fit(tfgg,"q","q",tfgg->GetParameter(1)-2*tfgg->GetParameter(1),tfgg->GetParameter(4)+2*tfgg->GetParameter(5));
		
		Integral[i1]->Write();
		
		IntegralSel2[i1]->Write();
		IntegralSel3[i1]->Write();
		for(int i2=0;i2<6;i2++)
		{
			IntegralSel[i1][i2]->SetLineColor(i2+1);
			IntegralSel[i1][i2]->Write();
		}
		AmplitudeoverRMS[i1]->Write();
	}
	
	outroot->Close();
	
	sprintf(hname,"mv PMTCalibration_CalibRunTest_%d.root %s/Histos/PMTCalibration_CalibRunTest_%d.root",RunNo,AnalysisFilePath,RunNo);system(hname);
}