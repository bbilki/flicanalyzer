void GetPMTCalibration_TrigRuns()
{
	sprintf(hname,"PMTCalibration_TrigRuns_%d.root",RunNo);
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
	TH1F* BLs[NPMTs];
	TH1F* Integral[NPMTs];
	TH1F* Amplitudes[NPMTs];
	TH1F* MaxAmplitudes[NPMTs];
	TH1F* PulseWidth[NPMTs];
	
	int linecolors[8]={1,3,4,6,2,1,3,4};
	
	for(int i1=0;i1<NPMTs;i1++)
	{
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
		
		sprintf(hname,"BLFitOverMean_%d",i1);
		BLFitOverMean[i1]=new TH1F(hname,hname,5000,0.9995,1.0005);
		
		sprintf(hname,"Integral_%d",i1);
// 		Integral[i1]=new TH1F(hname,hname,1000,0,200);
		Integral[i1]=new TH1F(hname,hname,250,0,200);
		Integral[i1]->SetLineColor(linecolors[i1]);
		Integral[i1]->SetLineWidth(2);
		
		sprintf(hname,"PulseWidth_%d",i1);
		PulseWidth[i1]=new TH1F(hname,hname,30,-0.5,29.5);
		sprintf(hname,"Amplitudes_%d",i1);
		Amplitudes[i1]=new TH1F(hname,hname,200,0,20);
		sprintf(hname,"MaxAmplitudes_%d",i1);
		MaxAmplitudes[i1]=new TH1F(hname,hname,200,0,20);
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
	
	TF1* g1=new TF1("g1","gaus",0.,1500.);
	TF1* lin=new TF1("lin","[0]",0.,15000);
	TF1* linplussine=new TF1("linplussine","[0]+sin([1]*x+[2])",0.,15000);
	TF1* gint=new TF1("gint","expo(0)+gaus(2)",0.,200.);
	TF1* gexpo=new TF1("gexpo","expo",0.,200.);
	
	TH1F* hb=new TH1F("hb","hb",15000,-0.5,14999.5);
	TH1F* hp=new TH1F("hp","hp",1500,-0.5,1499.5);
	
// 	TF1* tf=new TF1("GL","gaus(0)+landau(3)",-100.,200.);
// 	TF1* tf=new TF1("GLG","gaus(0)+landau(3)+gaus(6)",-100.,200.);
	TF1* tf=new TF1("GGG","gaus(0)+gaus(3)+gaus(6)",-50.,100.);
	TF1* tflg=new TF1("LG","landau(0)+gaus(3)",0.,200.);
	
// 	TF1* tf=new TF1("GG","gaus(0)+gaus(3)",-50.,100.);
// 	TF1* tf=new TF1("GL","gaus(0)+landau(3)",-50.,100.);
// 	TF1* tf=new TF1("GLG","gaus(0)+landau(3)+gaus(6)",-50.,100.);
	TF1* tfg=new TF1("G","gaus",-50.,200.);
	TF1* tfgg=new TF1("GG","gaus(0)+gaus(3)",-50.,100.);
	TF1* tfggg=new TF1("GGG","gaus(0)+gaus(3)+gaus(6)",-50.,100.);
	TF1* tfl=new TF1("L","landau",-50.,100.);
	
	TF1* tfLanGau= new TF1("LanGau",LanGau,-50.,200.,5);
	TF1* tfGauGau= new TF1("GauGau",GauGau,-50.,200.,5);
	
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
	int leftbin=0;int rightbin=0;
	
	vector <float> peds[NPMTs][4];//mean, lin fit, gaus fit mean, gaus fit sigma
	float AmpoverRMS[NPMTs]={0};
	float AmpoverRMSPrePeak[NPMTs]={0};
	float PedRMSCut[NPMTs]={0};
	bool EventOK=true;
	
	vector <float> amps;
	vector <int> ampcounts;
	int ampind=-1;
	bool tripleamp=false;
	
	int binsbelowsigma=0;
	int binsabovesigma=0;
	
	float fitlowlim=0.;
	float fituplim=0.;
	float minchi2=0.;
	float minchi2lim=0.;
	
	float SPEs[NPMTs]={0};
	
// 	cout<<T->GetEntries()<<endl;s
	for(int I=0;I<T->GetEntries();I++)
	{
		if(I%1000==0) cout<<I<<endl;
		T->GetEntry(I);
		EventOK=true;
		for(int i1=0;i1<NPMTs;i1++)
		{
			hped=0;
			hb->Reset();hp->Reset();
			for(int i2=0;i2<1500;i2++)
// 			for(int i2=13500;i2<15000;i2++)
			{
				hped+=ed.PMTWF->at(i1)[i2];
				hb->SetBinContent(i2+1,ed.PMTWF->at(i1)[i2]);
				hp->Fill(ed.PMTWF->at(i1)[i2]);
			}
			hped/=1500.;
			hb->Fit(lin,"q","q",0.,1500.);
			hped2=lin->GetParameter(0);
			
			hp->Fit(g1,"q","q",0.,1500.);
			hped3=g1->GetParameter(1);
			hsig3=g1->GetParameter(2);
			
			MeanPedestalsAll[i1]->Fill(hped3);
			BLvsE[i1]->SetPoint(BLvsE[i1]->GetN(),I,hped);
			
			peds[i1][0].push_back(hped);
			peds[i1][1].push_back(hped2);
			peds[i1][2].push_back(hped3);
			peds[i1][3].push_back(hsig3);
			
// 			if(I<200) continue;
			if(hped>0) BLFitOverMean[i1]->Fill(hped2/hped);
			MeanPedestals[i1]->Fill(hped);
			LinFitPedestals[i1]->Fill(hped2);
			GausFitPedestals[i1]->Fill(hped3);
			GausFitPedestalRMS[i1]->Fill(hsig3);
			
			if(!EventOK) continue;
			hb->Reset();
// 			for(int i2=10000;i2<ed.PMTWF->at(i1).size();i2++)
// 			for(int i2=8000;i2<13500;i2++)
			for(int i2=8000;i2<15000;i2++)
			{
				s=-1.*(ed.PMTWF->at(i1)[i2]-hped3);
				Amplitudes[i1]->Fill(s);
				hb->SetBinContent(i2+1,s);
				
// 				if(i2<1500 && s>=4.*hsig3)
// 				{
// 					EventOK=false;
// 					break;
// 				}
			}
			if(!EventOK) continue;
			if(hb->GetBinContent(hb->GetMaximumBin())>20.)
			{
				EventOK=false;
				continue;
			}
			
// 			for(int is1=10000;is1<=hb->GetNbinsX();is1++)
			for(int is1=8000;is1<=hb->GetNbinsX();is1++)
			{
				if(hb->GetBinContent(is1)>=4.*hsig3)
				{
					minbin=-1;
					maxbin=-1;
					for(int i3=is1;i3>=2;i3--)
					{
						if(hb->GetBinContent(i3)>0 && hb->GetBinContent(i3-1)<=0)
						{
							minbin=i3;
							break;
						}
					}
					for(int i3=is1;i3<hb->GetNbinsX();i3++)
					{
						if(hb->GetBinContent(i3)>0 && hb->GetBinContent(i3+1)<=0)
						{
							maxbin=i3;
							break;
						}
					}
					if(minbin==-1) minbin=1;
					if(maxbin==-1) maxbin=hb->GetNbinsX();
					PulseWidth[i1]->Fill((maxbin-minbin+1));
// 					if((maxbin-minbin+1)>20) cout<<I<<" "<<i1<<" "<<(maxbin-minbin+1)<<endl;
// 					if((maxbin-minbin+1)>3)
					if((maxbin-minbin+1)>5)
					{
						ss=0;maxamp=0;
						amps.clear();ampcounts.clear();
						binsabovesigma=0;
						binsbelowsigma=0;
						for(int i3=minbin;i3<=maxbin;i3++)
						{
							ss+=hb->GetBinContent(i3);
							if(hb->GetBinContent(i3)>maxamp){maxamp=hb->GetBinContent(i3);}
							ampind=-1;
							for(int i4=0;i4<amps.size();i4++)
							{
								if(fabs(amps[i4]-hb->GetBinContent(i3))<0.1){ampind=i4;break;}
							}
							if(ampind==-1)
							{
								amps.push_back(hb->GetBinContent(i3));
								ampcounts.push_back(1);
							}
							else
							{
								ampcounts[ampind]++;
							}
							if(hb->GetBinContent(i3)<hsig3){binsbelowsigma++;}
							else{binsabovesigma++;}
						}
// 						tripleamp=false;
// 						for(int i3=minbin+1;i3<=maxbin-1;i3++)
// 						{
// 							if(hb->GetBinContent(i3-1)==hb->GetBinContent(i3) && hb->GetBinContent(i3+1)==hb->GetBinContent(i3))
// 							{
// 								tripleamp=true;
// 								break;
// 							}
// 						}
// 						if(amps.size()>2 && !tripleamp)
// 						if(amps.size()>3)
						if((((float)binsabovesigma)/((float)binsbelowsigma))>=1.)
						{
							Integral[i1]->Fill(ss);
							MaxAmplitudes[i1]->Fill(maxamp);
						}
// 						if((i1==0 || i1==2) && ss<12)
// 						if((i1==2) && ss<8)
// 						{
// 							cout<<I<<" "<<i1<<" "<<is1<<" "<<ss<<endl;
// 						}
					}
					is1=maxbin+1;
				}
			}
		}
	}
	
	for(int i1=0;i1<NPMTs;i1++)
	{
		MeanPedestals[i1]->Fit(g1,"q","q",700.,1100.);
		LinFitPedestals[i1]->Fit(g1,"q","q",700.,1100.);
		GausFitPedestals[i1]->Fit(g1,"q","q",700.,1100.);
		GausFitPedestalRMS[i1]->Fit(g1,"q","q",0.5,1.5);
	}
	
	outroot->cd();
	
	for(int i1=0;i1<NPMTs;i1++)
	{
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
		PulseWidth[i1]->Write();
		Amplitudes[i1]->Write();
		MaxAmplitudes[i1]->Write();
		
// 		if(i1==0 || i1==2)
// 		{
// 			tfgg->SetParameter(0,Integral[i1]->GetBinContent(Integral[i1]->GetMaximumBin()));
// 			tfgg->SetParameter(1,Integral[i1]->GetBinCenter(Integral[i1]->GetMaximumBin()));
// 			tfgg->SetParameter(2,10);
// 			tfgg->SetParameter(3,Integral[i1]->GetBinContent(Integral[i1]->GetMaximumBin())/100);
// 			tfgg->SetParameter(4,Integral[i1]->GetBinCenter(Integral[i1]->GetMaximumBin())+20);
// 			tfgg->SetParameter(5,20);
// 			
// 			tfgg->SetParLimits(0,10,10000);
// 			tfgg->SetParLimits(1,2,100);
// 			tfgg->SetParLimits(2,0.1,100);
// 			tfgg->SetParLimits(3,1,1000);
// 			tfgg->SetParLimits(4,1,200);
// 			tfgg->SetParLimits(5,0.1,100);
// 			
// 			Integral[i1]->Fit(tfgg,"q","q",0,100);
// 			
// 			tfgg->SetNpx(1000);
// 		}
// 		else
// 		{
// 			tfl->SetParameter(0,Integral[i1]->GetBinContent(Integral[i1]->GetMaximumBin()));
// 			tfl->SetParameter(1,Integral[i1]->GetBinCenter(Integral[i1]->GetMaximumBin()));
// 			tfl->SetParameter(2,10);
// 			tfl->SetParLimits(0,10,10000);
// 			tfl->SetParLimits(1,2,100);
// 			tfl->SetParLimits(2,0.1,100);
// 			
// 			Integral[i1]->Fit(tfl,"q","q",0,20);
// 			
// 			tfl->SetNpx(1000);
// 		}
		
// 		fituplim=200.;
// 		if(i1==0) fituplim=120.;
// 		else if(i1==1) fituplim=30.;
// 		else if(i1==2) fituplim=100.;
		
		fituplim=200.;
		for(int is1=1;is1<=Integral[i1]->GetNbinsX();is1++)
		{
			if((Integral[i1]->Integral(1,is1)/Integral[i1]->Integral())>0.9)
			{
				fituplim=Integral[i1]->GetBinCenter(is1);
				break;
			}
		}
		
		if(i1==0 || i1==2)
		{
			if(i1==2) Integral[i1]->Rebin(2);
			
			Integral[i1]->GetXaxis()->SetRangeUser(10.,100.);
			
			tfLanGau->SetParameter(0,Integral[i1]->GetBinContent(Integral[i1]->GetMaximumBin()));
			tfLanGau->SetParameter(1,Integral[i1]->GetBinCenter(Integral[i1]->GetMaximumBin()));
			tfLanGau->SetParameter(2,10);
	// 		tfLanGau->SetParameter(3,Integral[i1]->GetBinContent(Integral[i1]->GetMaximumBin())/100);
			tfLanGau->SetParameter(3,Integral[i1]->GetBinContent(Integral[i1]->GetMaximumBin()));
			tfLanGau->SetParameter(4,20);
			
			tfLanGau->SetParLimits(0,10,10000);
			tfLanGau->SetParLimits(1,0.,100);
			tfLanGau->SetParLimits(2,0.,100);
			tfLanGau->SetParLimits(3,0.,10000);
			tfLanGau->SetParLimits(4,0.,1000);
			
			Integral[i1]->GetXaxis()->SetRangeUser(0.,200.);
			
			fitlowlim=10.;
			Integral[i1]->Fit(tfLanGau,"q","q",fitlowlim,fituplim);
			while((tfLanGau->GetChisquare()/tfLanGau->GetNDF())>1.6)
			{
				fitlowlim+=1.;
				
				Integral[i1]->GetXaxis()->SetRangeUser(10.,100.);
				
				tfLanGau->SetParameter(0,Integral[i1]->GetBinContent(Integral[i1]->GetMaximumBin()));
				tfLanGau->SetParameter(1,Integral[i1]->GetBinCenter(Integral[i1]->GetMaximumBin()));
				tfLanGau->SetParameter(2,10);
	// 			tfLanGau->SetParameter(3,Integral[i1]->GetBinContent(Integral[i1]->GetMaximumBin())/100);
				tfLanGau->SetParameter(3,Integral[i1]->GetBinContent(Integral[i1]->GetMaximumBin()));
				tfLanGau->SetParameter(4,20);
				
				tfLanGau->SetParLimits(0,10,10000);
				tfLanGau->SetParLimits(1,0.,100);
				tfLanGau->SetParLimits(2,0.,100);
				tfLanGau->SetParLimits(3,0.,10000);
				tfLanGau->SetParLimits(4,0.,1000);
				
				Integral[i1]->GetXaxis()->SetRangeUser(0.,200.);
				
				Integral[i1]->Fit(tfLanGau,"q","q",fitlowlim,fituplim);
				if((Integral[i1]->GetBinCenter(Integral[i1]->GetMaximumBin())-fitlowlim)<5.)
				{
	// 				Integral[i1]->Fit(tflg,"q","q",0.,200);
					fitlowlim=10.;
					break;
				}
			}
			
			Integral[i1]->Fit(tfLanGau,"q","q",fitlowlim,fituplim);
			tfLanGau->SetNpx(1000);
			
			if(tfLanGau->GetParameter(4)<1. || tfLanGau->GetParError(4)>100.)
			{
				tfLanGau->SetParameter(4,10);
				Integral[i1]->Fit(tfLanGau,"q","q",fitlowlim,fituplim);
			}
		}
		else if(i1==1)
		{
			Integral[i1]->Rebin(2);
			Integral[i1]->Fit(tfl,"q","q",0.,20.);
			tfl->SetNpx(1000);
		}
// 		else if(i1==2)
// 		{
// // 			Integral[i1]->GetXaxis()->SetRangeUser(10.,100.);
// // 			
// // 			tfGauGau->SetParameter(0,Integral[i1]->GetBinContent(Integral[i1]->GetMaximumBin()));
// // 			tfGauGau->SetParameter(1,Integral[i1]->GetBinCenter(Integral[i1]->GetMaximumBin()));
// // 			tfGauGau->SetParameter(2,10);
// // 	// 		tfLanGau->SetParameter(3,Integral[i1]->GetBinContent(Integral[i1]->GetMaximumBin())/100);
// // 			tfGauGau->SetParameter(3,Integral[i1]->GetBinContent(Integral[i1]->GetMaximumBin()));
// // 			tfGauGau->SetParameter(4,20);
// // 			
// // 			tfGauGau->SetParLimits(0,10,10000);
// // 			tfGauGau->SetParLimits(1,0.,100);
// // 			tfGauGau->SetParLimits(2,0.,100);
// // 			tfGauGau->SetParLimits(3,0.,10000);
// // 			tfGauGau->SetParLimits(4,0.,1000);
// // 			
// // 			Integral[i1]->GetXaxis()->SetRangeUser(0.,200.);
// // 			
// // 			fitlowlim=10.;
// // 			Integral[i1]->Fit(tfGauGau,"q","q",fitlowlim,fituplim);
// // 			while((tfGauGau->GetChisquare()/tfGauGau->GetNDF())>1.6)
// // 			{
// // 				fitlowlim+=1.;
// // 				
// // 				Integral[i1]->GetXaxis()->SetRangeUser(10.,100.);
// // 				
// // 				tfGauGau->SetParameter(0,Integral[i1]->GetBinContent(Integral[i1]->GetMaximumBin()));
// // 				tfGauGau->SetParameter(1,Integral[i1]->GetBinCenter(Integral[i1]->GetMaximumBin()));
// // 				tfGauGau->SetParameter(2,10);
// // 	// 			tfLanGau->SetParameter(3,Integral[i1]->GetBinContent(Integral[i1]->GetMaximumBin())/100);
// // 				tfGauGau->SetParameter(3,Integral[i1]->GetBinContent(Integral[i1]->GetMaximumBin()));
// // 				tfGauGau->SetParameter(4,20);
// // 				
// // 				tfGauGau->SetParLimits(0,10,10000);
// // 				tfGauGau->SetParLimits(1,0.,100);
// // 				tfGauGau->SetParLimits(2,0.,100);
// // 				tfGauGau->SetParLimits(3,0.,10000);
// // 				tfGauGau->SetParLimits(4,0.,1000);
// // 				
// // 				Integral[i1]->GetXaxis()->SetRangeUser(0.,200.);
// // 				
// // 				Integral[i1]->Fit(tfGauGau,"q","q",fitlowlim,fituplim);
// // 				if((Integral[i1]->GetBinCenter(Integral[i1]->GetMaximumBin())-fitlowlim)<5.)
// // 				{
// // 	// 				Integral[i1]->Fit(tflg,"q","q",0.,200);
// // 					fitlowlim=10.;
// // 					break;
// // 				}
// // 			}
// // 			
// // 			Integral[i1]->Fit(tfGauGau,"q","q",fitlowlim,fituplim);
// // 			tfGauGau->SetNpx(1000);
// 			
// 			
// 			
// 			
// 			
// 			Integral[i1]->GetXaxis()->SetRangeUser(10.,100.);
// 			
// 			tfg->SetParameter(0,Integral[i1]->GetBinContent(Integral[i1]->GetMaximumBin()));
// 			tfg->SetParameter(1,Integral[i1]->GetBinCenter(Integral[i1]->GetMaximumBin()));
// 			tfg->SetParameter(2,10);
// 			
// 			tfg->SetParLimits(0,10,10000);
// 			tfg->SetParLimits(1,0.,100);
// 			tfg->SetParLimits(2,0.,100);
// 			
// 			Integral[i1]->GetXaxis()->SetRangeUser(0.,200.);
// 			
// 			fitlowlim=10.;
// 			Integral[i1]->Fit(tfg,"q","q",fitlowlim,fituplim);
// 			while((tfg->GetChisquare()/tfg->GetNDF())>1.6)
// 			{
// 				fitlowlim+=1.;
// 				
// 				Integral[i1]->GetXaxis()->SetRangeUser(10.,100.);
// 				
// 				tfg->SetParameter(0,Integral[i1]->GetBinContent(Integral[i1]->GetMaximumBin()));
// 				tfg->SetParameter(1,Integral[i1]->GetBinCenter(Integral[i1]->GetMaximumBin()));
// 				tfg->SetParameter(2,10);
// 				
// 				tfg->SetParLimits(0,10,10000);
// 				tfg->SetParLimits(1,0.,100);
// 				tfg->SetParLimits(2,0.,100);
// 				
// 				Integral[i1]->GetXaxis()->SetRangeUser(0.,200.);
// 				
// 				Integral[i1]->Fit(tfg,"q","q",fitlowlim,fituplim);
// 				if((Integral[i1]->GetBinCenter(Integral[i1]->GetMaximumBin())-fitlowlim)<5.)
// 				{
// 	// 				Integral[i1]->Fit(tflg,"q","q",0.,200);
// 					fitlowlim=10.;
// 					break;
// 				}
// 			}
// 			
// 			Integral[i1]->Fit(tfg,"q","q",fitlowlim,fituplim);
// 			tfg->SetNpx(1000);
// 		}
		
		
		
// 		tflg->SetParameter(0,Integral[i1]->GetBinContent(Integral[i1]->GetMaximumBin()));
// 		tflg->SetParameter(1,Integral[i1]->GetBinCenter(Integral[i1]->GetMaximumBin()));
// 		tflg->SetParameter(2,10);
// 		tflg->SetParameter(3,Integral[i1]->GetBinContent(Integral[i1]->GetMaximumBin())/100);
// 		tflg->SetParameter(4,Integral[i1]->GetBinCenter(Integral[i1]->GetMaximumBin())+20);
// 		tflg->SetParameter(5,20);
// 		
// 		tflg->SetParLimits(0,10,10000);
// 		tflg->SetParLimits(1,2,100);
// // 		tflg->SetParLimits(2,1,100);
// 		tflg->SetParLimits(2,0.1,100);
// 		
// 		tflg->SetParLimits(3,1,1000);
// 		tflg->SetParLimits(4,1,200);
// // 		tflg->SetParLimits(5,5,100);
// 		tflg->SetParLimits(5,0.1,100);
// // 		
// // 		Integral[i1]->Fit(tflg,"q","q",0,200);
// // 		Integral[i1]->Fit(tflg,"q","q",tflg->GetParameter(1)-2.5*tflg->GetParameter(2),200);
// 		
// 		Integral[i1]->Fit(tflg,"q","q",0,200);
// 		
// 		fitlowlim=0.;
// 		while((tflg->GetChisquare()/tflg->GetNDF())>1.5)
// 		{
// 			fitlowlim+=1.;
// 			Integral[i1]->Fit(tflg,"q","q",fitlowlim,200);
// // 			if((Integral[i1]->GetBinCenter(Integral[i1]->GetMaximumBin())-fitlowlim)<5. || (Integral[i1]->GetBinCenter(Integral[i1]->GetMaximumBin())-fitlowlim)<5.)
// // 			{
// // 				Integral[i1]->Fit(tflg,"q","q",0.,200);
// // 				break;
// // 			}
// 		}
// 		tflg->SetNpx(1000);
		
// 		gexpo->SetParameter(1,-0.2);
// 		Integral[i1]->Fit(gexpo,"q","q",Integral[i1]->GetBinCenter(Integral[i1]->GetMaximumBin()),Integral[i1]->GetBinCenter(Integral[i1]->GetMaximumBin()+5.));
// 		float leftlim=gexpo->GetX(Integral[i1]->GetBinContent(Integral[i1]->GetMaximumBin())/10,Integral[i1]->GetBinCenter(Integral[i1]->GetMaximumBin()),30);
// 		Integral[i1]->GetXaxis()->SetRangeUser(leftlim,200);
// 		float speest=Integral[i1]->GetMean();
// 		Integral[i1]->GetXaxis()->SetRangeUser(0,200);
// 		
// 		gint->SetParameter(1,-0.2);
// 		gint->SetParameter(2,Integral[i1]->GetBinContent(Integral[i1]->GetMaximumBin())/10);
// 		gint->SetParLimits(2,0,1e4);
// 		gint->SetParameter(3,speest);
// 		gint->SetParLimits(3,10,150);
// 		gint->SetParameter(4,10.);
// 		gint->SetParLimits(4,5,100);
// 		Integral[i1]->Fit(gint,"q","q",Integral[i1]->GetBinCenter(Integral[i1]->GetMaximumBin()),200);
// 		Integral[i1]->Fit(gint,"q","q",Integral[i1]->GetBinCenter(Integral[i1]->GetMaximumBin()),gint->GetParameter(3)+1.5*gint->GetParameter(4));
		
// 		cout<<"PMT "<<i1<<" spe "<<gint->GetParameter(3)<<endl;
// 		cout<<"PMT "<<i1<<" spe "<<tflg->GetParameter(1)<<endl;
		
// 		if(tflg->GetParameter(0)>tflg->GetParameter(3))
// 		{
// 			SPEs[i1]=tflg->GetParameter(1);
// 		}
// 		else
// 		{
// 			SPEs[i1]=tflg->GetParameter(4);
// 		}
		
		if(i1==1) SPEs[i1]=tfl->GetParameter(1);
		else SPEs[i1]=tfLanGau->GetParameter(1);
		
		Integral[i1]->Write();
	}
	for(int i1=0;i1<NPMTs;i1++)
	{
		cout<<"PMT "<<i1<<" spe "<<SPEs[i1]<<endl;
	}
	
	outroot->Close();
	
	sprintf(hname,"mv PMTCalibration_TrigRuns_%d.root %s/Histos/PMTCalibration_TrigRuns_%d.root",RunNo,AnalysisFilePath,RunNo);system(hname);
}