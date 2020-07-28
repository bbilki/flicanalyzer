void PrintNevt()
{
	sprintf(hname,"%s/Run_%d.root",NTupleFilePath,RunNo);
	TFile* inroot=new TFile(hname);
	TTree* T =  (TTree*) inroot->Get("T");
	cout<<"Run : "<<RunNo<<" NEvt : "<<T->GetEntries()<<endl;
	inroot->Close();
}