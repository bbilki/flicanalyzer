void WriteWF()
{
	sprintf(hname,"%s/Run_%d.root",NTupleFilePath,RunNo);
	TFile* inroot=new TFile(hname);
	TTree* T =  (TTree*) inroot->Get("T");
	T->SetBranchAddress("TPCWF",&ed.TPCWF);
	T->SetBranchAddress("PMTWF",&ed.PMTWF);
	
	TH1F* Amp[2];
	Amp[0]=new TH1F("Amp_Col","Amp_Col",1000,-500,500);
	Amp[1]=new TH1F("Amp_Ind","Amp_Ind",1000,-500,500);
	
	ReadTPCBaselines();
	ReadPMTBaselines();

	TFile* outroot;
	
	sprintf(hname,"Waveforms_%d.root",RunNo);
	outroot=new TFile(hname,"recreate");
	outroot->mkdir("Ch0");
	outroot->mkdir("Ch1");
	outroot->mkdir("Ch2");
	
	TH1F* hb;
	TH1F* hh[2][128];
	for(int i1=0;i1<2;i1++)
	{
		for(int i2=0;i2<128;i2++)
		{
			sprintf(hname,"TPCWF_%s_%d",wt[i1].c_str(),i2);
			hh[i1][i2]=new TH1F(hname,hname,4096,-0.5,4095.5);
		}
	}
	sprintf(hname,"Ev2D");
	TH2F* h2=new TH2F(hname,hname,4096,-0.5,4095.5,256,-0.5,255.5);
	
	TH1F* hhPMT[3];
	for(int i1=0;i1<3;i1++)
	{
		sprintf(hname,"Int_%d",i1);
		hhPMT[i1]=new TH1F(hname,hname,1200,-20000,4000);
	}
	
	int ic=0;int gch=0;
	float ss=0.;
	int NSat[3]={0};
	bool isSaturated=false;
	float qint=0.;
	float sg=0.;
	
	for(int I=0;I<T->GetEntries();I++)
	{
		T->GetEntry(I);
		
		for(int i1=0;i1<256;i1++)
		{
			ic=i1/128;gch=i1-((i1/128)*128);
			for(int i2=0;i2<4096;i2++)
			{
// 				ss=((float)ed.TPCWF->at(i1)[i2])-TPCBaselines[ic][gch][1];
				ss=((float)ed.TPCWF->at(i1)[i2])-TPCBaselines[ic][gch][0];
				hh[ic][gch]->SetBinContent(hh[ic][gch]->FindBin(i2),ss);
				h2->Fill(i2,i1,ss);
				Amp[ic]->Fill(ss);
			}
		}
		outroot->cd();
		
		sprintf(hname2,"Ev2D_%d",I);
		h2->SetName(hname2);h2->SetTitle(hname2);
		h2->SetMinimum(-50);h2->SetMaximum(50);
		h2->Write();
		h2->SetName("Ev2D");h2->SetTitle("Ev2D");
		h2->Reset();
		
// 		for(int i1=0;i1<256;i1++)
// 		{
// 			for(int i2=0;i2<4096;i2++)
// 			{
// 				sprintf(hname2,"%s",hh[i1][i2]->GetName());
// 				sprintf(hname,"Ev_%d_%s",I,hh[i1][i2]->GetName());
// 				hh[i1][i2]->SetName(hname);hh[i1][i2]->SetTitle(hname);
// 				hh[i1][i2]->Write();
// 				hh[i1][i2]->SetName(hname2);hh[i1][i2]->SetTitle(hname2);
// 			}
// 		}
		
		for(int i1=0;i1<2;i1++)
		{
			for(int i2=0;i2<128;i2++)
			{
				hh[i1][i2]->Reset();
			}
		}
		
		for(int i1=0;i1<3;i1++)
		{
			sprintf(hname,"WF_PMT_%d_Ev_%d",i1,I);
			hb=new TH1F(hname,hname,15000,-0.5,14999.5);
			isSaturated=false;
			qint=0.;
			for(int i2=0;i2<ed.PMTWF->at(i1).size();i2++)
			{
				sg=(-1.)*(((float)ed.PMTWF->at(i1)[i2])-PMTBaselines[i1][0]);
				hb->SetBinContent(i2+1,sg);
				if(sg>ThPMT[i1]) qint+=sg;
				if(ed.PMTWF->at(i1)[i2]==0){isSaturated=true;}
			}
			if(isSaturated){NSat[i1]++;}
			outroot->cd();
			
			sprintf(hname2,"Int=%5.3f",qint);
			hb->SetTitle(hname2);
			if(isSaturated)
			{
				sprintf(hname,"%s Saturated",hname2);
				hb->SetTitle(hname);
			}
			sprintf(hname,"Ch%d",i1);
			outroot->cd(hname);
			
			hb->Write();
			hb->Reset();
			hhPMT[i1]->Fill(qint);
		}
		if(I%100==0) cout<<"Ev : "<<I<<" / "<<T->GetEntries()<<endl;
// 		if(I==100) break;
	}
	
	outroot->cd();
	Amp[0]->Write();
	Amp[1]->Write();
	
	for(int i1=0;i1<3;i1++)
	{
		hhPMT[i1]->Write();
	}
	outroot->Close();
	file.close();
	
	cout<<"Run "<<RunNo<<" saturations: "<<NSat[0]<<" "<<NSat[1]<<" "<<NSat[2]<<" out of "<<T->GetEntries()<<endl; 
	
	sprintf(hname,"cp Waveforms_%d.root %s/Histos/Waveforms_%d.root",RunNo,AnalysisFilePath,RunNo);system(hname);
}