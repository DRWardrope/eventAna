/************************************************************************************
*	B->bH->bbb analysis code
*	v1.0 5th October 2018
*	Note: Just a quick test, to help out Marco
*	David Wardrope
*************************************************************************************/
#ifndef VLQANALYSIS_H
#define VLQANALYSIS_H
#include <functional>
#include <memory>
#include <vector>
#include "TFile.h"

#include "BTrigger.h"
#include "ElectronProducer.h"
#include "Event.h"
#include "FatJetProducer.h"
#include "HiggsCandidate.h"
#include "HiggsCandidateProducer.h"
#include "JetProducer.h"
#include "MuonProducer.h"
#include "ParticleProducer.h"
#include "SampleHandler.h"
#include "TopCandidateProducer.h"
#include "TrackJetProducer.h"
#include "TRandom3.h"
#include "VLQTreeManager.h"

//TRandom3* m_higgsRand = new TRandom3(25151661);
void addFile(std::string, double);
void analyseEvents(std::vector<std::pair<std::string, std::pair<double, double> > >::const_iterator);////The main loop over events.
std::string getCategory(TFile* file);
std::string getCategory(const std::string& name_notnice);
int getDSID(const std::string& fileName);
std::string getNiceName(const std::string& origName);
std::string higgsCandCatName(const int& i);
bool failJetCleaning(const std::vector<Jet>&);
bool isData(const std::string& name);
bool isSignal(const std::string& name);
bool isX(const std::string& xString, const std::string& name);
std::vector<const Particle*> matchBToH(const std::vector<Particle>& higgses, const std::vector<Particle>& bQuarks);
int numBJets(const std::vector<Jet>&);
int processCommandLine(int argc, char** argv);
void setTruthHiggsId(HiggsCandidate&, const std::vector<const Particle*>&);
//std::vector<std::string> setupCategories();
void setupFiles(float);
void setupSystList();
void setupTriggerList();
bool setupBTriggerSF();

BTrigger * m_btrig;
std::vector<std::pair<std::string, std::pair<double, double> > > m_fileList; //This holds all the ntuples and corresponding event weights
std::vector<std::string> m_systList;
std::string m_singleSyst = "Baseline";
std::vector<std::string> m_trigList;
//Global variables that are not parameters.
std::vector<std::string> m_errorMessages;
std::string m_outputFileName; 
TFile* m_fOut;
TreeManager* m_treesOut;
SampleHandler m_sampleHandler;
#endif
