#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include <time.h>
#include <math.h>
#include <string.h>
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TF1.h"
#include "TF2.h"
#include "TProfile.h"
#include "TROOT.h"
#include "TApplication.h"
#include "TString.h"
#include "TStyle.h"
#include "TMath.h"
#include "TFile.h"
#include "TTree.h"
#include "TRandom3.h"
#include "TRandom.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TGraph2D.h"
#include "TGraphErrors.h"
#include "TSpectrum.h"

#include "TVirtualFFT.h"
#include "Math/Functor.h"
#include "TPolyLine3D.h"
#include "TPolyMarker3D.h"
#include "Math/Vector3D.h"
#include "Fit/Fitter.h"

#include <netinet/in.h>

using namespace std;
using namespace ROOT::Math;

int RunNo=0;
int EventNo=0;
int opt=0;
ifstream::pos_type size=2097416;
char hname[1000];
char hname2[1000];
char TPCinFilePath[1000];
char PMTinFilePath[1000];
char AnalysisFilePath[1000];
char NTupleFilePath[1000];
ifstream file;
int data0loc=0;
int TPCBaselineRun=0;
int PMTBaselineRun=0;
int TimeRange[2]={750,2500};
float HitColIndDist=5.;

int NPMTs=3;//number of PMTs

// int ThInd=7;
// int ThCol=7;
// int ThInd=3;
// int ThCol=4;
// float ThInd=3;
// float ThCol=4;
float ThColInd[2]={0};//Col ind
float ThColIndW[2][128]={{0}};//Col ind
float ThPMT[3]={0.};

float TPCBaselines[2][128][2]={{{0}}};
float PMTBaselines[3][2]={{0}};
float ThSigma[2]={4.5,4.5};//threshold for collection and induction signal sigmas above baseline for hit reconstruction

int IC[256]={0};
int CH[256]={0};

double pStart[5] = {0};
double fitParams[5]={0};

int peakSearchWidth=1;

string wt[2]={"Collection","Induction"};
// string PMTNames[3]={"LAr w/TPB","LAr no TPB","LXe"};
string PMTNames[6]={"LAr_wTPB","LAr_noTPB","LXe","LAr_wTPB_new","SC1","SC2"};

vector <int> B;

#include "include/common.cc"

#include "include/FindBaselineRun.cc"
#include "include/ReadTPCBaselines.cc"
#include "include/ReadPMTBaselines.cc"
#include "include/ColWireLines.cc"
#include "include/ShowerAnalysis.cc"
#include "include/TrackAnalysis.cc"
#include "include/TrackAnalysis3.cc"
#include "include/PMTAnalysis.cc"
#include "include/TrackAndPMTAnalysis.cc"
#include "include/GenerateMCInput_v2.cc"
#include "include/PrintNevt.cc"
#include "include/GetBaseline.cc"
#include "include/GetPMTCalibration.cc"
#include "include/GetPMTCalibration2.cc"
#include "include/GetPMTCalibration3.cc"
#include "include/GetPMTCalibration4.cc"
#include "include/GetPMTCalibration_CalibRuns.cc"
#include "include/GetPMTCalibration_CalibRunsTest.cc"
#include "include/GetPMTIntegral.cc"
#include "include/FitTracks.cc"
#include "include/FindDeltaT.cc"
#include "include/FindHitsAndFitTracks.cc"
#include "include/HitAnalysis.cc"
#include "include/HitAnalysis2.cc"
#include "include/WriteWF.cc"
#include "include/TPCCalibration.cc"
#include "include/TPCStats.cc"
#include "include/PMTCalibrationsAll.cc"
#include "include/PMTCalibrationsAll2.cc"
#include "include/TPCTiming.cc"
#include "include/HitFindingTest.cc"
#include "include/GetNPMTs.cc"
#include "include/CompareTPCBaselines.cc"
#include "include/HitAnalysis3.cc"
#include "include/TrackAnalysis4.cc"
#include "include/HitAnalysis4.cc"
#include "include/MIPsAndTracks.cc"
#include "include/GetPMTCalibration_TrigRuns.cc"
#include "include/GetPMTIntegral_v2.cc"
#include "include/MIPAnalysis.cc"

