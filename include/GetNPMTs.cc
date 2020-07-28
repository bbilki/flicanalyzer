void GetNPMTs()
{
	if(RunNo<1000)
	{
		if(RunNo<208) NPMTs=3;
		else if(RunNo<371) NPMTs=6;
		else NPMTs=8;
	}
	else
	{
		NPMTs=3;
	}
	cout<<"Number of v1751 channels: "<<NPMTs<<endl;
}