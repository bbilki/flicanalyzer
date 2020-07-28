void PMTCalibrationsAll()
{
// 	int Run1List[85]={50,7073,51,7074,7075,52,7076,57,7077,7078,7079,59,7080,7081,60,7082,61,7083,7084,7085,7086,7087,7088,68,7089,7090,70,7091,71,74,7092,7093,75,76,7094,7095,77,7096,7098,78,7099,7100,7101,79,7102,7103,7104,7105,80,7106,7107,7108,81,7109,7110,7120,82,7121,7122,83,7123,7124,84,7125,7126,7127,85,7128,7129,7130,86,7131,7132,7133,7134,7135,7136,7137,7138,7139,7140,7141,7142,7143,7144};
// 	int Run2List[73]={88,91,92,93,94,7240,7241,95,96,7242,7243,7244,7245,7246,97,98,7247,7248,7249,7250,7251,99,7252,7253,100,7254,7255,101,7256,7257,7258,102,7259,7260,103,7261,7262,7263,7264,7265,104,7266,7267,105,7268,7269,7270,106,7271,7272,107,7273,7274,108,7275,7276,109,7277,7278,110,7279,7280,111,7281,7282,112,7283,7284,113,7285,114,7286,7287};
// 	int Run3List[123]={115,7307,7308,116,7309,7310,117,7311,7312,118,7313,7314,119,7315,7316,120,7317,7318,121,7319,7320,122,7321,7322,7323,7324,123,7325,7326,124,7327,7328,125,7329,7330,126,7331,7332,127,7333,7334,7335,128,7336,7337,129,7338,7339,7340,7341,7342,7343,7344,7345,7346,7347,7348,7349,7350,7351,7352,130,7353,7354,7355,7356,7357,7358,7359,7360,7361,7362,7363,7364,7365,7366,7367,7368,7369,7370,131,132,7371,7372,7373,7374,7375,7376,7377,7378,133,7379,7380,7381,7382,7383,7384,134,7385,7386,135,7387,7388,7389,7390,7391,136,7392,7393,137,7394,7395,7396,138,7397,7398,7399,7400,7401,139,7402,7403,140};
	
	int NR1L=29;
	int NR2L=20;
	int NR3L=24;
	
	int Run1List[29]={50,51,7074,7076,57,59,60,61,7084,70,71,74,75,76,78,80,7106,7110,82,83,84,7126,85,7131,7132,7136,7141,7142,7143};//7087,7081,7082,7096,7099,7100,7123,7128,7129,7104,7107,7121,7140,7135,7133,7134,7144,7137,7138,7139,7092,77,86,79,81,68,7078,7089,7094,7125  
	int Run2List[20]={7241,7245,99,100,7254,101,102,7259,103,7264,104,105,7268,106,7271,107,7273,7281,7283,7285};//7252,7279,7287,92,94,96,97,93,95,98,7257,7263,108,7275,109,110,112,113,114,7266,7277 weird ,7256
	int Run3List[24]={115,116,117,118,119,120,7318,121,7320,122,7322,7323,123,7325,124,7327,125,126,127,128,129,7369,7394,7397};//,130,132,133,134,135,136,137,138,139 rate ,7371,7374,7375 something wrong with LXe ,7312,7314,7316 /// ,7331,7334,7336,7385,7389,7392
	
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
			tge[i1][i2][2]->SetMarkerStyle(20);
			tge[i1][i2][2]->SetMarkerColor(1);
			tge[i1][i2][2]->GetXaxis()->SetTitle("Run ID");tge[i1][i2][2]->GetXaxis()->CenterTitle();
			tge[i1][i2][2]->GetYaxis()->SetTitle("Single Photoelectron Charge (arbitrary units)");tge[i1][i2][2]->GetYaxis()->CenterTitle();
		}
	}
	
	TFile* outroot=new TFile("PMTCalibrationsAll.root","recreate");
	
	TH1F* hh;TF1* tf1;
	TF1* tf2=new TF1("GG","gaus(0)+gaus(3)",0.,200.);
	TF1* tf3=new TF1("gaus","gaus",0.,100.);
	TFile* inroot;
	float fitulim[3]={45,0,35};
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
				
