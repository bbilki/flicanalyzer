void FindDeltaT()
{
// 	sprintf(hname,"%s/Histos/DeltaTFinding_%d.root",AnalysisFilePath,RunNo);
// 	TFile* outroot=new TFile(hname,"recreate");
// 	TH1F* Tfirst=new TH1F("Tfirst","Tfirst",4096,-0.5,4095.5);
// 	TH1F* Tlast=new TH1F("Tlast","Tlast",4096,-0.5,4095.5);
// 	TH1F* DeltaT=new TH1F("DeltaT","DeltaT",4096,-0.5,4095.5);
// 	
// 	sprintf(hname,"%s/Run00%d.dat",TPCinFilePath,RunNo);
// 	file.open(hname, ios::in|ios::binary|ios::ate);
// 	
// 	ReadTPCBaselines();
// 	
// 	int Ev=0;
// 	int integral=0;int integral2=0;int integral3=0;
// 	int peakbin=0;
// 	int tmin=-1;int tmax=0;
// 	float ss=0;
// 	file.seekg(0, ios::beg);
// 	while(!file.eof())
// 	{
// 		ReadEvent();
// 		
// 		tmin=-1;
// 		for(int i2=0;i2<4096;i2++)
// 		{
// 			for(int i1=0;i1<256;i1++)
// 			{
// 				if(IC[i1]!=0) continue;
// 				ss=((float)e.signal[i1][i2])-TPCBaselines[IC[i1]][CH[i1]][0];
// 				if(ss>ThCol)
// 				{
// 					if(tmin==-1) tmin=i2;
// 					tmax=i2;
// 				}
// 			}
// 		}
// 		Tfirst->Fill(tmin);
// 		Tlast->Fill(tmax);
// 		DeltaT->Fill(tmax-tmin);
// 			
// 		Ev++;
// 		for(int i1=0;i1<256;i1++)
// 		{
// 			e.signal[i1].clear();
// 		}
// 		if(Ev%10==0) cout<<"Event: "<<Ev<<endl;
// 	}
// 	file.close();
// 	outroot->cd();
// 	outroot->Write();
// 	outroot->Close();
}