/************************************************************************************
*	X->hh->4b analysis code v2
*	v2.0 1st October 2013
*	David Wardrope
*	Ben Cooper, Luke Lambourne
*************************************************************************************/
#include <memory>
#include <vector>
#include "TFile.h"

#include "Event.h"
#include "HiggsCandidate.h"
#include "LittlePlotter.h"
#include "JetProducer.h"
#include "ParticleProducer.h"
#include "FatJetProducer.h"
#include "TrackJetProducer.h"
 
void addFile(TString, double, double);
void analyseEvents(std::vector<std::pair<TString, std::pair<double, double> > >::const_iterator, LittlePlotter&); //The main loop over events.
void bookPlots(LittlePlotter&);
int combinedCategory(const int& cat1, const int& cat2);
TString eventCatName(const int& i);
std::vector<HiggsCandidate> findHiggsCandidates(const std::vector<Jet>&, const std::vector<Jet>&, const std::vector<Jet>&,
												 const std::vector<const Particle*>&);
std::vector<HiggsCandidate> findHiggsCandidates(const std::vector<Jet>&, const std::vector<Jet>&, const std::vector<const Particle*>&);
std::vector<HiggsCandidate> findMergedHiggsCandidates(const std::vector<Jet>&, const std::vector<Jet>&, const std::vector<const Particle*>&);
std::vector<HiggsCandidate> findResolvedHiggsCandidates(const std::vector<Jet>&, const std::vector<const Particle*>&);
TString higgsCandCatName(const int& i);
void makePlots(LittlePlotter&);
std::vector<const Particle* > matchBToH(const std::vector<Particle>&, const std::vector<Particle>&);
bool notAllBQuarksFound(const std::vector<const Particle*>&);
void plotHiggsCandidates(const std::string& prefix, const Event&, LittlePlotter&, 
							const std::vector<HiggsCandidate>&, const std::vector<Particle>& higgs, const std::vector<const Particle*>& bquarks);
std::vector<TString> setupCategories();
void setupFiles(float);

std::vector<std::pair<TString, std::pair<double, double> > > m_fileList; //This holds all the ntuples and corresponding event weights
std::vector<TString> m_systList;
TString m_singleSyst = "Baseline";
std::vector<TString> m_trigList;
//Global variables that are not parameters.
//TreeManagerDiboson* m_treesOut;
std::vector<TString> m_errorMessages;
