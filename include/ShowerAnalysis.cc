void ShowerAnalysis()
{
	int tmin=1000;
	if(RunNo>7300) tmin=1500;
	sprintf(hname,"ShowerPlots_%d.root",RunNo);
	TFile* outroot=new TFile(hname,"recreate");
	TH1F* TotalCollectionCharge=new TH1F("TotalCollectionCharge","TotalCollectionCharge",5000,0,1000000);
	TH2F* TCCvsNHits=new TH2F("TCCvsNHits","TCCvsNHits",1000,0,1000,1000,0,500000);
	TH1F* AllHitX=new TH1F("AllHitX","AllHitX",128,-0.5,127.5);
	TH1F* AllHitY=new TH1F("AllHitY","AllHitY",128,-0.5,127.5);
	TH1F* AllHitT=new TH1F("AllHitT","AllHitT",4096,-0.5,4095.5);
	TH1F* AllHitQ=new TH1F("AllHitQ","AllHitQ",10000,-0.5,9999.5);
	TH2F* Qvst=new TH2F("Qvst","Qvst",1400,1000,2400,1000,0,1000);
	TH3F* NHitsvst3d=new TH3F("NHitsvst3d","NHitsvst3d",128,-0.5,127.5,128,-0.5,127.5,14,1000,2400);
// 	int hc[128][128][140]={{{0}}};
	TGraph2D* tg2d=new TGraph2D();
	NHitsvst3d->SetFillColor(4);
	TH2F* TrackHits[7][3];
	for(int i1=0;i1<7;i1++)
	{
		sprintf(hname,"MeanCharge_%d-%d",(i1*200)+tmin,((i1+1)*200)+tmin);
		TrackHits[i1][0]=new TH2F(hname,hname,128,-0.5,127.5,128,-0.5,127.5);
// 		sprintf(hname,"NHits_%d-%d",(i1*200)+1000,((i1+1)*200)+1000);
// 		TrackHits[i1][1]=new TH2F(hname,hname,128,-0.5,127.5,128,-0.5,127.5);
		
		sprintf(hname,"NHits_%d-%d",(i1*200)+tmin,((i1+1)*200)+tmin);
		TrackHits[i1][1]=new TH2F(hname,hname,128,-0.5,127.5,128,-0.5,127.5);
		
		sprintf(hname,"NHitsN_%d-%d",(i1*200)+tmin,((i1+1)*200)+tmin);
		TrackHits[i1][2]=new TH2F(hname,hname,128,-0.5,127.5,128,-0.5,127.5);
	}
	
	int b[7]={0};
	int Ev=0;
	bool found=false;
	bool skipEvent=false;
	int curInd=0;
	float qtot=0;
	int nh=0;
	
// 	TH3D* H3d=new TH3D("H3d","H3d",128,-0.5,127.5,128,-0.5,127.5,100,1000,2000);
	TH3D* H3d=new TH3D("H3d","H3d",128,-0.5,127.5,128,-0.5,127.5,100,tmin,tmin+1000);
	
	vector <hits> H;
	
	sprintf(hname,"%s/Histos/Hits_%d.root",AnalysisFilePath,RunNo);
	TFile* inroot2=new TFile(hname);
	TTree* THit =  (TTree*) inroot2->Get("T");
	THit->SetBranchAddress("E",&hd.E);
	THit->SetBranchAddress("ColIndT",&hd.ColIndT);
	THit->SetBranchAddress("Int",&hd.Int);
	THit->SetBranchAddress("QColTot",&hd.QColTot);
	THit->SetBranchAddress("QColTotZS",&hd.QColTotZS);
	THit->SetBranchAddress("PMTIntegral",&hd.PMTIntegral);
	
	for(int I=0;I<THit->GetEntries();I++)
	{
		THit->GetEntry(I);
		TotalCollectionCharge->Fill(hd.QColTot);
		TCCvsNHits->Fill(hd.ColIndT->size(),hd.QColTot);
// 		if(hd.QColTot>30000)
		{
			for(int i2=0;i2<hd.ColIndT->size();i2++)
			{
				AllHitX->Fill(hd.ColIndT->at(i2)[0]);
				AllHitY->Fill(hd.ColIndT->at(i2)[1]);
				AllHitT->Fill(hd.ColIndT->at(i2)[2]);
				AllHitQ->Fill(hd.Int->at(i2)[2]);
				Qvst->Fill(hd.ColIndT->at(i2)[2],hd.Int->at(i2)[2]);
				NHitsvst3d->Fill(hd.ColIndT->at(i2)[0],hd.ColIndT->at(i2)[1],hd.ColIndT->at(i2)[2]);
// 				if(!(hd.ColIndT->at(i2)[2]<1000 || hd.ColIndT->at(i2)[2]>=2000))
// 				if(!(hd.ColIndT->at(i2)[2]<1500 || hd.ColIndT->at(i2)[2]>=2500))
				if(!(hd.ColIndT->at(i2)[2]<tmin || hd.ColIndT->at(i2)[2]>=(tmin+1000)))
				{
// 					TrackHits[(hd.ColIndT->at(i2)[2]-1000)/200][0]->Fill(hd.ColIndT->at(i2)[0],hd.ColIndT->at(i2)[1],hd.Int->at(i2)[2]);
// 					TrackHits[(hd.ColIndT->at(i2)[2]-1000)/200][1]->Fill(hd.ColIndT->at(i2)[0],hd.ColIndT->at(i2)[1]);
// 					TrackHits[(hd.ColIndT->at(i2)[2]-1500)/200][0]->Fill(hd.ColIndT->at(i2)[0],hd.ColIndT->at(i2)[1],hd.Int->at(i2)[2]);
// 					TrackHits[(hd.ColIndT->at(i2)[2]-1500)/200][1]->Fill(hd.ColIndT->at(i2)[0],hd.ColIndT->at(i2)[1]);
					TrackHits[(hd.ColIndT->at(i2)[2]-tmin)/200][0]->Fill(hd.ColIndT->at(i2)[0],hd.ColIndT->at(i2)[1],hd.Int->at(i2)[2]);
					TrackHits[(hd.ColIndT->at(i2)[2]-tmin)/200][1]->Fill(hd.ColIndT->at(i2)[0],hd.ColIndT->at(i2)[1]);
					tg2d->SetPoint(tg2d->GetN(),hd.ColIndT->at(i2)[0],hd.ColIndT->at(i2)[1],hd.ColIndT->at(i2)[2]);
// 						hc[H[i2].col][H[i2].ind][(H[i2].t-1000)/100]++;
					H3d->AddBinContent(H3d->FindBin(hd.ColIndT->at(i2)[0],hd.ColIndT->at(i2)[1],hd.ColIndT->at(i2)[2]));
				}
			}
		}
	}
	
	for(int i1=0;i1<7;i1++)
	{
		float nmax=0.;
		for(int is1=1;is1<=TrackHits[i1][1]->GetNbinsX();is1++)
		{
			for(int is2=1;is2<=TrackHits[i1][1]->GetNbinsY();is2++)
			{
				if(TrackHits[i1][1]->GetBinContent(is1,is2)>nmax) nmax=TrackHits[i1][1]->GetBinContent(is1,is2);
			}
		}
		for(int is1=1;is1<=TrackHits[i1][2]->GetNbinsX();is1++)
		{
			for(int is2=1;is2<=TrackHits[i1][2]->GetNbinsY();is2++)
			{
				TrackHits[i1][2]->SetBinContent(is1,is2,TrackHits[i1][1]->GetBinContent(is1,is2)/nmax);
			}
		}
	}
	
	inroot2->Close();
	
	TotalCollectionCharge->Write();
	TCCvsNHits->Write();
	outroot->cd();
//	EntryXY->Write();
//	EntryT->Write();
//	ExitXY->Write();
//	ExitT->Write();
	AllHitX->Write();
	AllHitY->Write();
	AllHitT->Write();
	AllHitQ->Write();
	Qvst->Write();
	H3d->Write();
	
	TCanvas*cc=new TCanvas("cc","cc",600,600);
//	tg2d->Draw("surf4");
	H3d->Draw("iso");
	cc->SetName("H3d_iso");
	cc->Write();
	H3d->Draw("box");
	cc->SetName("H3d_box");
	cc->Write();
	H3d->Draw("box1");
	cc->SetName("H3d_box1");
	cc->Write();
	H3d->Draw("box2");
	cc->SetName("H3d_box2");
	cc->Write();
	H3d->Draw("box2z");
	cc->SetName("H3d_box2z");
	cc->Write();
	H3d->Draw("box3");
	cc->SetName("H3d_box3");
	cc->Write();
	H3d->Draw("lego");
	cc->SetName("H3d_lego");
	cc->Write();
	
	NHitsvst3d->Write();
	tg2d->Write();
	for(int i1=0;i1<7;i1++)
	{
		TrackHits[i1][0]->Divide(TrackHits[i1][1]);
		TrackHits[i1][0]->Write();
		TrackHits[i1][1]->Write();
		TrackHits[i1][2]->Write();
	}
	
	outroot->Close();
//	inrootPMTWF->Close();
}
