/***********************************************************************************************
*	TopCandidateProducer
*	A class to create TopCandidate objects from XhhMiniNtuple contents and store in Event.h
*
*	David Wardrope, 22nd March 2016
*
***********************************************************************************************/
#ifndef TOPCANDIDATEPRODUCER_H
#define TOPCANDIDATEPRODUCER_H

//#include "ProducerBase.h"
#include "HiggsCandidate.h"
#include "TopCandidate.h"
#include "Muon.h"
#include <stdlib.h> 

extern bool m_debug;

class TopCandidateProducer 
{
	public:
        //TopCandidateProducer()
		//{ 
		//}
//        ~TopCandidateProducer(){
  //      }
		std::vector<TopCandidate> produce(const std::vector<Jet>&) const;
		std::vector<TopCandidate> produce(const std::array<HiggsCandidate, 2>&, const std::vector<Jet>&) const;
//std::vector<TopCandidate> produceBTagged(const float& ptMin = -99.) { return produce(ptMin, true); }
	private:
		void setupBranches();
};
//The following functions can be used to pick the "best" pair of Top candidates. These are defined so the best candidates have scores closest to zero.
//float highestMV2
#endif
