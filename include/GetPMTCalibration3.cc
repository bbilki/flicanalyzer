void GetPMTCalibration3()
{
	sprintf(hname,"%s/Run_%d.root",NTupleFilePath,RunNo);
	TFile* inroot=new TFile(hname);
	TTree* T =  (TTree*) inroot->Get("T");
	T->SetBranchAddress("TPCWF",&ed.TPCWF);
	T->SetBranchAddress("PMTWF",&ed.PMTWF);
	
	ReadPMTBaselines();
	
	sprintf(hname,"PMTCalibration3_%d.root",RunNo);
	TFile* outroot=new TFile(hname,"recreate");
	
// 	string PMTNames[3]={"LAr w/TPB","LAr no TPB","LXe"};
	
	TH1F* Integral_30[3];TH2F* Integral_vs_width[3];TH1F* Integral_w[3][30];
	TH1F* MaxAmplitude[3];TH1F* Integral_MAgt2[3];TH1F* Integral_MAgt2_wgt10[3];
	for(int i1=0;i1<3;i1++)
	{
// 		sprintf(hname,"Integral_30_%s",PMTNames[i1].c_str());
		sprintf(hname,"Integral_30_%d",i1);
		Integral_30[i1]=new TH1F(hname,hname,200,0,200);
		sprintf(hname,"MaxAmplitude_%d",i1);
		MaxAmplitude[i1]=new TH1F(hname,hname,200,0,200);
		sprintf(hname,"Integral_MAgt2_%d",i1);
		Integral_MAgt2[i1]=new TH1F(hname,hname,200,0,200);
		sprintf(hname,"Integral_MAgt2_wgt10_%d",i1);
		Integral_MAgt2_wgt10[i1]=new TH1F(hname,hname,200,0,200);
// 		sprintf(hname,"Integral_vs_width_%s",PMTNames[i1].c_str());
		sprintf(hname,"Integral_vs_width_%d",i1);
		Integral_vs_width[i1]=new TH2F(hname,hname,50,0,50,200,0,200);
		for(int i2=0;i2<30;i2++)
		{
// 			sprintf(hname,"Integral_w_%s_%d",PMTNames[i1].c_str(),i2);
			sprintf(hname,"Integral_w_%d_%d",i1,i2);
			Integral_w[i1][i2]=new TH1F(hname,hname,200,0,200);
		}
	}
	
	TH1F* hh=new TH1F("PMTWF","PMTWF",15000,-0.5,14999.5);
	
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
	
	vector <float> Amp;
	float hsum[20]={0.};
	float hped=0;
	float hped2=0;float hsig2=0;
	float xlim=0;int fitstartbin=0;int xlimbin=0;
	float qint=0;int sbin=0;int minbin=0;int maxbin=0;int peakbin=0;
	float maxamp=0;int nc=0;
	
	for(int I=0;I<T->GetEntries();I++)
	{
		T->GetEntry(I);
		for(int i1=0;i1<3;i1++)
		{
			for(int i2=0;i2<ed.PMTWF->at(i1).size();i2++)
			{
				hh->SetBinContent(i2+1,-1.*(ed.PMTWF->at(i1)[i2]-PMTBaselines[i1][0]));
			}
			if(hh->GetNbinsX()==0) continue;
// 			if(I<10)
// 			{
				sprintf(hname,"PMTWF_%d_%d",i1,I);
				hh->SetName(hname);hh->SetTitle(hname);
				
				for(int ik1=hh->GetMaximumBin();ik1<hh->GetNbinsX();ik1++)
				{
					if(hh->GetBinContent(ik1)>0.1*hh->GetBinContent(hh->GetMaximumBin()))
// 					if(hh->GetBinContent(ik1)>10)
					{
						fitstartbin=ik1;
					}
					else break;
				}
				
				hh->Fit(tf1,"q","q",hh->GetBinCenter(fitstartbin),10000.);
				xlim=tf1->GetX(1,0,15000);
				xlimbin=hh->FindBin(xlim);
				
				sbin=xlimbin;
// 				cout<<I<<" "<<i1<<" "<<sbin<<endl;
				while(sbin<10000)
				{
					nc=0;
					for(int ik1=sbin;ik1<hh->GetNbinsX();ik1++)
					{
						if(hh->GetBinContent(ik1+1)>0 && hh->GetBinContent(ik1)<0)
						{
							nc++;
							minbin=ik1+1;
							for(int ik2=ik1+1;ik2<hh->GetNbinsX();ik2++)
							{
								if(hh->GetBinContent(ik2+1)<0 && hh->GetBinContent(ik2)>0)
								{
									maxbin=ik2;
									break;
								}
							}
							qint=0;maxamp=0;
							for(int ik2=minbin;ik2<=maxbin;ik2++)
							{
								qint+=hh->GetBinContent(ik2);
								if(hh->GetBinContent(ik2)>maxamp) maxamp=hh->GetBinContent(ik2);
							}
							MaxAmplitude[i1]->Fill(maxamp);
							if(maxamp>2) Integral_MAgt2[i1]->Fill(qint);
							if(maxamp>2 && (maxbin-minbin+1)>10) Integral_MAgt2_wgt10[i1]->Fill(qint);
							if((maxbin-minbin+1)>=5)
							{
								Integral_vs_width[i1]->Fill(maxbin-minbin+1,qint);
								for(int ik3=0;ik3<30;ik3++)
								{
									if((maxbin-minbin+1)>=(ik3+5))
									{
										Integral_w[i1][ik3]->Fill(qint);
									}
								}
							}
	// 						if(i1==2) cout<<I<<" "<<i1<<" "<<(maxbin-minbin+1)<<endl;
// 							nc=0;
							sbin=maxbin+1;
							break;
						}
// 						if(sbin>10000) break;
					}
					if(nc==0) break;
				}
// 				for(int ik1=sbin;ik1<hh->GetNbinsX();ik1++)
// 				{
// 					if(hh->GetBinContent(ik1+1)>0 && hh->GetBinContent(ik1)<0)
// 					{
// 						minbin=ik1;
// 						for(int ik2=ik1;ik2<hh->GetNbinsX();ik2++)
// 						{
// 							if(hh->GetBinContent(ik2+1)<0 && hh->GetBinContent(ik2)>0)
// 							{
// 								maxbin=ik2;
// 								break;
// 							}
// 						}
// 						qint=0;maxamp=0;
// 						for(int ik2=minbin;ik2<=maxbin;ik2++)
// 						{
// 							qint+=hh->GetBinContent(ik2);
// 							if(hh->GetBinContent(ik2)>maxamp) maxamp=hh->GetBinContent(ik2);
// 						}
// 						if(maxamp>2) Integral_MAgt2[i1]->Fill(qint);
// 						if(maxamp>2 && (maxbin-minbin+1)>10) Integral_MAgt2_wgt10[i1]->Fill(qint);
// 						if((maxbin-minbin+1)>=5)
// 						{
// 							Integral_vs_width[i1]->Fill(maxbin-minbin+1,qint);
// 							for(int ik3=0;ik3<30;ik3++)
// 							{
// 								if((maxbin-minbin+1)>=(ik3+5))
// 								{
// 									Integral_w[i1][ik3]->Fill(qint);
// 								}
// 							}
// 						}
// // 						if(i1==2) cout<<I<<" "<<i1<<" "<<(maxbin-minbin+1)<<endl;
// 					}
// 					sbin=maxbin+1;
// 					if(sbin>10000) break;
// 				}
// 				cout<<I<<" "<<i1<<" "<<xlim<<endl;
				for(int ik1=xlimbin;ik1<hh->GetNbinsX();ik1+=30)
				{
					qint=0;
					for(int ik2=ik1;ik2<(ik1+30);ik2++)
					{
						qint+=hh->GetBinContent(ik2);
					}
					Integral_30[i1]->Fill(qint);
				}
				
				
// 				hh->Write();
// 			}
			hh->Reset();
		}
		if(I%1000==0) cout<<I<<" / "<<T->GetEntries()<<endl;
// 		if(I>1000) break;
	}
	
	outroot->cd();
	for(int i1=0;i1<3;i1++)
	{
		Integral_30[i1]->Write();
		Integral_vs_width[i1]->Write();
		MaxAmplitude[i1]->Write();
		Integral_MAgt2[i1]->Write();
		Integral_MAgt2_wgt10[i1]->Write();
		for(int i2=0;i2<30;i2++)
		{
			Integral_w[i1][i2]->Write();
		}
	}
	outroot->Close();
	
	sprintf(hname,"cp PMTCalibration3_%d.root %s/Histos/PMTCalibration3_%d.root",RunNo,AnalysisFilePath,RunNo);system(hname);
}