// 				tf1=(TF1*)hh->GetFunction("GGG");
// 				tge[i2][0][0]->SetPoint(tge[i2][0][0]->GetN(),i1,tf1->GetParameter(4));
// 				tge[i2][0][0]->SetPointError(tge[i2][0][0]->GetN()-1,0,tf1->GetParError(4));
// 				cout<<" "<<i2<<" "<<tf1->GetParameter(4)<<" "<<tf1->GetParError(4)<<endl;
				
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
				cout<<" "<<i2<<" "<<(tf2->GetParameter(4)-tf2->GetParameter(1))<<" "<<sqrt(pow(tf2->GetParError(4),2)+pow(tf2->GetParError(1),2))<<endl;
				
				
				
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
				if(Run1List[i1]<7095 && i2==2) sprintf(hname,"Integral_w_%d_20",i2);
				else sprintf(hname,"Integral_w_%d_6",i2);
// 				sprintf(hname,"Integral_w_%d_6",i2);
				inroot->GetObject(hname,hh);
				
// 				tf2->SetParameter(0,500);
// 				tf2->SetParameter(1,8);
// 				tf2->SetParameter(2,2);
// 				tf2->SetParameter(3,1000);
// 				tf2->SetParameter(4,30);
// 				tf2->SetParameter(5,10);
// 				hh->Fit(tf2,"q","q",0.,fitulim[i2]);
				
				if(i2==0)
				{
					hh->GetXaxis()->SetRangeUser(0,20);
					maxbin1=hh->GetMaximumBin();
					hh->GetXaxis()->SetRangeUser(20,80);
					maxbin2=hh->GetMaximumBin();
					hh->GetXaxis()->SetRangeUser(hh->GetBinCenter(maxbin1),hh->GetBinCenter(maxbin2));
					minbin1=hh->GetMinimumBin();
					hh->GetXaxis()->SetRangeUser(0,200);
					if(minbin1==maxbin1)
					{
						minbin1-=5;
					}
					tf3->SetParameter(0,hh->GetBinContent(maxbin2));
					tf3->SetParameter(1,hh->GetBinCenter(maxbin2));
					tf3->SetParameter(2,20);
					xmin=hh->GetBinCenter(minbin1);
					xmax=2*hh->GetBinCenter(maxbin2)-hh->GetBinCenter(minbin1);
					hh->Fit(tf3,"q","q",xmin,xmax);
					xmin=tf3->GetParameter(1)-1.5*tf3->GetParameter(2);
					xmax=tf3->GetParameter(1)+1.5*tf3->GetParameter(2);
					if(xmin<hh->GetBinCenter(minbin1)) xmin=hh->GetBinCenter(minbin1);
					hh->Fit(tf3,"q","q",xmin,xmax);
				}
				else
				{
// 					hh->GetXaxis()->SetRangeUser(0,20);
// 					maxbin1=hh->GetMaximumBin();
// 					hh->GetXaxis()->SetRangeUser(20,80);
// 					maxbin2=hh->GetMaximumBin();
// 					hh->GetXaxis()->SetRangeUser(hh->GetBinCenter(maxbin1),hh->GetBinCenter(maxbin2));
// 					minbin1=hh->GetMinimumBin();
// 					hh->GetXaxis()->SetRangeUser(0,200);
// 					if(minbin1==maxbin1) minbin1-=5;
// 					if(minbin1==maxbin2) maxbin2+=5;
// 					tf3->SetParameter(0,hh->GetBinContent(maxbin2));
// 					tf3->SetParameter(1,hh->GetBinCenter(maxbin2));
// 					tf3->SetParameter(2,20);
// 					xmin=hh->GetBinCenter(minbin1);
// 					xmax=2*hh->GetBinCenter(maxbin2)-hh->GetBinCenter(minbin1);
// 					hh->Fit(tf3,"q","q",xmin,xmax);
// 					xmin=tf3->GetParameter(1)-1.5*tf3->GetParameter(2);
// 					xmax=tf3->GetParameter(1)+1.5*tf3->GetParameter(2);
// 					if(xmin<hh->GetBinCenter(minbin1)) xmin=hh->GetBinCenter(minbin1);
// 					hh->Fit(tf3,"q","q",xmin,xmax);
					
					
					hh->Fit(tf3,"q","q",0,100);
					xmin=tf3->GetParameter(1)-1.5*tf3->GetParameter(2);
					xmax=tf3->GetParameter(1)+1.5*tf3->GetParameter(2);
					hh->Fit(tf3,"q","q",xmin,xmax);
					
					if(Run1List[i1]<7095)
					{
						hh->GetXaxis()->SetRangeUser(0,20);
						maxbin1=hh->GetMaximumBin();
						hh->GetXaxis()->SetRangeUser(30,80);
						maxbin2=hh->GetMaximumBin();
						hh->GetXaxis()->SetRangeUser(hh->GetBinCenter(maxbin1),hh->GetBinCenter(maxbin2));
						minbin1=hh->GetMinimumBin();
						hh->GetXaxis()->SetRangeUser(0,200);
						tf3->SetParameter(0,hh->GetBinContent(maxbin2));
						tf3->SetParameter(1,hh->GetBinCenter(maxbin2));
						tf3->SetParameter(2,20);
						xmin=hh->GetBinCenter(minbin1);
						xmax=2*hh->GetBinCenter(maxbin2)-hh->GetBinCenter(minbin1);
						hh->Fit(tf3,"q","q",xmin,xmax);
						xmin=tf3->GetParameter(1)-1.5*tf3->GetParameter(2);
						xmax=tf3->GetParameter(1)+1.5*tf3->GetParameter(2);
						if(xmin<hh->GetBinCenter(minbin1)) xmin=hh->GetBinCenter(minbin1);
						hh->Fit(tf3,"q","q",xmin,xmax);
					}
					else if(Run1List[i1]>7131)
					{
						hh->GetXaxis()->SetRangeUser(15,80);
						maxbin2=hh->GetMaximumBin();
						hh->GetXaxis()->SetRangeUser(0,200);
						tf3->SetParameter(0,hh->GetBinContent(maxbin2));
						tf3->SetParameter(1,hh->GetBinCenter(maxbin2));
						tf3->SetParameter(2,20);
						hh->Fit(tf3,"q","q",hh->GetBinCenter(maxbin2)-5,hh->GetBinCenter(maxbin2)+5);
						xmin=tf3->GetParameter(1)-1.5*tf3->GetParameter(2);
						xmax=tf3->GetParameter(1)+1.5*tf3->GetParameter(2);
						hh->Fit(tf3,"q","q",xmin,xmax);
					}
				}
