#ifndef SIGCONTAMINATIONANALYSIS_H
#define SIGCONTAMINATIONANALYSIS_H
#include <iostream>
#include <functional>
#include <memory>
#include "AtlasStyle.h"
#include "SimpleEvent.h"
#include "SimpleHistSet.h"
#include "TFile.h"
#include "TKey.h"
#include "TMath.h"
#include "TRegexp.h"
#include "TTree.h"

TFile* m_fIn;
TString m_plotDir; 
//float calculateMuQCD(HistSetCollection&, HistSetCollection&, HistSetCollection&, HistSetCollection&);
std::map<std::string, double> calcSigFracs(SimpleEvent& data, std::vector<TTree*> sigTrees);
TTree* combineTTrees(TTree* a, TTree* b, const TString& newName);
std::vector<TTree*> getSignals(const int&);
void printFractions(const std::map<std::string, double>&, const std::map<std::string, double>&); 
int processCommandLine(int argc, char** argv);
SimpleHistSetCollection makePlots(std::string title, SimpleEvent&);

#endif
