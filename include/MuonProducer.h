/***********************************************************************************************
*	MuonProducer
*	A class to create Jets
*
*	David Wardrope, 29th November 2012
*
***********************************************************************************************/
#ifndef MUONPRODUCER_H
#define MUONPRODUCER_H

#include "Event.h"
#include "Muon.h"
#include <stdlib.h> 

extern bool m_debug;
//extern float m_minBWeight;
class MuonProducer 
{
	public:
        MuonProducer(Event& evt):m_evt(evt)
		{ 
			//if(m_debug) std::cout<<"MuonProducer("<< name <<", " <<")"<< std::endl;
			setupBranches(); 
		}
        ~MuonProducer(){
        }
		std::vector<Muon> produce(const float& ptMin = -99., bool reqPassMedium=true);
	private:
		void cleanUp();
		void fillArrays();
		void nullPtrs();
		void setupBranches();

		Event& m_evt;
		std::vector<float> *m_muon_pt, *m_muon_eta, *m_muon_phi, *m_muon_eLoss;
		std::vector<int> *m_muon_quality, *m_muon_isolated;
};
#endif
