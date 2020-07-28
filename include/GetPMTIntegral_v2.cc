void GetPMTIntegral_v2()
{
	vector <int> RunNos;
// 	NPMTs=3;
	if(RunNo==1000)
	{
		RunNos.push_back(7074);
		RunNos.push_back(7076);
		RunNos.push_back(7078);
		RunNos.push_back(7082);
		RunNos.push_back(7084);
		RunNos.push_back(7092);
		RunNos.push_back(7094);
		RunNos.push_back(7096);
		RunNos.push_back(7104);
		RunNos.push_back(7106);
		RunNos.push_back(7107);
		RunNos.push_back(7110);
		RunNos.push_back(7121);
	}
	else if(RunNo==2000)
	{
		RunNos.push_back(7264);
		RunNos.push_back(7266);
		RunNos.push_back(7268);
		RunNos.push_back(7271);
		RunNos.push_back(7273);
		RunNos.push_back(7275);
	}
	else if(RunNo==3000)
	{
		RunNos.push_back(7308);
		RunNos.push_back(7312);
		RunNos.push_back(7314);
		RunNos.push_back(7316);
		RunNos.push_back(7318);
		RunNos.push_back(7320);
		RunNos.push_back(7322);
		RunNos.push_back(7323);
		RunNos.push_back(7325);
		RunNos.push_back(7327);
		RunNos.push_back(7379);
		RunNos.push_back(7385);
		RunNos.push_back(7388);
	}
	
	PMTCalibData pd;
	vector <PMTCalibData> PD;
	sprintf(hname,"%s/Files/PMTCalibrations_051120.txt",AnalysisFilePath);
	ifstream incalib(hname);
	int cal1[2]={0};float cal2[2]={0};
	pd.RunNo=0;
	while(!incalib.eof())
	{
		incalib>>cal1[0]>>cal1[1]>>cal2[0]>>cal2[1];
		if(cal1[0]==pd.RunNo)
		{
			pd.spe.push_back(cal2[0]);
			pd.speerr.push_back(cal2[1]);
		}
		else
		{
			if(pd.RunNo!=0)
			{
				PD.push_back(pd);
				pd.spe.clear();
				pd.speerr.clear();
			}
			pd.RunNo=cal1[0];
			pd.spe.push_back(cal2[0]);
			pd.speerr.push_back(cal2[1]);
		}
	}
	pd.spe.pop_back();
	pd.speerr.pop_back();
	PD.push_back(pd);
	incalib.close();
// 	for(int i1=0;i1<PD.size();i1++)
// 	{
// 		cout<<PD[i1].RunNo<<" : ";
// 		for(int i2=0;i2<PD[i1].spe.size();i2++)
// 		{
// 			cout<<PD[i1].spe[i2]<<" "<<PD[i1].speerr[i2]<<" ; ";
// 		}
// 		cout<<endl;
// 	}
	
	sprintf(hname,"PMTIntegrals_%d_v2.root",RunNo);
	TFile* outroot=new TFile(hname,"recreate");
	TTree* T1 =  new TTree("PMTIntegrals","PMTIntegrals");
	T1->Branch("RunNo",&pint.RunNo);
	T1->Branch("EventNo",&pint.EventNo);
	
	T1->Branch("integral_1500_2500",&pint.integral_1500_2500);
	T1->Branch("integral_1500_6000",&pint.integral_1500_6000);
	T1->Branch("integral_1500_8000",&pint.integral_1500_8000);
	T1->Branch("integral_1500_10000",&pint.integral_1500_10000);
	T1->Branch("integral_1500_15000",&pint.integral_1500_15000);
	
	T1->Branch("integralZS_1500_2500",&pint.integralZS_1500_2500);
	T1->Branch("integralZS_1500_6000",&pint.integralZS_1500_6000);
	T1->Branch("integralZS_1500_8000",&pint.integralZS_1500_8000);
	T1->Branch("integralZS_1500_10000",&pint.integralZS_1500_10000);
	T1->Branch("integralZS_1500_15000",&pint.integralZS_1500_15000);
	
	T1->Branch("integral4sigma_1500_2500",&pint.integral4sigma_1500_2500);
	T1->Branch("integral4sigma_1500_6000",&pint.integral4sigma_1500_6000);
	T1->Branch("integral4sigma_1500_8000",&pint.integral4sigma_1500_8000);
	T1->Branch("integral4sigma_1500_10000",&pint.integral4sigma_1500_10000);
	T1->Branch("integral4sigma_1500_15000",&pint.integral4sigma_1500_15000);
	
	T1->Branch("isSaturated",&pint.isSaturated);
	T1->Branch("isPedestalOK",&pint.isPedestalOK);
	T1->Branch("undershoot",&pint.undershoot);
	T1->Branch("spe",&pint.spe);
	T1->Branch("speerr",&pint.speerr);
	outroot->mkdir("Histos");
	
	TFile* inroot;
	
	TH1F* hb=new TH1F("hb","hb",15000,-0.5,14999.5);
	TH1F* hp=new TH1F("hp","hp",2000,-0.5,1999.5);
	TH1F* hf=new TH1F("hf","hf",15000,-0.5,14999.5);
	TH1F* hcalib;
	TF1* tfcalib;
	
	float hped=0;
	float hped2=0;
	float hped3=0;float hsig3=0;
	float s=0.;
	float ss=0.;
	
	float integral_1500_2500=0;
	float integral_1500_6000=0;
	float integral_1500_8000=0;
	float integral_1500_10000=0;
	float integral_1500_15000=0;
	
	float integralZS_1500_2500=0;
	float integralZS_1500_6000=0;
	float integralZS_1500_8000=0;
	float integralZS_1500_10000=0;
	float integralZS_1500_15000=0;
	
	float integral4sigma_1500_2500=0;
	float integral4sigma_1500_6000=0;
	float integral4sigma_1500_8000=0;
	float integral4sigma_1500_10000=0;
	float integral4sigma_1500_15000=0;
	
	int isSaturated=0;
	int isPedestalOK=1;
	float undershoot=0;
	bool isUndershot=false;
	int UndershotCheck=0;
	
	int minbin=0;
	int maxbin=0;
	int bn=0;
	
	int calibind=0;
	
	TF1* g1=new TF1("g1","gaus",-1000.,2000.);
	TF1* lin=new TF1("lin","[0]",0.,15000);
	
	for(int J=0;J<RunNos.size();J++)
	{
		sprintf(hname,"%s/Run_%d.root",NTupleFilePath,RunNos[J]);
		inroot=new TFile(hname);
		TTree* T =  (TTree*) inroot->Get("T");
		T->SetBranchAddress("TPCWF",&ed.TPCWF);
		T->SetBranchAddress("PMTWF",&ed.PMTWF);
		pint.RunNo=RunNos[J];
		
		cout<<"Run "<<RunNos[J]<<" "<<T->GetEntries()<<" events"<<endl;
		
		for(int i1=0;i1<PD.size();i1++)
		{
			if(PD[i1].RunNo==RunNos[J]){calibind=i1;break;}
		}
		
		pint.spe->clear();
		pint.speerr->clear();
		for(int i1=0;i1<PD[calibind].spe.size();i1++)
		{
			pint.spe->push_back(PD[calibind].spe[i1]);
			pint.speerr->push_back(PD[calibind].speerr[i1]);
		}
		
		for(int I=0;I<T->GetEntries();I++)
		{
			pint.EventNo=I;
			
			pint.integral_1500_2500->clear();
			pint.integral_1500_6000->clear();
			pint.integral_1500_8000->clear();
			pint.integral_1500_10000->clear();
			pint.integral_1500_15000->clear();
			
			pint.integralZS_1500_2500->clear();
			pint.integralZS_1500_6000->clear();
			pint.integralZS_1500_8000->clear();
			pint.integralZS_1500_10000->clear();
			pint.integralZS_1500_15000->clear();
			
			pint.integral4sigma_1500_2500->clear();
			pint.integral4sigma_1500_6000->clear();
			pint.integral4sigma_1500_8000->clear();
			pint.integral4sigma_1500_10000->clear();
			pint.integral4sigma_1500_15000->clear();
			
			pint.isSaturated->clear();
			pint.isPedestalOK->clear();
			pint.undershoot->clear();
			
			T->GetEntry(I);
			for(int i1=0;i1<NPMTs;i1++)
			{
				integral_1500_2500=0;
				integral_1500_6000=0;
				integral_1500_8000=0;
				integral_1500_10000=0;
				integral_1500_15000=0;
				
				integralZS_1500_2500=0;
				integralZS_1500_6000=0;
				integralZS_1500_8000=0;
				integralZS_1500_10000=0;
				integralZS_1500_15000=0;
				
				integral4sigma_1500_2500=0;
				integral4sigma_1500_6000=0;
				integral4sigma_1500_8000=0;
				integral4sigma_1500_10000=0;
				integral4sigma_1500_15000=0;
				
				isSaturated=0;
				isPedestalOK=1;
				undershoot=0;
			
				hped=0;
				hb->Reset();hp->Reset();
				for(int i2=0;i2<1500;i2++)
				{
					hped+=ed.PMTWF->at(i1)[i2];
					hb->SetBinContent(i2+1,ed.PMTWF->at(i1)[i2]);
					hp->Fill(ed.PMTWF->at(i1)[i2]);
				}
				hped/=1500.;
				hb->Fit(lin,"q","q",0.,1500.);
				hped2=lin->GetParameter(0);
				hp->Fit(g1,"q","q",0.,1500.);
				hped3=g1->GetParameter(1);
				hsig3=g1->GetParameter(2);
				for(int i2=0;i2<1500;i2++)
				{
					s=-1.*(ed.PMTWF->at(i1)[i2]-hped3);
					if(s>=4.*hsig3)
					{
						isPedestalOK=0;
						break;
					}
				}
				
// 				if(hsig3>0.75) isPedestalOK=0;
				
				hf->Reset();
				isUndershot=false;
				for(int i2=0;i2<15000;i2++)
				{
					if(ed.PMTWF->at(i1)[i2]==0) isSaturated=1;
					s=-1.*(ed.PMTWF->at(i1)[i2]-hped3);
					hf->SetBinContent(i2+1,s);
					
					if(i2>=1500 && i2<=2500) integral_1500_2500+=s;
					if(i2>=1500 && i2<=6000) integral_1500_6000+=s;
					if(i2>=1500 && i2<=8000) integral_1500_8000+=s;
					if(i2>=1500 && i2<=10000) integral_1500_10000+=s;
					if(i2>=1500 && i2<=15000) integral_1500_15000+=s;
					
					if(i2>=1500 && i2<=2500 && s>0) integralZS_1500_2500+=s;
					if(i2>=1500 && i2<=6000 && s>0) integralZS_1500_6000+=s;
					if(i2>=1500 && i2<=8000 && s>0) integralZS_1500_8000+=s;
					if(i2>=1500 && i2<=10000 && s>0) integralZS_1500_10000+=s;
					if(i2>=1500 && i2<=15000 && s>0) integralZS_1500_15000+=s;
					
					if(i2>=2000)
					{
						if(s<0)
						{
							undershoot+=s;
						}
					}
				}
				for(int is1=1;is1<=hf->GetNbinsX();is1++)
				{
					if(hf->GetBinContent(is1)>=4.*hsig3)
					{
						minbin=-1;
						maxbin=-1;
						for(int i3=is1;i3>=2;i3--)
						{
							if(hf->GetBinContent(i3)>0 && hf->GetBinContent(i3-1)<=0)
							{
								minbin=i3;
								break;
							}
						}
						for(int i3=is1;i3<hf->GetNbinsX();i3++)
						{
							if(hf->GetBinContent(i3)>0 && hf->GetBinContent(i3+1)<=0)
							{
								maxbin=i3;
								break;
							}
						}
						if(minbin==-1) minbin=1;
						if(maxbin==-1) maxbin=hf->GetNbinsX();
						if((maxbin-minbin+1)>5)
						{
							for(int i3=minbin-1;i3<maxbin;i3++)
							{
								ss=hf->GetBinContent(i3+1);
								if(i3>=1500 && i3<=2500) integral4sigma_1500_2500+=ss;
								if(i3>=1500 && i3<=6000) integral4sigma_1500_6000+=ss;
								if(i3>=1500 && i3<=8000) integral4sigma_1500_8000+=ss;
								if(i3>=1500 && i3<=10000) integral4sigma_1500_10000+=ss;
								if(i3>=1500 && i3<=15000) integral4sigma_1500_15000+=ss;
							}
						}
						is1=maxbin+1;
					}
				}
				
				pint.integral_1500_2500->push_back(integral_1500_2500);
				pint.integral_1500_6000->push_back(integral_1500_6000);
				pint.integral_1500_8000->push_back(integral_1500_8000);
				pint.integral_1500_10000->push_back(integral_1500_10000);
				pint.integral_1500_15000->push_back(integral_1500_15000);
				
				pint.integralZS_1500_2500->push_back(integralZS_1500_2500);
				pint.integralZS_1500_6000->push_back(integralZS_1500_6000);
				pint.integralZS_1500_8000->push_back(integralZS_1500_8000);
				pint.integralZS_1500_10000->push_back(integralZS_1500_10000);
				pint.integralZS_1500_15000->push_back(integralZS_1500_15000);
				
				pint.integral4sigma_1500_2500->push_back(integral4sigma_1500_2500);
				pint.integral4sigma_1500_6000->push_back(integral4sigma_1500_6000);
				pint.integral4sigma_1500_8000->push_back(integral4sigma_1500_8000);
				pint.integral4sigma_1500_10000->push_back(integral4sigma_1500_10000);
				pint.integral4sigma_1500_15000->push_back(integral4sigma_1500_15000);
				
				pint.isSaturated->push_back(isSaturated);
				pint.isPedestalOK->push_back(isPedestalOK);
				pint.undershoot->push_back(undershoot);
			}
			T1->Fill();
			if(I%500==0) cout<<I<<endl;
// 			if(I==2) break;
		}
	}
	outroot->cd();
	T1->Write();
	outroot->Close();
	
	sprintf(hname,"mv PMTIntegrals_%d_v2.root %s/Histos/PMTIntegrals_%d_v2.root",RunNo,AnalysisFilePath,RunNo);system(hname);
}