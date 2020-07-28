void TPCCalibration()
{
	sprintf(hname,"TPCCalibration.root");
	TFile* outroot=new TFile(hname,"recreate");
	
	ReadTPCBaselines();
	
	int RunNos[2]={7384,7383};//coll ind
	
	TH1F* NegtoPosTime=new TH1F("NegtoPosTime","NegtoPosTime",20,0,20);
	NegtoPosTime->GetXaxis()->SetTitle("Time (#mus)");NegtoPosTime->GetXaxis()->CenterTitle();
	TH1F* PeakToPeakTime=new TH1F("PeakToPeakTime","PeakToPeakTime",1000,0,400);
	PeakToPeakTime->GetXaxis()->SetTitle("Time (#mus)");PeakToPeakTime->GetXaxis()->CenterTitle();
	TH1F* NegTime=new TH1F("NegTime","NegTime",200,0,20);
	TH1F* PosTime=new TH1F("PosTime","PosTime",200,0,20);
	TH1F* NegPulseArea[2][128];
	TH1F* PosPulseArea[2][128];
	TH1F* TotalPulseArea[2][128];
	TH1F* CentralPulseArea[2][128];
	TH1F* NegToPosAmp[2][128];
	TH1F* NegPeakAmp[2][128];
	TH1F* PosPeakAmp[2][128];
	TH1F* fCperADC[2][128];
	for(int i1=0;i1<2;i1++)
	{
		for(int i2=0;i2<128;i2++)
		{
			sprintf(hname,"NegPulseArea_%s_%d",wt[i1].c_str(),i2);
			NegPulseArea[i1][i2]=new TH1F(hname,hname,200,0,2000);
			sprintf(hname,"PosPulseArea_%s_%d",wt[i1].c_str(),i2);
			PosPulseArea[i1][i2]=new TH1F(hname,hname,200,0,2000);
			sprintf(hname,"TotalPulseArea_%s_%d",wt[i1].c_str(),i2);
			TotalPulseArea[i1][i2]=new TH1F(hname,hname,200,0,2000);
			sprintf(hname,"NegToPosAmp_%s_%d",wt[i1].c_str(),i2);
			NegToPosAmp[i1][i2]=new TH1F(hname,hname,500,0,500);
			NegToPosAmp[i1][i2]->GetXaxis()->SetTitle("Amplitude (ADC counts)");NegToPosAmp[i1][i2]->GetXaxis()->CenterTitle();
			sprintf(hname,"CentralPulseArea_%s_%d",wt[i1].c_str(),i2);
			CentralPulseArea[i1][i2]=new TH1F(hname,hname,200,0,2000);
			sprintf(hname,"NegPeakAmp_%s_%d",wt[i1].c_str(),i2);
			NegPeakAmp[i1][i2]=new TH1F(hname,hname,200,0,2000);
			NegPeakAmp[i1][i2]->GetXaxis()->SetTitle("Amplitude (ADC counts)");NegPeakAmp[i1][i2]->GetXaxis()->CenterTitle();
			sprintf(hname,"PosPeakAmp_%s_%d",wt[i1].c_str(),i2);
			PosPeakAmp[i1][i2]=new TH1F(hname,hname,200,0,2000);
			PosPeakAmp[i1][i2]->GetXaxis()->SetTitle("Amplitude (ADC counts)");PosPeakAmp[i1][i2]->GetXaxis()->CenterTitle();
			sprintf(hname,"fCperADC_%s_%d",wt[i1].c_str(),i2);
			fCperADC[i1][i2]=new TH1F(hname,hname,1000,0,0.1);
			fCperADC[i1][i2]->GetXaxis()->SetTitle("fC/ADC");fCperADC[i1][i2]->GetXaxis()->CenterTitle();
		}
	}
	
	TH1F* PeakAmp[2];
	TH1D* AllAmp[2];
	TH1F* NegAreaPerAmp[2];
	
	PeakAmp[0]=new TH1F("ColPeakAmp","ColPeakAmp",400,0,200);
	PeakAmp[1]=new TH1F("IndPeakAmp","IndPeakAmp",400,0,200);
	
	AllAmp[0]=new TH1D("ColAllAmp","ColAllAmp",800,-200,200);
	AllAmp[1]=new TH1D("IndAllAmp","IndAllAmp",800,-200,200);
	
	NegAreaPerAmp[0]=new TH1F("NegAreaPerAmp_Col","NegAreaPerAmp_Col",400,0,100);
	NegAreaPerAmp[1]=new TH1F("NegAreaPerAmp_Ind","NegAreaPerAmp_Ind",400,0,100);
	
	TH1F* hh[2][128];
	float QPos=0;float QNeg=0;float QTot=0;float QCenter=0;
	float tleft=0;float tcenter=0;float tright=0;float tpospeak=0;float tnegpeak=0;
	int binleft=0;int bincenter=0;int binright=0;int binpospeak=0;int binnegpeak=0;
	float PPeak=0;float NPeak=0;
	vector <float> tPosPeaks;
	int startbin=0;
	float ss=0;
	int ic=0;int gch=0;
	
	for(int i1=0;i1<2;i1++)
	{
		for(int i2=0;i2<128;i2++)
		{
			sprintf(hname,"WF_%s_%d",wt[i1].c_str(),i2);
			hh[i1][i2]=new TH1F(hname,hname,4096,-0.5,4095.5);
		}
	}
	
	int peakID=0;
	int cbin=0;
	int peakbin=0;
	float th=20;
	TFile* inroot;
	TTree* T;
	for(int J=0;J<2;J++)
	{
		RunNo=RunNos[J];
		sprintf(hname,"%s/Run_%d.root",NTupleFilePath,RunNo);
		inroot=new TFile(hname);
		T =  (TTree*) inroot->Get("T");
		T->SetBranchAddress("TPCWF",&ed.TPCWF);
		T->SetBranchAddress("PMTWF",&ed.PMTWF);
		
		for(int I=0;I<T->GetEntries();I++)
		{
			T->GetEntry(I);
			if(I%100==0) cout<<I<<endl;
			for(int i1=0;i1<256;i1++)
			{
				ic=i1/128;gch=i1-((i1/128)*128);
				for(int i2=0;i2<4096;i2++)
				{
					ss=((float)ed.TPCWF->at(i1)[i2])-TPCBaselines[ic][gch][0];
					
					AllAmp[J]->Fill(ss);
					
					hh[ic][gch]->SetBinContent(hh[ic][gch]->FindBin(i2),ss);
				}
			}
// 			for(int i1=0;i1<2;i1++)
			{
				int i1=J;
				for(int i2=0;i2<128;i2++)
				{
					startbin=FindBinAbove(hh[i1][i2],th,100);
					peakID=1;
					while(startbin>=0)
					{
// 						cout<<i1<<" "<<i2<<" pos "<<startbin<<endl;
						for(int ik1=startbin;ik1<hh[i1][i2]->GetNbinsX();ik1++)
						{
							if(hh[i1][i2]->GetBinContent(ik1+1)<hh[i1][i2]->GetBinContent(ik1))
							{
								PPeak=hh[i1][i2]->GetBinContent(ik1);
								tpospeak=hh[i1][i2]->GetBinCenter(ik1);
								PeakAmp[i1]->Fill(PPeak);
								binpospeak=ik1;
								break;
							}
						}
						for(int ik1=binpospeak;ik1<=hh[i1][i2]->GetNbinsX();ik1++)
						{
							if(hh[i1][i2]->GetBinContent(ik1+1)>hh[i1][i2]->GetBinContent(ik1))
							{
								tright=hh[i1][i2]->GetBinCenter(ik1);
								binright=ik1;
								break;
							}
						}
						for(int ik1=binpospeak;ik1>=1;ik1--)
						{
							if(hh[i1][i2]->GetBinContent(ik1)<0)
							{
								bincenter=ik1;
								tcenter=hh[i1][i2]->GetBinCenter(ik1)-hh[i1][i2]->GetBinContent(ik1)*(hh[i1][i2]->GetBinCenter(ik1-1)-hh[i1][i2]->GetBinCenter(ik1))/(hh[i1][i2]->GetBinContent(ik1-1)-hh[i1][i2]->GetBinContent(ik1));
								break;
							}
						}
						for(int ik1=bincenter;ik1>=1;ik1--)
						{
							if(hh[i1][i2]->GetBinContent(ik1-1)>hh[i1][i2]->GetBinContent(ik1))
							{
								tnegpeak=hh[i1][i2]->GetBinCenter(ik1);
								NPeak=hh[i1][i2]->GetBinContent(ik1);
								binnegpeak=ik1;
								break;
							}
						}
						for(int ik1=binnegpeak;ik1>=1;ik1--)
						{
							if(hh[i1][i2]->GetBinContent(ik1-1)<hh[i1][i2]->GetBinContent(ik1))
							{
								tleft=hh[i1][i2]->GetBinCenter(ik1);
								binleft=ik1;
								break;
							}
						}
						
						NegtoPosTime->Fill((tpospeak-tnegpeak)*0.4);
						tPosPeaks.push_back(tpospeak*0.4);
// 						if(tPosPeaks.size()>2)
// 						{
// 							if((tPosPeaks[tPosPeaks.size()-1]-tPosPeaks[tPosPeaks.size()-2])<98 || (tPosPeaks[tPosPeaks.size()-1]-tPosPeaks[tPosPeaks.size()-2])>102)
// 							{
// 								cout<<RunNo<<" "<<I<<" "<<i1<<" "<<i2<<" "<<peakID<<" "<<((tpospeak-tnegpeak)*0.4)<<" "<<tpospeak<<" "<<tnegpeak<<" "<<tleft<<" "<<tcenter<<" "<<tright<<" "<<tPosPeaks[tPosPeaks.size()-1]<<" "<<tPosPeaks[tPosPeaks.size()-2]<<endl;
// 							}
// 						}
						
						NegTime->Fill((tcenter-tleft)*0.4);
						PosTime->Fill((tright-tcenter)*0.4);
						NegToPosAmp[i1][i2]->Fill(PPeak-NPeak);
						NegPeakAmp[i1][i2]->Fill(-1.*NPeak);
						PosPeakAmp[i1][i2]->Fill(PPeak);
						
						QPos=0;QNeg=0;QTot=0;QCenter=0;
						for(int ik1=binleft;ik1<=binright;ik1++)
						{
							if(ik1<=bincenter){QNeg+=(-1.*hh[i1][i2]->GetBinContent(ik1));}
							else {QPos+=hh[i1][i2]->GetBinContent(ik1);}
							if(ik1>=binnegpeak && ik1<=binpospeak){QCenter+=fabs(hh[i1][i2]->GetBinContent(ik1));}
						}
						QTot=QPos+QNeg;
						NegPulseArea[i1][i2]->Fill(QNeg);
						PosPulseArea[i1][i2]->Fill(QPos);
						TotalPulseArea[i1][i2]->Fill(QTot);
						CentralPulseArea[i1][i2]->Fill(QCenter);
						
						NegAreaPerAmp[i1]->Fill(QNeg/(-1.*NPeak));
						fCperADC[i1][i2]->Fill(10./1.88/QNeg);
						
						startbin=FindBinAbove(hh[i1][i2],th,binright+1);
						if(startbin>4000) break;
						peakID++;
					}
					for(int ik1=1;ik1<tPosPeaks.size();ik1++)
					{
						PeakToPeakTime->Fill(tPosPeaks[ik1]-tPosPeaks[ik1-1]);
					}
					tPosPeaks.clear();
				}
			}
			for(int i1=0;i1<2;i1++)
			{
				for(int i2=0;i2<128;i2++)
				{
					hh[i1][i2]->Reset();
				}
			}
		}
		inroot->Close();
	}
	
	TF1* tf1=new TF1("gaus","gaus",0.,2000.);
	NegtoPosTime->Fit(tf1,"q","q",0.,10.);
	PeakToPeakTime->Fit(tf1,"q","q",0.,200.);
	PeakAmp[0]->Fit(tf1,"q","q",0.,100.);
	PeakAmp[1]->Fit(tf1,"q","q",0.,100.);
	
	TGraphErrors* tg[5][2];
	tg[0][0]=new TGraphErrors();
	tg[0][0]->SetName("NegInt_Col");tg[0][0]->SetTitle("NegInt_Col");
	tg[0][0]->SetMarkerStyle(20);tg[0][0]->SetMarkerColor(1);
	tg[0][1]=new TGraphErrors();
	tg[0][1]->SetName("NegInt_Ind");tg[0][1]->SetTitle("NegInt_Ind");
	tg[0][1]->SetMarkerStyle(21);tg[0][1]->SetMarkerColor(2);
	tg[1][0]=new TGraphErrors();
	tg[1][0]->SetName("NegAmp_Col");tg[1][0]->SetTitle("NegAmp_Col");
	tg[1][0]->SetMarkerStyle(22);tg[1][0]->SetMarkerColor(3);
	tg[1][1]=new TGraphErrors();
	tg[1][1]->SetName("NegAmp_Ind");tg[1][1]->SetTitle("NegAmp_Ind");
	tg[1][1]->SetMarkerStyle(23);tg[1][1]->SetMarkerColor(4);
	tg[2][0]=new TGraphErrors();
	tg[2][0]->SetName("NegToPosAmp_Col");tg[1][0]->SetTitle("NegToPosAmp_Col");
	tg[2][0]->SetMarkerStyle(22);tg[1][0]->SetMarkerColor(3);
	tg[2][1]=new TGraphErrors();
	tg[2][1]->SetName("NegToPosAmp_Ind");tg[1][1]->SetTitle("NegToPosAmp_Ind");
	tg[2][1]->SetMarkerStyle(23);tg[1][1]->SetMarkerColor(4);
	tg[3][0]=new TGraphErrors();
	tg[3][0]->SetName("fCperADC_Col");tg[3][0]->SetTitle("fCperADC_Col");
	tg[3][0]->SetMarkerStyle(20);tg[3][0]->SetMarkerColor(1);
	tg[3][1]=new TGraphErrors();
	tg[3][1]->SetName("fCperADC_Ind");tg[3][1]->SetTitle("fCperADC_Ind");
	tg[3][1]->SetMarkerStyle(21);tg[3][1]->SetMarkerColor(2);
	
	tg[4][0]=new TGraphErrors();
	tg[4][0]->SetName("fCperADC_Col_Run1");tg[4][0]->SetTitle("fCperADC_Col_Run1");
	tg[4][0]->SetMarkerStyle(22);tg[3][0]->SetMarkerColor(3);
	tg[4][1]=new TGraphErrors();
	tg[4][1]->SetName("fCperADC_Ind_Run1");tg[4][1]->SetTitle("fCperADC_Ind_Run1");
	tg[4][1]->SetMarkerStyle(23);tg[4][1]->SetMarkerColor(4);
	
	int WireMap[2][128]={{0}};//boards were swapped from Run1 to Run2, Run3 was the same as Run2, mapping done with CompareTPCBaselines
	for(int i2=0;i2<2;i2++)
	{
		for(int i1=0;i1<64;i1++){WireMap[i2][i1]=i1+64;}
		for(int i1=64;i1<128;i1++){WireMap[i2][i1]=i1-64;}
	}
	
	ofstream outfile("TPCCalibration_Run3.txt");
	ofstream outfile1("TPCCalibration_Run1.txt");
	
	for(int i1=0;i1<2;i1++)
	{
		for(int i2=0;i2<128;i2++)
		{
			NegPulseArea[i1][i2]->Fit(tf1,"q","q",0.,1000.);
			tg[0][i1]->SetPoint(tg[0][i1]->GetN(),i2,tf1->GetParameter(1));
			tg[0][i1]->SetPointError(tg[0][i1]->GetN()-1,0,tf1->GetParError(1));
			
			NegPeakAmp[i1][i2]->Fit(tf1,"q","q",0.,200.);
			tg[1][i1]->SetPoint(tg[1][i1]->GetN(),i2,tf1->GetParameter(1));
			tg[1][i1]->SetPointError(tg[1][i1]->GetN()-1,0,tf1->GetParError(1));
			
			NegToPosAmp[i1][i2]->Fit(tf1,"q","q",0.,200.);
			tg[2][i1]->SetPoint(tg[2][i1]->GetN(),i2,tf1->GetParameter(1));
			tg[2][i1]->SetPointError(tg[2][i1]->GetN()-1,0,tf1->GetParError(1));
			
			fCperADC[i1][i2]->Fit(tf1,"q","q",0.,0.1);
			fCperADC[i1][i2]->Fit(tf1,"q","q",tf1->GetParameter(1)-1.5*tf1->GetParameter(2),tf1->GetParameter(1)+1.5*tf1->GetParameter(2));
			tg[3][i1]->SetPoint(tg[3][i1]->GetN(),i2,tf1->GetParameter(1));
			tg[3][i1]->SetPointError(tg[3][i1]->GetN()-1,0,tf1->GetParError(1));
			
			tg[4][i1]->SetPoint(tg[4][i1]->GetN(),WireMap[i1][i2],tf1->GetParameter(1));
			tg[4][i1]->SetPointError(tg[4][i1]->GetN()-1,0,tf1->GetParError(1));
			
			outfile<<i1<<" "<<i2<<" "<<tf1->GetParameter(1)<<" "<<tf1->GetParError(1)<<endl;
			
			outfile1<<i1<<" "<<WireMap[i1][i2]<<" "<<tf1->GetParameter(1)<<" "<<tf1->GetParError(1)<<endl;
		}
	}
	outfile.close();
	sprintf(hname,"cp TPCCalibration_Run3.txt TPCCalibration_Run2.txt;wait;");system(hname);
	sprintf(hname,"mv TPCCalibration_Run3.txt %s/Files/;wait;",AnalysisFilePath);system(hname);
	sprintf(hname,"mv TPCCalibration_Run2.txt %s/Files/;wait;",AnalysisFilePath);system(hname);
	sprintf(hname,"mv TPCCalibration_Run1.txt %s/Files/;wait;",AnalysisFilePath);system(hname);
	
	outroot->cd();
	NegtoPosTime->Write();
	NegTime->Write();
	PosTime->Write();
	PeakToPeakTime->Write();
	for(int i1=0;i1<5;i1++)
	{
		for(int i2=0;i2<2;i2++)
		{
			tg[i1][i2]->Write();
		}
	}
	for(int i1=0;i1<2;i1++){AllAmp[i1]->Write();}
	for(int i1=0;i1<2;i1++){PeakAmp[i1]->Write();}
	for(int i1=0;i1<2;i1++){NegAreaPerAmp[i1]->Write();}
	
	for(int i1=0;i1<2;i1++)
	{
		for(int i2=0;i2<128;i2++)
		{
			NegToPosAmp[i1][i2]->Write();
			NegPulseArea[i1][i2]->Write();
			PosPulseArea[i1][i2]->Write();
			TotalPulseArea[i1][i2]->Write();
			CentralPulseArea[i1][i2]->Write();
			NegPeakAmp[i1][i2]->Write();
			PosPeakAmp[i1][i2]->Write();
			fCperADC[i1][i2]->Write();
		}
	}
	
	outroot->Close();
	sprintf(hname,"mv TPCCalibration.root %s/Histos/;wait;",AnalysisFilePath);system(hname);
}