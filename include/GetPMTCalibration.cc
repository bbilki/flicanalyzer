void GetPMTCalibration()
{
// 	ReadPMTBaselines();
// 	sprintf(hname,"%s/Histos/PMTCalibration_%d.root",AnalysisFilePath,RunNo);
// 	TFile* outroot=new TFile(hname,"recreate");
// 	
// 	TH1F* hh[4][3];
// 	
// 	for(int i1=0;i1<3;i1++)
// 	{
// 		sprintf(hname,"BL_amp_%d",i1);
// 		hh[0][i1]=new TH1F(hname,hname,1500,-1000,2000);
// 		sprintf(hname,"BL_amp2_%d",i1);
// 		hh[1][i1]=new TH1F(hname,hname,1500,-1000,2000);
// 		sprintf(hname,"BL_amp3_%d",i1);
// 		hh[2][i1]=new TH1F(hname,hname,1500,-1000,2000);
// 		sprintf(hname,"BL_amp4_%d",i1);
// 		hh[3][i1]=new TH1F(hname,hname,1500,-1000,2000);
// 	}
// 	
// 	TF1* lin=new TF1("lin","[0]",0.,4095.);
// 	TH1F* hb=new TH1F("hb","hb",15000,-0.5,14999.5);
// // 	TF1* tf=new TF1("GL","gaus(0)+landau(3)",-100.,200.);
// // 	TF1* tf=new TF1("GLG","gaus(0)+landau(3)+gaus(6)",-100.,200.);
// 	TF1* tf=new TF1("GGG","gaus(0)+gaus(3)+gaus(6)",-100.,200.);
// 	
// 	char cNum[10];
// 	int ii=0;int Ev=0;
// 	ifstream pmtinfile;
// 	for(int i1=0;i1<3;i1++)
// 	{
// 		Ev=0;
// 		sprintf(hname,"%s/waveforms_ch%d_run%d.txt",PMTinFilePath,i1,RunNo);
// 		cout<<hname<<endl;
// 		pmtinfile.open(hname);
// 		string strarr;
// 		while(getline(pmtinfile,strarr,'\n'))
// 		{
// 			istringstream iss(strarr);
// 			ii=0;
// // 			float qint=0;
// 			float qint[4]={0};
// 			for(std::string s; iss >> s; )
// 			{
// 				if(ii>0)
// 				{
// 					hb->SetBinContent(ii,atof(s.c_str())-PMTBaselines[i1][0]);
// // 					qint+=((-1.)*(atof(s.c_str())-PMTBaselines[i1][0]));
// 					for(int is1=0;is1<4;is1++)
// 					{
// 						qint[is1]+=((-1.)*(atof(s.c_str())-PMTBaselines[i1][0]));
// 					}
// 				}
// 				ii++;
// 				if(ii%20==0){hh[0][i1]->Fill(qint[0]);qint[0]=0;}
// 				if(ii%10==0){hh[1][i1]->Fill(qint[1]);qint[1]=0;}
// 				if(ii%100==0){hh[2][i1]->Fill(qint[2]);qint[2]=0;}
// 				hh[3][i1]->Fill(qint[3]);qint[3]=0;
// // 				hh[i1]->Fill(qint);qint=0;
// 			}
// // 			hh[i1]->Fill(qint);
// 			Ev++;
// 		}
// 		pmtinfile.close();
// 	}
// 	
// 	for(int i1=0;i1<3;i1++)
// 	{
// 		for(int i2=0;i2<4;i2++)
// 		{
// 			tf->SetParameter(0,hh[i2][i1]->GetBinContent(hh[i2][i1]->GetMaximumBin()));
// 			tf->SetParameter(1,0);
// 			tf->SetParameter(2,4);
// 			tf->SetParameter(3,hh[i2][i1]->GetBinContent(hh[i2][i1]->GetMaximumBin())/1000);
// 			tf->SetParameter(4,30);
// 			tf->SetParameter(5,10);
// 			tf->SetParameter(6,hh[i2][i1]->GetBinContent(hh[i2][i1]->GetMaximumBin())/1e6);
// 			tf->SetParameter(7,100);
// 			tf->SetParameter(8,50);
// 			
// 			hh[i2][i1]->Fit(tf,"q","q",-100.,200.);
// 		}
// 		
// // 		cout<<"PMT "<<i1<<" "<<(tf->GetParameter(4)-tf->GetParameter(1))<<" ("<<sqrt(pow(tf->GetParError(4),2)+pow(tf->GetParError(1),2))<<")"<<endl;
// 	}
// 	
// 	
// // 	sprintf(hname,"Files/Calibrations_%d.txt",RunNo);
// // 	ofstream outfile(hname);
// 	
// 	outroot->cd();
// 	for(int i1=0;i1<3;i1++)
// 	{
// 		for(int i2=0;i2<4;i2++)
// 		{
// // 		outfile<<i1<<" "<<hh[i1][1]->GetBinCenter(hh[i1][1]->GetMaximumBin())<<endl;
// 			hh[i2][i1]->Write();
// 		}
// 	}
// // 	outfile.close();
// 	outroot->Close();
}