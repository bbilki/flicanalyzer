void HitFindingTest()
{
// 	sprintf(hname,"HitFindingTest_%d.root",RunNo);
// 	TFile* outroot=new TFile(hname,"recreate");
	
	ReadTPCBaselines();
	
	//below is for 7285
	int NevtSel=33;
	int EventNos[33]={2,12,18,21,22,26,32,35,38,39,42,46,50,52,57,58,63,64,65,68,70,71,72,74,76,78,80,81,89,90,99,101,104};
	
	TH2F* Map=new TH2F("Map","Map",128,-0.5,127.5,128,-0.5,127.5);
	TH1F* hh[2][128];
	float scol=0;float sind=0;float stot=0;
	int iccol=0;int gchcol=0;
	int icind=0;int gchind=0;
	
	TFile* inroot;
	TTree* T;
	
	sprintf(hname,"%s/Run_%d.root",NTupleFilePath,RunNo);
	inroot=new TFile(hname);
	T =  (TTree*) inroot->Get("T");
	T->SetBranchAddress("TPCWF",&ed.TPCWF);
	T->SetBranchAddress("PMTWF",&ed.PMTWF);
	
	TCanvas* cc=new TCanvas("cc","cc",600,600);
	gStyle->SetOptStat(0);
	
	for(int I=0;I<NevtSel;I++)
	{
		T->GetEntry(EventNos[I]);
		for(int i2=1900;i2<2200;i2++)
		{
			Map->Reset();
			for(int i1=0;i1<128;i1++)
			{
				iccol=i1/128;gchcol=i1-((i1/128)*128);
				scol=((float)ed.TPCWF->at(i1)[i2])-TPCBaselines[iccol][gchcol][0];
				if(scol>0)
				{
					stot=scol;
					for(int i3=128;i3<256;i3++)
					{
						icind=i3/128;gchind=i3-((i3/128)*128);
						sind=((float)ed.TPCWF->at(i3)[i2])-TPCBaselines[icind][gchind][0];
						if(sind<0) stot+=(-1.*sind);
						Map->Fill(i1,i3-128,stot);
					}
				}
			}
			cc->cd();
			Map->Draw("colz");
			Map->SetMaximum(50);
			sprintf(hname,"%d",i2);
			Map->SetTitle(hname);
			Map->SetMaximum(50);
			Map->SetMinimum(0);
			sprintf(hname,"R%d_E%d_%d.jpg",RunNo,EventNos[I],i2);
			cc->SaveAs(hname);
		}
		sprintf(hname,"convert -delay 1 -loop 0 R%d_E%d_*.jpg R%d_E%d.gif",RunNo,EventNos[I],RunNo,EventNos[I]);
		system(hname);
		sprintf(hname,"rm *.jpg");system(hname);
	}
// 	outroot->Close();
// 	sprintf(hname,"mv TPCTiming_%d.root %s/Histos/;wait;",RunNo,AnalysisFilePath);system(hname);
}