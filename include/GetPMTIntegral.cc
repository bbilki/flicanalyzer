void GetPMTIntegral()
{
	vector <int> RunNos;
	vector <int> Calibs;
	NPMTs=3;
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
		
		Calibs.push_back(51);
		Calibs.push_back(51);
		Calibs.push_back(57);
		Calibs.push_back(60);
		Calibs.push_back(61);
		Calibs.push_back(71);
		Calibs.push_back(76);
		Calibs.push_back(77);
		Calibs.push_back(79);
		Calibs.push_back(80);
		Calibs.push_back(80);
		Calibs.push_back(81);
		Calibs.push_back(82);
	}
	else if(RunNo==2000)
	{
		RunNos.push_back(7252);
		RunNos.push_back(7254);
		RunNos.push_back(7256);
		RunNos.push_back(7259);
		RunNos.push_back(7268);
		RunNos.push_back(7271);
		RunNos.push_back(7273);
		
		Calibs.push_back(99);
		Calibs.push_back(100);
		Calibs.push_back(101);
		Calibs.push_back(102);
		Calibs.push_back(105);
		Calibs.push_back(106);
		Calibs.push_back(107);
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
		RunNos.push_back(7325);
		RunNos.push_back(7327);
		RunNos.push_back(7385);
		RunNos.push_back(7388);
		
		Calibs.push_back(115);
		Calibs.push_back(117);
		Calibs.push_back(118);
		Calibs.push_back(119);
		Calibs.push_back(120);
		Calibs.push_back(121);
		Calibs.push_back(122);
		Calibs.push_back(123);
		Calibs.push_back(124);
		Calibs.push_back(134);
		Calibs.push_back(135);
	}
	
	sprintf(hname,"PMTIntegrals_%d.root",RunNo);
	TFile* outroot=new TFile(hname,"recreate");
	TTree* T1 =  new TTree("PMTIntegrals","PMTIntegrals");
	T1->Branch("RunNo",&pint.RunNo);
	T1->Branch("EventNo",&pint.EventNo);
	T1->Branch("integral_1500_2500",&pint.integral_1500_2500);
	T1->Branch("integral_1500_6000",&pint.integral_1500_6000);
	T1->Branch("integral_1500_8000",&pint.integral_1500_8000);
	T1->Branch("integral_1500_10000",&pint.integral_1500_10000);
	T1->Branch("integral_1500_15000",&pint.integral_1500_15000);
	T1->Branch("isSaturated",&pint.isSaturated);
	T1->Branch("isPedestalOK",&pint.isPedestalOK);
	T1->Branch("undershoot",&pint.undershoot);
	T1->Branch("spe",&pint.spe);
	T1->Branch("speerr",&pint.speerr);
	outroot->mkdir("Histos");
	
	TFile* inroot;
	TFile* incalib;
	
	TH1F* hb=new TH1F("hb","hb",15000,-0.5,14999.5);
	TH1F* hp=new TH1F("hp","hp",2000,-0.5,1999.5);
	TH1F* hcalib;
	TF1* tfcalib;
	
	float hped=0;
	float hped2=0;
	float hped3=0;float hsig3=0;
	
	float integral_1500_2500=0;
	float integral_1500_6000=0;
	float integral_1500_8000=0;
	float integral_1500_10000=0;
	float integral_1500_15000=0;
	int isSaturated=0;
	int isPedestalOK=1;
	float undershoot=0;
	bool isUndershot=false;
	int UndershotCheck=0;
	
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
		
		sprintf(hname,"%s/Histos/PMTCalibration_CalibRuns_%d.root",AnalysisFilePath,Calibs[J]);
		incalib=new TFile(hname);
		pint.spe->clear();
		pint.speerr->clear();
		for(int i1=0;i1<NPMTs;i1++)
		{
			sprintf(hname,"Integral_%d",i1);
			incalib->GetObject(hname,hcalib);
// 			cout<<hcalib->GetName()<<" "<<hcalib->GetNbinsX()<<endl;
// 			if(hcalib->GetListOfFunctions()->Contains("gint"))
// 			{
				tfcalib=hcalib->GetFunction("gint");
// 			}
			pint.spe->push_back(tfcalib->GetParameter(3));
			pint.speerr->push_back(tfcalib->GetParError(3));
		}
		incalib->Close();
		
		for(int I=0;I<T->GetEntries();I++)
		{
			pint.EventNo=I;
			pint.integral_1500_2500->clear();
			pint.integral_1500_6000->clear();
			pint.integral_1500_8000->clear();
			pint.integral_1500_10000->clear();
			pint.integral_1500_15000->clear();
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
				isSaturated=0;
				isPedestalOK=1;
				undershoot=0;
			
				hped=0;
				hb->Reset();hp->Reset();
				for(int i2=0;i2<1000;i2++)
				{
					hped+=ed.PMTWF->at(i1)[i2];
					hb->SetBinContent(i2+1,ed.PMTWF->at(i1)[i2]);
					hp->Fill(ed.PMTWF->at(i1)[i2]);
				}
				hped/=1000.;
				hb->Fit(lin,"q","q",0.,1000.);
				hped2=lin->GetParameter(0);
				hp->Fit(g1,"q","q",0.,2000.);
				hped3=g1->GetParameter(1);
				hsig3=g1->GetParameter(2);
				if(hsig3>0.75) isPedestalOK=0;
				
				isUndershot=false;
				for(int i2=0;i2<15000;i2++)
				{
					if(ed.PMTWF->at(i1)[i2]==0) isSaturated=1;
					if(i2>=1500 && i2<=2500) integral_1500_2500+=(-1*(ed.PMTWF->at(i1)[i2]-hped3));
					if(i2>=1500 && i2<=6000) integral_1500_6000+=(-1*(ed.PMTWF->at(i1)[i2]-hped3));
					if(i2>=1500 && i2<=8000) integral_1500_8000+=(-1*(ed.PMTWF->at(i1)[i2]-hped3));
					if(i2>=1500 && i2<=10000) integral_1500_10000+=(-1*(ed.PMTWF->at(i1)[i2]-hped3));
					if(i2>=1500 && i2<=15000) integral_1500_15000+=(-1*(ed.PMTWF->at(i1)[i2]-hped3));
					if(i2>=2000)
					{
						if((-1*(ed.PMTWF->at(i1)[i2]-hped3))<0)
						{
							undershoot+=(-1*(ed.PMTWF->at(i1)[i2]-hped3));
						}
					}
				}
				pint.integral_1500_2500->push_back(integral_1500_2500);
				pint.integral_1500_6000->push_back(integral_1500_6000);
				pint.integral_1500_8000->push_back(integral_1500_8000);
				pint.integral_1500_10000->push_back(integral_1500_10000);
				pint.integral_1500_15000->push_back(integral_1500_15000);
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
	
	sprintf(hname,"mv PMTIntegrals_%d.root %s/Histos/PMTIntegrals_%d.root",RunNo,AnalysisFilePath,RunNo);system(hname);
}