// 				hh->GetXaxis()->SetRangeUser(0,20);
// 				maxbin1=hh->GetMaximumBin();
// 				hh->GetXaxis()->SetRangeUser(hh->GetBinCenter(maxbin1),30);
// 				minbin1=hh->GetMinimumBin();
// 				hh->GetXaxis()->SetRangeUser(hh->GetBinCenter(minbin1),80);
// 				maxbin2=hh->GetMaximumBin();
// 				hh->GetXaxis()->SetRangeUser(hh->GetBinCenter(minbin1),hh->GetBinCenter(maxbin2));
// 				minbin1=hh->GetMinimumBin();
// 				hh->GetXaxis()->SetRangeUser(0,200);
// 				tf3->SetParameter(0,hh->GetBinContent(maxbin2));
// 				tf3->SetParameter(1,hh->GetBinCenter(maxbin2));
// 				tf3->SetParameter(2,20);
// 				
// 				for(int ik1=maxbin2;ik1>=1;ik1--)
// 				{
// 					if((hh->GetBinContent(ik1)/hh->GetBinContent(maxbin2))<0.7){minbin=ik1;break;}
// 				}
// 				
// 				for(int ik1=maxbin2;ik1<=hh->GetNbinsX();ik1++)
// 				{
// 					if((hh->GetBinContent(ik1)/hh->GetBinContent(maxbin2))<0.7){maxbin=ik1;break;}
// 				}
// 				
// 				if(minbin<minbin1) minbin=minbin1;
// 				
// 				hh->Fit(tf3,"q","q",hh->GetBinCenter(minbin),hh->GetBinCenter(maxbin));
// // 				fitchi21=tf3->GetChisquare()/tf3->GetNDF();
// 				xmin=tf3->GetParameter(1)-1.5*tf3->GetParameter(2);
// 				xmax=tf3->GetParameter(1)+1.5*tf3->GetParameter(2);
// 				if(xmin<hh->GetBinCenter(minbin1)) xmin=hh->GetBinCenter(minbin1);
// 				hh->Fit(tf3,"q","q",xmin,xmax);
// // 				fitchi22=tf3->GetChisquare()/tf3->GetNDF();
// // 				if(fitchi21<fitchi22) hh->Fit(tf3,"q","q",hh->GetBinCenter(minbin),hh->GetBinCenter(maxbin));
// // 				hh->Fit(tf3,"q","q",tf3->GetParameter(1)-1.5*tf3->GetParameter(2),tf3->GetParameter(1)+1.5*tf3->GetParameter(2));
// 				
				
