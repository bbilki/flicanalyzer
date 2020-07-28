int FindBaselineRun(int r)
{
	ifstream inRunList("RunList.txt");
	int a[2]={0};
	while(!inRunList.eof())
	{
		inRunList>>a[0]>>a[1];
		if(a[0]==r) return a[1];
	}
	inRunList.close();
	return -1;
}