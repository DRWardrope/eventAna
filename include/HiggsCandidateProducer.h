/***********************************************************************************************
*	HiggsCandidateProducer
*	A class to create HiggsCandidate objects from XhhMiniNtuple contents and store in Event.h
*
*	David Wardrope, 22nd March 2016
*
***********************************************************************************************/
#ifndef HIGGSCANDIDATEPRODUCER_H
#define HIGGSCANDIDATEPRODUCER_H

//#include "ProducerBase.h"
#include "Event.h"
#include "HiggsCandidate.h"
#include "Muon.h"
#include <random>
#include <stdlib.h> 

extern bool m_debug;

class HiggsCandidateProducer 
{
	public:
        HiggsCandidateProducer(Event& evt): m_evt(evt)
		{ 
			std::random_device rd;
    		m_ranGen = std::mt19937(rd());
		}
//        ~HiggsCandidateProducer(){
  //      }
		std::vector<HiggsCandidate> produceResolved(const std::vector<Jet>&, const std::vector<Muon>&, 
											const float& ptMin = -99., const float& ptSplit = 13000.);
		std::vector<HiggsCandidate> produceMerged(const std::vector<Jet>&, const std::vector<Muon>&,
												const float& semiPtMin = -99., const float& ptSplit = 13000.);
		//std::vector<HiggsCandidate> produceBTagged(const float& ptMin = -99.) { return produce(ptMin, true); }
	private:
		void calculateXt(HiggsCandidate&, const int&, const Jet&, const Jet&);
		void calculateXt(HiggsCandidate& hcand, const std::vector<Jet>& jets);
		std::vector<HiggsCandidate> chooseBestPair(const std::vector<HiggsCandidate>&,
														 std::function<float(const HiggsCandidate&, const HiggsCandidate&)>&);
		void cleanUp();
		void fillArrays();
		//std::vector<Jet>::const_iterator findLeadingTrackJet(const Jet& jet, const std::vector<Jet>& trackJets);
		std::shared_ptr<Jet> findLeadingTrackJet(const Jet& jet, const std::vector<Jet>& trackJets);
		void muonInJetCorrectionBoosted(HiggsCandidate&, const std::vector<Muon>&);
		void muonInJetCorrectionResolved(HiggsCandidate&, const std::vector<Muon>&);
		void nullPtrs();
		//Returns 0, 1 or 2 Higgs candidates that do not share jets and are "best" by metric decided by std::function passed
		std::vector<HiggsCandidate> resolveAmbiguities(const std::vector<HiggsCandidate>&,
														 std::function<float(const HiggsCandidate&, const HiggsCandidate&)>&,
														 std::function<bool(const HiggsCandidate&, const HiggsCandidate&)>&);
		void setupBranches();

		Event& m_evt;
		std::mt19937 m_ranGen;	
};
//The following functions can be used to pick the "best" pair of Higgs candidates. These are defined so the best candidates have scores closest to zero.
float calcDhhScore(const HiggsCandidate&, const HiggsCandidate&);
float calcMV2c20Score(const HiggsCandidate&, const HiggsCandidate&);
bool highestMV2c20(const HiggsCandidate&, const HiggsCandidate&);
//float highestMV2
#endif
