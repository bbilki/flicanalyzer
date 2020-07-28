void ReadPMTBaselines()
{
	sprintf(hname,"cp %s/Files/PMTBaselines_%d.txt .;wait;",AnalysisFilePath,PMTBaselineRun);system(hname);
	sprintf(hname,"PMTBaselines_%d.txt",PMTBaselineRun);
	ifstream inPMTBaselines(hname);
	int a[2]={0};float c(0);float b(0);
	while(!inPMTBaselines.eof())
	{
		inPMTBaselines>>a[0]>>b>>c;
		PMTBaselines[a[0]][0]=b;
		PMTBaselines[a[0]][1]=c;
	}
	inPMTBaselines.close();
	sprintf(hname,"rm PMTBaselines_%d.txt",PMTBaselineRun);system(hname);
	
	sprintf(hname,"%s/Histos/Baselines_%d.root",AnalysisFilePath,PMTBaselineRun);
	TFile* inroot=new TFile(hname);
	TF1* g;TH1D* h1;
	for(int i1=0;i1<3;i1++)
	{
		sprintf(hname,"AllPMTAmp_%d",i1);
		inroot->GetObject(hname,h1);g=(TF1*)h1->GetFunction("gaus");ThPMT[i1]=g->GetParameter(2)*3.;//3 sigma
		cout<<"PMT "<<i1<<" Threshold = "<<ThPMT[i1]<<endl;
	}
	inroot->Close();
}