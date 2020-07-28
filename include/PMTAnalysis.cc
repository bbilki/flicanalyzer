void PMTAnalysis()
{
	int PRunNo=RunNo;
	vector <int> RunNos;
	if(PRunNo==1000)
	{
		//up to 7095 different voltages
// 		RunNos.push_back(7074);
// 		RunNos.push_back(7076);
// 		RunNos.push_back(7078);
// 		RunNos.push_back(7082);
// 		RunNos.push_back(7084);
// 		RunNos.push_back(7092);
// 		RunNos.push_back(7094);
		
// 		RunNos.push_back(7096);
		RunNos.push_back(7104);
		RunNos.push_back(7106);
		RunNos.push_back(7107);
		RunNos.push_back(7110);
		RunNos.push_back(7121);
	}
	else if(PRunNo==2000)
	{
		RunNos.push_back(7252);
		RunNos.push_back(7254);
		RunNos.push_back(7256);
		RunNos.push_back(7259);
		RunNos.push_back(7268);
		RunNos.push_back(7271);
		RunNos.push_back(7273);
	}
	else if(PRunNo==3000)
	{
		RunNos.push_back(7308);
		RunNos.push_back(7312);
		RunNos.push_back(7314);
		RunNos.push_back(7316);
		RunNos.push_back(7318);
		RunNos.push_back(7320);
		RunNos.push_back(7322);
		RunNos.push_back(7325);
		RunNos.push_back(7327);
		RunNos.push_back(7385);
		RunNos.push_back(7388);
	}
	
	TH1F* hPMT[3];
// 	string PMTNames[3]={"LAr w/TPB","LAr no TPB","LXe"};
	int PMTlc[3]={1,2,4};
	float TOTThresholds[20]={10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200};
// 	float ThresholdOffset[3]={70,0,10};
	float ThresholdOffset[3]={0,0,0};
	float threshold=0;
	
	TF1* tf1=new TF1("gaus","gaus",0.,300.);
	TF1* tf2=new TF1("lin","[0]*x-[1]",0.,15000.);
	
	sprintf(hname,"PMTAnalysis_%d.root",RunNo);
	TFile* outroot=new TFile(hname,"recreate");
	
	TH1D* Amplitude[3];TH1D* Integral[3];TH1D* MaxAmplitude[3];
	TH1D* IntegralforThreshold[3][20];TH1D* DeltaTforThreshold[3][20];TH1D* DeltaTforThresholdSat[3][20];
	TProfile* TOT[3][20];TProfile* TOT2[3][20];TProfile* TOT3[3];
	
// 	TH1F* WFt0=new TH1F("WFt0","WFt0",15000,-0.5,14999.5);
	TH1F* AvgWFSat[3][2][15];
	TH1F* Ht0[3];TH1F* Ht02[3][15];
	for(int i1=0;i1<3;i1++)
	{
		for(int i2=0;i2<2;i2++)
		{
			for(int i3=0;i3<RunNos.size();i3++)
			{
				sprintf(hname,"AvgWFSat_%d_%d_R%d",i1,i2,RunNos[i3]);
				AvgWFSat[i1][i2][i3]=new TH1F(hname,hname,15000,-0.5,14999.5);
			}
		}
		sprintf(hname,"Amplitude_%d",i1);
		Amplitude[i1]=new TH1D(hname,hname,1000,-0.5,999.5);
		sprintf(hname,"Ht0_%d",i1);
		Ht0[i1]=new TH1F(hname,hname,15000,0,15000);
		for(int i2=0;i2<RunNos.size();i2++)
		{
			sprintf(hname,"Ht02_%d_R%d",i1,RunNos[i2]);
			Ht02[i1][i2]=new TH1F(hname,hname,15000,0,15000);
		}
		
		sprintf(hname,"MaxAmplitude_%d",i1);
		MaxAmplitude[i1]=new TH1D(hname,hname,1000,-0.5,999.5);
		sprintf(hname,"Integral_%d",i1);
		Integral[i1]=new TH1D(hname,hname,1000,0,1e5);
		for(int i2=0;i2<20;i2++)
		{
			threshold=TOTThresholds[i2]+ThresholdOffset[i1];
			sprintf(hname,"IntegralforThreshold_%d_%5.2f",i1,threshold);
			IntegralforThreshold[i1][i2]=new TH1D(hname,hname,1000,0,50000);
			sprintf(hname,"TOT_%d_%5.2f",i1,threshold);
// 			TOT[i1][i2]=new TProfile(hname,hname,300,0.,300.,0.,30000);
			TOT[i1][i2]=new TProfile(hname,hname,400,0.,100.,0.,20000);
			sprintf(hname,"TOT2_%d_%5.2f",i1,threshold);
			TOT2[i1][i2]=new TProfile(hname,hname,400,0.,100.,0.,20000);
			sprintf(hname,"DeltaTforThreshold_%d_%5.2f",i1,threshold);
			DeltaTforThreshold[i1][i2]=new TH1D(hname,hname,300,0,300);
			sprintf(hname,"DeltaTforThresholdSat_%d_%5.2f",i1,threshold);
			DeltaTforThresholdSat[i1][i2]=new TH1D(hname,hname,300,0,300);
		}
		sprintf(hname,"TOT_%d_400",i1,threshold);
		TOT3[i1]=new TProfile(hname,hname,400,0.,100.,0.,20000);
	}
	
// 	vector <TH1F*> HDeltaT[3];
// 	TH1F* HDeltaT[3][40];
// 	vector <float> bmin[3];
// 	vector <float> bmax[3];
// 	TH1F* hdt=new TH1F("hd","hd",300,0,300);
// 	for(int i1=0;i1<40;i1++)
// 	{
// 		for(int i2=0;i2<3;i2++)
// 		{
// 			sprintf(hname,"Th_%d_%d",5+(i1*5),i2);
// 			HDeltaT[i2][i1]=new TH1F(hname,hname,300,0,300);
// // 			hdt->SetName(hname);hdt->SetTitle(hname);
// // 			HDeltaT[i2].push_back(hdt);
// 			bmin[i2].push_back(0);
// 			bmax[i2].push_back(0);
// 		}
// 	}
	
	float maxamp=0;float amp10p=0;float amp90p=0;float amp25p=0;float amp75p=0;
// 	float PMTXY[3][2]={{96.5,63.5},{63.5,30.5},{63.5,96.5}};
	float PMTXY[3][2]={{95.,63.5},{63.5,32.},{63.5,95.}};
	int I=0;int J=0;int II=0;
	float qint=0;float sg=0;
	float qintTOT[20]={0};float qintTOT400=0;int binmin400=0;int binmax400=0;
	int binmin[20]={0};int binmax[20]={0};
	
	TFile* inroot;
	TFile* inroot2;
	TTree* TH;
	TTree* TT;
	TTree* T;
	bool PMTsat=false;
	bool foundt0=false;int tt0=0;
	for(int J=0;J<RunNos.size();J++)
	{
		RunNo=RunNos[J];
		PMTBaselineRun=FindBaselineRun(RunNo);
		cout<<"Run: "<<RunNo<<" PMTBaselineRun: "<<PMTBaselineRun<<endl;
		ReadPMTBaselines();
		sprintf(hname,"%s/Histos/HitsAndTracks_%d.root",AnalysisFilePath,RunNo);
		inroot=new TFile(hname);
		TH =  (TTree*) inroot->Get("Hits");
		TH->SetBranchAddress("E",&hd.E);
		TH->SetBranchAddress("ColIndT",&hd.ColIndT);
		TH->SetBranchAddress("Int",&hd.Int);
		TH->SetBranchAddress("QColTot",&hd.QColTot);
		TH->SetBranchAddress("QColTotZS",&hd.QColTotZS);
		TH->SetBranchAddress("QHitTot",&hd.QHitTot);
		TH->SetBranchAddress("PMTIntegral",&hd.PMTIntegral);
		TH->SetBranchAddress("ColID",&hd.ColID);
		TH->SetBranchAddress("ColT",&hd.ColT);
		TH->SetBranchAddress("ColA",&hd.ColA);
		TH->SetBranchAddress("ColInt",&hd.ColInt);
		TH->SetBranchAddress("Colw",&hd.Colw);
		TH->SetBranchAddress("IndID",&hd.IndID);
		TH->SetBranchAddress("IndT",&hd.IndT);
		TH->SetBranchAddress("IndA",&hd.IndA);
		TH->SetBranchAddress("Indw",&hd.Indw);
		TH->SetBranchAddress("EventType",&hd.EventType);
		TT =  (TTree*) inroot->Get("Tracks");
		TT->SetBranchAddress("E",&td.E);
		TT->SetBranchAddress("StartEndColIndT",&td.StartEndColIndT);
		TT->SetBranchAddress("FitParams",&td.FitParams);
		TT->SetBranchAddress("FitNormChi2",&td.FitNormChi2);
		TT->SetBranchAddress("NHits",&td.NHits);
		TT->SetBranchAddress("Nexcl",&td.Nexcl);
		TT->SetBranchAddress("PMTIntegral",&td.PMTIntegral);
		TT->SetBranchAddress("ColTStartEnd",&td.ColTStartEnd);
		TT->SetBranchAddress("ColHitTStartEnd",&td.ColHitTStartEnd);
		
		sprintf(hname,"%s/Run_%d.root",NTupleFilePath,RunNo);
// 		cout<<hname<<endl;
		inroot2=new TFile(hname);
		T =  (TTree*) inroot2->Get("T");
		T->SetBranchAddress("TPCWF",&ed.TPCWF);
		T->SetBranchAddress("PMTWF",&ed.PMTWF);
		
		for(int I=0;I<TH->GetEntries();I++)
		{
			TH->GetEntry(I);
			if(!(hd.EventType==0 || hd.EventType==1)) continue;
			T->GetEntry(hd.E);
			for(int i1=0;i1<3;i1++)
			{
				if(i1==1) continue;
				
				if(hd.PMTIntegral->at(i1)<0)//saturated
				{
					for(int i2=0;i2<ed.PMTWF->at(i1).size();i2++)
					{
						sg=(-1.)*(((float)ed.PMTWF->at(i1)[i2])-PMTBaselines[i1][0]);
						AvgWFSat[i1][0][J]->Fill(i2,sg);
						AvgWFSat[i1][1][J]->Fill(i2);
					}
				}
				
				qint=0.;
				maxamp=0;
				foundt0=false;
				if(hd.PMTIntegral->at(i1)>0)//unsaturated
				{
					for(int i2=1900;i2<2100;i2++)
					{
						sg=(-1.)*(((float)ed.PMTWF->at(i1)[i2])-PMTBaselines[i1][0]);
						if(sg>400)
						{
							if(binmin400==0) binmin400=i2;
							binmax400=i2;
							qintTOT400+=sg;
						}
						else
						{
							if(binmin400!=0) break;
						}
					}
				}
// 				for(int i2=0;i2<20;i2++){qintTOT[i2]=0;binmin[i2]=0;binmax[i2]=0;}
				for(int i2=1900;i2<2100;i2++)
				{
					sg=(-1.)*(((float)ed.PMTWF->at(i1)[i2])-PMTBaselines[i1][0]);
					if(hd.PMTIntegral->at(i1)>=0) Amplitude[i1]->Fill(sg);//unsaturated
					if(sg>maxamp) maxamp=sg;
					if(sg>10 && !foundt0){tt0=i2;foundt0=true;}
					qint+=sg;
					
// 					for(int i3=0;i3<20;i3++)
// 					{
// 						threshold=TOTThresholds[i3]+ThresholdOffset[i1];
// 						if(sg>threshold)
// 						{
// 							qintTOT[i3]+=sg;
// 							if(binmin[i3]==0) binmin[i3]=i2;
// 							binmax[i3]=i2;
// 						}
// 						else
// 						{
// 							if(binmin[i3]!=0) continue;
// 						}
// 					}
// 					for(int i3=0;i3<40;i3++)
// 					{
// 						if(sg>((i3*5)+5))
// 						{
// 							if(bmin[i1][i3]==0) bmin[i1][i3]=i2;
// 							bmax[i1][i3]=i2;
// 						}
// 						else
// 						{
// 							if(bmin[i1][i3]!=0) continue;
// 						}
// 					}
				}
				if(hd.PMTIntegral->at(i1)>=0)
				{
					Integral[i1]->Fill(qint);//unsaturated
					MaxAmplitude[i1]->Fill(maxamp);//unsaturated
// 					for(int i3=0;i3<40;i3++)
// 					{
// 						if((bmax[i1][i3]-bmin[i1][i3])>0)
// 						{
// 							HDeltaT[i1][i3]->Fill(bmax[i1][i3]-bmin[i1][i3]);
// 						}
// 						bmax[i1][i3]=0;bmin[i1][i3]=0;
// 					}
					
// 					amp25p=0.25*maxamp;
// 					amp75p=0.75*maxamp;
// 					WFt0->Reset();
// 					for(int i2=1900;i2<2100;i2++)
// 					{
// 						sg=(-1.)*(((float)ed.PMTWF->at(i1)[i2])-PMTBaselines[i1][0]);
// 						WFt0->SetBinContent(i2,sg);
// 					}
// 					WFt0->Fit(tf2,"q","q",WFt0->FindBin(amp25p),WFt0->FindBin(amp75p));
// 					Ht0[i1]->Fill(tf2->GetParameter(1)/tf2->GetParameter(0));
					Ht0[i1]->Fill(tt0);
					Ht02[i1][J]->Fill(tt0);
				}
				if(maxamp>600 && hd.PMTIntegral->at(i1)>=0)
				{
					TOT3[i1]->Fill(binmax400-binmin400,qintTOT400);
				}
// 				for(int i2=0;i2<20;i2++)
// 				{
// 					threshold=TOTThresholds[i2]+ThresholdOffset[i1];
// 					if(hd.PMTIntegral->at(i1)>=0) IntegralforThreshold[i1][i2]->Fill(qintTOT[i2]);
// 					if((binmax[i2]-binmin[i2])>2)
// 					{
// 						if(hd.PMTIntegral->at(i1)>=0)//unsaturated
// 						{
// 							DeltaTforThreshold[i1][i2]->Fill(binmax[i2]-binmin[i2]);
// 							TOT[i1][i2]->Fill(binmax[i2]-binmin[i2],qintTOT[i2]);
// 						}
// 						else//saturated
// 						{
// 							DeltaTforThresholdSat[i1][i2]->Fill(binmax[i2]-binmin[i2]);
// 						}
// 					}
// 				}
			}
		}
		inroot->Close();
		inroot2->Close();
	}
// 	TGraph* tg1[3];
// 	tg1[0]=new TGraph();tg1[2]=new TGraph();
// 	tg1[0]->SetName("chi2vstot_0");tg1[0]->SetTitle("chi2vstot_0");
// 	tg1[0]->SetMarkerStyle(20);tg1[0]->SetMarkerColor(1);
// 	tg1[0]->GetXaxis()->SetTitle("Time Over Threshold");tg1[0]->GetXaxis()->CenterTitle();
// 	tg1[0]->GetYaxis()->SetTitle("#chi^{2}/ndf ");tg1[0]->GetYaxis()->CenterTitle();
// 	tg1[2]->SetName("chi2vstot_2");tg1[2]->SetTitle("chi2vstot_2");
// 	tg1[2]->SetMarkerStyle(20);tg1[2]->SetMarkerColor(1);
// 	tg1[2]->GetXaxis()->SetTitle("Time Over Threshold");tg1[2]->GetXaxis()->CenterTitle();
// 	tg1[2]->GetYaxis()->SetTitle("#chi^{2}/ndf ");tg1[2]->GetYaxis()->CenterTitle();
// 	outroot->cd();
// 	for(int i1=0;i1<3;i1++)
// 	{
// 		if(i1==1) continue;
// 		for(int i3=0;i3<40;i3++)
// 		{
// 			HDeltaT[i1][i3]->Fit(tf1,"q","q",0.,300.);
// 			HDeltaT[i1][i3]->Fit(tf1,"q","q",tf1->GetParameter(1)-2*tf1->GetParameter(2),tf1->GetParameter(1)+2*tf1->GetParameter(2));
// 			tg1[i1]->SetPoint(tg1[i1]->GetN(),(i3*5)+5,tf1->GetChisquare()/tf1->GetNDF());
// 			HDeltaT[i1][i3]->Write();
// 		}
// 	}
	
// 	float TOTRange[3][20][2]={{{0}}};
// 	for(int i1=0;i1<3;i1++)
// 	{
// 		if(i1==1) continue;
// 		for(int i2=0;i2<20;i2++)
// 		{
// 			DeltaTforThreshold[i1][i2]->Fit(tf1,"q","q",0.,50.);
// 			DeltaTforThreshold[i1][i2]->Fit(tf1,"q","q",tf1->GetParameter(1)-2*tf1->GetParameter(2),tf1->GetParameter(1)+2*tf1->GetParameter(2));
// 			TOTRange[i1][i2][0]=tf1->GetParameter(1)-2.5*tf1->GetParameter(2);
// 			TOTRange[i1][i2][1]=tf1->GetParameter(1)+2.5*tf1->GetParameter(2);
// 		}
// 	}
// 	for(int J=0;J<RunNos.size();J++)
// 	{
// 		RunNo=RunNos[J];
// 		PMTBaselineRun=FindBaselineRun(RunNo);
// 		cout<<"Run: "<<RunNo<<" PMTBaselineRun: "<<PMTBaselineRun<<endl;
// 		ReadPMTBaselines();
// 		sprintf(hname,"%s/Histos/HitsAndTracks_%d.root",AnalysisFilePath,RunNo);
// 		inroot=new TFile(hname);
// 		TH =  (TTree*) inroot->Get("Hits");
// 		TH->SetBranchAddress("E",&hd.E);
// 		TH->SetBranchAddress("ColIndT",&hd.ColIndT);
// 		TH->SetBranchAddress("Int",&hd.Int);
// 		TH->SetBranchAddress("QColTot",&hd.QColTot);
// 		TH->SetBranchAddress("QColTotZS",&hd.QColTotZS);
// 		TH->SetBranchAddress("QHitTot",&hd.QHitTot);
// 		TH->SetBranchAddress("PMTIntegral",&hd.PMTIntegral);
// 		TH->SetBranchAddress("ColID",&hd.ColID);
// 		TH->SetBranchAddress("ColT",&hd.ColT);
// 		TH->SetBranchAddress("ColA",&hd.ColA);
// 		TH->SetBranchAddress("ColInt",&hd.ColInt);
// 		TH->SetBranchAddress("Colw",&hd.Colw);
// 		TH->SetBranchAddress("IndID",&hd.IndID);
// 		TH->SetBranchAddress("IndT",&hd.IndT);
// 		TH->SetBranchAddress("IndA",&hd.IndA);
// 		TH->SetBranchAddress("Indw",&hd.Indw);
// 		TH->SetBranchAddress("EventType",&hd.EventType);
// 		TT =  (TTree*) inroot->Get("Tracks");
// 		TT->SetBranchAddress("E",&td.E);
// 		TT->SetBranchAddress("StartEndColIndT",&td.StartEndColIndT);
// 		TT->SetBranchAddress("FitParams",&td.FitParams);
// 		TT->SetBranchAddress("FitNormChi2",&td.FitNormChi2);
// 		TT->SetBranchAddress("NHits",&td.NHits);
// 		TT->SetBranchAddress("Nexcl",&td.Nexcl);
// 		TT->SetBranchAddress("PMTIntegral",&td.PMTIntegral);
// 		TT->SetBranchAddress("ColTStartEnd",&td.ColTStartEnd);
// 		TT->SetBranchAddress("ColHitTStartEnd",&td.ColHitTStartEnd);
// 		
// 		sprintf(hname,"%s/Run_%d.root",NTupleFilePath,RunNo);
// // 		cout<<hname<<endl;
// 		inroot2=new TFile(hname);
// 		T =  (TTree*) inroot2->Get("T");
// 		T->SetBranchAddress("TPCWF",&ed.TPCWF);
// 		T->SetBranchAddress("PMTWF",&ed.PMTWF);
// 		
// 		for(int I=0;I<TH->GetEntries();I++)
// 		{
// 			TH->GetEntry(I);
// 			if(!(hd.EventType==0 || hd.EventType==1)) continue;
// 			T->GetEntry(hd.E);
// 			for(int i1=0;i1<3;i1++)
// 			{
// 				if(i1==1) continue;
// 				if(hd.PMTIntegral->at(i1)>=0) //unsaturated
// 				{
// 					qint=0.;
// 					for(int i2=0;i2<20;i2++){qintTOT[i2]=0;binmin[i2]=0;binmax[i2]=0;}
// 					for(int i2=1900;i2<2100;i2++)
// 					{
// 						sg=(-1.)*(((float)ed.PMTWF->at(i1)[i2])-PMTBaselines[i1][0]);
// 						qint+=sg;
// 						for(int i3=0;i3<20;i3++)
// 						{
// 							threshold=TOTThresholds[i3]+ThresholdOffset[i1];
// 							if(sg>threshold)
// 							{
// 								qintTOT[i3]+=sg;
// 								if(binmin[i3]==0) binmin[i3]=i2;
// 								binmax[i3]=i2;
// 							}
// 							else
// 							{
// 								if(binmin[i3]!=0) continue;
// 							}
// 						}
// 					}
// // 					Integral[i1]->Fill(qint);
// 					for(int i2=0;i2<20;i2++)
// 					{
// 						threshold=TOTThresholds[i2]+ThresholdOffset[i1];
// // 						IntegralforThreshold[i1][i2]->Fill(qintTOT[i2]);
// 						if((binmax[i2]-binmin[i2])>2)
// 						{
// 							if(((float)(binmax[i2]-binmin[i2]))>TOTRange[i1][i2][0] && ((float)(binmax[i2]-binmin[i2]))<TOTRange[i1][i2][1])
// 							{
// 	// 							DeltaTforThreshold[i1][i2]->Fill(binmax[i2]-binmin[i2]);
// 								TOT2[i1][i2]->Fill(binmax[i2]-binmin[i2],qintTOT[i2]);
// 							}
// 						}
// 					}
// 				}
// 			}
// 		}
// 		inroot->Close();
// 		inroot2->Close();
// 	}
	
// 	sprintf(hname,"TOTParameters_%d.txt",PRunNo);
// 	ofstream outfile(hname);
// 	TF1* tfpol4=new TF1("tfpol4","[0]+[1]*x+[2]*x^2+[3]*x^3+[4]*x^4",0.,300.);
// 	for(int i1=0;i1<3;i1++)
// 	{
// 		if(i1==1) continue;
// 		for(int i2=0;i2<20;i2++)
// 		{
// 			TOT[i1][i2]->Fit(tfpol4,"q","q",TOTRange[i1][i2][0],TOTRange[i1][i2][1]);
// 			outfile<<i1<<" "<<(TOTThresholds[i2]+ThresholdOffset[i1])<<" "<<tfpol4->GetParameter(0)<<" "<<tfpol4->GetParameter(1)<<" "<<tfpol4->GetParameter(2)<<" "<<tfpol4->GetParameter(3)<<" "<<tfpol4->GetParameter(4)<<" "<<(tfpol4->GetChisquare()/tfpol4->GetNDF())<<endl;
// 		}
// 	}
// 	outfile.close();
	
	outroot->cd();
	for(int i1=0;i1<3;i1++)
	{
		if(i1==1) continue;
		Amplitude[i1]->Write();
		Ht0[i1]->Write();
		for(int i2=0;i2<RunNos.size();i2++)
		{
			Ht02[i1][i2]->Write();
		}
		MaxAmplitude[i1]->Write();
		Integral[i1]->Write();
		for(int i2=0;i2<20;i2++)
		{
			IntegralforThreshold[i1][i2]->Write();
			DeltaTforThreshold[i1][i2]->Write();
			DeltaTforThresholdSat[i1][i2]->Write();
			TOT[i1][i2]->Write();
			TOT2[i1][i2]->Write();
		}
		TOT3[i1]->Write();
		for(int i3=0;i3<RunNos.size();i3++)
		{
			AvgWFSat[i1][0][i3]->Divide(AvgWFSat[i1][1][i3]);
			AvgWFSat[i1][0][i3]->Write();
		}
	}
// 	tg1[0]->Write();
// 	tg1[2]->Write();
	
// 	TCanvas* cc=new TCanvas("cc","cc",600,600);
// 	TLegend* legend = new TLegend(0.6,0.7,0.9,0.9);
// 	for(int i1=0;i1<3;i1++)
// 	{
// 		if(i1==1) continue;
// 		legend->Clear();
// 		for(int i2=0;i2<20;i2++)
// 		{
// 			threshold=TOTThresholds[i2]+ThresholdOffset[i1];
// 			sprintf(hname,"Th. %d",((int)threshold));
// 			TOT[i1][i2]->SetName(hname);TOT[i1][i2]->SetTitle(hname);
// 			TOT[i1][i2]->SetLineColor(i2+1);TOT[i1][i2]->SetMarkerStyle(20);TOT[i1][i2]->SetMarkerColor(i2+1);
// 			TOT[i1][i2]->GetFunction("tfpol4")->SetLineColor(i2+1);
// 			if(i2==0)
// 			{
// 				TOT[i1][i2]->GetXaxis()->SetTitle("Time over Threshold");TOT[i1][i2]->GetXaxis()->CenterTitle();
// 				TOT[i1][i2]->GetYaxis()->SetTitle("Integral in Time over Threshold");TOT[i1][i2]->GetYaxis()->CenterTitle();
// 				TOT[i1][i2]->Draw();
// 				legend->AddEntry(TOT[i1][i2],TOT[i1][i2]->GetName(),"l");
// 			}
// 			else if(i2<19)
// 			{
// 				TOT[i1][i2]->Draw("same");
// 				legend->AddEntry(TOT[i1][i2],TOT[i1][i2]->GetName(),"l");
// 			}
// 			else
// 			{
// 				TOT[i1][i2]->Draw("same");
// 				legend->AddEntry(TOT[i1][i2],TOT[i1][i2]->GetName(),"l");
// 				legend->Draw("same");
// 				sprintf(hname,"IvsTOT_PMT_%d",i1);
// 				cc->SetName(hname);
// 				cc->Write();
// 			}
// 		}
// 	}
	
	
	outroot->Close();
// 	sprintf(hname,"cp PMTAnalysis_%d.root %s/Histos/PMTAnalysis_%d.root;wait;",PRunNo,AnalysisFilePath,RunNo);system(hname);
}