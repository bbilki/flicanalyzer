void ReadTPCBaselines()
{
	sprintf(hname,"cp %s/Files/TPCBaselines_%d.txt .;wait;",AnalysisFilePath,TPCBaselineRun);system(hname);
	sprintf(hname,"TPCBaselines_%d.txt",TPCBaselineRun);
	ifstream inTPCBaselines(hname);
	int a[2]={0};float b=0;float c=0;
	while(!inTPCBaselines.eof())
	{
		inTPCBaselines>>a[0]>>a[1]>>b>>c;
		TPCBaselines[a[0]][a[1]][0]=b;
		TPCBaselines[a[0]][a[1]][1]=c;
	}
	inTPCBaselines.close();
	sprintf(hname,"rm TPCBaselines_%d.txt",TPCBaselineRun);system(hname);
	
	sprintf(hname,"%s/Histos/Baselines_%d.root",AnalysisFilePath,TPCBaselineRun);
	TFile* inroot=new TFile(hname);
	TF1* g;TH1D* h1;
	inroot->GetObject("AllCollAmp",h1);g=(TF1*)h1->GetFunction("gaus");
	ThColInd[0]=g->GetParameter(2)*ThSigma[0];
	inroot->GetObject("AllIndAmp",h1);g=(TF1*)h1->GetFunction("gaus");
	ThColInd[1]=g->GetParameter(2)*ThSigma[1];
	
// 	ThColInd[0]=g->GetParameter(2)*4.;//3.5 sigma
// 	ThColInd[1]=g->GetParameter(2)*4.;//3.5 sigma
	
// 	cout<<"float TPCBaselines[256]={";
// 	for(int i1=0;i1<2;i1++)
// 	{
// 		for(int i2=0;i2<128;i2++)
// 		{
// 			cout<<TPCBaselines[i1][i2][0];
// 			if(!(i1==1 && i2==127)) cout<<",";
// 			else cout<<"}"<<endl;
// 		}
// 	}
	
	inroot->Close();
	cout<<"Collection Threshold = "<<ThColInd[0]<<" Induction Threshold = "<<ThColInd[1]<<endl;
}