void TPCTiming()
{
	sprintf(hname,"TPCTiming_%d.root",RunNo);
	TFile* outroot=new TFile(hname,"recreate");
	
	ReadTPCBaselines();
	
	//below is for 7285
	int NevtSel=33;
	int EventNos[33]={2,12,18,21,22,26,32,35,38,39,42,46,50,52,57,58,63,64,65,68,70,71,72,74,76,78,80,81,89,90,99,101,104};
	
	TH1F* hh[2][128];
	float ss=0;
	int ic=0;int gch=0;
	
	TGraph* tg[2][128];
	TGraph* tg2[2][128];
	
	for(int i1=0;i1<2;i1++)
	{
		for(int i2=0;i2<128;i2++)
		{
			sprintf(hname,"WF_%s_%d",wt[i1].c_str(),i2);
			hh[i1][i2]=new TH1F(hname,hname,4096,-0.5,4095.5);
			hh[i1][i2]->SetLineColor(i1+1);
			
			tg[i1][i2]=new TGraph();
			sprintf(hname,"HP_%s_%d",wt[i1].c_str(),i2);
			tg[i1][i2]->SetName(hname);
			tg[i1][i2]->SetTitle(hname);
			tg[i1][i2]->SetMarkerStyle(i1+20);
			tg[i1][i2]->SetMarkerColor(i1+1);
			
			tg2[i1][i2]=new TGraph();
			sprintf(hname,"NP_%s_%d",wt[i1].c_str(),i2);
			tg2[i1][i2]->SetName(hname);
			tg2[i1][i2]->SetTitle(hname);
			tg2[i1][i2]->SetMarkerStyle(i1+20);
			tg2[i1][i2]->SetMarkerColor(i1+1);
		}
	}
	TGraph* tga=new TGraph();
	tga->SetPoint(0,0,-40);tga->SetPoint(1,4095,100);
	
	TFile* inroot;
	TTree* T;
	
	sprintf(hname,"%s/Run_%d.root",NTupleFilePath,RunNo);
	inroot=new TFile(hname);
	T =  (TTree*) inroot->Get("T");
	T->SetBranchAddress("TPCWF",&ed.TPCWF);
	T->SetBranchAddress("PMTWF",&ed.PMTWF);
	
	int binpeak=0;int binzero=0;int binnegpeak=0;int binright=0;
	float xpeak=0;float xcenter=0;float xnegpeak=0;float ypeak=0;
	int startbin=0;
	
	TCanvas* cc=new TCanvas("cc","cc",600,600);
	
	for(int I=0;I<NevtSel;I++)
	{
		T->GetEntry(EventNos[I]);
// 		cout<<EventNos[I]<<endl;
		for(int i1=0;i1<256;i1++)
		{
			ic=i1/128;gch=i1-((i1/128)*128);
			for(int i2=0;i2<4096;i2++)
			{
				ss=((float)ed.TPCWF->at(i1)[i2])-TPCBaselines[ic][gch][0];
				hh[ic][gch]->SetBinContent(hh[ic][gch]->FindBin(i2),ss);
			}
		}
		cc->cd();
		hh[0][0]->Draw();
		hh[1][0]->Draw("same");
		hh[0][0]->GetYaxis()->SetRangeUser(-40,100);
		for(int i1=0;i1<2;i1++)
		{
			for(int i2=1;i2<128;i2++)
			{
				hh[i1][i2]->Draw("same");
			}
		}
		sprintf(hname,"Event_%d",EventNos[I]);
		cc->SetName(hname);
		outroot->cd();
		cc->Write();
		
		//find collection peaks and induction zero crossings, put them in tg, 
		for(int i1=0;i1<128;i1++)
		{
			ic=i1/128;gch=i1-((i1/128)*128);
			startbin=FindBinAbove(hh[ic][gch],ThColInd[0],100);
			while(startbin>=0)
			{
				for(int ik1=startbin;ik1<hh[ic][gch]->GetNbinsX();ik1++)
				{
					if(hh[ic][gch]->GetBinContent(ik1+1)<hh[ic][gch]->GetBinContent(ik1))
					{
						binpeak=ik1;
						xpeak=hh[ic][gch]->GetBinCenter(ik1);
						ypeak=hh[ic][gch]->GetBinContent(ik1);
						tg[ic][gch]->SetPoint(tg[ic][gch]->GetN(),xpeak,ypeak);
						tg2[ic][gch]->SetPoint(tg[ic][gch]->GetN(),xpeak,ypeak);
// 						cout<<EventNos[I]<<" "<<ic<<" "<<gch<<" "<<xpeak<<" "<<ypeak<<endl;
						for(int ik2=binpeak;ik2<hh[ic][gch]->GetNbinsX();ik2++)
						{
							if(hh[ic][gch]->GetBinContent(ik2+1)>hh[ic][gch]->GetBinContent(ik2))
							{
								binright=ik2;
								break;
							}
						}
						break;
					}
				}
				startbin=FindBinAbove(hh[ic][gch],ThColInd[0],binright+1);
				if(startbin>4000) break;
			}
		}
		for(int i1=128;i1<256;i1++)
		{
			ic=i1/128;gch=i1-((i1/128)*128);
			startbin=FindBinAbove(hh[ic][gch],ThColInd[1],100);
			while(startbin>=0)
			{
				for(int ik1=startbin;ik1<hh[ic][gch]->GetNbinsX();ik1++)
				{
					if(hh[ic][gch]->GetBinContent(ik1+1)<hh[ic][gch]->GetBinContent(ik1))
					{
						binpeak=ik1;
						binzero=4050;binnegpeak=4050;binright=4050;
						for(int ik2=binpeak;ik2<hh[ic][gch]->GetNbinsX();ik2++)
						{
							if(hh[ic][gch]->GetBinContent(ik2)<0)
							{
								binzero=ik2;
								xcenter=hh[ic][gch]->GetBinCenter(ik2);
								break;
							}
						}
						for(int ik2=binzero;ik2<hh[ic][gch]->GetNbinsX();ik2++)
						{
							if(hh[ic][gch]->GetBinContent(ik2+1)>hh[ic][gch]->GetBinContent(ik2))
							{
								binnegpeak=ik2;
								xnegpeak=hh[ic][gch]->GetBinCenter(ik2);
								ypeak=(-1.)*hh[ic][gch]->GetBinContent(ik2);
								break;
							}
						}
						for(int ik2=binnegpeak;ik2<hh[ic][gch]->GetNbinsX();ik2++)
						{
							if(hh[ic][gch]->GetBinContent(ik2+1)<hh[ic][gch]->GetBinContent(ik2))
							{
								binright=ik2;
								break;
							}
						}
						tg[ic][gch]->SetPoint(tg[ic][gch]->GetN(),xcenter,ypeak);
						tg2[ic][gch]->SetPoint(tg[ic][gch]->GetN(),xnegpeak,ypeak);
// 						cout<<EventNos[I]<<" "<<ic<<" "<<gch<<" "<<xcenter<<" "<<ypeak<<endl;
						break;
					}
				}
				startbin=FindBinAbove(hh[ic][gch],ThColInd[1],binright+1);
				if(startbin>4000) break;
			}
		}
		cc->cd();
		tga->Draw("ap");
		for(int i1=0;i1<2;i1++)
		{
			for(int i2=0;i2<128;i2++)
			{
				if(tg[i1][i2]->GetN()>0) tg[i1][i2]->Draw("same p");
			}
		}
		sprintf(hname,"Points_%d",EventNos[I]);
		cc->SetName(hname);
		outroot->cd();
		cc->Write();
		
		cc->cd();
		tga->Draw("ap");
		for(int i1=0;i1<2;i1++)
		{
			for(int i2=0;i2<128;i2++)
			{
				if(tg2[i1][i2]->GetN()>0) tg2[i1][i2]->Draw("same p");
			}
		}
		sprintf(hname,"NegPeakPoints_%d",EventNos[I]);
		cc->SetName(hname);
		outroot->cd();
		cc->Write();
		
		for(int i1=0;i1<2;i1++)
		{
			for(int i2=0;i2<128;i2++)
			{
				hh[i1][i2]->Reset();
				tg[i1][i2]->Set(0);
				tg2[i1][i2]->Set(0);
			}
		}
	}
	outroot->Close();
	sprintf(hname,"mv TPCTiming_%d.root %s/Histos/;wait;",RunNo,AnalysisFilePath);system(hname);
}