void PMTCalibrationsAll2()
{
	int NR1L=28;
	int NR2L=18;
	int NR3L=21;
	
	int Run1List[28]={50,51,7074,7076,57,59,60,61,7084,70,71,74,75,76,78,80,7110,82,83,84,7126,85,7131,7132,7136,7141,7142,7143};//7087,7081,7082,7096,7099,7100,7123,7128,7129,7104,7107,7121,7140,7135,7133,7134,7144,7137,7138,7139,7092,77,86,79,81,68,7078,7089,7094,7125,7106
	int Run2List[18]={7241,7245,99,100,7254,101,102,7259,103,7264,104,105,7268,106,7271,107,7273,7281};//7252,7279,7287,92,94,96,97,93,95,98,7257,7263,108,7275,109,110,112,113,114,7266,7277 weird ,7256,7285,7283
	int Run3List[21]={116,117,118,119,120,121,7320,122,7322,7323,123,7325,124,125,126,127,128,129,7369,7394,7397};//,130,132,133,134,135,136,137,138,139 rate ,7371,7374,7375 something wrong with LXe ,7312,7314,7316 /// ,7331,7334,7336,7385,7389,7392 ,7318,7327 115,
	
	TGraphErrors* tge[3][3][3];//pmt runperiod method
	for(int i1=0;i1<3;i1++)
	{
		if(i1==1) continue;
		for(int i2=0;i2<3;i2++)
		{
			for(int i3=0;i3<2;i3++)
			{
				sprintf(hname,"PMT_%s_Run%d_M%d",PMTNames[i1].c_str(),i2+1,i3+1);
				tge[i1][i2][i3]=new TGraphErrors();
				tge[i1][i2][i3]->SetName(hname);tge[i1][i2][i3]->SetTitle(hname);
				tge[i1][i2][i3]->SetMarkerStyle(20+i3);
				tge[i1][i2][i3]->SetMarkerColor(1+i3*3+i1);
				tge[i1][i2][i3]->GetXaxis()->SetTitle("Run ID");tge[i1][i2][i3]->GetXaxis()->CenterTitle();
				tge[i1][i2][i3]->GetYaxis()->SetTitle("Single Photoelectron Charge (arbitrary units)");tge[i1][i2][i3]->GetYaxis()->CenterTitle();
			}
			sprintf(hname,"PMT_%s_Run%d_All",PMTNames[i1].c_str(),i2+1);
			tge[i1][i2][2]=new TGraphErrors();
			tge[i1][i2][2]->SetName(hname);tge[i1][i2][2]->SetTitle(hname);
			tge[i1][i2][2]->SetMarkerStyle(25);
			tge[i1][i2][2]->SetMarkerColor(4);
			tge[i1][i2][2]->GetXaxis()->SetTitle("Run ID");tge[i1][i2][2]->GetXaxis()->CenterTitle();
			tge[i1][i2][2]->GetYaxis()->SetTitle("Single Photoelectron Charge (arbitrary units)");tge[i1][i2][2]->GetYaxis()->CenterTitle();
		}
	}
	
	TFile* outroot=new TFile("PMTCalibrationsAll.root","recreate");
	
	TH1F* hh;TF1* tf1;
	TF1* tf2=new TF1("GG","gaus(0)+gaus(3)",0.,200.);
	TF1* tf3=new TF1("gaus","gaus",0.,300.);
	TF1* tf4=new TF1("gg","[0]*exp(-0.5*((x-[1])/[2])^2)+[3]*exp(-0.5*((x-(2.*[1]))/[4])^2)",0.,300.);
	TF1* tf5=new TF1("ggg","[0]*exp(-0.5*((x-[1])/[2])^2)+[3]*exp(-0.5*((x-(2.*[1]))/[4])^2)+[5]*exp(-0.5*((x-[6])/[7])^2)",0.,300.);
	TF1* tf6=new TF1("ggg2","gaus(0)+gaus(3)+gaus(6)",0.,300.);
// 	TF1* tf7=new TF1("gl","gaus(0)+landau(3)",0.,300.);
	TF1* tf7=new TF1("gg","gaus(0)+gaus(3)",0.,300.);
	TF1* tf8=new TF1("l","landau",0.,300.);
	TFile* inroot;
	float fitulim[3]={45,0,35};float fitlims[2]={0};
	int maxbin1=0;int maxbin2=0;int minbin1=0;
	int minbin=0;int maxbin=0;
	float xmin=0;float xmax=0;
	float fitchi21=0;float fitchi22=0;
	for(int i1=0;i1<NR1L;i1++)
	{
// 		if(Run1List[i1]==52) continue;
		cout<<i1<<" "<<Run1List[i1];
		if(Run1List[i1]<7000)
		{
			sprintf(hname,"%s/Histos/PMTCalibration_%d.root",AnalysisFilePath,Run1List[i1]);
			inroot=new TFile(hname);
			for(int i2=0;i2<3;i2++)
			{
				if(i2==1) continue;
				sprintf(hname,"SignalWindow2_%d",i2);
				inroot->GetObject(hname,hh);
				
				maxbin1=hh->GetMaximumBin();
				hh->GetXaxis()->SetRangeUser(hh->GetBinCenter(maxbin1),30);
				minbin1=hh->GetMinimumBin();
				hh->GetXaxis()->SetRangeUser(hh->GetBinCenter(minbin1),60);
				maxbin2=hh->GetMaximumBin();
				hh->GetXaxis()->SetRangeUser(-50,500);
				
				for(int ik1=maxbin1;ik1>=1;ik1--)
				{
					if((hh->GetBinContent(maxbin1)/hh->GetBinContent(ik1))>100){minbin=ik1;break;}
				}
				for(int ik1=maxbin2;ik1<=hh->GetNbinsX();ik1++)
				{
					if((hh->GetBinContent(maxbin2)/hh->GetBinContent(ik1))>5){maxbin=ik1;break;}
				}
				tf2->SetParameter(0,hh->GetBinContent(maxbin1));
				tf2->SetParameter(1,hh->GetBinCenter(maxbin1));
				tf2->SetParameter(2,5);
				tf2->SetParameter(3,hh->GetBinContent(maxbin2));
				tf2->SetParameter(4,hh->GetBinCenter(maxbin2));
				tf2->SetParameter(5,20);
				tf2->SetParLimits(5,0,50);
				hh->Fit(tf2,"q","q",hh->GetBinCenter(minbin),hh->GetBinCenter(maxbin));
				tge[i2][0][0]->SetPoint(tge[i2][0][0]->GetN(),i1,tf2->GetParameter(4)-tf2->GetParameter(1));
				tge[i2][0][0]->SetPointError(tge[i2][0][0]->GetN()-1,0,sqrt(pow(tf2->GetParError(4),2)+pow(tf2->GetParError(1),2)));
				tge[i2][0][2]->SetPoint(tge[i2][0][2]->GetN(),i1,tf2->GetParameter(4)-tf2->GetParameter(1));
				tge[i2][0][2]->SetPointError(tge[i2][0][2]->GetN()-1,0,sqrt(pow(tf2->GetParError(4),2)+pow(tf2->GetParError(1),2)));
// 				cout<<" "<<i2<<" "<<(tf2->GetParameter(4)-tf2->GetParameter(1))<<" "<<sqrt(pow(tf2->GetParError(4),2)+pow(tf2->GetParError(1),2))<<endl;
				
				sprintf(hname,"%d_%d",Run1List[i1],i2);
				hh->SetName(hname);hh->SetTitle(hname);
				outroot->cd();hh->Write();
			}
			inroot->Close();
		}
		else
		{
			sprintf(hname,"%s/Histos/PMTCalibration3_%d.root",AnalysisFilePath,Run1List[i1]);
			inroot=new TFile(hname);
			for(int i2=0;i2<3;i2++)
			{
				if(i2==1) continue;
				sprintf(hname,"Integral_MAgt2_wgt10_%d",i2);
				inroot->GetObject(hname,hh);
				
				if(i2==2 && Run1List[i1]<=7084)
				{
					fitlims[0]=25;fitlims[1]=55;
					if(Run1List[i1]==7084){fitlims[0]=15;fitlims[1]=45;}
					hh->Fit(tf3,"q","q",fitlims[0],fitlims[1]);
					tge[i2][0][1]->SetPoint(tge[i2][0][1]->GetN(),i1,tf3->GetParameter(1));
					tge[i2][0][1]->SetPointError(tge[i2][0][1]->GetN()-1,0,tf3->GetParError(1));
					tge[i2][0][2]->SetPoint(tge[i2][0][2]->GetN(),i1,tf3->GetParameter(1));
					tge[i2][0][2]->SetPointError(tge[i2][0][2]->GetN()-1,0,tf3->GetParError(1));
				}
				else if(i2==2)
				{
					fitlims[0]=0;fitlims[1]=50;
					hh->Fit(tf8,"q","q",fitlims[0],fitlims[1]);
					tge[i2][0][1]->SetPoint(tge[i2][0][1]->GetN(),i1,tf8->GetParameter(1));
					tge[i2][0][1]->SetPointError(tge[i2][0][1]->GetN()-1,0,tf8->GetParError(1));
					tge[i2][0][2]->SetPoint(tge[i2][0][2]->GetN(),i1,tf8->GetParameter(1));
					tge[i2][0][2]->SetPointError(tge[i2][0][2]->GetN()-1,0,tf8->GetParError(1));
				}
				else
				{
					tf4->SetParameter(0,hh->GetBinContent(hh->GetMaximumBin()));
					tf4->SetParameter(1,hh->GetBinCenter(hh->GetMaximumBin()));
					tf4->SetParameter(2,5);
					tf4->SetParameter(3,hh->GetBinContent(hh->GetMaximumBin())/10);
					tf4->SetParameter(4,20);
					
					hh->Fit(tf4,"q","q",0.,100.);
	// 				hh->Fit(tf4,"q","q",tf4->GetParameter(1)-2*tf4->GetParameter(2),2*tf4->GetParameter(1)+2*tf4->GetParameter(4));
					hh->Fit(tf4,"q","q",tf4->GetParameter(1)-2*tf4->GetParameter(2),2*tf4->GetParameter(1)+20.);
	// 				
					tge[i2][0][1]->SetPoint(tge[i2][0][1]->GetN(),i1,tf4->GetParameter(1));
					tge[i2][0][1]->SetPointError(tge[i2][0][1]->GetN()-1,0,tf4->GetParError(1));
					tge[i2][0][2]->SetPoint(tge[i2][0][2]->GetN(),i1,tf4->GetParameter(1));
					tge[i2][0][2]->SetPointError(tge[i2][0][2]->GetN()-1,0,tf4->GetParError(1));
	// 				cout<<" "<<i2<<" "<<tf3->GetParameter(1)<<" "<<tf3->GetParError(1)<<endl;
				}
				
				sprintf(hname,"%d_%d",Run1List[i1],i2);
				hh->SetName(hname);hh->SetTitle(hname);
				outroot->cd();hh->Write();
			}
			inroot->Close();
		}
	}
	for(int i1=0;i1<NR2L;i1++)
	{
// 		if(Run2List[i1]==88 || Run2List[i1]==91) continue;
// 		cout<<i1<<" "<<Run2List[i1];
		if(Run2List[i1]<7000)
		{
			sprintf(hname,"%s/Histos/PMTCalibration_%d.root",AnalysisFilePath,Run2List[i1]);
			inroot=new TFile(hname);
			for(int i2=0;i2<3;i2++)
			{
				if(i2==1) continue;
				sprintf(hname,"SignalWindow2_%d",i2);
				inroot->GetObject(hname,hh);
				
				maxbin1=hh->GetMaximumBin();
				hh->GetXaxis()->SetRangeUser(hh->GetBinCenter(maxbin1),30);
				minbin1=hh->GetMinimumBin();
				hh->GetXaxis()->SetRangeUser(hh->GetBinCenter(minbin1),60);
				maxbin2=hh->GetMaximumBin();
				hh->GetXaxis()->SetRangeUser(-50,500);
				
				for(int ik1=maxbin1;ik1>=1;ik1--)
				{
					if((hh->GetBinContent(maxbin1)/hh->GetBinContent(ik1))>100){minbin=ik1;break;}
				}
				for(int ik1=maxbin2;ik1<=hh->GetNbinsX();ik1++)
				{
					if((hh->GetBinContent(maxbin2)/hh->GetBinContent(ik1))>5){maxbin=ik1;break;}
				}
				tf2->SetParameter(0,hh->GetBinContent(maxbin1));
				tf2->SetParameter(1,hh->GetBinCenter(maxbin1));
				tf2->SetParameter(2,5);
				tf2->SetParameter(3,hh->GetBinContent(maxbin2));
				tf2->SetParameter(4,hh->GetBinCenter(maxbin2));
				tf2->SetParameter(5,20);
				tf2->SetParLimits(5,0,50);
				hh->Fit(tf2,"q","q",hh->GetBinCenter(minbin),hh->GetBinCenter(maxbin));
				tge[i2][1][0]->SetPoint(tge[i2][1][0]->GetN(),i1,tf2->GetParameter(4)-tf2->GetParameter(1));
				tge[i2][1][0]->SetPointError(tge[i2][1][0]->GetN()-1,0,sqrt(pow(tf2->GetParError(4),2)+pow(tf2->GetParError(1),2)));
				tge[i2][1][2]->SetPoint(tge[i2][1][2]->GetN(),i1,tf2->GetParameter(4)-tf2->GetParameter(1));
				tge[i2][1][2]->SetPointError(tge[i2][1][2]->GetN()-1,0,sqrt(pow(tf2->GetParError(4),2)+pow(tf2->GetParError(1),2)));
// 				cout<<" "<<i2<<" "<<(tf2->GetParameter(4)-tf2->GetParameter(1))<<" "<<sqrt(pow(tf2->GetParError(4),2)+pow(tf2->GetParError(1),2))<<endl;
				
				sprintf(hname,"%d_%d",Run2List[i1],i2);
				hh->SetName(hname);hh->SetTitle(hname);
				outroot->cd();hh->Write();
			}
			inroot->Close();
		}
		else
		{
			sprintf(hname,"%s/Histos/PMTCalibration3_%d.root",AnalysisFilePath,Run2List[i1]);
			inroot=new TFile(hname);
			for(int i2=0;i2<3;i2++)
			{
				if(i2==1) continue;
				sprintf(hname,"Integral_MAgt2_wgt10_%d",i2);
				inroot->GetObject(hname,hh);
				
				tf4->SetParameter(0,hh->GetBinContent(hh->GetMaximumBin()));
				tf4->SetParameter(1,hh->GetBinCenter(hh->GetMaximumBin()));
				tf4->SetParameter(2,5);
				tf4->SetParameter(3,hh->GetBinContent(hh->GetMaximumBin())/10);
				tf4->SetParameter(4,20);
				
				hh->Fit(tf4,"q","q",0.,100.);
// 				hh->Fit(tf4,"q","q",tf4->GetParameter(1)-2*tf4->GetParameter(2),2*tf4->GetParameter(1)+2*tf4->GetParameter(4));
				hh->Fit(tf4,"q","q",tf4->GetParameter(1)-2*tf4->GetParameter(2),2*tf4->GetParameter(1)+20.);
// 				
// 				if(tf4->GetChisquare()/tf4->GetNDF()>10)
// 				{
// 					maxbin1=hh->GetMaximumBin();
// 					hh->GetXaxis()->SetRangeUser(hh->GetBinCenter(maxbin1),hh->GetBinCenter(maxbin1)+20.);
// 					minbin1=hh->GetMinimumBin();
// 					hh->GetXaxis()->SetRangeUser(hh->GetBinCenter(minbin1),60);
// 					maxbin2=hh->GetMaximumBin();
// 					hh->GetXaxis()->SetRangeUser(-50,500);
// 					
// 					hh->Fit(tf4,"q","q",hh->GetBinCenter(minbin1),100.);
// // 					hh->Fit(tf4,"q","q",hh->GetBinCenter(minbin1),2*tf5->GetParameter(1)+2*tf5->GetParameter(4));
// 					hh->Fit(tf4,"q","q",hh->GetBinCenter(minbin1),2*tf4->GetParameter(1)+20.);
// 				}
				tge[i2][1][1]->SetPoint(tge[i2][1][1]->GetN(),i1,tf4->GetParameter(1));
				tge[i2][1][1]->SetPointError(tge[i2][1][1]->GetN()-1,0,tf4->GetParError(1));
				tge[i2][1][2]->SetPoint(tge[i2][1][2]->GetN(),i1,tf4->GetParameter(1));
				tge[i2][1][2]->SetPointError(tge[i2][1][2]->GetN()-1,0,tf4->GetParError(1));
// 				cout<<" "<<i2<<" "<<tf3->GetParameter(1)<<" "<<tf3->GetParError(1)<<endl;
				
				sprintf(hname,"%d_%d",Run2List[i1],i2);
				hh->SetName(hname);hh->SetTitle(hname);
				outroot->cd();hh->Write();
			}
			inroot->Close();
		}
	}
	for(int i1=0;i1<NR3L;i1++)
	{
// 		if(Run3List[i1]==131) continue;
		cout<<i1<<" "<<Run3List[i1];
		if(Run3List[i1]<7000)
		{
			sprintf(hname,"%s/Histos/PMTCalibration_%d.root",AnalysisFilePath,Run3List[i1]);
			inroot=new TFile(hname);
			for(int i2=0;i2<3;i2++)
			{
				if(i2==1) continue;
				sprintf(hname,"SignalWindow2_%d",i2);
				inroot->GetObject(hname,hh);
				
				maxbin1=hh->GetMaximumBin();
				hh->GetXaxis()->SetRangeUser(hh->GetBinCenter(maxbin1),30);
				minbin1=hh->GetMinimumBin();
				hh->GetXaxis()->SetRangeUser(hh->GetBinCenter(minbin1),60);
				maxbin2=hh->GetMaximumBin();
				hh->GetXaxis()->SetRangeUser(-50,500);
				
				for(int ik1=maxbin1;ik1>=1;ik1--)
				{
					if((hh->GetBinContent(maxbin1)/hh->GetBinContent(ik1))>100){minbin=ik1;break;}
				}
				for(int ik1=maxbin2;ik1<=hh->GetNbinsX();ik1++)
				{
					if((hh->GetBinContent(maxbin2)/hh->GetBinContent(ik1))>5){maxbin=ik1;break;}
				}
				tf2->SetParameter(0,hh->GetBinContent(maxbin1));
				tf2->SetParameter(1,hh->GetBinCenter(maxbin1));
				tf2->SetParameter(2,5);
				tf2->SetParameter(3,hh->GetBinContent(maxbin2));
				tf2->SetParameter(4,hh->GetBinCenter(maxbin2));
				tf2->SetParameter(5,20);
				tf2->SetParLimits(5,0,50);
				hh->Fit(tf2,"q","q",hh->GetBinCenter(minbin),hh->GetBinCenter(maxbin));
				tge[i2][2][0]->SetPoint(tge[i2][2][0]->GetN(),i1,tf2->GetParameter(4)-tf2->GetParameter(1));
				tge[i2][2][0]->SetPointError(tge[i2][2][0]->GetN()-1,0,sqrt(pow(tf2->GetParError(4),2)+pow(tf2->GetParError(1),2)));
				tge[i2][2][2]->SetPoint(tge[i2][2][2]->GetN(),i1,tf2->GetParameter(4)-tf2->GetParameter(1));
				tge[i2][2][2]->SetPointError(tge[i2][2][2]->GetN()-1,0,sqrt(pow(tf2->GetParError(4),2)+pow(tf2->GetParError(1),2)));
// 				cout<<" "<<i2<<" "<<(tf2->GetParameter(4)-tf2->GetParameter(1))<<" "<<sqrt(pow(tf2->GetParError(4),2)+pow(tf2->GetParError(1),2))<<endl;
				
				sprintf(hname,"%d_%d",Run3List[i1],i2);
				hh->SetName(hname);hh->SetTitle(hname);
				outroot->cd();hh->Write();
			}
			inroot->Close();
			
		}
		else
		{
			sprintf(hname,"%s/Histos/PMTCalibration3_%d.root",AnalysisFilePath,Run3List[i1]);
			inroot=new TFile(hname);
			for(int i2=0;i2<3;i2++)
			{
				if(i2==1) continue;
				sprintf(hname,"Integral_MAgt2_wgt10_%d",i2);
				inroot->GetObject(hname,hh);
				
				tf4->SetParameter(0,hh->GetBinContent(hh->GetMaximumBin()));
				tf4->SetParameter(1,hh->GetBinCenter(hh->GetMaximumBin()));
				tf4->SetParameter(2,5);
				tf4->SetParameter(3,hh->GetBinContent(hh->GetMaximumBin())/10);
				tf4->SetParameter(4,20);
				
				hh->Fit(tf4,"q","q",0.,100.);
// 				hh->Fit(tf4,"q","q",tf4->GetParameter(1)-2*tf4->GetParameter(2),2*tf4->GetParameter(1)+2*tf4->GetParameter(4));
				hh->Fit(tf4,"q","q",tf4->GetParameter(1)-2*tf4->GetParameter(2),2*tf4->GetParameter(1)+20.);
// 				
// 				if(tf4->GetChisquare()/tf4->GetNDF()>10)
// 				{
// 					maxbin1=hh->GetMaximumBin();
// 					hh->GetXaxis()->SetRangeUser(hh->GetBinCenter(maxbin1),hh->GetBinCenter(maxbin1)+20.);
// 					minbin1=hh->GetMinimumBin();
// 					hh->GetXaxis()->SetRangeUser(hh->GetBinCenter(minbin1),60);
// 					maxbin2=hh->GetMaximumBin();
// 					hh->GetXaxis()->SetRangeUser(-50,500);
// 					
// 					hh->Fit(tf4,"q","q",hh->GetBinCenter(minbin1),100.);
// // 					hh->Fit(tf4,"q","q",hh->GetBinCenter(minbin1),2*tf5->GetParameter(1)+2*tf5->GetParameter(4));
// 					hh->Fit(tf4,"q","q",hh->GetBinCenter(minbin1),2*tf4->GetParameter(1)+20.);
// 				}
				tge[i2][2][1]->SetPoint(tge[i2][2][1]->GetN(),i1,tf4->GetParameter(1));
				tge[i2][2][1]->SetPointError(tge[i2][2][1]->GetN()-1,0,tf4->GetParError(1));
				tge[i2][2][2]->SetPoint(tge[i2][2][2]->GetN(),i1,tf4->GetParameter(1));
				tge[i2][2][2]->SetPointError(tge[i2][2][2]->GetN()-1,0,tf4->GetParError(1));
// 				cout<<" "<<i2<<" "<<tf3->GetParameter(1)<<" "<<tf3->GetParError(1)<<endl;
				
				sprintf(hname,"%d_%d",Run3List[i1],i2);
				hh->SetName(hname);hh->SetTitle(hname);
				outroot->cd();hh->Write();
			}
			inroot->Close();
		}
	}
	outroot->cd();
	for(int i1=0;i1<3;i1++)
	{
		if(i1==1) continue;
		for(int i2=0;i2<3;i2++)
		{
			for(int i3=0;i3<3;i3++)
			{
				tge[i1][i2][i3]->Write();
			}
		}
	}
	outroot->Close();
}