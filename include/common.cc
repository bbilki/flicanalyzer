struct edata
{
	vector <vector<int>>* TPCWF;
	vector <vector<int>>* PMTWF;
};
edata ed;

struct hitdata
{
	int E;//event
	vector <vector<int>>* ColIndT;
// 	vector <vector<float>>* ColIndT;
	vector <vector<float>>* Int;
	float QColTot;
	float QColTotZS;
	float QHitTot;
	vector <float>* PMTIntegral;
	vector <int>* ColID;
	vector <float>* ColT;
	vector <float>* ColA;
	vector <float>* ColInt;
	vector <int>* Colw;
	vector <int>* IndID;
	vector <float>* IndT;
	vector <float>* IndA;
	vector <float>* IndInt;
	vector <int>* Indw;
	int EventType;
	//0 - single track traversing the entire drift region; 
	//1 - single track travesing partially;
	//2 - showers (includes partial showers and multiple tracks)
	//3 - accidentals (scattered shower fragments triggering)
};
hitdata hd;

struct trackdata
{
	int E;//event
	vector <int>* StartEndColIndT;//start col, start ind, start t, end col, end ind, end t
	vector <float>* FitParams;
	float FitNormChi2;
	float QColTot;
	int NHits;
	int Nexcl;
	vector <float>* PMTIntegral;
	vector <float>* ColTStartEnd;
	vector <float>* ColHitTStartEnd;
};
trackdata td;

struct peaks
{
	int IC;
	int CH;
	vector <int> t;
};
peaks p;

struct peaks2
{
	float t;//time
	int w;//wire id
	float integral;
	float integral2;
	float amplitude;
	float fwhm;
	float w10perc;//width at 10% amplitude
	float wfull;//full width
};

struct PMTIntegrals
{
	int RunNo;
	int EventNo;
	
	vector <float>* integral_1500_2500;
	vector <float>* integral_1500_6000;
	vector <float>* integral_1500_8000;
	vector <float>* integral_1500_10000;
	vector <float>* integral_1500_15000;
	
	vector <float>* integralZS_1500_2500;
	vector <float>* integralZS_1500_6000;
	vector <float>* integralZS_1500_8000;
	vector <float>* integralZS_1500_10000;
	vector <float>* integralZS_1500_15000;
	
	vector <float>* integral4sigma_1500_2500;
	vector <float>* integral4sigma_1500_6000;
	vector <float>* integral4sigma_1500_8000;
	vector <float>* integral4sigma_1500_10000;
	vector <float>* integral4sigma_1500_15000;
	
	vector <int>* isSaturated;
	vector <int>* isPedestalOK;
	vector <float>* undershoot;
	vector <float>* spe;
	vector <float>* speerr;
};
PMTIntegrals pint;

struct hits
{
	int col;
	int ind;
	int t;
	int Int;
	int Int2;
	int Int3;
	
	float colT;
	float indT;
	int colWidth;
	int indWidth;
	float colAmp;
	float indAmp;
};
hits hit;

struct hitcandidates
{
	vector <int> col;
	vector <int> ind;
	vector <int> colstartw;
	vector <int> colendw;
	int t;
	vector <int> n;
	vector <int> Int2;//induction
	vector <int> Int3;//collection
};
hitcandidates hc;

struct hitcandidateshort
{
	int ci;//collection 0 induction 1
	int wid;//wire id
	float t;//positive peak time
	float t2;//zero crossing time
	float t3;//negative peak time
	int Int;//default collection integral
	int Int2;//undershoot corrected collection integral
	int w;//width
	float a;//positive peak amplitude
	float a2;//pos peak to neg peak
	float a3;//negative peak amplitude
	float us;//undershoot
};
hitcandidateshort hcs;

struct clusters
{
	vector <int> hi;//hit id
};
clusters cl;

struct tlist
{
	int t;
	vector <int> ch;//0-127 col; 128-255 ind
};
tlist tl;

struct fullevent
{
	int E;
	float fitParams[5];
	float fitnormchi2;
	int clsize;
	float PMTint[3];
	vector <hits> H;
};
fullevent fe;

struct MCEvent
{
	int R;
	int E;
	float XYT[2][3];
};

Double_t fitf2f(Double_t *x,Double_t *par)
{
	Double_t fitval=0.;
	Double_t sigma=0.;
	Double_t tt=(x[0]-par[1]);
	if(x[0]<par[1])
	{
		sigma=exp(par[2]*x[0]);
	}
	else
	{
		sigma=exp(-x[0]/par[3])+exp(-x[0]/par[4])+exp(-x[0]/par[5]);
	}
	fitval=par[0]*sigma;
	return fitval;
}

Double_t fitf3f(Double_t *x,Double_t *par)
{
	Double_t fitval=0.;
	Double_t sigma=0.;
	Double_t tt=(x[0]-par[1]);
	if(x[0]<par[1])
	{
		sigma=par[2]+par[3]*pow(par[1]-x[0],par[4]);
	}
	else
	{
		sigma=par[2]+par[5]*pow(x[0]-par[1],par[6]);
	}
	fitval=par[0]*exp(-0.5*pow(tt/sigma,2));
	return fitval;
}

Double_t fitf(Double_t *x,Double_t *par)
{
	Double_t fitval=0.;
	Double_t sigma=0.;
	Double_t tt=(x[0]-par[1]);
	if(x[0]<par[1])
	{
		sigma=par[2]+par[3]*pow(par[1]-x[0],par[4]);
	}
	else
	{
		sigma=par[2]+par[5]*pow(x[0]-par[1],par[6]);
	}
	fitval=par[0]*exp(-0.5*pow(tt/sigma,2));
	return fitval;
}

