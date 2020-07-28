void HitAnalysis4()
{
	sprintf(hname,"HitAnalysis4_%d.root",RunNo);
	TFile* outroot=new TFile(hname,"recreate");
	
	sprintf(hname,"%s/Run_%d.root",NTupleFilePath,RunNo);
	TFile* inroot=new TFile(hname);
	TTree* T =  (TTree*) inroot->Get("T");
	T->SetBranchAddress("TPCWF",&ed.TPCWF);
	T->SetBranchAddress("PMTWF",&ed.PMTWF);
	
	TH1F* HitTime=new TH1F("HitTime","HitTime",4096,-0.5,4095.5);
	HitTime->GetXaxis()->SetTitle("Hit Time (0.4 #mus)");HitTime->GetXaxis()->CenterTitle();
	TH1F* HitIntegral=new TH1F("HitIntegral","HitIntegral",500,0,3000);
	HitIntegral->GetXaxis()->SetTitle("Hit Integral");HitIntegral->GetXaxis()->CenterTitle();
	TH1F* HitAmplitude=new TH1F("HitAmplitude","HitAmplitude",300,0,300);
	HitAmplitude->GetXaxis()->SetTitle("Hit Amplitude");HitAmplitude->GetXaxis()->CenterTitle();
	TH1F* HitWidth=new TH1F("HitWidth","HitWidth",300,-0.5,299.5);
	HitWidth->GetXaxis()->SetTitle("Hit Width");HitWidth->GetXaxis()->CenterTitle();
	TH1F* HitFWHM=new TH1F("HitFWHM","HitFWHM",300,-0.5,299.5);
	HitFWHM->GetXaxis()->SetTitle("Hit FWHM");HitFWHM->GetXaxis()->CenterTitle();
	TH1F* HitWidth_10percent=new TH1F("HitWidth_10percent","HitWidth_10percent",300,-0.5,299.5);
	HitWidth_10percent->GetXaxis()->SetTitle("Hit Width at 10 % Amplitude");HitWidth_10percent->GetXaxis()->CenterTitle();
	
	TH2F* Amp_vs_Int_2D=new TH2F("Amp_vs_Int_2D","Amp_vs_Int_2D",500,0,3000,200,0,200);
	Amp_vs_Int_2D->GetXaxis()->SetTitle("Hit Integral");Amp_vs_Int_2D->GetXaxis()->CenterTitle();
	Amp_vs_Int_2D->GetYaxis()->SetTitle("Hit Amplitude");Amp_vs_Int_2D->GetYaxis()->CenterTitle();
	TH2F* Int_vs_Width_2D=new TH2F("Int_vs_Width_2D","Int_vs_Width_2D",300,-0.5,299.5,500,0,3000);
	Int_vs_Width_2D->GetXaxis()->SetTitle("Hit FWHM");Int_vs_Width_2D->GetXaxis()->CenterTitle();
	Int_vs_Width_2D->GetYaxis()->SetTitle("Hit Integral");Int_vs_Width_2D->GetYaxis()->CenterTitle();
	TH2F* Int_vs_Width_10percent_2D=new TH2F("Int_vs_Width_10percent_2D","Int_vs_Width_10percent_2D",300,-0.5,299.5,500,0,3000);
	Int_vs_Width_10percent_2D->GetXaxis()->SetTitle("Hit Width at 10 % Amplitude");Int_vs_Width_10percent_2D->GetXaxis()->CenterTitle();
	Int_vs_Width_10percent_2D->GetYaxis()->SetTitle("Hit Integral");Int_vs_Width_10percent_2D->GetYaxis()->CenterTitle();
	
	TH2F* Wire_vs_T=new TH2F("Wire_vs_T","Wire_vs_T",4096,-0.5,4095.5,128,-0.5,127.5);
	
	TH2F* Amp_vs_Int_2D_Ind[128];
	TProfile* Amp_vs_Int_Prof[128];
	for(int i1=0;i1<128;i1++)
	{
		sprintf(hname,"Amp_vs_Int_2D_%d",i1);
		Amp_vs_Int_2D_Ind[i1]=new TH2F(hname,hname,500,0,3000,200,0,200);
		sprintf(hname,"Amp_vs_Int_Prof_%d",i1);
		Amp_vs_Int_Prof[i1]=new TProfile(hname,hname,300,0,3000,0,300);
	}
	
	TProfile* Amp_vs_Int_Profile=new TProfile("Amp_vs_Int_Profile","Amp_vs_Int_Profile",300,0,3000,0,300);
	Amp_vs_Int_Profile->GetXaxis()->SetTitle("Hit Integral");Amp_vs_Int_Profile->GetXaxis()->CenterTitle();
	Amp_vs_Int_Profile->GetYaxis()->SetTitle("Hit Amplitude");Amp_vs_Int_Profile->GetYaxis()->CenterTitle();
	TProfile* Amp_vs_T_Profile=new TProfile("Amp_vs_T_Profile","Amp_vs_T_Profile",300,0,3000,0,4000);
	Amp_vs_T_Profile->GetXaxis()->SetTitle("Hit Time");Amp_vs_T_Profile->GetXaxis()->CenterTitle();
	Amp_vs_T_Profile->GetYaxis()->SetTitle("Hit Amplitude");Amp_vs_T_Profile->GetYaxis()->CenterTitle();
	
	TSpectrum *TS;
	TSpectrum *TS2;
	TS = new TSpectrum();
	TS2 = new TSpectrum(4096);
	
	Double_t *xpeaks;
	int nfoundpeaks;
// 	vector <int> xpeaks;
	vector <int> inCL;
	double x1(0),y1(0),z1(0),t1(0);
	double x2(0),y2(0),z2(0),t2(0);
	int nid=0;
	
	TH1F* hh[128];//bl subtracted WF
	TH1F* hh2[128];//background removed WF
	TH1D* hh3[128];//replica of hh2
// // 	collection wires only
	for(int i2=0;i2<128;i2++)
	{
		sprintf(hname,"WF_%s_%d",wt[0].c_str(),i2);
		hh[i2]=new TH1F(hname,hname,4096,-0.5,4095.5);
		sprintf(hname,"WF2_%s_%d",wt[0].c_str(),i2);
		hh2[i2]=new TH1F(hname,hname,4096,-0.5,4095.5);
	}
	
	int maxbin=0;float maxamp=0;
	int leftbin=0;int rightbin=0;
	int bin1=0;int bin2=0;
	float left_10per=0;float right_10per=0;
	float left_50per=0;float right_50per=0;
	float integral=0;
	int width=0;int width_10percent=0;
	
	TH1F* BLhist=new TH1F("BLhist","BLhist",8000,-4000,4000);
	TF1* fgaus=new TF1("fgaus","gaus",-1000,6000);
	Double_t BLmean=0;
	Double_t BLsigma=0;
	Double_t WFamp=0;
	vector <float> BLmeans;
	vector <float> BLsigmas;
	
	peaks2 pp;
	vector <peaks2> PP;
	int peakbin=0;
	int nhitsabove10=0;
	
	Double_t s[4096];
	Double_t s2[4096];
	
	vector <int> cutbins;
	for(int i2=0;i2<=4096;i2++){cutbins.push_back(0);}
	int minbin=0;
	TGraph* tgr=new TGraph();
	TF1* tflin=new TF1("lin","[0]+[1]*x",0,4500);
	vector <float> xpeaksAll[128];
	
	cout<<T->GetEntries()<<" events"<<endl;
	for(int I=0;I<T->GetEntries();I++)
	{
		T->GetEntry(I);
		
		BLmeans.clear();
		BLsigmas.clear();
		int npeaksTot[128]={0};
		int npeaksAbove10[128]={0};
		vector <float> xpeaksAll[128];
		Wire_vs_T->Reset();
		PP.clear();
		
		for(int i1=0;i1<128;i1++)
		{
			hh[i1]->Reset();
			hh2[i1]->Reset();
// 			hh3[i1]->Reset();
			BLhist->Reset();
			for(int i2=0;i2<500;i2++){BLhist->Fill(ed.TPCWF->at(i1)[i2]);}
// 			cout<<"before BL fit"<<endl;
			BLhist->Fit(fgaus,"q","q",0.,2000.);
// 			cout<<"after BL fit"<<endl;
			BLmean=fgaus->GetParameter(1);
			BLsigma=fgaus->GetParameter(2);
			BLmeans.push_back(BLmean);
			BLsigmas.push_back(BLsigma);
// 			cout<<i1<<" "<<BLmean<<endl;
			for(int i2=0;i2<4096;i2++)
			{
				WFamp=(Double_t)ed.TPCWF->at(i1)[i2]-BLmean;
				hh[i1]->SetBinContent(i2+1,WFamp);
			}
		}
		for(int i1=0;i1<128;i1++)
		{
			xpeaksAll[i1].clear();
			for(int i2=0;i2<4096;i2++){cutbins[i2]=0;}
			for(int i2=0;i2<4096;i2++)
			{
				if(hh[i1]->GetBinContent(i2+1)>5.*BLsigmas[i1])
				{
					leftbin=-1;rightbin=-1;
					for(int is1=i2;is1<4095;is1++)
					{
						if(hh[i1]->GetBinContent(is1+1)>=0 && hh[i1]->GetBinContent(is1+2)<0)
						{
							rightbin=is1+1;
							break;
						}
					}
					for(int is1=i2;is1>=2;is1--)
					{
						if(hh[i1]->GetBinContent(is1+1)>=0 && hh[i1]->GetBinContent(is1)<0)
						{
							leftbin=is1+1;
							break;
						}
					}
					if(leftbin!=-1 && rightbin!=-1)
					{
						for(int is1=leftbin;is1<=rightbin;is1++){cutbins[is1]=1;}
						i2=rightbin;
					}
				}
			}
			for(int i2=0;i2<4096;i2++)
			{
				if(cutbins[i2]==0) s[i2]=hh[i1]->GetBinContent(i2+1);
				else s[i2]=0;
				s2[i2]=s[i2];
			}
// 			TS = new TSpectrum();
			TS->Background(s,4096,20,TSpectrum::kBackDecreasingWindow,TSpectrum::kBackOrder2,kFALSE,TSpectrum::kBackSmoothing3,kTRUE);
			TS->Background(s2,4096,2,TSpectrum::kBackDecreasingWindow,TSpectrum::kBackOrder6,kFALSE,TSpectrum::kBackSmoothing3,kTRUE);
			for(int i2=0;i2<4096;i2++)
			{
				if(cutbins[i2]==0)
				{
					hh2[i1]->SetBinContent(i2+1,hh[i1]->GetBinContent(i2+1)-s2[i2]);
					Wire_vs_T->SetBinContent(i2+1,i1+1,hh[i1]->GetBinContent(i2+1)-s2[i2]);
				}
				else
				{
					hh2[i1]->SetBinContent(i2+1,hh[i1]->GetBinContent(i2+1)-s[i2]);
					Wire_vs_T->SetBinContent(i2+1,i1+1,hh[i1]->GetBinContent(i2+1)-s[i2]);
				}
			}
			
// 			TS2 = new TSpectrum(4096);
			nfoundpeaks = TS2->Search(hh2[i1],3,"nobackground",0.05);
			xpeaks = TS2->GetPositionX();
			nhitsabove10=0;
			for(int is1=0;is1<nfoundpeaks;is1++)
			{
				HitTime->Fill(xpeaks[is1]);
				if(hh2[i1]->GetBinContent(hh2[i1]->GetXaxis()->FindBin(xpeaks[is1]))>=10){nhitsabove10++;}
				xpeaksAll[i1].push_back(xpeaks[is1]);
			}
			npeaksAbove10[i1]=nhitsabove10;
			npeaksTot[i1]=nfoundpeaks;
		}
		bool isMIP=true;
		nhitsabove10=0;
		int nww1peak=0;
		int nwwmorepeaks=0;
		for(int i1=0;i1<128;i1++)
		{
			if(npeaksAbove10[i1]!=0){nhitsabove10++;}
		}
		for(int i1=0;i1<128;i1++)
		{
			if(npeaksAbove10[i1]==1) nww1peak++;
			if(npeaksAbove10[i1]>1) nwwmorepeaks++;
// 			if(npeaksAbove10[i1]>1){isMIP=false;break;}
		}
		if(nhitsabove10<10) isMIP=false;
		if((((float)nww1peak)/((float)(nww1peak+nwwmorepeaks)))<0.8) isMIP=false;
		
// 		cout<<I<<" "<<nww1peak<<" "<<nwwmorepeaks<<" "<<(((float)nww1peak)/((float)(nww1peak+nwwmorepeaks)))<<" "<<isMIP<<endl;
		
		if(isMIP)
		{
			for(int i1=0;i1<128;i1++)
			{
				sprintf(hname,"WF1_%d",i1);hh3[i1]=Wire_vs_T->ProjectionX(hname,i1+1,i1+1,"");
				for(int is1=0;is1<xpeaksAll[i1].size();is1++)
				{
					if(hh3[i1]->GetBinContent(hh3[i1]->FindBin(xpeaksAll[i1][is1]))<10) continue;
					
					pp.w=i1;
					peakbin=hh3[i1]->GetXaxis()->FindBin(xpeaksAll[i1][is1]);
// 					cout<<peakbin<<" "<<hh3[i1]->Integral()<<endl;
// 					cout<<"before peak fit"<<endl;
					hh3[i1]->Fit(fgaus,"q","q",hh3[i1]->GetBinCenter(peakbin-2),hh3[i1]->GetBinCenter(peakbin+2));
// 					cout<<"after peak fit"<<endl;
	// 				pp.amplitude=hh2[i1]->GetBinContent(peakbin);
					pp.amplitude=fgaus->GetParameter(0);
// 					cout<<"amplitude "<<pp.amplitude<<endl;
					
// 					if(pp.amplitude<10) continue;
					
					pp.t=fgaus->GetParameter(1);
					leftbin=-1;rightbin=-1;
					for(int i3=peakbin;i3<4096;i3++)
					{
						if(hh3[i1]->GetBinContent(i3)<=0.1){rightbin=i3;break;}
					}
					for(int i3=peakbin;i3>0;i3--)
					{
						if(hh3[i1]->GetBinContent(i3)<=0.1){leftbin=i3;break;}
					}
// 					if(leftbin==-1) leftbin=1;
// 					if(rightbin==-1) rightbin=4096;
					if(leftbin!=-1 && rightbin!=-1)
					{
						pp.integral=0;
						for(int ik1=leftbin;ik1<=rightbin;ik1++)
						{
							if(hh3[i1]->GetBinContent(ik1)<(pp.amplitude/10.) && hh3[i1]->GetBinContent(ik1+1)>=(pp.amplitude/10.))
							{
								bin1=ik1;bin2=ik1+1;
								left_10per=hh3[i1]->GetBinCenter(bin1)+(hh3[i1]->GetBinCenter(bin2)-hh3[i1]->GetBinCenter(bin1))*((pp.amplitude/10.)-hh3[i1]->GetBinContent(bin1))/(hh3[i1]->GetBinContent(bin2)-hh3[i1]->GetBinContent(bin1));
							}
							if(hh3[i1]->GetBinContent(ik1)<(pp.amplitude/2.) && hh3[i1]->GetBinContent(ik1+1)>=(pp.amplitude/2.))
							{
								bin1=ik1;bin2=ik1+1;
								left_50per=hh3[i1]->GetBinCenter(bin1)+(hh3[i1]->GetBinCenter(bin2)-hh3[i1]->GetBinCenter(bin1))*((pp.amplitude/2.)-hh3[i1]->GetBinContent(bin1))/(hh3[i1]->GetBinContent(bin2)-hh3[i1]->GetBinContent(bin1));
							}
							if(hh3[i1]->GetBinContent(ik1)>=(pp.amplitude/10.) && hh3[i1]->GetBinContent(ik1+1)<(pp.amplitude/10.))
							{
								bin1=ik1+1;bin2=ik1;
								right_10per=hh3[i1]->GetBinCenter(bin1)+(hh3[i1]->GetBinCenter(bin2)-hh3[i1]->GetBinCenter(bin1))*((pp.amplitude/10.)-hh3[i1]->GetBinContent(bin1))/(hh3[i1]->GetBinContent(bin2)-hh3[i1]->GetBinContent(bin1));
							}
							if(hh3[i1]->GetBinContent(ik1)>=(pp.amplitude/2.) && hh3[i1]->GetBinContent(ik1+1)<(pp.amplitude/2.))
							{
								bin1=ik1+1;bin2=ik1;
								right_50per=hh3[i1]->GetBinCenter(bin1)+(hh3[i1]->GetBinCenter(bin2)-hh3[i1]->GetBinCenter(bin1))*((pp.amplitude/2.)-hh3[i1]->GetBinContent(bin1))/(hh3[i1]->GetBinContent(bin2)-hh3[i1]->GetBinContent(bin1));
							}
							pp.integral+=hh3[i1]->GetBinContent(ik1);
						}
					}
					pp.fwhm=right_50per-left_50per;
					pp.w10perc=right_10per-left_10per;
					pp.wfull=hh3[i1]->GetBinCenter(rightbin)-hh3[i1]->GetBinCenter(leftbin);
					PP.push_back(pp);
				}
			}
			for(int is1=0;is1<PP.size();is1++)
			{
				HitIntegral->Fill(PP[is1].integral);
				HitAmplitude->Fill(PP[is1].amplitude);
				HitWidth->Fill(PP[is1].wfull);
				HitWidth_10percent->Fill(PP[is1].w10perc);
				HitFWHM->Fill(PP[is1].fwhm);
				Amp_vs_Int_2D->Fill(PP[is1].integral,PP[is1].amplitude);
				Int_vs_Width_2D->Fill(PP[is1].fwhm,PP[is1].integral);
				Int_vs_Width_10percent_2D->Fill(PP[is1].w10perc,PP[is1].integral);
				Amp_vs_Int_Profile->Fill(PP[is1].integral,PP[is1].amplitude);
				Amp_vs_T_Profile->Fill(PP[is1].t,PP[is1].amplitude);
				Amp_vs_Int_2D_Ind[PP[is1].w]->Fill(PP[is1].integral,PP[is1].amplitude);
				Amp_vs_Int_Prof[PP[is1].w]->Fill(PP[is1].integral,PP[is1].amplitude);
			}
		}
		if(I<50)
		{
			outroot->cd();
			sprintf(hname,"Wire_vs_T_%d",I);
			Wire_vs_T->SetName(hname);
			Wire_vs_T->SetMinimum(-20);Wire_vs_T->SetMaximum(20);
			Wire_vs_T->Write();
		}
		if(I%100==0) cout<<"Event: "<<I<<endl;
// 		cout<<"Event: "<<I<<endl;
// 		if(I>50) break;
	}
	
	outroot->cd();
	
	HitTime->Write();
	HitIntegral->Write();
	HitAmplitude->Write();
	HitWidth->Write();
	HitWidth_10percent->Write();
	HitFWHM->Write();
	
	Amp_vs_Int_2D->Write();
	Int_vs_Width_2D->Write();
	Int_vs_Width_10percent_2D->Write();
	
	Amp_vs_Int_Profile->Write();
	Amp_vs_T_Profile->Write();
	
	for(int i1=0;i1<128;i1++)
	{
		Amp_vs_Int_2D_Ind[i1]->Write();
	}
	for(int i1=0;i1<128;i1++)
	{
		Amp_vs_Int_Prof[i1]->Write();
	}
	
	inroot->Close();
	
	sprintf(hname,"mv HitAnalysis4_%d.root %s/Histos/;wait;",RunNo,AnalysisFilePath);system(hname);
}