void HitAnalysis()
{
	sprintf(hname,"%s/Run_%d.root",NTupleFilePath,RunNo);
	TFile* inroot=new TFile(hname);
	TTree* T =  (TTree*) inroot->Get("T");
	T->SetBranchAddress("TPCWF",&ed.TPCWF);
	T->SetBranchAddress("PMTWF",&ed.PMTWF);
	
	ReadTPCBaselines();
	ReadPMTBaselines();
	
	sprintf(hname,"HitAnalysis_%d.root",RunNo);
	TFile* outroot=new TFile(hname,"recreate");
	TH1F* FirstColIndTimeDiff=new TH1F("FirstColIndTimeDiff","FirstColIndTimeDiff",39,-19.5,19.5);
	TH1F* InterIndTimeDiff=new TH1F("InterIndTimeDiff","InterIndTimeDiff",99,-49.5,49.5);
	
	TH1F* NColperWT=new TH1F("NColperWt","NColperWt",150,-0.5,149.5);
	TH1F* NColperTW=new TH1F("NColpertW","NColpertW",150,-0.5,149.5);
	TH1F* ColWidthZSWT=new TH1F("ColWidthZSWt","ColWidthZSWt",250,-0.5,249.5);
	TH1F* ColWidthZSTW=new TH1F("ColWidthZStW","ColWidthZStW",250,-0.5,249.5);
	TH1F* ColIntZS=new TH1F("ColIntZS","ColIntZS",1000,0,100000);
	TH1F* ColIntZSWT=new TH1F("ColIntZSWT","ColIntZSWT",1000,0,100000);
	TH1F* ColIntZSTW=new TH1F("ColIntZSTW","ColIntZSTW",1000,0,100000);
	TH1F* QTotColZSWT=new TH1F("QTotColZSWt","QTotColZSWt",1000,0.,10000.);
	TH1F* QTotColZSTW=new TH1F("QTotColZStW","QTotColZStW",1000,0.,10000.);
	
	TGraph* tgint1=new TGraph();
	tgint1->SetName("ColIntegralWt");tgint1->SetTitle("ColIntegralWt");
	tgint1->SetMarkerStyle(25);tgint1->SetMarkerColor(1);
	tgint1->GetXaxis()->SetTitle("Event ID");tgint1->GetXaxis()->CenterTitle();
	tgint1->GetYaxis()->SetTitle("Total Charge (arbitrary units)");tgint1->GetYaxis()->CenterTitle();
	
	TGraph* tgint2=new TGraph();
	tgint2->SetName("ColIntegraltW");tgint2->SetTitle("ColIntegraltW");
	tgint2->SetMarkerStyle(26);tgint2->SetMarkerColor(2);
	tgint2->GetXaxis()->SetTitle("Event ID");tgint2->GetXaxis()->CenterTitle();
	tgint2->GetYaxis()->SetTitle("Total Charge (arbitrary units)");tgint2->GetYaxis()->CenterTitle();
	
	TGraph* tgint3=new TGraph();
	tgint3->SetName("ColIntegralTrue");tgint3->SetTitle("ColIntegralTrue");
	tgint3->SetMarkerStyle(24);tgint3->SetMarkerColor(4);
	tgint3->GetXaxis()->SetTitle("Event ID");tgint3->GetXaxis()->CenterTitle();
	tgint3->GetYaxis()->SetTitle("Total Charge (arbitrary units)");tgint3->GetYaxis()->CenterTitle();
	
	TCanvas* cc1=new TCanvas("ColWires","ColWires",600,600);
	TCanvas* cc2=new TCanvas("IndWiresInv","IndWires",600,600);
	TCanvas* cc3=new TCanvas("IndWires","IndWires",600,600);
	
	TSpectrum *s;
	Double_t *xpeaks;Double_t *xpeaks2;
	TH1F* hh[2][128];TH1F* hh2[2][128];TH1F* hh3[2][128];
	TH1F* ht[4096];TH1F* ht2[4096];TH1F* ht3[4096];
	int wcolors[5]={1,2,3,4,6};
	
	for(int i1=0;i1<2;i1++)
	{
		for(int i2=0;i2<128;i2++)
		{
			sprintf(hname,"WF_%s_%d",wt[i1].c_str(),i2);
			hh[i1][i2]=new TH1F(hname,hname,4096,-0.5,4095.5);
			hh[i1][i2]->SetLineColor(wcolors[i2%5]);
			sprintf(hname,"WFZS_%s_%d",wt[i1].c_str(),i2);
			hh2[i1][i2]=new TH1F(hname,hname,4096,-0.5,4095.5);
			hh2[i1][i2]->SetLineColor(wcolors[i2%5]);
			sprintf(hname,"WFInvZS_%s_%d",wt[i1].c_str(),i2);
			hh3[i1][i2]=new TH1F(hname,hname,4096,-0.5,4095.5);
			hh3[i1][i2]->SetLineColor(wcolors[i2%5]);
		}
	}
	for(int i1=0;i1<4096;i1++)
	{	
		sprintf(hname,"WFt_%d",i1);
		ht[i1]=new TH1F(hname,hname,256,-0.5,255.5);
		sprintf(hname,"WFtZS_%d",i1);
		ht2[i1]=new TH1F(hname,hname,256,-0.5,255.5);
		sprintf(hname,"WFtZSIndInv_%d",i1);
		ht3[i1]=new TH1F(hname,hname,256,-0.5,255.5);
	}
	
	TGraph2D* tg2d=new TGraph2D();
	sprintf(hname,"Ev3d");
	tg2d->SetName(hname);tg2d->SetTitle(hname);
	tg2d->SetMarkerStyle(20);tg2d->SetMarkerColor(4);
	int nh=0;
	vector <hits> H;
	TH3F *frame3d=new TH3F("frame3d","frame3d",1,-0.5,127.5,1,-0.5,127.5,1,-0.5,4095.5);
	TPolyMarker3D *pm3d1;
	TCanvas* cc=new TCanvas("cc","cc",600,600);
	cc->cd();
	gStyle->SetOptStat(0);
	
	bool isSaturated=false;
	float sg=0.;
	float qint=0.;
	
	TGraph* tgacol=new TGraph();tgacol->SetPoint(tgacol->GetN(),-0.5,-20);tgacol->SetPoint(tgacol->GetN(),4095.5,100);
	TGraph* tgaind=new TGraph();tgaind->SetPoint(tgaind->GetN(),-0.5,-30);tgaind->SetPoint(tgaind->GetN(),4095.5,30);
	
	vector <int> bb;
	vector <float> bc;
	int Ev=0;
	int integral=0;int integral2=0;int integral3=0;
	int peakbin=0;int binmin=0;int binmax=0;
	int ic=0;int gch=0;
	float QColTot=0;
	float QColTotZSWt=0;
	float QColTotZStW=0;
	int minTcol=0;int minTind=0;int minTindDiff=0;
	int ncolpeaks=0;
	for(int I=0;I<T->GetEntries();I++)
	{
		T->GetEntry(I);
		int npeaks = 200;
		int nfound=0;
		int nfound2=0;
		float ss=0;
		bool found=true;
		bool HitOK=true;
		
		for(int i1=0;i1<256;i1++)
		{
			ic=i1/128;gch=i1-((i1/128)*128);
			for(int i2=0;i2<4096;i2++)
			{
				ss=((float)ed.TPCWF->at(i1)[i2])-TPCBaselines[ic][gch][0];
				
				hh[ic][gch]->SetBinContent(hh[ic][gch]->FindBin(i2),ss);
				if(ss>ThColInd[ic]) {hh2[ic][gch]->SetBinContent(hh2[ic][gch]->FindBin(i2),ss);}
				
				ht[i2]->SetBinContent(ht[i2]->FindBin(ic*128+gch),ss);
				if(ss>ThColInd[ic]) {ht2[i2]->SetBinContent(ht2[i2]->FindBin(ic*128+gch),ss);}
				
				if(i1<128)
				{
					if(ss>ThColInd[ic])
					{
						ht3[i2]->SetBinContent(ht3[i2]->FindBin(ic*128+gch),ss);
					}
				}
				else
				{
					ss*=-1;
					if(ss>ThColInd[ic])
					{
						hh3[ic][gch]->SetBinContent(hh3[ic][gch]->FindBin(i2),ss);
						ht3[i2]->SetBinContent(ht3[i2]->FindBin(ic*128+gch),ss);
					}
				}
			}
		}
		QColTot=0;
		for(int i1=0;i1<128;i1++)
		{
			QColTot+=hh2[0][i1]->Integral();
		}
		tgint3->SetPoint(tgint3->GetN(),I,QColTot);
		ColIntZS->Fill(QColTot);
		
		minTcol=5000;minTind=5000;
		QColTotZSWt=0;
		for(int i1=0;i1<128;i1++)
		{
			s = new TSpectrum(2*npeaks);
			nfound = s->Search(hh2[0][i1],3.,"",0.1);
			xpeaks = s->GetPositionX();
			NColperWT->Fill(nfound);
			for(int is1=0;is1<nfound;is1++)
			{
				if(xpeaks[is1]<minTcol){minTcol=xpeaks[is1];}
				peakbin=hh2[0][i1]->FindBin(xpeaks[is1]);
				integral3=0;
				for(int ip1=peakbin;ip1<=hh2[0][i1]->GetNbinsX();ip1++)
				{
					if(hh2[0][i1]->GetBinContent(ip1)>ThColInd[0])
					{
						integral3+=hh2[0][i1]->GetBinContent(ip1);
						binmax=ip1;
					}
					else break;
				}
				for(int ip1=peakbin-1;ip1>=1;ip1--)
				{
					if(hh2[0][i1]->GetBinContent(ip1)>ThColInd[0])
					{
						integral3+=hh2[0][i1]->GetBinContent(ip1);
						binmin=ip1;
					}
					else break;
				}
				ColWidthZSWT->Fill(binmax-binmin+1);
				QTotColZSWT->Fill(integral3);
				QColTotZSWt+=integral3;
			}
			delete s;
		}
		ColIntZSWT->Fill(QColTotZSWt);
		tgint1->SetPoint(tgint1->GetN(),I,QColTotZSWt);
		for(int i2=0;i2<128;i2++)
		{
			s = new TSpectrum(2*npeaks);
			nfound = s->Search(hh3[1][i2],3.,"",0.1);
			xpeaks = s->GetPositionX();
			nfound2 = s->Search(hh2[1][i2],3.,"",0.1);
			xpeaks2 = s->GetPositionX();
			minTindDiff=1000;
			for(int is1=0;is1<nfound;is1++)
			{
				if(xpeaks[is1]<minTind){minTind=xpeaks[is1];}
				for(int is2=0;is2<nfound2;is2++)
				{
					if(abs(xpeaks[is1]-xpeaks2[is2])<minTindDiff)
					{
						minTindDiff=abs(xpeaks[is1]-xpeaks2[is2]);
					}
				}
				if(minTindDiff<1000)
				{
					InterIndTimeDiff->Fill(minTindDiff);
				}
			}
			delete s;
		}
		FirstColIndTimeDiff->Fill(minTcol-minTind);
		
		QColTotZStW=0;
		for(int i1=0;i1<4096;i1++)
		{
			s = new TSpectrum(2*npeaks);
			nfound = s->Search(ht3[i1],3.,"",0.1);
			xpeaks = s->GetPositionX();
			ncolpeaks=0;
			for(int is1=0;is1<nfound;is1++)
			{
				if(xpeaks[is1]>=128) continue;
				ncolpeaks++;
				peakbin=ht3[i1]->FindBin(xpeaks[is1]);
				integral3=0;
				for(int ip1=peakbin;ip1<=128;ip1++)
				{
					if(ht3[i1]->GetBinContent(ip1)>ThColInd[0])
					{
						integral3+=ht3[i1]->GetBinContent(ip1);
						binmax=ip1;
					}
					else break;
				}
				for(int ip1=peakbin-1;ip1>=1;ip1--)
				{
					if(ht3[i1]->GetBinContent(ip1)>ThColInd[0])
					{
						integral3+=ht3[i1]->GetBinContent(ip1);
						binmin=ip1;
					}
					else break;
				}
				ColWidthZSTW->Fill(binmax-binmin+1);
				QTotColZSTW->Fill(integral3);
				QColTotZStW+=integral3;
			}
			NColperTW->Fill(ncolpeaks);
			delete s;
		}
		ColIntZSTW->Fill(QColTotZStW);
		tgint2->SetPoint(tgint2->GetN(),I,QColTotZStW);
		
// 		outroot->cd();
// 		cc1->cd();
// 		tgacol->Draw("AP");
// 		for(int i2=0;i2<128;i2++)
// 		{
// 			hh2[0][i2]->Draw("same");
// 		}
// 		sprintf(hname,"Ev_%d_Col_%d",I,minTcol);
// 		cc1->SetName(hname);
// 		cc1->Write();
// 		
// 		cc2->cd();
// 		tgaind->Draw("AP");
// 		for(int i2=0;i2<128;i2++)
// 		{
// 			hh2[1][i2]->Draw("same");
// 		}
// 		sprintf(hname,"Ev_%d_Ind_%d",I,minTind);
// 		cc2->SetName(hname);
// 		cc2->Write();
// 		
// 		cc3->cd();
// 		tgaind->Draw("AP");
// 		for(int i2=0;i2<128;i2++)
// 		{
// 			hh[1][i2]->Draw("same");
// 		}
// 		sprintf(hname,"Ev_%d_IndFull_%d",I,minTind);
// 		cc3->SetName(hname);
// 		cc3->Write();
// 		
// 		FirstColIndTimeDiff->Fill(minTcol-minTind);
		
		for(int i1=0;i1<2;i1++)
		{
			for(int i2=0;i2<128;i2++)
			{
				hh[i1][i2]->Reset();
				hh2[i1][i2]->Reset();
				hh3[i1][i2]->Reset();
			}
		}
		for(int i1=0;i1<4096;i1++)
		{
			ht[i1]->Reset();
			ht2[i1]->Reset();
			ht3[i1]->Reset();
		}
		if(I%10==0) cout<<"Event: "<<I<<endl;
// 		if(I==200) break;
	}
	outroot->cd();
	FirstColIndTimeDiff->Write();
	InterIndTimeDiff->Write();
	NColperWT->Write();
	NColperTW->Write();
	ColWidthZSWT->Write();
	ColWidthZSTW->Write();
	ColIntZS->Write();
	QTotColZSWT->Write();
	QTotColZSTW->Write();
	tgint1->Write();
	tgint2->Write();
	tgint3->Write();
	outroot->Close();
	inroot->Close();
	
	sprintf(hname,"cp HitAnalysis_%d.root %s/Histos/HitAnalysis_%d.root;wait;",RunNo,AnalysisFilePath,RunNo);system(hname);
}