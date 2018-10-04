/**************************************************************************************************
*	SimpleEvent
*	Reads and holds data from the output TTree from xhhAnalysis 
*	David Wardrope, 8th November 2016
**************************************************************************************************/
#ifndef SIMPLEEVENT_H
#define SIMPLEEVENT_H
#include <iostream>
#include <functional>
#include <memory>
#include "AtlasStyle.h"
#include "TCanvas.h"
#include "TColor.h"
#include "TF1.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TKey.h"
#include "TLegend.h"
#include "TLine.h"
#include "TMath.h"
#include "TMVA/Reader.h"
#include "TPaveText.h"
#include "TRegexp.h"
#include "TTree.h"
#include "TVirtualFitter.h"

class SimpleEvent
{
	public:
		SimpleEvent(TTree* treeIn);
		long getEntry(const unsigned int i); 
		bool inControlRegion1() const { return rhh > 55 && rhh < 75;}
		bool inControlRegion2() const { return rhh < 55 && !inSignalRegion();}
		bool inSignalRegion() const { return xhh < 1.6;}
		bool inSuperRegion() const { return true;}//return rhh < 75;}
		bool inValRegion1() const;
		bool inValRegion2() const;
		std::string name() const { return m_name; }
		long numEntries() const { return tree->GetEntries(); }
		//void setupOutputTreeBranches(TTree* tOut=nullptr);
	private:
		void resetVariables();
		void setupBranch(const TString& name, void* address);
		std::string m_name;

	//Make lots of variables public, to ease access.
	public:
		TTree* tree;
		int h1_numTightTags, h2_numTightTags;
		int h1_j1_flav, h1_j2_flav, h2_j1_flav, h2_j2_flav;
		int hlt_b80_xe60, hlt_j380, hlt_b225, hlt_j100_2b55, hlt_b75_3j75, hlt_2b35_2j35;
		int jet_n;
		float weight, weight1, weight2;
		float h1_m, h2_m, xhh, rhh;
		float h1_pt, h2_pt;
		float h1_j1_pt, h1_j2_pt, h2_j1_pt, h2_j2_pt;
		float hh_m;
		float extraMuon1_pt, extraMuon1_mt;
		float extraJet1_pt, extraJet1_HH_dPhi;
		float klfit_6j, klfit_7j;
		float met;
};
#endif
