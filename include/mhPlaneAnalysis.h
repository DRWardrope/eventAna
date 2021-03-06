#ifndef FLAVOURANALYSIS_H
#define FLAVOURANALYSIS_H
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

TFile* m_fIn;
TString m_plotDir; 
//float calculateMuQCD(HistSetCollection&, HistSetCollection&, HistSetCollection&, HistSetCollection&);
std::vector<TTree*> getDijets(const int&);
int processCommandLine(int argc, char** argv);
void writePlots(std::string title, SimpleEvent&);

#endif
