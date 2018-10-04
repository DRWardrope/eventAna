#ifndef SROPTIMISER_H
#define SROPTIMISER_H
#include <iostream>
#include <functional>
#include <memory>
#include "AtlasStyle.h"
#include "SimpleEvent.h"
#include "TFile.h"
#include "TKey.h"
#include "TMath.h"
#include "TRegexp.h"
#include "TTree.h"

class SRDefinition
{
	public:
		SRDefinition(): leadM0(-99), sublM0(-99), threshold(-99) {};
		SRDefinition(const double& leadM0In, const double& sublM0In, const double& thresholdIn): 
			leadM0(leadM0In), sublM0(sublM0In), threshold(thresholdIn) {};
		double leadM0, sublM0, threshold;
};

TFile* m_fIn;
TString m_plotDir; 
//float calculateMuQCD(HistSetCollection&, HistSetCollection&, HistSetCollection&, HistSetCollection&);
std::vector<TTree*> getSignals();
int processCommandLine(int argc, char** argv);
SRDefinition findOptimum(SimpleEvent&, SimpleEvent&, SimpleEvent&);
double calcXhh(const double&, const double&, const double&, const double&);//evt.h1_m, evt.h2_m, leadM0, sublM0) < xhhThreshold)
double yieldPassingSeln(SimpleEvent&, const double&, const double&, const double&);

#endif
