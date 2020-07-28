void CompareTPCBaselines()
{
	vector <int> RunNos;
	if(RunNo==1000)
	{
		RunNos.push_back(7075);
		RunNos.push_back(7077);
		RunNos.push_back(7080);
		RunNos.push_back(7083);
		RunNos.push_back(7085);
		RunNos.push_back(7086);
		RunNos.push_back(7088);
		RunNos.push_back(7090);
		RunNos.push_back(7093);
		RunNos.push_back(7095);
		RunNos.push_back(7098);
		RunNos.push_back(7101);
		RunNos.push_back(7103);
		RunNos.push_back(7105);
		RunNos.push_back(7107);
		RunNos.push_back(7108);
		RunNos.push_back(7120);
		RunNos.push_back(7122);
		RunNos.push_back(7127);
		RunNos.push_back(7130);
	}
	else if(RunNo==2000)
	{
		RunNos.push_back(7240);
		RunNos.push_back(7242);
		RunNos.push_back(7243);
		RunNos.push_back(7244);
		RunNos.push_back(7246);
		RunNos.push_back(7247);
		RunNos.push_back(7248);
		RunNos.push_back(7251);
		RunNos.push_back(7253);
		RunNos.push_back(7255);
		RunNos.push_back(7258);
		RunNos.push_back(7260);
		RunNos.push_back(7261);
		RunNos.push_back(7262);
		RunNos.push_back(7265);
		RunNos.push_back(7267);
		RunNos.push_back(7269);
		RunNos.push_back(7270);
		RunNos.push_back(7272);
		RunNos.push_back(7274);
		RunNos.push_back(7276);
		RunNos.push_back(7278);
		RunNos.push_back(7280);
		RunNos.push_back(7282);
		RunNos.push_back(7284);
	}
	else if(RunNo==3000)
	{
		RunNos.push_back(7307);
		RunNos.push_back(7309);
		RunNos.push_back(7311);
		RunNos.push_back(7313);
		RunNos.push_back(7315);
		RunNos.push_back(7317);
		RunNos.push_back(7319);
		RunNos.push_back(7321);
		RunNos.push_back(7324);
// 		RunNos.push_back(7326);//not good??
		RunNos.push_back(7328);
		RunNos.push_back(7330);
		RunNos.push_back(7332);
		RunNos.push_back(7335);
		RunNos.push_back(7337);
// 		RunNos.push_back(7343);//failed run
// 		RunNos.push_back(7347);//not good high wire bias
		RunNos.push_back(7350);
		RunNos.push_back(7352);
		RunNos.push_back(7367);
		RunNos.push_back(7370);
		RunNos.push_back(7372);
		RunNos.push_back(7378);
		RunNos.push_back(7380);
		RunNos.push_back(7386);
		RunNos.push_back(7390);
		RunNos.push_back(7391);
		RunNos.push_back(7393);
		RunNos.push_back(7396);
// 		RunNos.push_back(7401);
// 		RunNos.push_back(7403);
	}
	
	sprintf(hname,"CompareTPCBaselines_%d.root",RunNo);
	TFile* outroot=new TFile(hname,"recreate");
	TGraphErrors* tg[RunNos.size()];
	
	ifstream inTPCBaselines;
	int a[2]={0};float b=0;float c=0;
	
	for(int i1=0;i1<RunNos.size();i1++)
	{
		sprintf(hname,"Run_%d",RunNos[i1]);
		tg[i1]=new TGraphErrors();
		tg[i1]->SetName(hname);tg[i1]->SetTitle(hname);
		tg[i1]->SetMarkerStyle(20);tg[i1]->SetMarkerColor(1+(i1%4));
		tg[i1]->GetXaxis()->SetTitle("Wire ID");tg[i1]->GetXaxis()->CenterTitle();
		tg[i1]->GetYaxis()->SetTitle("Baseline");tg[i1]->GetYaxis()->CenterTitle();
		
		sprintf(hname,"%s/Files/TPCBaselines_%d.txt",AnalysisFilePath,RunNos[i1]);
		inTPCBaselines.open(hname);
		while(!inTPCBaselines.eof())
		{
			inTPCBaselines>>a[0]>>a[1]>>b>>c;
			tg[i1]->SetPoint(tg[i1]->GetN(),128*a[0]+a[1],b);
			tg[i1]->SetPointError(tg[i1]->GetN()-1,0,c);
		}
		inTPCBaselines.close();
	}
	
	outroot->cd();
	sprintf(hname,"CC_%d",RunNo);
	TCanvas* cc=new TCanvas(hname,hname,600,600);
	for(int i1=0;i1<RunNos.size();i1++)
	{
		if(i1==0) tg[i1]->Draw("ap");
		else tg[i1]->Draw("p same");
		tg[i1]->Write();
	}
	cc->Write();
	outroot->Close();
	sprintf(hname,"mv CompareTPCBaselines_%d.root %s/Histos/;wait;",RunNo,AnalysisFilePath);system(hname);
}