// 				tf3->SetParameter(1,50);
// // 				tf3->SetParLimits(1,20,60);
// 				hh->Fit(tf3,"q","q",20,80);
// 				if(tf3->GetParameter(1)<0)
// 				{
// 					tf3->SetParameter(1,50);
// 					hh->Fit(tf3,"q","q",15,45);
// 				}
// 				hh->Fit(tf3,"q","q",tf3->GetParameter(1)-1.5*tf3->GetParameter(2),tf3->GetParameter(1)+1.5*tf3->GetParameter(2));
				tge[i2][0][1]->SetPoint(tge[i2][0][1]->GetN(),i1,tf3->GetParameter(1));
				tge[i2][0][1]->SetPointError(tge[i2][0][1]->GetN()-1,0,tf3->GetParError(1));
				tge[i2][0][2]->SetPoint(tge[i2][0][2]->GetN(),i1,tf3->GetParameter(1));
				tge[i2][0][2]->SetPointError(tge[i2][0][2]->GetN()-1,0,tf3->GetParError(1));
				cout<<" "<<i2<<" "<<tf3->GetParameter(1)<<" "<<tf3->GetParError(1)<<endl;
				
// 				tge[i2][0][1]->SetPoint(tge[i2][0][1]->GetN(),i1,tf2->GetParameter(4));
// 				tge[i2][0][1]->SetPointError(tge[i2][0][1]->GetN()-1,0,tf2->GetParError(4));
// 				cout<<" "<<i2<<" "<<tf2->GetParameter(4)<<" "<<tf2->GetParError(4)<<endl;
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
		cout<<i1<<" "<<Run2List[i1];
		if(Run2List[i1]<7000)
		{
			sprintf(hname,"%s/Histos/PMTCalibration_%d.root",AnalysisFilePath,Run2List[i1]);
			inroot=new TFile(hname);
			for(int i2=0;i2<3;i2++)
			{
				if(i2==1) continue;
				sprintf(hname,"SignalWindow2_%d",i2);
				inroot->GetObject(hname,hh);
				
// 				tf1=(TF1*)hh->GetFunction("GGG");
// 				tge[i2][0][0]->SetPoint(tge[i2][0][0]->GetN(),i1,tf1->GetParameter(4));
// 				tge[i2][0][0]->SetPointError(tge[i2][0][0]->GetN()-1,0,tf1->GetParError(4));
// 				cout<<" "<<i2<<" "<<tf1->GetParameter(4)<<" "<<tf1->GetParError(4)<<endl;
				
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
				cout<<" "<<i2<<" "<<(tf2->GetParameter(4)-tf2->GetParameter(1))<<" "<<sqrt(pow(tf2->GetParError(4),2)+pow(tf2->GetParError(1),2))<<endl;
				
				
				
				sprintf(hname,"%d_%d",Run2List[i1],i2);
				hh->SetName(hname);hh->SetTitle(hname);
				outroot->cd();hh->Write();
			}
			inroot->Close();
// 			for(int i2=0;i2<3;i2++)
// 			{
// 				if(i2==1) continue;
// 				sprintf(hname,"SignalWindow2_%d",i2);
// 				inroot->GetObject(hname,hh);
// 				tf1=(TF1*)hh->GetFunction("GGG");
// 				tge[i2][1][0]->SetPoint(tge[i2][1][0]->GetN(),i1,tf1->GetParameter(4));
// 				tge[i2][1][0]->SetPointError(tge[i2][1][0]->GetN()-1,0,tf1->GetParError(4));
// 				cout<<" "<<i2<<" "<<tf1->GetParameter(4)<<" "<<tf1->GetParError(4)<<endl;
// 				sprintf(hname,"%d_%d",Run2List[i1],i2);
// 				hh->SetName(hname);hh->SetTitle(hname);
// 				outroot->cd();hh->Write();
// 			}
// 			inroot->Close();
		}
		else
		{
			sprintf(hname,"%s/Histos/PMTCalibration3_%d.root",AnalysisFilePath,Run2List[i1]);
			inroot=new TFile(hname);
			for(int i2=0;i2<3;i2++)
			{
				if(i2==1) continue;
				sprintf(hname,"Integral_w_%d_6",i2);
				inroot->GetObject(hname,hh);
				
// 				tf2->SetParameter(0,500);
// 				tf2->SetParameter(1,8);
// 				tf2->SetParameter(2,2);
// 				tf2->SetParameter(3,1000);
// 				tf2->SetParameter(4,30);
// 				tf2->SetParameter(5,10);
// 				hh->Fit(tf2,"q","q",0.,fitulim[i2]);
				
				if(i2==0)
				{
					hh->GetXaxis()->SetRangeUser(0,20);
					maxbin1=hh->GetMaximumBin();
					hh->GetXaxis()->SetRangeUser(25,80);
					maxbin2=hh->GetMaximumBin();
					hh->GetXaxis()->SetRangeUser(hh->GetBinCenter(maxbin1),hh->GetBinCenter(maxbin2));
					minbin1=hh->GetMinimumBin();
					hh->GetXaxis()->SetRangeUser(0,200);
					if(minbin1==maxbin1)
					{
						minbin1-=5;
					}
					tf3->SetParameter(0,hh->GetBinContent(maxbin2));
					tf3->SetParameter(1,hh->GetBinCenter(maxbin2));
					tf3->SetParameter(2,20);
					xmin=hh->GetBinCenter(minbin1);
					xmax=2*hh->GetBinCenter(maxbin2)-hh->GetBinCenter(minbin1);
					hh->Fit(tf3,"q","q",xmin,xmax);
					xmin=tf3->GetParameter(1)-1.5*tf3->GetParameter(2);
					xmax=tf3->GetParameter(1)+1.5*tf3->GetParameter(2);
					if(xmin<hh->GetBinCenter(minbin1)) xmin=hh->GetBinCenter(minbin1);
					hh->Fit(tf3,"q","q",xmin,xmax);
				}
				else
				{
					hh->Fit(tf3,"q","q",0,50);
					xmin=tf3->GetParameter(1)-1.5*tf3->GetParameter(2);
					xmax=tf3->GetParameter(1)+1.5*tf3->GetParameter(2);
					hh->Fit(tf3,"q","q",xmin,xmax);
// 					
					if(Run2List[i1]==7241 || Run2List[i1]==7271 || Run2List[i1]==7273)
					{
						hh->GetXaxis()->SetRangeUser(0,15);
						maxbin1=hh->GetMaximumBin();
						hh->GetXaxis()->SetRangeUser(15,50);
						maxbin2=hh->GetMaximumBin();
						hh->GetXaxis()->SetRangeUser(hh->GetBinCenter(maxbin1),hh->GetBinCenter(maxbin2));
						minbin1=hh->GetMinimumBin();
						hh->GetXaxis()->SetRangeUser(0,200);
						tf3->SetParameter(0,hh->GetBinContent(maxbin2));
						tf3->SetParameter(1,hh->GetBinCenter(maxbin2));
						tf3->SetParameter(2,20);
						xmin=hh->GetBinCenter(minbin1);
						xmax=2*hh->GetBinCenter(maxbin2)-hh->GetBinCenter(minbin1);
						hh->Fit(tf3,"q","q",xmin,xmax);
						xmin=tf3->GetParameter(1)-1.5*tf3->GetParameter(2);
						xmax=tf3->GetParameter(1)+1.5*tf3->GetParameter(2);
						if(xmin<hh->GetBinCenter(minbin1)) xmin=hh->GetBinCenter(minbin1);
						hh->Fit(tf3,"q","q",xmin,xmax);
					}
				}
				tge[i2][1][1]->SetPoint(tge[i2][1][1]->GetN(),i1,tf3->GetParameter(1));
				tge[i2][1][1]->SetPointError(tge[i2][1][1]->GetN()-1,0,tf3->GetParError(1));
				tge[i2][1][2]->SetPoint(tge[i2][1][2]->GetN(),i1,tf3->GetParameter(1));
				tge[i2][1][2]->SetPointError(tge[i2][1][2]->GetN()-1,0,tf3->GetParError(1));
				cout<<" "<<i2<<" "<<tf3->GetParameter(1)<<" "<<tf3->GetParError(1)<<endl;
				
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
				cout<<" "<<i2<<" "<<(tf2->GetParameter(4)-tf2->GetParameter(1))<<" "<<sqrt(pow(tf2->GetParError(4),2)+pow(tf2->GetParError(1),2))<<endl;
				
				sprintf(hname,"%d_%d",Run3List[i1],i2);
				hh->SetName(hname);hh->SetTitle(hname);
				outroot->cd();hh->Write();
			}
			inroot->Close();
			
			
// 			for(int i2=0;i2<3;i2++)
// 			{
// 				if(i2==1) continue;
// 				sprintf(hname,"SignalWindow2_%d",i2);
// 				inroot->GetObject(hname,hh);
// 				tf1=(TF1*)hh->GetFunction("GGG");
// 				tge[i2][2][0]->SetPoint(tge[i2][2][0]->GetN(),i1,tf1->GetParameter(4));
// 				tge[i2][2][0]->SetPointError(tge[i2][2][0]->GetN()-1,0,tf1->GetParError(4));
// 				cout<<" "<<i2<<" "<<tf1->GetParameter(4)<<" "<<tf1->GetParError(4)<<endl;
// 				sprintf(hname,"%d_%d",Run3List[i1],i2);
// 				hh->SetName(hname);hh->SetTitle(hname);
// 				outroot->cd();hh->Write();
// 			}
// 			inroot->Close();
		}
		else
		{
			sprintf(hname,"%s/Histos/PMTCalibration3_%d.root",AnalysisFilePath,Run3List[i1]);
			inroot=new TFile(hname);
			for(int i2=0;i2<3;i2++)
			{
				if(i2==1) continue;
				sprintf(hname,"Integral_w_%d_6",i2);
				inroot->GetObject(hname,hh);
				
				if(i2==0)
				{
					hh->GetXaxis()->SetRangeUser(0,20);
					maxbin1=hh->GetMaximumBin();
					hh->GetXaxis()->SetRangeUser(25,80);
					maxbin2=hh->GetMaximumBin();
					hh->GetXaxis()->SetRangeUser(hh->GetBinCenter(maxbin1),hh->GetBinCenter(maxbin2));
					minbin1=hh->GetMinimumBin();
					hh->GetXaxis()->SetRangeUser(0,200);
					if(minbin1==maxbin1)
					{
						minbin1-=5;
					}
					tf3->SetParameter(0,hh->GetBinContent(maxbin2));
					tf3->SetParameter(1,hh->GetBinCenter(maxbin2));
					tf3->SetParameter(2,20);
					xmin=hh->GetBinCenter(minbin1);
					xmax=2*hh->GetBinCenter(maxbin2)-hh->GetBinCenter(minbin1);
					hh->Fit(tf3,"q","q",xmin,xmax);
					xmin=tf3->GetParameter(1)-1.5*tf3->GetParameter(2);
					xmax=tf3->GetParameter(1)+1.5*tf3->GetParameter(2);
					if(xmin<hh->GetBinCenter(minbin1)) xmin=hh->GetBinCenter(minbin1);
					hh->Fit(tf3,"q","q",xmin,xmax);
				}
				else
				{
					hh->Fit(tf3,"q","q",0,50);
					xmin=tf3->GetParameter(1)-1.5*tf3->GetParameter(2);
					xmax=tf3->GetParameter(1)+1.5*tf3->GetParameter(2);
					hh->Fit(tf3,"q","q",xmin,xmax);
// 					
// 					if(Run2List[i1]==7241 || Run2List[i1]==7271 || Run2List[i1]==7273)
// 					{
// 						hh->GetXaxis()->SetRangeUser(0,15);
// 						maxbin1=hh->GetMaximumBin();
// 						hh->GetXaxis()->SetRangeUser(15,50);
// 						maxbin2=hh->GetMaximumBin();
// 						hh->GetXaxis()->SetRangeUser(hh->GetBinCenter(maxbin1),hh->GetBinCenter(maxbin2));
// 						minbin1=hh->GetMinimumBin();
// 						hh->GetXaxis()->SetRangeUser(0,200);
// 						tf3->SetParameter(0,hh->GetBinContent(maxbin2));
// 						tf3->SetParameter(1,hh->GetBinCenter(maxbin2));
// 						tf3->SetParameter(2,20);
// 						xmin=hh->GetBinCenter(minbin1);
// 						xmax=2*hh->GetBinCenter(maxbin2)-hh->GetBinCenter(minbin1);
// 						hh->Fit(tf3,"q","q",xmin,xmax);
// 						xmin=tf3->GetParameter(1)-1.5*tf3->GetParameter(2);
// 						xmax=tf3->GetParameter(1)+1.5*tf3->GetParameter(2);
// 						if(xmin<hh->GetBinCenter(minbin1)) xmin=hh->GetBinCenter(minbin1);
// 						hh->Fit(tf3,"q","q",xmin,xmax);
// 					}
				}
				tge[i2][2][1]->SetPoint(tge[i2][2][1]->GetN(),i1,tf3->GetParameter(1));
				tge[i2][2][1]->SetPointError(tge[i2][2][1]->GetN()-1,0,tf3->GetParError(1));
				tge[i2][2][2]->SetPoint(tge[i2][2][2]->GetN(),i1,tf3->GetParameter(1));
				tge[i2][2][2]->SetPointError(tge[i2][2][2]->GetN()-1,0,tf3->GetParError(1));
				cout<<" "<<i2<<" "<<tf3->GetParameter(1)<<" "<<tf3->GetParError(1)<<endl;
				
				sprintf(hname,"%d_%d",Run3List[i1],i2);
				hh->SetName(hname);hh->SetTitle(hname);
				outroot->cd();hh->Write();
			}
			inroot->Close();
			
			
			
// 			for(int i2=0;i2<3;i2++)
// 			{
// 				if(i2==1) continue;
// 				sprintf(hname,"Integral_w_%d_6",i2);
// 				inroot->GetObject(hname,hh);
// 				tf2->SetParameter(0,500);
// 				tf2->SetParameter(1,8);
// 				tf2->SetParameter(2,2);
// 				tf2->SetParameter(3,1000);
// 				tf2->SetParameter(4,30);
// 				tf2->SetParameter(5,10);
// 				hh->Fit(tf2,"q","q",0.,fitulim[i2]);
// 				tge[i2][2][1]->SetPoint(tge[i2][2][1]->GetN(),i1,tf2->GetParameter(4));
// 				tge[i2][2][1]->SetPointError(tge[i2][2][1]->GetN()-1,0,tf2->GetParError(4));
// 				cout<<" "<<i2<<" "<<tf2->GetParameter(4)<<" "<<tf2->GetParError(4)<<endl;
// 				sprintf(hname,"%d_%d",Run3List[i1],i2);
// 				hh->SetName(hname);hh->SetTitle(hname);
// 				outroot->cd();hh->Write();
// 			}
// 			inroot->Close();
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