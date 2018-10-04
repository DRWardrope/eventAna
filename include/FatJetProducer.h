/***********************************************************************************************
*	FatJetProducer
*	A class to create Jet objects from MixedRecoNtuple contents and store in Event.h
*
*	David Wardrope
*
***********************************************************************************************/
#ifndef FATJETPRODUCER_H
#define FATJETPRODUCER_H

#include "Event.h"
#include "Jet.h"
#include <stdlib.h> 

extern bool m_debug;
class FatJetProducer 
{
	public:
        FatJetProducer(Event& evt): m_evt(evt)
		{ 
			setupBranches(); 
		}
        ~FatJetProducer(){
        }
		std::vector<Jet> produce(const float& ptMin, const float& etaMax, const std::vector<Jet>& trackJets);
	private:
		void cleanUp();
		void fillArrays();
		void nullPtrs();
		void setupBranches();

		Event& m_evt;
		//TTree variables
		std::vector<float> *m_jet_pt, *m_jet_eta, *m_jet_phi, *m_jet_m;
		std::vector<std::vector<int> >* m_jet_trackJetIdx;
};
#endif
