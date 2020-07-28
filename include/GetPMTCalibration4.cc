void GetPMTCalibration4()//for PMT calibration runs with VME
{
	sprintf(hname,"%s/Run_%d.root",NTupleFilePath,RunNo);
	TFile* inroot=new TFile(hname);
	TTree* T =  (TTree*) inroot->Get("T");
	T->SetBranchAddress("TPCWF",&ed.TPCWF);
	T->SetBranchAddress("PMTWF",&ed.PMTWF);
	
	sprintf(hname,"PMTCalibration4_%d.root",RunNo);
	TFile* outroot=new TFile(hname,"recreate");
	
	if(RunNo>=208) NPMTs=4;
	
	TH1F* Integral[NPMTs];TH1F* IntegralSel[NPMTs];
	TH1F* MaxAmplitude[NPMTs];TH1F* PulseWidth[NPMTs];TH1F* PulseWidthSel[NPMTs];
	TH1F* Baseline[NPMTs];TH1F* SelAmplitude[NPMTs];TH1F* SelAmplitudeOverRMS[NPMTs];
	for(int i1=0;i1<NPMTs;i1++)
	{
		sprintf(hname,"Integral_%d",i1);
// 		Integral[i1]=new TH1F(hname,hname,1200,0,200);
		Integral[i1]=new TH1F(hname,hname,2400,0,200);
		sprintf(hname,"IntegralSel_%d",i1);
		IntegralSel[i1]=new TH1F(hname,hname,1200,0,200);
		sprintf(hname,"MaxAmplitude_%d",i1);
		MaxAmplitude[i1]=new TH1F(hname,hname,1000,0,200);
		sprintf(hname,"SelAmplitude_%d",i1);
		SelAmplitude[i1]=new TH1F(hname,hname,1000,0,200);
		sprintf(hname,"SelAmplitudeOverRMS_%d",i1);
// 		SelAmplitudeOverRMS[i1]=new TH1F(hname,hname,1200,0,200);
		SelAmplitudeOverRMS[i1]=new TH1F(hname,hname,1600,0,200);
		sprintf(hname,"PulseWidth_%d",i1);
		PulseWidth[i1]=new TH1F(hname,hname,200,-0.5,199.5);
		sprintf(hname,"PulseWidthSel_%d",i1);
		PulseWidthSel[i1]=new TH1F(hname,hname,200,-0.5,199.5);
		sprintf(hname,"Baseline_%d",i1);
		Baseline[i1]=new TH1F(hname,hname,4500,0,1500);
	}
	
	TH1F* hh=new TH1F("PMTWF","PMTWF",15000,-0.5,14999.5);
	TH1F* hbl=new TH1F("BaselineHist","BaselineHist",1500,-0.5,1499.5);
	
	TF1* g1=new TF1("g1","gaus",-1000.,2000.);
// 	TF1* tf1=new TF1("tf1","[0]*(exp([0]+[1]*(x-[2]))+exp([3]+[4]*(x-[2]))+exp([5]+[6]*(x-[2]))+exp([7]+[8]*(x-[2])))",0.,15000.);
// 	TF1* tf1=new TF1("tf1","(exp([0]+[1]*(x-[2]))+exp([3]+[4]*(x-[2]))+exp([5]+[6]*(x-[2])))",0.,15000.);
// 	TF1* tf1=new TF1("tf1","gaus(0)+expo(3)+expo(5)",0.,15000.);
// 	TF1* tf1=new TF1("tf1","[0]*pow(x-[1],[2])*([3]*exp(-(x-[1])/[4])+[5]*exp(-(x-[1])/[6]))",0.,15000.);
// 	TF1* tf1=new TF1("tf1","[0]*pow(x-[1],[2])*(exp(-(x-[1])/[3])+exp(-(x-[1])/[4]))",0.,15000.);
// 	TF1* tf1=new TF1("tf1","[0]*exp(-0.5*(pow((log((x)/[2]))/[3],2)))",0.,15000.);
// 	TF1* tf1=new TF1("tf1","landau(0)+expo(3)",0.,15000.);
// 	TF1* tf1=new TF1("tf1","[0]*exp(-pow((x-[1])/[2],2))+[6]*exp(-x/[3])+[7]*exp(-x/[4])+[8]*exp(-x/[5])",0.,15000.);
// 	TF1 *tf1 = new TF1("fit",fitf2f,0.,15000,6);
// 	TF1 *tf1 = new TF1("fit",fitf3f,0.,15000,7);
//  	TF1* tf1=new TF1("tf1","([0]/[3])*exp(-(log(x-[1])-[2])^2/(2.*[3]^2))",0.,15000.);
	TF1* tf1=new TF1("tf1","expo",0.,15000.);
	TF1* tflin=new TF1("tflin","[0]",0.,2000.);
	
	vector <float> Amp;
	float hsum[20]={0.};
	float hped=0;
	float hped2=0;float hsig2=0;
	float xlim=0;int fitstartbin=0;int xlimbin=0;
	float maxamp=0;int nc=0;
	
	float baseline[NPMTs];baseline[NPMTs]={0.};
	float baselineRMS[NPMTs];baselineRMS[NPMTs]={0.};
	float lmax=0;float pulseamp=0;
	float qint=0;int sbin=0;int minbin=0;int maxbin=0;int peakbin=0;
	
	for(int I=0;I<T->GetEntries();I++)
	{
		T->GetEntry(I);
		if(I<100) continue;
		for(int i1=0;i1<NPMTs;i1++)
		{
			hbl->Reset();
			for(int i2=0;i2<1000;i2++)
			{
				hbl->Fill(ed.PMTWF->at(i1)[i2]);
			}
			hbl->Fit(g1,"q","q",hbl->GetMean()-3*hbl->GetRMS(),hbl->GetMean()+3*hbl->GetRMS());
			baseline[i1]=g1->GetParameter(1);
			baselineRMS[i1]=g1->GetParameter(2);
			Baseline[i1]->Fill(baseline[i1]);
			hh->Reset();
			lmax=0;
			for(int i2=0;i2<ed.PMTWF->at(i1).size();i2++)
			{
				pulseamp=-1.*(ed.PMTWF->at(i1)[i2]-baseline[i1]);
				hh->SetBinContent(i2+1,pulseamp);
				if(pulseamp>lmax){lmax=pulseamp;}
				if(i2>0 && i2%1000==0)
				{
					MaxAmplitude[i1]->Fill(lmax);
					lmax=0;
				}
			}
			sbin=0;
			while(sbin<14000)
			{
				minbin=-1;
// 				minbin=FindBinAbove(hh,3*baselineRMS[i1],sbin);
				minbin=FindBinAbove(hh,3.5*baselineRMS[i1],sbin);
				if(minbin==-1) break;
				maxbin=-1;
				for(int ik1=minbin;ik1<=hh->GetNbinsX();ik1++)
				{
					if(hh->GetBinContent(ik1)<0) {maxbin=ik1-1;break;}
				}
				if(maxbin==-1) break;
				for(int ik1=minbin;ik1>=1;ik1--)
				{
					if(hh->GetBinContent(ik1)<0) {minbin=ik1+1;break;}
				}
				if(minbin==2) break;
				qint=0.;
				lmax=0.;
				for(int ik1=minbin;ik1<=maxbin;ik1++)
				{
					qint+=hh->GetBinContent(ik1);
					if(hh->GetBinContent(ik1)>lmax) lmax=hh->GetBinContent(ik1);
				}
				Integral[i1]->Fill(qint);
				PulseWidth[i1]->Fill(maxbin-minbin+1);
				SelAmplitude[i1]->Fill(lmax);
				SelAmplitudeOverRMS[i1]->Fill(lmax/baselineRMS[i1]);
// 				if((lmax/baselineRMS[i1])>4.)
// 				{
// 					IntegralSel[i1]->Fill(qint);
// 					PulseWidthSel[i1]->Fill(maxbin-minbin+1);
// 				}
				sbin=maxbin+1;
			}
		}
		if(I%1000==0) cout<<I<<" / "<<T->GetEntries()<<endl;
	}
	
// 	float maxampth[NPMTs];
// 	for(int i1=0;i1<NPMTs;i1++)
// 	{
// 		maxbin=SelAmplitudeOverRMS[i1]->GetMaximumBin();
// 		for(int is1=maxbin;is1<SelAmplitudeOverRMS[i1]->GetNbinsX();is1++)
// 		{
// 			if(SelAmplitudeOverRMS[i1]->GetBinContent(is1+1)>SelAmplitudeOverRMS[i1]->GetBinContent(is1))
// 			{
// 				sbin=is1;
// 				break;
// 			}
// 		}
// 		
// 		
// // 		maxamp=SelAmplitudeOverRMS[i1]->GetBinContent(maxbin);
// // 		for(int is1=maxbin;is1<=SelAmplitudeOverRMS[i1]->GetNbinsX();is1++)
// // 		{
// // 			if((SelAmplitudeOverRMS[i1]->GetBinContent(is1)/maxamp)<0.01){minbin=is1;break;}
// // 		}
// // 		sbin=minbin;
// // 		for(int is1=minbin;is1<SelAmplitudeOverRMS[i1]->GetNbinsX();is1++)
// // 		{
// // 			if(SelAmplitudeOverRMS[i1]->GetBinContent(is1+1)>SelAmplitudeOverRMS[i1]->GetBinContent(is1))
// // 			{
// // 				sbin=is1;
// // 				break;
// // 			}
// // 		}
// 		maxampth[i1]=SelAmplitudeOverRMS[i1]->GetBinCenter(sbin);
// 		
// // 		g1->SetParameter(0,SelAmplitudeOverRMS[i1]->GetBinContent(SelAmplitudeOverRMS[i1]->GetMaximumBin()));
// // 		g1->SetParameter(1,SelAmplitudeOverRMS[i1]->GetBinCenter(SelAmplitudeOverRMS[i1]->GetMaximumBin()));
// // 		g1->SetParameter(2,0.25);
// // 		SelAmplitudeOverRMS[i1]->Fit(g1,"q","q",SelAmplitudeOverRMS[i1]->GetBinCenter(SelAmplitudeOverRMS[i1]->GetMaximumBin())-1,SelAmplitudeOverRMS[i1]->GetBinCenter(SelAmplitudeOverRMS[i1]->GetMaximumBin())+1);
// 	}
// 	
// 	for(int I=0;I<T->GetEntries();I++)
// 	{
// 		T->GetEntry(I);
// 		if(I<100) continue;
// 		for(int i1=0;i1<NPMTs;i1++)
// 		{
// 			hbl->Reset();
// 			for(int i2=0;i2<1000;i2++)
// 			{
// 				hbl->Fill(ed.PMTWF->at(i1)[i2]);
// 			}
// 			hbl->Fit(g1,"q","q",hbl->GetMean()-3*hbl->GetRMS(),hbl->GetMean()+3*hbl->GetRMS());
// 			baseline[i1]=g1->GetParameter(1);
// 			baselineRMS[i1]=g1->GetParameter(2);
// // 			Baseline[i1]->Fill(baseline[i1]);
// 			hh->Reset();
// 			lmax=0;
// 			for(int i2=0;i2<ed.PMTWF->at(i1).size();i2++)
// 			{
// 				pulseamp=-1.*(ed.PMTWF->at(i1)[i2]-baseline[i1]);
// 				hh->SetBinContent(i2+1,pulseamp);
// // 				if(pulseamp>lmax){lmax=pulseamp;}
// // 				if(i2>0 && i2%1000==0)
// // 				{
// // 					MaxAmplitude[i1]->Fill(lmax);
// // 					lmax=0;
// // 				}
// 			}
// 			sbin=0;
// 			while(sbin<14000)
// 			{
// 				minbin=-1;
// 				minbin=FindBinAbove(hh,maxampth[i1],sbin);
// // 				minbin=FindBinAbove(hh,3*baselineRMS[i1],sbin);
// // 				minbin=FindBinAbove(hh,2*baselineRMS[i1],sbin);
// 				if(minbin==-1) break;
// 				maxbin=-1;
// 				for(int ik1=minbin;ik1<=hh->GetNbinsX();ik1++)
// 				{
// 					if(hh->GetBinContent(ik1)<0) {maxbin=ik1-1;break;}
// 				}
// 				if(maxbin==-1) break;
// 				for(int ik1=minbin;ik1>=1;ik1--)
// 				{
// 					if(hh->GetBinContent(ik1)<0) {minbin=ik1+1;break;}
// 				}
// 				if(minbin==2) break;
// 				qint=0.;
// 				lmax=0.;
// 				for(int ik1=minbin;ik1<=maxbin;ik1++)
// 				{
// 					qint+=hh->GetBinContent(ik1);
// 					if(hh->GetBinContent(ik1)>lmax) lmax=hh->GetBinContent(ik1);
// 				}
// // 				Integral[i1]->Fill(qint);
// // 				PulseWidth[i1]->Fill(maxbin-minbin+1);
// // 				SelAmplitude[i1]->Fill(lmax);
// // 				SelAmplitudeOverRMS[i1]->Fill(lmax/baselineRMS[i1]);
// // 				if((lmax/baselineRMS[i1])>4.)
// // 				{
// 					IntegralSel[i1]->Fill(qint);
// 					PulseWidthSel[i1]->Fill(maxbin-minbin+1);
// // 				}
// 				sbin=maxbin+1;
// 			}
// 		}
// 		if(I%1000==0) cout<<I<<" / "<<T->GetEntries()<<endl;
// 	}
	
	float PMTMeans[4]={36,37,24,12};
	float PMTSigmaF[4]={1.5,1.5,1,1.5};
	if(RunNo>=297 && RunNo<=338){PMTMeans[1]=88;PMTMeans[3]=20;}
	if(RunNo==310){PMTMeans[1]=37;PMTMeans[3]=12;}
	
	outroot->cd();
	for(int i1=0;i1<NPMTs;i1++)
	{
		g1->SetParameter(0,500);
		g1->SetParameter(1,PMTMeans[i1]);
		g1->SetParameter(2,10);
		//The problematic runs first
		if(RunNo==208 && i1==3) Integral[i1]->Fit(g1,"q","q",9,20);
		else if(RunNo==210 && i1==3) Integral[i1]->Fit(g1,"q","q",8,20);
		else if(RunNo==217 && i1==1) Integral[i1]->Fit(g1,"q","q",9,18);
		else if(RunNo==245 && i1==2) Integral[i1]->Fit(g1,"q","q",18,35);
		else if(RunNo==260 && i1==2) Integral[i1]->Fit(g1,"q","q",18,35);
		else if(RunNo==273 && i1==1) Integral[i1]->Fit(g1,"q","q",25,50);
		else if(RunNo==302 && i1==1) Integral[i1]->Fit(g1,"q","q",40,120);
		else if(RunNo==329 && i1==3) Integral[i1]->Fit(g1,"q","q",14,28);
		else
		{
			Integral[i1]->Fit(g1,"q","q",PMTMeans[i1]-10,PMTMeans[i1]+10);
			Integral[i1]->Fit(g1,"q","q",g1->GetParameter(1)-PMTSigmaF[i1]*g1->GetParameter(2),g1->GetParameter(1)+PMTSigmaF[i1]*g1->GetParameter(2));
			Integral[i1]->Fit(g1,"q","q",g1->GetParameter(1)-PMTSigmaF[i1]*g1->GetParameter(2),g1->GetParameter(1)+PMTSigmaF[i1]*g1->GetParameter(2));
		}
		Integral[i1]->Write();
		
// 		g1->SetParameter(0,IntegralSel[i1]->GetBinContent(IntegralSel[i1]->GetMaximumBin()));
// 		g1->SetParameter(1,IntegralSel[i1]->GetBinCenter(IntegralSel[i1]->GetMaximumBin()));
// 		g1->SetParameter(2,10);
// 		IntegralSel[i1]->Fit(g1,"q","q",IntegralSel[i1]->GetBinContent(IntegralSel[i1]->GetMaximumBin())-10,IntegralSel[i1]->GetBinContent(IntegralSel[i1]->GetMaximumBin())+10);
// 		IntegralSel[i1]->Fit(g1,"q","q",g1->GetParameter(1)-1.5*g1->GetParameter(2),g1->GetParameter(1)+1.5*g1->GetParameter(2));
// 		IntegralSel[i1]->Fit(g1,"q","q",g1->GetParameter(1)-1.5*g1->GetParameter(2),g1->GetParameter(1)+1.5*g1->GetParameter(2));
// 		IntegralSel[i1]->Write();
		
		MaxAmplitude[i1]->Write();
		SelAmplitude[i1]->Write();
		SelAmplitudeOverRMS[i1]->Write();
		PulseWidth[i1]->Write();
// 		PulseWidthSel[i1]->Write();
		Baseline[i1]->Write();
	}
	outroot->Close();
	
	sprintf(hname,"cp PMTCalibration4_%d.root %s/Histos/PMTCalibration4_%d.root",RunNo,AnalysisFilePath,RunNo);system(hname);
}