int main( int argc, const char* argv[] )
{
	if(argc==1)
	{
		cout<<"0 : GetTPCBaseline"<<endl;
		cout<<"1 : WriteTPCWF -- provide TPCBaselineRun"<<endl;
		cout<<"2 : FindHits -- provide TPCBaselineRun"<<endl;
		cout<<"3 : PlotHitsSummary"<<endl;
		cout<<"4 : PlotHits"<<endl;
		cout<<"5 : FitTracks"<<endl;
		cout<<"6 : GetPMTBaseline"<<endl;
		cout<<"7 : GetPMTCalibration -- provide PMTBaselineRun"<<endl;
		cout<<"8 : WritePMTWF -- provide PMTBaselineRun"<<endl;
		cout<<"9 : GetPMTCalibration2"<<endl;
		cout<<"10 : GetPMTIntegral -- provide PMTBaselineRun"<<endl;
		return -1;
	}
	opt=atoi(argv[1]);
	RunNo=atoi(argv[2]);
	
 	sprintf(TPCinFilePath,"/eos/project/f/flic2019/Data/TPC/Runs");//where the Run00X.dat files reside
 	sprintf(PMTinFilePath,"/eos/project/f/flic2019/Data/PMT/WaveForms");//where the waveforms_chX.txt files reside
	sprintf(AnalysisFilePath,"/home/bbilki/Analyzer/Flic/flicanalyzer");//where Files and Histos will be placed
	sprintf(NTupleFilePath,"/Data/Flic/NTuple");//where the Run_XX.root files reside
	
	TPCBaselineRun=FindBaselineRun(RunNo);
	PMTBaselineRun=TPCBaselineRun;
	
	GetNPMTs();
	
	if(opt==0){GetBaseline();}
	if(opt==1){WriteWF();}
	if(opt==2){FindHitsAndFitTracks();}
	if(opt==3){FitTracks();}
	if(opt==4){ColWireLines();}
	if(opt==6){GetPMTCalibration3();}
	if(opt==7){GetPMTCalibration();}
	if(opt==8){}
	if(opt==9){GetPMTCalibration2();}
	if(opt==10){GetPMTIntegral();}
	if(opt==102){ShowerAnalysis();}
	if(opt==103){FindDeltaT();}
	if(opt==104){TrackAnalysis();}
	if(opt==106){PrintNevt();}
	if(opt==107){HitAnalysis();}
	if(opt==108){HitAnalysis2();}
	if(opt==109){TPCCalibration();}
	if(opt==110){TPCStats();}
	if(opt==111){TrackAnalysis3();}
	if(opt==112){PMTAnalysis();}
	if(opt==113){PMTCalibrationsAll();}
	if(opt==114){TrackAndPMTAnalysis();}
	if(opt==115){PMTCalibrationsAll2();}
	if(opt==116){GenerateMCInput_v2();}
	if(opt==117){GetPMTCalibration_CalibRuns();}
	if(opt==118){GetPMTCalibration4();}
	if(opt==119){TPCTiming();}
	if(opt==120){HitFindingTest();}
	if(opt==121){GetPMTCalibration_CalibRunsTest();}
	if(opt==122){CompareTPCBaselines();}
	if(opt==123){HitAnalysis3();}
	if(opt==124){TrackAnalysis4();}
	if(opt==125){HitAnalysis4();}
	if(opt==126){MIPsAndTracks();}
	if(opt==127){GetPMTCalibration_TrigRuns();}
	if(opt==128){GetPMTIntegral_v2();}
	if(opt==129){MIPAnalysis();}
}

