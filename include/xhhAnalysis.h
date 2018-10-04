/************************************************************************************
*	hh->4b analysis code
*	v3.0 22nd March 2016
*	Note: should be migrated into XhhMixed when possible.
*	David Wardrope
*************************************************************************************/
#ifndef XHHANALYSIS_H
#define XHHANALYSIS_H
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
#include "TreeManager.h"

//TRandom3* m_higgsRand = new TRandom3(25151661);
void addFile(std::string, double);
void analyseEvents(std::vector<std::pair<std::string, std::pair<double, double> > >::const_iterator);////The main loop over events.
void applyBTagSF(Event&, std::array<HiggsCandidate, 2>&);
//float calcDhhScore(const HiggsCandidate& h1, const HiggsCandidate& h2);
std::vector<HiggsCandidate> chooseBestPair(const std::vector<HiggsCandidate>& hcands,
                                                std::function<float(const HiggsCandidate&, const HiggsCandidate&)>& bestPairChooser);
std::vector<HiggsCandidate> chooseHiggsCandidates(const std::vector<HiggsCandidate>&, const std::vector<HiggsCandidate>&);
int combinedCategory(const int& cat1, const int& cat2);
int countTags(const std::array<HiggsCandidate, 2>&);
std::string eventCatName(const int& i);
std::vector<HiggsCandidate> findHiggsCandidates(const std::vector<Jet>&, const std::vector<Jet>&, const std::vector<Jet>&,
												 const std::vector<const Particle*>&);
std::vector<HiggsCandidate> findHiggsCandidates(const std::vector<Jet>&, const std::vector<Jet>&, const std::vector<const Particle*>&);
std::vector<HiggsCandidate> findMergedHiggsCandidates(const std::vector<Jet>&, const std::vector<Jet>&, const std::vector<const Particle*>&);
std::vector<HiggsCandidate> findResolvedHiggsCandidates(const std::vector<Jet>&, const std::vector<const Particle*>&);
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
