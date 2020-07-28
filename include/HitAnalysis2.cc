void HitAnalysis2()
{
	sprintf(hname,"%s/Run_%d.root",NTupleFilePath,RunNo);
	TFile* inroot=new TFile(hname);
	TTree* T =  (TTree*) inroot->Get("T");
	T->SetBranchAddress("TPCWF",&ed.TPCWF);
	T->SetBranchAddress("PMTWF",&ed.PMTWF);
	
	ReadTPCBaselines();
	
	sprintf(hname,"HitAnalysis2_%d.root",RunNo);
	TFile* outroot=new TFile(hname,"recreate");
	
	TGraph* Col_vs_t=new TGraph();
	Col_vs_t->SetName("Col_vs_t");Col_vs_t->SetTitle("Col_vs_t");
	Col_vs_t->SetMarkerStyle(25);Col_vs_t->SetMarkerColor(1);
	
	TGraph* Ind_vs_t=new TGraph();
	Ind_vs_t->SetName("Ind_vs_t");Ind_vs_t->SetTitle("Ind_vs_t");
	Ind_vs_t->SetMarkerStyle(26);Ind_vs_t->SetMarkerColor(2);
	
	TH2F* Col_vs_Ind=new TH2F("Col_vs_Ind","Col_vs_Ind",128,-0.5,127.5,128,-0.5,127.5);
	
	int ic=0;int gch=0;
	float ss=0;
	outroot->cd();
	for(int I=0;I<T->GetEntries();I++)
	{
		T->GetEntry(I);
		Col_vs_t->Set(0);Ind_vs_t->Set(0);
		for(int i1=0;i1<256;i1++)
		{
			ic=i1/128;gch=i1-((i1/128)*128);
			for(int i2=750;i2<2500;i2++)
			{
				ss=((float)ed.TPCWF->at(i1)[i2])-TPCBaselines[ic][gch][0];
				if(i1<128)
				{
					if(ss>(TPCBaselines[ic][gch][1]*ThSigma[ic]))
					{
						Col_vs_t->SetPoint(Col_vs_t->GetN(),i2,gch);
					}
				}
				else
				{
					ss*=-1;
					if(ss>(TPCBaselines[ic][gch][1]*ThSigma[ic]))
					{
						Ind_vs_t->SetPoint(Ind_vs_t->GetN(),i2,gch);
					}
				}
			}
		}
// 		for(int i2=750;i2<2500;i2++)
// 		{
// 			for(int i1=0;i1<256;i1++)
// 			{
// 				ic=i1/128;gch=i1-((i1/128)*128);
// 				ss=((float)ed.TPCWF->at(i1)[i2])-TPCBaselines[ic][gch][0];
				
				
				
				
		
		
		sprintf(hname,"Col_vs_t_%d",I);
		Col_vs_t->SetName(hname);Col_vs_t->SetTitle(hname);
		Col_vs_t->Write();
		sprintf(hname,"Ind_vs_t_%d",I);
		Ind_vs_t->SetName(hname);Ind_vs_t->SetTitle(hname);
		Ind_vs_t->Write();
		if(I==10) break;
	}
	outroot->Close();
}