Double_t fitf2(Double_t *x,Double_t *par)
{
	Double_t fitval=0.;
	Double_t sigma=0.;
	Double_t tt=(x[0]-par[1]);
	if(x[0]<par[1])
	{
		sigma=par[2]+par[3]*pow(par[1]-x[0],par[4]);
	}
	else
	{
		sigma=par[2];
	}
	fitval=par[0]*exp(-0.5*pow(tt/sigma,2));
	return fitval;
}

void line(double t, const double *p, double &x, double &y, double &z)
{
	x = p[0] + p[1]*(t-p[4]);
	y = p[2] + p[3]*(t-p[4]);
	z = t;
}

struct SumDistance2
{
	TGraph2D *fGraph;
	SumDistance2(TGraph2D *g) : fGraph(g) {}
	double distance2(double x,double y,double z, const double *p)
	{
		XYZVector xp(x,y,z);
// 		XYZVector x0(p[0], p[2], 0. );
		XYZVector x0(p[0]-p[1]*p[4], p[2]-p[3]*p[4], 0. );
// 		XYZVector x1(p[0] + p[1], p[2] + p[3], 1. );
		XYZVector x1(p[0] + p[1]*(z-p[4]), p[2] + p[3]*(z-p[4]), z );
		XYZVector u = (x1-x0).Unit();
		double d2 = ((xp-x0).Cross(u)).Mag2();
		return d2;
	}
	double operator() (const double *par)
	{
		assert(fGraph != 0);
		double * x = fGraph->GetX();
		double * y = fGraph->GetY();
		double * z = fGraph->GetZ();
		int npoints = fGraph->GetN();
		double sum = 0;
		for (int i  = 0; i < npoints; ++i)
		{
			double d = distance2(x[i],y[i],z[i],par);
			sum += d;
		}
		return sum;
	}
};

float line3Dfit(TGraph2D * gr)
{
	ROOT::Fit::Fitter  fitter;
	SumDistance2 sdist(gr);
// 	ROOT::Math::Functor fcn(sdist,4);
	ROOT::Math::Functor fcn(sdist,5);
	fitter.SetFCN(fcn,pStart);
	for (int i = 0; i < 5; ++i) fitter.Config().ParSettings(i).SetStepSize(0.01);
	
// 	fitter.Config().ParSettings(0).SetLimits(-50,50);
// 	fitter.Config().ParSettings(1).SetLimits(-5,5);
// 	fitter.Config().ParSettings(2).SetLimits(-50,50);
// 	fitter.Config().ParSettings(3).SetLimits(-5,5);
// 	fitter.Config().ParSettings(4).SetLimits(0,4100);
	
// 	fitter.Config().ParSettings(0).SetStepSize(0.1);
// 	fitter.Config().ParSettings(1).SetStepSize(0.01);
// 	fitter.Config().ParSettings(2).SetStepSize(0.1);
// 	fitter.Config().ParSettings(3).SetStepSize(0.01);
// 	fitter.Config().ParSettings(4).SetStepSize(1);
	
// 	fitter.Config().SetMinimizer("Minuit","Migrad");
	
	bool ok = fitter.FitFCN();
// 	if (!ok)
// 	{
// // 		cout<<"Line3D Fit failed"<<endl;
// 		return -1;
// 	}
	const ROOT::Fit::FitResult & result = fitter.Result();
	
// 	parFit=result.GetParams();
	const double * parFit = result.GetParams();
	for(int i1=0;i1<5;i1++)
	{
		fitParams[i1]=parFit[i1];
	}
	return sqrt(result.MinFcnValue()/gr->GetN());
}

int FindBinAbove(TH1F* h,float th,int sb)
{
	for(int ip1=sb;ip1<=h->GetNbinsX();ip1++)
	{
		if(h->GetBinContent(ip1)>th){return ip1;}
	}
	return -1;
}

bool compareByHitTime(const hits &a, const hits &b)
{
    return a.t < b.t;
}

bool compareByTLTime(const tlist &a, const tlist &b)
{
    return a.t < b.t;
}

bool SortPeaksByT(const peaks2 &a,const peaks2 &b){return (a.t < b.t);}

struct singlehit
{
	float x;
	float y;
	float t;
	vector <float> Int;
};

struct PMTCalibData
{
	int RunNo;
	vector <float> spe;
	vector <float> speerr;
};

Double_t LanGau(Double_t *x, Double_t *par)
{
        Double_t result=par[0]*(TMath::Gaus(x[0],par[1],par[2]))+par[3]*(TMath::Landau(x[0],par[1],par[4]));
        return result;
}

// Double_t LanGau(Double_t *x, Double_t *par)
// {
//         Double_t result=par[0]*par[3]*(TMath::Gaus(x[0],par[1],par[2]))*(TMath::Landau(x[0],par[1],par[4]));
//         return result;
// }

Double_t GauGau(Double_t *x, Double_t *par)
{
        Double_t result=par[0]*(TMath::Gaus(x[0],par[1],par[2]))+par[3]*(TMath::Gaus(x[0],par[1],par[4]));
        return result;
}

struct FlicMCShort{
	int RunNo;
	int EvNo;
	vector <float>* PMTPE;
	vector <float>* PMTPE2;
	float dETot;
};
FlicMCShort fc;