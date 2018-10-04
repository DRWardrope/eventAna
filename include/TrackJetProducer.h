/***********************************************************************************************
*	TrackJetProducer
*	A class to create Jet objects from MixedRecoNtuple contents and store in Event.h
*
*	David Wardrope
*
***********************************************************************************************/
#ifndef TRACKJETPRODUCER_H
#define TRACKJETPRODUCER_H

//#include "ProducerBase.h"
#include "Event.h"
#include "Jet.h"
#include <stdlib.h> 

extern bool m_debug;
//extern float m_minBWeight;
class TrackJetProducer 
{
	public:
        TrackJetProducer(Event& evt, const int& btagWP=77): m_evt(evt)
		{ 
			setupBranches(); 
			setMinBTag(btagWP);
		}
        ~TrackJetProducer(){
        }
		std::vector<Jet> produce(const float& ptMin = -99., bool rejectNonBTagged=false);
		void setMinBTag(const int& btagWP) { m_minBWeight = findMinBTag(btagWP); }
		float findMinBTag(const int& btagWP)
		{
			// R=0.2 track-jets:
			if(btagWP == 60) return 0.8529;
			else if(btagWP == 70) return 0.6455;
			else if(btagWP == 77) return 0.3706;
			else if(btagWP == 85) return -0.1416;
			else return -99.;
			//2015 mv2c20 thresholds:
			/*if(btagWP == 60) m_minBWeight = 0.1899;
			else if(btagWP == 70) m_minBWeight = -0.3098;
			else if(btagWP == 77) m_minBWeight = -0.6134;
			else if(btagWP == 85) m_minBWeight = -0.8433;
			else m_minBWeight = -99.;*/
		}
	private:
		void cleanUp();
		void fillArrays();
		void nullPtrs();
		void setupBranches();

		Event& m_evt;
		//TString m_systLabel;
		float m_minBWeight;
		
		//TTree variables
		std::vector<float> *m_jet_pt, *m_jet_eta, *m_jet_phi, *m_jet_m, *m_jet_mv2c10;
		std::vector<float> *m_jet_mv2c10_SF;
		std::vector<int> *m_jet_pdgId;
		std::vector<std::vector<int> > *m_jet_partIdx;
};
#endif
